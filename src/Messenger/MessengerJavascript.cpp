#include "MessengerJavascript.h"

#include "check.h"
#include "Log.h"
#include "makeJsFunc.h"
#include "SlotWrapper.h"
#include "utils.h"
#include "Paths.h"

#include "Wallet.h"
#include "WalletRsa.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

MessengerJavascript::MessengerJavascript(QObject *parent)
    : QObject(parent)
{
    CHECK(connect(this, &MessengerJavascript::callbackCall, this, &MessengerJavascript::onCallbackCall), "not connect onCallbackCall");
    CHECK(connect(this, &MessengerJavascript::newMessegesSig, this, &MessengerJavascript::onNewMesseges), "not connect onNewMesseges");

    setPaths(getWalletPath(), "");
}

void MessengerJavascript::onCallbackCall(const std::function<void()> &callback) {
BEGIN_SLOT_WRAPPER
    callback();
END_SLOT_WRAPPER
}

template<typename... Args>
void MessengerJavascript::makeAndRunJsFuncParams(const QString &function, const TypedException &exception, Args&& ...args) {
    const QString res = makeJsFunc2<false>(function, "", exception, std::forward<Args>(args)...);
    runJs(res);
}

static QJsonDocument messagesToJson(const std::vector<Message> &messages, const WalletRsa &walletRsa) {
    QJsonArray messagesArrJson;
    for (const Message &message: messages) {
        QJsonObject messageJson;

        messageJson.insert("collocutor", message.collocutor);
        messageJson.insert("isInput", message.isInput);
        messageJson.insert("timestamp", QString::fromStdString(std::to_string(message.timestamp)));
        if (message.isCanDecrypted) {
            const std::string decryptedData = toHex(walletRsa.decryptMessage(message.data.toStdString()));
            messageJson.insert("data", QString::fromStdString(decryptedData));
        }
        messageJson.insert("isDecrypter", message.isCanDecrypted);
        messageJson.insert("counter", QString::fromStdString(std::to_string(message.counter)));
        messageJson.insert("fee", QString::fromStdString(std::to_string(message.fee)));
        messageJson.insert("isConfirmed", message.isConfirmed);
        messagesArrJson.push_back(messageJson);
    }
    return QJsonDocument(messagesArrJson);
}

static QJsonDocument allPosToJson(const std::vector<std::pair<QString, Message::Counter>> &pos) {
    QJsonArray messagesArrJson;
    for (const auto &pair: pos) {
        QJsonObject messageJson;
        messageJson.insert("address", pair.first);
        messageJson.insert("counter", pair.second);
        messagesArrJson.push_back(messageJson);
    }
    return QJsonDocument(messagesArrJson);
}

