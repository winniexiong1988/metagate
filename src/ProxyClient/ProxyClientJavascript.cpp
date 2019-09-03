#include "ProxyClientJavascript.h"

#include "ProxyClient.h"

#include "Log.h"
#include "check.h"

#include "qt_utilites/SlotWrapper.h"
#include "qt_utilites/QRegister.h"
#include "qt_utilites/WrapperJavascriptImpl.h"

#include <QJsonObject>
#include <QJsonArray>

namespace proxy_client {

ProxyClientJavascript::ProxyClientJavascript(ProxyClient &proxyClient)
    : WrapperJavascript(false, LOG_FILE)
    , proxyClient(proxyClient)
{

}

static QJsonDocument statusToJson(const ProxyStatus &status) {
    QJsonObject result;
    QString statusJson;
    if (status.status == ProxyStatus::Status::not_set) {
        statusJson = "not_set";
    } else if (status.status == ProxyStatus::Status::connect_to_server_error) {
        statusJson = "connect_to_server_error";
    } else {
        throwErr("Unknown status");
    }

    result.insert("status", statusJson);
    result.insert("description", status.description);

    return QJsonDocument(result);
}

void ProxyClientJavascript::refreshStatus(const QString &callback) {
BEGIN_SLOT_WRAPPER
    const auto makeFunc = makeJavascriptReturnAndErrorFuncs(callback, JsTypeReturn<QJsonDocument>(QJsonDocument()));

    wrapOperation([&, this](){
        emit proxyClient.refreshStatus(ProxyClient::RefreshStatusCallback([makeFunc](const ProxyStatus &status){
            LOG << "Status proxy: " << status.description;
            makeFunc.func(TypedException(), statusToJson(status));
        }, makeFunc.error, signalFunc));
    }, makeFunc.error);
END_SLOT_WRAPPER
}

} // namespace proxy_client