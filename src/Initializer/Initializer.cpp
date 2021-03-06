#include "Initializer.h"

#include <QSettings>

#include "check.h"
#include "qt_utilites/SlotWrapper.h"
#include "Paths.h"
#include "qt_utilites/QRegister.h"

#include "InitializerJavascript.h"
#include "InitInterface.h"
#include "qt_utilites/ManagerWrapperImpl.h"

SET_LOG_NAMESPACE("INIT");

namespace initializer {

InitState::InitState(const QString &type, const QString &subType, const QString &message, bool isCritical, bool isScipped, const TypedException &exception)
    : type(type)
    , subType(subType)
    , message(message)
    , isCritical(isCritical)
    , isScipped(isScipped)
    , exception(exception)
{}

Initializer::Initializer(InitializerJavascript &javascriptWrapper, QObject *parent)
    : QObject(parent)
    , javascriptWrapper(javascriptWrapper)
{
    Q_CONNECT(this, &Initializer::resendAllStatesSig, this, &Initializer::onResendAllStates);
    Q_CONNECT(this, &Initializer::javascriptReadySig, this, &Initializer::onJavascriptReady);
    Q_CONNECT2(this, &Initializer::sendState, this, &Initializer::onSendState, Qt::ConnectionType::QueuedConnection);
    Q_CONNECT2(this, &Initializer::getAllTypes, this, &Initializer::onGetAllTypes, Qt::ConnectionType::QueuedConnection);
    Q_CONNECT2(this, &Initializer::getAllSubTypes, this, &Initializer::onGetAllSubTypes, Qt::ConnectionType::QueuedConnection);

    Q_REG(Initializer::Callback, "Initializer::Callback");
    Q_REG(GetAllStatesCallback, "GetAllStatesCallback");
    Q_REG(ReadyCallback, "ReadyCallback");
    Q_REG3(InitState, "InitState", "initialize");
    Q_REG(GetTypesCallback, "GetTypesCallback");
    Q_REG(GetSubTypesCallback, "GetSubTypesCallback");
}

Initializer::~Initializer() = default;

void Initializer::complete() {
    isCompleteSets = true;
}

void Initializer::sendStateToJs(const InitState &state, int number, int numberCritical) {
    emit javascriptWrapper.stateChangedSig(number, totalStates, numberCritical, totalCriticalStates, state);
}

void Initializer::sendInitializedToJs(bool isErrorExist) {
    emit javascriptWrapper.initializedSig(!isErrorExist, TypedException());
}

void Initializer::sendCriticalInitializedToJs(bool isErrorExist) {
    emit javascriptWrapper.initializedCriticalSig(!isErrorExist, TypedException());
}

void Initializer::onSendState(const InitState &state) {
BEGIN_SLOT_WRAPPER
    CHECK(isCompleteSets, "Not complete initializer"); // Запросы приходят в QueuedConnection, поэтому флаг isComplete в этот момент уже должен быть установлен
    CHECK(existStates.size() == states.size(), "Incorrect existStates struct");

    CHECK(existStates.find(std::make_pair(state.type, state.subType)) == existStates.end(), "State already setted");
    CHECK((int)states.size() < totalStates, "Incorrect state number");

    CHECK(countStatesForInits.find(state.type) != countStatesForInits.end(), "Not registered type: " + state.type.toStdString());
    CHECK(countStatesForInits.at(state.type) > 0, "States overflow for type: " + state.type.toStdString());
    countStatesForInits[state.type]--;
    if (state.isCritical) {
        CHECK(countCriticalStatesForInits.find(state.type) != countCriticalStatesForInits.end(), "Not registered type: " + state.type.toStdString());
        CHECK(countCriticalStatesForInits.at(state.type) > 0, "States overflow for type: " + state.type.toStdString());
        countCriticalStatesForInits[state.type]--;
    }

    existStates.emplace(state.type, state.subType);
    states.emplace_back(state);
    if (state.exception.isSet()) {
        isErrorExist = true;
        if (state.isCritical) {
            isErrorCritical = true;
        }
    }
    if (state.isCritical) {
        CHECK(countCritical < totalCriticalStates, "Incorrect count critical");
        countCritical++;
    }
    sendStateToJs(state, (int)states.size(), countCritical);
    if (!isCriticalInitFinished && countCritical == totalCriticalStates) {
        for (const auto &pair: countCriticalStatesForInits) {
            CHECK(pair.second == 0, "Incorrect countCriticalStatesForInits");
        }
        isCriticalInitFinished = true;
        sendCriticalInitializedToJs(isErrorCritical);
    }
    if (!isInitFinished && (int)states.size() == totalStates) {
        for (const auto &pair: countStatesForInits) {
            CHECK(pair.second == 0, "Incorrect countStatesForInits");
        }
        isInitFinished = true;
        sendInitializedToJs(isErrorExist);
    }
END_SLOT_WRAPPER
}

void Initializer::onResendAllStates(const GetAllStatesCallback &callback) {
BEGIN_SLOT_WRAPPER
    runAndEmitCallback([&, this] {
        if (isCompleteSets) {
            int cCritical = 0;
            for (size_t i = 0; i < states.size(); i++) {
                if (states[i].isCritical) {
                    cCritical++;
                }
                sendStateToJs(states[i], (int)(i + 1), cCritical);
            }
            if (isCriticalInitFinished) {
                sendCriticalInitializedToJs(isErrorCritical);
            }
            if (isInitFinished) {
                sendInitializedToJs(isErrorExist);
            }
        }
    }, callback);
END_SLOT_WRAPPER
}

void Initializer::onJavascriptReady(bool force, const ReadyCallback &callback) {
BEGIN_SLOT_WRAPPER
    runAndEmitCallback([&, this] {
        if (!isCriticalInitFinished) {
            return ReadyType::CriticalAdvance;
        }
        if (isErrorCritical) {
            return ReadyType::NotSuccessCritical;
        }
        if (!isInitFinished && !force) {
            return ReadyType::Advance;
        }
        if (isErrorExist && !force) {
            return ReadyType::NotSuccess;
        }
        for (std::unique_ptr<InitInterface> &init: initializiers) {
            init->complete();
        }
        return ReadyType::Finish;
    }, callback, ReadyType::Error);
END_SLOT_WRAPPER
}

void Initializer::onGetAllTypes(const GetTypesCallback &callback) {
BEGIN_SLOT_WRAPPER
    runAndEmitCallback([&, this] {
        std::vector<QString> result;
        for (const auto &init: initializiers) {
            result.emplace_back(init->getType());
        }
        return result;
    }, callback);
END_SLOT_WRAPPER
}

void Initializer::onGetAllSubTypes(const GetSubTypesCallback &callback) {
BEGIN_SLOT_WRAPPER
    runAndEmitCallback([&, this] {
        std::vector<StateType> result;
        for (const auto &init: initializiers) {
            const QString type = init->getType();
            const auto subTypes = init->getSubtypes();
            for (const auto &subType: subTypes) {
                result.emplace_back(type, std::get<0>(subType), std::get<1>(subType), std::get<2>(subType));
            }
        }
        return result;
    }, callback);
END_SLOT_WRAPPER
}

}