void MessengerJavascript::getHistoryAddress(QString address, QString from, QString to) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgGetHistoryAddressJs";

    LOG << "get messages " << address << " " << from << " " << to;

    const Message::Counter fromC = std::stoll(from.toStdString());
    const Message::Counter toC = std::stoll(to.toStdString());
    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getHistoryAddress(address, fromC, toC, [this, JS_NAME_RESULT, address](const std::vector<Message> &messages, const TypedException &exception) {
            Opt<QJsonDocument> result;
            const TypedException exception2 = apiVrapper2([&, this](){
                if (exception.isSet()) {
                    throw exception;
                }

                LOG << "Count messages " << address << " " << messages.size();
                result = messagesToJson(messages, walletManager.getWalletRsa(address.toStdString()));
            });
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception2, Opt<QString>(address), result);
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QJsonDocument>());
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getHistoryAddressAddress(QString address, QString collocutor, QString from, QString to) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgGetHistoryAddressAddressJs";

    LOG << "get messages " << address << " " << collocutor << " " << from << " " << to;

    const Message::Counter fromC = std::stoll(from.toStdString());
    const Message::Counter toC = std::stoll(to.toStdString());
    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getHistoryAddressAddress(address, collocutor, fromC, toC, [this, JS_NAME_RESULT, address, collocutor](const std::vector<Message> &messages, const TypedException &exception) {
            Opt<QJsonDocument> result;
            const TypedException exception2 = apiVrapper2([&, this](){
                if (exception.isSet()) {
                    throw exception;
                }

                LOG << "Count messages " << address << " " << collocutor << " " << messages.size();
                result = messagesToJson(messages, walletManager.getWalletRsa(address.toStdString()));
            });
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception2, Opt<QString>(address), Opt<QString>(collocutor), result);
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<QJsonDocument>());
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getHistoryAddressAddressCount(QString address, QString collocutor, QString count, QString to) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgGetHistoryAddressAddressCountJs";

    LOG << "get messagesC " << address << " " << collocutor << " " << count << " " << to;

    const Message::Counter countC = std::stoll(count.toStdString());
    const Message::Counter toC = std::stoll(to.toStdString());
    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getHistoryAddressAddressCount(address, collocutor, countC, toC, [this, JS_NAME_RESULT, address, collocutor](const std::vector<Message> &messages, const TypedException &exception) {
            Opt<QJsonDocument> result;
            const TypedException exception2 = apiVrapper2([&, this](){
                if (exception.isSet()) {
                    throw exception;
                }

                LOG << "Count messagesC " << address << " " << collocutor << " " << messages.size();
                result = messagesToJson(messages, walletManager.getWalletRsa(address.toStdString()));
            });
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception2, Opt<QString>(address), Opt<QString>(collocutor), result);
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<QJsonDocument>());
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::registerAddress(bool isForcibly, QString address, QString feeStr) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgAddressAppendToMessengerJs";

    LOG << "registerAddress " << address;

    const uint64_t fee = std::stoull(feeStr.toStdString());
    const TypedException exception = apiVrapper2([&, this](){
        const QString pubkeyRsa = QString::fromStdString(walletManager.getWalletRsa(address.toStdString()).getPublikKey());
        const QString messageToSign = Messenger::makeTextForSignRegisterRequest(address, pubkeyRsa, fee);
        std::string pubkey;
        const std::string &sign = walletManager.getWallet(address.toStdString()).sign(messageToSign.toStdString(), pubkey);
        emit messenger->registerAddress(isForcibly, address, pubkeyRsa, QString::fromStdString(pubkey), QString::fromStdString(sign), fee, [this, JS_NAME_RESULT, address, isForcibly](bool isNew, const TypedException &exception) {
            const TypedException exception2 = apiVrapper2([&, this](){
                if (exception.isSet() && !isForcibly) {
                    throw exception;
                }

                if (isNew || isForcibly) {
                    const std::vector<QString> messagesForSign = Messenger::stringsForSign();
                    std::vector<QString> result;
                    for (const QString &msg: messagesForSign) {
                        std::string tmp;
                        const std::string sign = walletManager.getWallet(address.toStdString()).sign(msg.toStdString(), tmp);
                        result.emplace_back(QString::fromStdString(sign));
                    }
                    emit messenger->signedStrings(address, result, [this, JS_NAME_RESULT, address](const TypedException &exception){
                        LOG << "Address registered " << address;
                        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>("Ok"));
                    });
                }
            });
            if (exception2.isSet()) {
                makeAndRunJsFuncParams(JS_NAME_RESULT, exception2, Opt<QString>("Not ok"));
            }
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>("Not ok"));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::savePublicKeyCollocutor(bool isForcibly, QString address, QString collocutor) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgPublicKeyCollocutorGettedJs";

    LOG << "savePublicKeyCollocutor " << address << " " << collocutor;

    const TypedException exception = apiVrapper2([&, this](){
        const QString messageToSign = Messenger::makeTextForGetPubkeyRequest(collocutor);
        std::string pubkey;
        const std::string &sign = walletManager.getWallet(address.toStdString()).sign(messageToSign.toStdString(), pubkey);

        emit messenger->savePubkeyAddress(isForcibly, collocutor, QString::fromStdString(pubkey), QString::fromStdString(sign), [this, JS_NAME_RESULT, address, collocutor](bool /*isNew*/, const TypedException &exception) {
            LOG << "Pubkey saved " << collocutor;
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor));
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::sendMessage(QString address, QString collocutor, QString dataHex, QString timestampStr, QString feeStr) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgMessageSendedJs";

    LOG << "sendMessage " << " " << address << " " << collocutor << " " << timestampStr << " " << feeStr;

    const TypedException exception = apiVrapper2([&, this](){
        const uint64_t fee = std::stoull(feeStr.toStdString());
        const uint64_t timestamp = std::stoull(timestampStr.toStdString());
        emit messenger->getPubkeyAddress(collocutor, [this, JS_NAME_RESULT, address, collocutor, dataHex, fee, timestamp](const QString &pubkey, const TypedException &exception) {
            const TypedException exception2 = apiVrapper2([&, this](){
                if (exception.isSet()) {
                    throw exception;
                }

                const std::string data = fromHex(dataHex.toStdString());
                const WalletRsa walletRsa = WalletRsa::fromPublicKey(pubkey.toStdString());
                const QString encryptedDataToWss = QString::fromStdString(walletRsa.encrypt(data));

                const QString messageToSign = Messenger::makeTextForSendMessageRequest(collocutor, encryptedDataToWss, fee);
                std::string pub;
                const std::string &sign = walletManager.getWallet(address.toStdString()).sign(messageToSign.toStdString(), pub);

                const QString encryptedDataToBd = QString::fromStdString(walletManager.getWalletRsa(address.toStdString()).encrypt(data));
                emit messenger->sendMessage(address, collocutor, encryptedDataToWss, QString::fromStdString(pub), QString::fromStdString(sign), fee, timestamp, encryptedDataToBd, [this, JS_NAME_RESULT, address, collocutor](const TypedException &exception) {
                    LOG << "Message sended " << address << " " << collocutor;
                    makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor));
                });
            });
            if (exception2.isSet()) {
                makeAndRunJsFuncParams(JS_NAME_RESULT, exception2, Opt<QString>(address), Opt<QString>(collocutor));
            }
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getLastMessageNumber(QString address) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgLastMessegesJs";

    LOG << "getLastMessageNumber " << address;

    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getLastMessage(address, [this, JS_NAME_RESULT, address](const Message::Counter &pos, const TypedException &exception) {
            LOG << "Last message number " << address << " " << pos;
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<Message::Counter>(pos));
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<Message::Counter>(0));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getSavedPos(QString address, const QString &collocutor) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgSavedPosJs";

    LOG << "getSavedPos " << address << " " << collocutor;

    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getSavedPos(address, collocutor, [this, JS_NAME_RESULT, address, collocutor](const Message::Counter &pos, const TypedException &exception) {
            LOG << "Saved pos " << address << " " << collocutor << " " << pos;
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<Message::Counter>(pos));
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<Message::Counter>(0));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getSavedsPos(QString address) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgSavedsPosJs";

    LOG << "getSavedsPos " << address;

    const TypedException exception = apiVrapper2([&, this](){
        emit messenger->getSavedsPos(address, [this, JS_NAME_RESULT, address](const std::vector<std::pair<QString, Message::Counter>> &pos, const TypedException &exception) {
            const Opt<QJsonDocument> result(allPosToJson(pos));

            LOG << "Get saveds pos " << pos.size();
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), result);
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>());
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::savePos(QString address, const QString &collocutor, QString counterStr) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgStorePosJs";

    LOG << "savePos " << address << " " << collocutor << " " << counterStr;

    const TypedException exception = apiVrapper2([&, this](){
        const Message::Counter counter = std::stoll(counterStr.toStdString());
        emit messenger->savePos(address, collocutor, counter, [this, JS_NAME_RESULT, address, collocutor](const TypedException &exception){
            LOG << "Save pos ok " << address << " " << collocutor;
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<QString>("Ok"));
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<QString>("Not ok"));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::getCountMessages(QString address, const QString &collocutor, QString from) {
BEGIN_SLOT_WRAPPER
    CHECK(messenger != nullptr, "Messenger not set");

    const QString JS_NAME_RESULT = "msgCountMessagesJs";

    LOG << "getCountMessages " << address << " " << collocutor << " " << from;

    const TypedException exception = apiVrapper2([&, this](){
        const Message::Counter fromI = std::stoll(from.toStdString());
        emit messenger->getCountMessages(address, collocutor, fromI, [this, JS_NAME_RESULT, address, collocutor, fromI](const Message::Counter &count, const TypedException &exception) {
            LOG << "Count messages " << address << " " << collocutor << " " << fromI << " " << count;
            makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<Message::Counter>(count));
        });
    });

    if (exception.isSet()) {
        makeAndRunJsFuncParams(JS_NAME_RESULT, exception, Opt<QString>(address), Opt<QString>(collocutor), Opt<Message::Counter>(0));
    }
END_SLOT_WRAPPER
}

void MessengerJavascript::onNewMesseges(QString address, Message::Counter lastMessage) {
BEGIN_SLOT_WRAPPER
    const QString JS_NAME_RESULT = "msgNewMessegesJs";

    LOG << "New messages " << lastMessage;

    makeAndRunJsFuncParams(JS_NAME_RESULT, TypedException(), Opt<QString>(address), Opt<Message::Counter>(lastMessage));
END_SLOT_WRAPPER
}

void MessengerJavascript::setPaths(QString newPatch, QString /*newUserName*/) {
BEGIN_SLOT_WRAPPER
    const QString JS_NAME_RESULT = "setMessengerPathsJs";

    LOG << "Set messenger javascript path " << newPatch;

    Opt<QString> result;
    const TypedException exception = apiVrapper2([&, this]() {
        walletPath = makePath(newPatch, Wallet::WALLET_PATH_MTH);
        CHECK(!walletPath.isNull() && !walletPath.isEmpty(), "Incorrect path to wallet: empty");

        result = "Ok";
    });

    if (exception.isSet()) {
        result = "Not ok";
    }
    makeAndRunJsFuncParams(JS_NAME_RESULT, exception, result);
END_SLOT_WRAPPER
}

void MessengerJavascript::unlockWallet(QString address, QString password, QString passwordRsa, int timeSeconds) {
    LOG << "Unlock wallet " << address << " Wallet path " << walletPath;
    walletManager.unlockWallet(walletPath, address.toStdString(), password.toStdString(), passwordRsa.toStdString(), seconds(timeSeconds));
}

void MessengerJavascript::lockWallet() {
    LOG << "lock wallets";
    walletManager.lockWallet();
}

void MessengerJavascript::runJs(const QString &script) {
    LOG << "Javascript " << script;
    emit jsRunSig(script);
}