#ifndef JAVASCRIPTWRAPPER_H
#define JAVASCRIPTWRAPPER_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>
#include <QDir>

#include "TypedException.h"

#include "client.h"

class NsLookup;
class WebSocketClient;

template<bool isLastArg, typename... Args>
struct JsFunc;

class JavascriptWrapper : public QObject
{
    Q_OBJECT
public:
    explicit JavascriptWrapper(WebSocketClient &wssClient, NsLookup &nsLookup, QObject *parent = nullptr);

    void setWidget(QWidget *widget);

signals:

    void jsRunSig(QString jsString);

    void setHasNativeToolbarVariableSig();

    void setCommandLineTextSig(QString text);

    void setUserNameSig(QString name);

    void setMappingsSig(QString name);

    void lineEditReturnPressedSig(QString text);

public slots:

    Q_INVOKABLE void createWallet(QString requestId, QString password);

    Q_INVOKABLE QString getAllWalletsJson();

    Q_INVOKABLE QString getAllWalletsAndPathsJson();

    Q_INVOKABLE void checkAddress(QString requestId, QString address);

    Q_INVOKABLE void signMessage(QString requestId, QString keyName, QString text, QString password);

    Q_INVOKABLE void signMessageV2(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString dataHex);

    Q_INVOKABLE void signMessageDelegate(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString valueDelegate, bool isDelegate);

    Q_INVOKABLE void getOnePrivateKey(QString requestId, QString keyName, bool isCompact);

    void savePrivateKey(QString requestId, QString privateKey, QString password);

public slots:

    Q_INVOKABLE void createWalletMHC(QString requestId, QString password);

    Q_INVOKABLE QString getAllMHCWalletsJson();

    Q_INVOKABLE QString getAllMHCWalletsAndPathsJson();

    Q_INVOKABLE void signMessageMHC(QString requestId, QString keyName, QString text, QString password);

    Q_INVOKABLE void signMessageMHCV2(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString dataHex);

    Q_INVOKABLE void signMessageMHCDelegate(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString valueDelegate, bool isDelegate);

    Q_INVOKABLE void getOnePrivateKeyMHC(QString requestId, QString keyName, bool isCompact);

    void savePrivateKeyMHC(QString requestId, QString privateKey, QString password);

public slots:

    Q_INVOKABLE void createRsaKey(QString requestId, QString address, QString password);

    Q_INVOKABLE void getRsaPublicKey(QString requestId, QString address);

    Q_INVOKABLE void encryptMessage(QString requestId, QString publicKey, QString message);

    Q_INVOKABLE void decryptMessage(QString requestId, QString addr, QString password, QString encryptedMessageHex);

public slots:

    Q_INVOKABLE void createWalletEth(QString requestId, QString password);

    Q_INVOKABLE void signMessageEth(QString requestId, QString address, QString password, QString nonce, QString gasPrice, QString gasLimit, QString to, QString value, QString data);

    Q_INVOKABLE void checkAddressEth(QString requestId, QString address);

    //Q_INVOKABLE void signMessageTokensEth(QString requestId, QString address, QString password, QString nonce, QString gasPrice, QString gasLimit, QString contractAddress, QString to, QString value);

    Q_INVOKABLE QString getAllEthWalletsJson();

    Q_INVOKABLE QString getAllEthWalletsAndPathsJson();

    Q_INVOKABLE void getOnePrivateKeyEth(QString requestId, QString keyName);

    void savePrivateKeyEth(QString requestId, QString privateKey, QString password);

public slots:

    Q_INVOKABLE void createWalletBtc(QString requestId);

    Q_INVOKABLE void createWalletBtcPswd(QString requestId, QString password);

    Q_INVOKABLE void checkAddressBtc(QString requestId, QString address);

    Q_INVOKABLE void signMessageBtc(QString requestId, QString address, QString jsonInputs, QString toAddress, QString value, QString estimateComissionInSatoshi, QString fees);

    Q_INVOKABLE void signMessageBtcPswd(QString requestId, QString address, QString password, QString jsonInputs, QString toAddress, QString value, QString estimateComissionInSatoshi, QString fees);

    Q_INVOKABLE void signMessageBtcPswdUsedUtxos(QString requestId, QString address, QString password, QString jsonInputs, QString toAddress, QString value, QString estimateComissionInSatoshi, QString fees, QString jsonUsedUtxos);

    Q_INVOKABLE QString getAllBtcWalletsJson();

    Q_INVOKABLE QString getAllBtcWalletsAndPathsJson();

