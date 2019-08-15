#include "InitMetaGate.h"

#include <functional>
using namespace std::placeholders;

#include "MetaGate/MetaGate.h"
#include "MetaGate/MetaGateJavascript.h"

#include "mainwindow.h"

#include "Paths.h"
#include "check.h"
#include "TypedException.h"
#include "qt_utilites/SlotWrapper.h"
#include "qt_utilites/QRegister.h"

SET_LOG_NAMESPACE("INIT");

namespace initializer {

QString InitMetaGate::stateName() {
    return "metagate";
}

InitMetaGate::InitMetaGate(QThread *mainThread, Initializer &manager)
    : InitInterface(stateName(), mainThread, manager, false)
{
    Q_CONNECT(this, &InitMetaGate::callbackCall, this, &InitMetaGate::onCallbackCall);
    Q_REG(InitMetaGate::Callback, "InitMetaGate::Callback");

    registerStateType("init", "metagate initialized", true, true);
}

InitMetaGate::~InitMetaGate() = default;

void InitMetaGate::onCallbackCall(const Callback &callback) {
BEGIN_SLOT_WRAPPER
    callback();
END_SLOT_WRAPPER
}

void InitMetaGate::completeImpl() {
    CHECK(manager != nullptr, "manager not initialized");
    CHECK(javascript != nullptr, "javascript not initialized");
}

void InitMetaGate::sendInitSuccess(const TypedException &exception) {
    sendState("init", false, exception);
}

InitMetaGate::Return InitMetaGate::initialize(std::shared_future<MainWindow*> mainWindow) {
    const TypedException exception = apiVrapper2([&, this] {
        manager = std::make_unique<metagate::MetaGate>();
        manager->moveToThread(mainThread);
        javascript = std::make_unique<metagate::MetaGateJavascript>(*manager);
        javascript->moveToThread(mainThread);
        MainWindow &mw = *mainWindow.get();
        emit mw.setMetaGateJavascript(javascript.get(), MainWindow::SetMetaGateJavascriptCallback([this, mainWindow]() {
            sendInitSuccess(TypedException());
        }, std::bind(&InitMetaGate::sendInitSuccess, this, _1), std::bind(&InitMetaGate::callbackCall, this, _1)));
    });

    if (exception.isSet()) {
        sendInitSuccess(exception);
        throw exception;
    }
    return std::make_pair(manager.get(), javascript.get());
}

}