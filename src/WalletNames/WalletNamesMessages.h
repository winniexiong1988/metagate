#ifndef WALLETNAMESMESSAGES_H
#define WALLETNAMESMESSAGES_H

#include <QString>
#include <QJsonDocument>

#include "WalletInfo.h"

namespace wallet_names {

QByteArray makeRenameMessage(const QString &address, const QString &name, size_t id, const QString &token, const QString &hwid);

QByteArray makeRenameMessageHttp(const QString &address, const QString &name, const QString &currency, size_t id, const QString &token, const QString &hwid);

QByteArray makeSetWalletsMessage(const std::vector<WalletInfo> &infos, size_t id, const QString &token, const QString &hwid);

QByteArray makeGetWalletsMessage(size_t id, const QString &token, const QString &hwid);

QByteArray makeGetWalletsAppsMessage(size_t id, const QString &token, const QString &hwid);

QByteArray makeCreateWatchWalletMessage(size_t id, const QString &token, const QString &hwid, const QString &address, bool isMhc);

QByteArray makeRemoveWatchWalletMessage(size_t id, const QString &token, const QString &hwid, const QString &address, bool isMhc);

enum METHOD: int {
    RENAME = 0, SET_WALLETS = 1, GET_WALLETS = 2,
    NOT_SET = 1000,
    ALIEN = 1001
};

struct ResponseType {
    METHOD method = METHOD::NOT_SET;

    enum class ERROR_TYPE {
        NO_ERROR, OTHER
    };

    bool isError = false;
    QString error;
    ERROR_TYPE errorType = ERROR_TYPE::NO_ERROR;
    size_t id = -1;
};

ResponseType getMethodAndAddressResponse(const QJsonDocument &response);

std::vector<WalletInfo> parseGetWalletsMessage(const QJsonDocument &response);

std::vector<WalletInfo> parseAddressListResponse(const QString &response);

} // namespace wallet_names

#endif // WALLETNAMESMESSAGES_H