    Q_INVOKABLE void getOnePrivateKeyBtc(QString requestId, QString keyName);

    void savePrivateKeyBtc(QString requestId, QString privateKey, QString password);

public slots:

    Q_INVOKABLE void savePrivateKeyAny(QString requestId, QString privateKey, QString password);

public slots:

    Q_INVOKABLE bool migrateKeysToPath(QString newPath);

    Q_INVOKABLE void updateAndReloadApplication();

    Q_INVOKABLE void qtOpenInBrowser(QString url);

    Q_INVOKABLE void getWalletFolders();

    Q_INVOKABLE void setPaths(QString newPatch, QString newUserName);

    Q_INVOKABLE QString openFolderDialog(QString beginPath, QString caption);

    Q_INVOKABLE void exitApplication();

    Q_INVOKABLE void restartBrowser();

    Q_INVOKABLE QString backupKeys(QString caption);

    Q_INVOKABLE QString restoreKeys(QString caption);

    Q_INVOKABLE void getMachineUid();

    Q_INVOKABLE void setUserName(const QString &userName);

    Q_INVOKABLE void setHasNativeToolbarVariable();

    Q_INVOKABLE void lineEditReturnPressed(QString text);

    Q_INVOKABLE void setCommandLineText(const QString &text);

    Q_INVOKABLE void openWalletPathInStandartExplorer();

    Q_INVOKABLE void setPagesMapping(QString mapping);

    Q_INVOKABLE void getIpsServers(QString requestId, QString type, int length, int count);

    Q_INVOKABLE void saveFileFromUrl(QString url, QString saveFileWindowCaption, QString fileName, bool openAfterSave);

    Q_INVOKABLE void printUrl(QString url, QString printWindowCaption, QString text);

    Q_INVOKABLE void chooseFileAndLoad(QString requestId, QString openFileWindowCaption, QString fileName);

    Q_INVOKABLE void getAppInfo(const QString requestId);

    Q_INVOKABLE void qrEncode(QString requestId, QString textHex);

    Q_INVOKABLE void qrDecode(QString requestId, QString pngBase64);

    Q_INVOKABLE void metaOnline();

private slots:

    void onCallbackCall(ReturnCallback callback);

    void onDirChanged(const QString &dir);

    void onWssMessageReceived(QString message);

private:

    void createWalletMTHS(QString requestId, QString password, QString walletPath, QString jsNameResult);

    void getOnePrivateKeyMTHS(QString requestId, QString keyName, bool isCompact, QString walletPath, QString jsNameResult, bool isTmh);

    void savePrivateKeyMTHS(QString requestId, QString privateKey, QString password, QString walletPath, QString jsNameResult);

    QString getAllMTHSWalletsJson(QString walletPath);

    QString getAllMTHSWalletsAndPathsJson(QString walletPath);

    void signMessageMTHS(QString requestId, QString keyName, QString text, QString password, QString walletPath, QString jsNameResult);

    void signMessageMTHS(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString dataHex, QString walletPath, QString jsNameResult);

    void signMessageDelegateMTHS(QString requestId, QString keyName, QString password, QString toAddress, QString value, QString fee, QString nonce, QString valueDelegate, bool isDelegate, QString walletPath, QString jsNameResult);

    template<class Function>
    TypedException apiVrapper2(const Function &func);

    template<typename... Args>
    void makeAndRunJsFuncParams(const QString &function, const QString &lastArg, const TypedException &exception, Args&& ...args);

    template<typename... Args>
    void makeAndRunJsFuncParams(const QString &function, const TypedException &exception, Args&& ...args);

    void runJs(const QString &script);

    void openFolderInStandartExplored(const QString &folder);

private:

    WebSocketClient &wssClient;

    NsLookup &nsLookup;

    QString hardwareId;

    QString walletDefaultPath;

    QString walletPath;

    QString walletPathMth;

    QString walletPathOldTmh;

    QString walletPathTmh;

    QString walletPathEth;

    QString walletPathBtc;

    QString userName;

    QWidget *widget_ = nullptr;

    SimpleClient client;

    struct FolderWalletInfo {
        QDir walletPath;
        QString nameWallet;

        FolderWalletInfo(const QDir &walletPath, const QString &nameWallet)
            : walletPath(walletPath)
            , nameWallet(nameWallet)
        {}
    };

    std::vector<FolderWalletInfo> folderWalletsInfos;

    QFileSystemWatcher fileSystemWatcher;

};

#endif // JAVASCRIPTWRAPPER_H
