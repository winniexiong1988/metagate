#include "MessengerMessages.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "check.h"
#include "Log.h"

SET_LOG_NAMESPACE("MSG");

namespace messenger {

const static QString MSG_GET_MY_REQUEST = "msg_get_my";
const static QString MSG_GET_CHANNEL_REQUEST = "msg_get_channel";
const static QString MSG_GET_CHANNELS_REQUEST = "msg_get_channels";
const static QString MSG_GET_MY_CHANNELS_REQUEST = "msg_get_my_channels";
const static QString MSG_APPEND_KEY_ONLINE_REQUEST = "msg_append_key_online";

const static QString APPEND_KEY_TO_ADDR_RESPONSE = "msg_append_key_to_addr";
const static QString APPEND_KEY_TO_ADDR_BLOCKCHAIN_RESPONSE = "msg_append_key_to_addr_blockchain";
const static QString GET_KEY_BY_ADDR_RESPONSE = "msg_get_key_by_addr";
const static QString SEND_TO_ADDR_RESPONSE = "msg_send_to_addr";
const static QString NEW_MSGS_RESPONSE = "msg_get_my";
const static QString NEW_MSG_RESPONSE = "new_msg";
const static QString COUNT_MESSAGES_RESPONSE = "msg_append_key_online";

const static QString CHANNEL_CREATE_RESPONSE = "msg_channel_create";
const static QString CHANNEL_ADD_WRITER_RESPONSE = "msg_channel_add_writer";
const static QString CHANNEL_DEL_WRITER_RESPONSE = "msg_channel_del_writer";
const static QString SEND_TO_CHANNEL_RESPONSE = "msg_send_to_channel";
const static QString GET_CHANNEL_RESPONSE = "msg_get_channel";
const static QString GET_CHANNELS_RESPONSE = "msg_get_channels";
const static QString GET_MY_CHANNELS_RESPONSE = "msg_get_my_channels";
const static QString ADD_TO_CHANNEL_RESPONSE = "add_to_channel";
const static QString DEL_FROM_CHANNEL_RESPONSE = "del_from_channel";

const static QString ADD_ALL_WALLETS_REQUEST = "msg_append_key_online_list";
const static QString ADD_ALL_WALLETS_RESPONSE = "msg_append_key_online_list";
const static QString WANT_TO_TALK_REQUEST = "msg_send_request";
const static QString WANT_TO_TALK_RESPONSE = "msg_send_request";
const static QString REQUIRES_PUBKEY_RESPONSE = "msg_request_pubkey";
const static QString COLLOCUTOR_ADDED_PUBKEY_RESPONSE = "msg_new_pubkey";

QString makeTextForSignRegisterRequest(const QString &address, const QString &rsaPubkeyHex, uint64_t fee) {
    return address + QString::number(fee) + rsaPubkeyHex;
}

QString makeTextForSignRegisterBlockchainRequest(const QString &address, uint64_t fee, const QString &txHash, const QString &blockchain, const QString &blockchainName) {
    return address + QString::number(fee) + txHash + blockchain + blockchainName;
}

QString makeTextForGetPubkeyRequest(const QString &address) {
    return address;
}

QString makeTextForSendMessageRequest(const QString &address, const QString &dataHex, const QString &encryptedSelfDataHex, uint64_t fee, uint64_t timestamp) {
    return address + QString::number(fee) + dataHex + encryptedSelfDataHex + QString::number(timestamp);
}

QString makeTextForGetMyMessagesRequest() {
    return MSG_GET_MY_REQUEST;
}

QString makeTextForChannelCreateRequest(const QString &title, const QString titleSha, uint64_t fee) {
    return title + QString::number(fee) + titleSha;
}

QString makeTextForChannelAddWriterRequest(const QString &titleSha, const QString &address) {
    return "add" + titleSha + address;
}

QString makeTextForChannelDelWriterRequest(const QString &titleSha, const QString &address) {
    return "del" + titleSha + address;
}

QString makeTextForSendToChannelRequest(const QString &titleSha, const QString &text, uint64_t fee, uint64_t timestamp) {
    return QString::number(fee) + titleSha + text + QString::number(timestamp);
}

QString makeTextForGetChannelRequest() {
    return MSG_GET_CHANNEL_REQUEST;
}

QString makeTextForGetChannelsRequest() {
    return MSG_GET_CHANNELS_REQUEST;
}

QString makeTextForGetMyChannelsRequest() {
    return MSG_GET_MY_CHANNELS_REQUEST;
}

QString makeTextForMsgAppendKeyOnlineRequest() {
    return MSG_APPEND_KEY_ONLINE_REQUEST;
}

QString makeTextForWantTalkRequest(const QString &address) {
    return WANT_TO_TALK_REQUEST + address;
}

QString makeRegisterRequest(const QString &rsaPubkeyHex, const QString &pubkeyAddressHex, const QString &signHex, uint64_t fee, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_append_key_to_addr");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("fee", QString::number(fee));
    params.insert("rsa_pub_key", rsaPubkeyHex);
    params.insert("pubkey", pubkeyAddressHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeRegisterBlockchainRequest(const QString &pubkeyAddressHex, const QString &signHex, uint64_t fee, const QString &txHash, const QString &blockchain, const QString &blockchainName, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_append_key_to_addr_blockchain");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("fee", QString::number(fee));
    params.insert("pubkey", pubkeyAddressHex);
    params.insert("sign", signHex);
    params.insert("tx_hash", txHash);
    params.insert("blockchain", blockchain);
    params.insert("blockchain_name", blockchainName);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeGetPubkeyRequest(const QString &address, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_get_key_by_addr");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("addr", address);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeSendMessageRequest(const QString &toAddress, const QString &dataHex, const QString &encryptedSelfDataHex, const QString &pubkeyHex, const QString &signHex, uint64_t fee, uint64_t timestamp, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_send_to_addr");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("to", toAddress);
    params.insert("data", dataHex);
    params.insert("dataSender", encryptedSelfDataHex);
    params.insert("fee", QString::number(fee));
    params.insert("timestamp", QString::number(timestamp));
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeGetMyMessagesRequest(const QString &pubkeyHex, const QString &signHex, Message::Counter from, Message::Counter to, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", MSG_GET_MY_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("cnt_from", QString::number(from));
    if (to != -1)
        params.insert("cnt_to", QString::number(to));
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeCreateChannelRequest(const QString &title, const QString &titleSha, uint64_t fee, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_channel_create");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("fee", QString::number(fee));
    params.insert("title", title);
    params.insert("title_sha256", titleSha);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeChannelAddWriterRequest(const QString &titleSha, const QString &address, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_channel_add_writer");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("addr", address);
    params.insert("title_sha256", titleSha);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeChannelDelWriterRequest(const QString &titleSha, const QString &address, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_channel_del_writer");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("addr", address);
    params.insert("title_sha256", titleSha);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeSendToChannelRequest(const QString &titleSha, const QString &dataHex, uint64_t fee, uint64_t timestamp, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", "msg_send_to_channel");
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("fee", QString::number(fee));
    params.insert("timestamp", QString::number(timestamp));
    params.insert("title_sha256", titleSha);
    params.insert("data", dataHex);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeGetChannelRequest(const QString &titleSha, Message::Counter from, Message::Counter to, const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", MSG_GET_CHANNEL_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("cnt_from", QString::number(from));
    params.insert("cnt_to", QString::number(to));
    params.insert("title_sha256", titleSha);
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeGetChannelsRequest(const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", MSG_GET_CHANNELS_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeGetMyChannelsRequest(const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", MSG_GET_MY_CHANNELS_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeAppendKeyOnlineRequest(const QString &pubkeyHex, const QString &signHex, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", MSG_APPEND_KEY_ONLINE_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("pubkey", pubkeyHex);
    params.insert("sign", signHex);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeAddAllKeysRequest(const std::vector<QString> &wallets, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", ADD_ALL_WALLETS_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonArray addresses;
    for (const QString &wallet: wallets) {
        addresses.push_back(wallet);
    }
    QJsonObject params;
    params.insert("address", addresses);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

QString makeWantToTalkRequest(const QString &toAddress, const QString &pubkey, const QString &sign, size_t id) {
    QJsonObject json;
    json.insert("jsonrpc", "2.0");
    json.insert("method", WANT_TO_TALK_REQUEST);
    json.insert("request_id", QString::number(id));
    QJsonObject params;
    params.insert("addr", toAddress);
    params.insert("pubkey", pubkey);
    params.insert("sign", sign);
    json.insert("params", params);
    return QJsonDocument(json).toJson(QJsonDocument::Compact);
}

ResponseType getMethodAndAddressResponse(const QJsonDocument &response) {
    ResponseType result;
    QString type;

    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    /*CHECK(root.contains("result") && root.value("result").isString(), "data field not found");
    const QString res = root.value("result").toString();
    CHECK(res == "ok", "result != ok"); // ok даже в случае ошибки*/
    if (root.contains("method") && root.value("method").isString()) {
        type = root.value("method").toString();
    }
    if (root.contains("addr") && root.value("addr").isString()) {
        result.address = root.value("addr").toString();
    }
    if (root.contains("error") && root.value("error").isString()) {
        result.error = root.value("error").toString();
        result.isError = true;
        if (result.error == "KEY_EXIST" || result.error == "RSA_PUB_KEY_EXIST") {
            result.errorType = ResponseType::ERROR_TYPE::ADDRESS_EXIST;
        } else if (result.error == "INVALID_SIGN" || result.error == "INVALID_ADDR_OR_PUBKEY") {
            result.errorType = ResponseType::ERROR_TYPE::SIGN_OR_ADDRESS_INVALID;
        } else if (result.error == "NO_REQ_FIELD" || result.error == "UNSUPPORTED_METHOD" || result.error == "NO_METHOD_OR_PARAMS" || result.error == "JSON_PARSE_ERROR") {
            result.errorType = ResponseType::ERROR_TYPE::INCORRECT_JSON;
        } else if (result.error == "NOT_FOUND" || result.error == "ADDR_NOT_FOUND") {
            result.errorType = ResponseType::ERROR_TYPE::ADDRESS_NOT_FOUND;
        } else if (result.error == "TITLE_IS_BUSY") {
            result.errorType = ResponseType::ERROR_TYPE::CHANNEL_EXIST;
        } else if (result.error == "NO_PERMISSION") {
            result.errorType = ResponseType::ERROR_TYPE::CHANNEL_NOT_PERMISSION;
        } else if (result.error == "CHANNEL_NOT_FOUND") {
            result.errorType = ResponseType::ERROR_TYPE::CHANNEL_NOT_FOUND;
        } else if (result.error == "INVALID_ADDR") {
            result.errorType = ResponseType::ERROR_TYPE::INVALID_ADDRESS;
        } else {
            result.errorType = ResponseType::ERROR_TYPE::OTHER;
        }
        if (root.contains("data")) {
            QJsonObject obj;
            obj.insert("data", root.value("data"));
            result.error += ". " + QJsonDocument(obj).toJson(QJsonDocument::JsonFormat::Compact);
        }
    }
    if (root.contains("request_id") && root.value("request_id").isString()) {
        result.id = std::stoull(root.value("request_id").toString().toStdString());
    }

    if (type == APPEND_KEY_TO_ADDR_RESPONSE || type == APPEND_KEY_TO_ADDR_BLOCKCHAIN_RESPONSE) {
        result.method = METHOD::APPEND_KEY_TO_ADDR;
    } else if (type == GET_KEY_BY_ADDR_RESPONSE) {
        result.method = METHOD::GET_KEY_BY_ADDR;
    } else if (type == SEND_TO_ADDR_RESPONSE) {
        result.method = METHOD::SEND_TO_ADDR;
    } else if (type == NEW_MSGS_RESPONSE) {
        result.method = METHOD::NEW_MSGS;
    } else if (type == NEW_MSG_RESPONSE) {
        result.method = METHOD::NEW_MSG;
    } else if (type == COUNT_MESSAGES_RESPONSE) {
        result.method = METHOD::COUNT_MESSAGES;
    } else if (type == CHANNEL_CREATE_RESPONSE) {
        result.method = METHOD::CHANNEL_CREATE;
    } else if (type == CHANNEL_ADD_WRITER_RESPONSE) {
        result.method = METHOD::CHANNEL_ADD_WRITER;
    } else if (type == CHANNEL_DEL_WRITER_RESPONSE) {
        result.method = METHOD::CHANNEL_DEL_WRITER;
    } else if (type == SEND_TO_CHANNEL_RESPONSE) {
        result.method = METHOD::SEND_TO_CHANNEL;
    } else if (type == GET_CHANNEL_RESPONSE) {
        result.method = METHOD::GET_CHANNEL;
    } else if (type == GET_CHANNELS_RESPONSE) {
        result.method = METHOD::GET_CHANNELS;
    } else if (type == GET_MY_CHANNELS_RESPONSE) {
        result.method = METHOD::GET_MY_CHANNELS;
    } else if (type == ADD_TO_CHANNEL_RESPONSE) {
        result.method = METHOD::ADD_TO_CHANNEL;
    } else if (type == DEL_FROM_CHANNEL_RESPONSE) {
        result.method = METHOD::DEL_FROM_CHANNEL;
    } else if (type == ADD_ALL_WALLETS_RESPONSE) {
        result.method = METHOD::ALL_KEYS_ADDED;
    } else if (type == WANT_TO_TALK_RESPONSE) {
        result.method = METHOD::WANT_TO_TALK;
    } else if (type == REQUIRES_PUBKEY_RESPONSE) {
        result.method = METHOD::REQUIRES_PUBKEY;
    } else if (type == COLLOCUTOR_ADDED_PUBKEY_RESPONSE) {
        result.method = METHOD::COLLOCUTOR_ADDED_PUBKEY;
    } else if (type.isEmpty()) {
        // ignore
    } else {
        throwErr(("Incorrect response: " + type).toStdString());
    }
    return result;
}

static NewMessageResponse parseOneMessage(const QJsonObject &messageJson) {
    NewMessageResponse result;
    CHECK(messageJson.contains("from") && messageJson.value("from").isString(), "from field not found");
    result.collocutor = messageJson.value("from").toString();
    CHECK(messageJson.contains("data") && messageJson.value("data").isString(), "data field not found");
    result.data = messageJson.value("data").toString();
    CHECK(messageJson.contains("fee") && messageJson.value("fee").isString(), "fee field not found");
    result.fee = std::stoull(messageJson.value("fee").toString().toStdString());
    CHECK(messageJson.contains("cnt") && messageJson.value("cnt").isString(), "cnt field not found");
    result.counter = std::stoll(messageJson.value("cnt").toString().toStdString());
    CHECK(messageJson.contains("timestamp") && messageJson.value("timestamp").isString(), "timestamp field not found");
    result.timestamp = std::stoull(messageJson.value("timestamp").toString().toStdString());
    CHECK(messageJson.contains("type") && messageJson.value("type").isString(), "type field not found");
    const QString type = messageJson.value("type").toString();
    CHECK(type == "in" || type == "out" || type == "channel", "type field incorrect");
    if (type != "channel") {
        result.isInput = type == "in";
        result.isChannel = false;
    } else {
        CHECK(messageJson.contains("channel") && messageJson.value("channel").isString(), "channel field not found");
        result.channelName = messageJson.value("channel").toString();
        result.isChannel = true;
    }

    return result;
}

NewMessageResponse parseNewMessageResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("params") && root.value("params").isObject(), "params field not found");
    const QJsonObject data = root.value("params").toObject();

    return parseOneMessage(data);
}

std::vector<NewMessageResponse> parseNewMessagesResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("messages") && data.value("messages").isArray(), "messages field not found");
    const QJsonArray messages = data.value("messages").toArray();

    std::vector<NewMessageResponse> result;
    for (const QJsonValue &messageValue: messages) {
        const QJsonObject message = messageValue.toObject();
        result.emplace_back(parseOneMessage(message));
    }

    std::sort(result.begin(), result.end());
    return result;
}

std::vector<NewMessageResponse> parseGetChannelResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("messages") && data.value("messages").isArray(), "messages field not found");
    const QJsonArray messages = data.value("messages").toArray();

    std::vector<NewMessageResponse> result;
    for (const QJsonValue &messageValue: messages) {
        const QJsonObject message = messageValue.toObject();
        result.emplace_back(parseOneMessage(message));
    }

    std::sort(result.begin(), result.end());
    return result;
}

static ChannelInfo parseChannel(const QJsonObject &channelJson) {
    ChannelInfo info;
    CHECK(channelJson.contains("title") && channelJson.value("title").isString(), "title field not found");
    info.title = channelJson.value("title").toString();
    CHECK(channelJson.contains("title_sha256") && channelJson.value("title_sha256").isString(), "title_sha256 field not found");
    info.titleSha = channelJson.value("title_sha256").toString();
    CHECK(channelJson.contains("admin") && channelJson.value("admin").isString(), "admin field not found");
    info.admin = channelJson.value("admin").toString();
    CHECK(channelJson.contains("fee") && channelJson.value("fee").isString(), "fee field not found");
    info.fee = std::stoull(channelJson.value("fee").toString().toStdString());
    if (channelJson.contains("cnt_last") && channelJson.value("cnt_last").isString()) {
        info.counter = std::stoll(channelJson.value("cnt_last").toString().toStdString());
    }
    return info;
}

static std::vector<ChannelInfo> parseChannels(const QJsonArray &response) {
    std::vector<ChannelInfo> result;
    for (const QJsonValue &resp: response) {
        CHECK(resp.isObject(), "channels incorrect response");
        const QJsonObject &channelJson = resp.toObject();

        result.emplace_back(parseChannel(channelJson));
    }

    return result;
}

std::vector<ChannelInfo> parseGetChannelsResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("messages") && data.value("messages").isArray(), "messages field not found");
    const QJsonArray messages = data.value("messages").toArray();

    return parseChannels(messages);
}

std::vector<ChannelInfo> parseGetMyChannelsResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("messages") && data.value("messages").isArray(), "messages field not found");
    const QJsonArray messages = data.value("messages").toArray();

    return parseChannels(messages);
}

ChannelInfo parseAddToChannelResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("params") && root.value("params").isObject(), "params field not found");
    const QJsonObject data = root.value("params").toObject();

    return parseChannel(data);
}

ChannelInfo parseDelToChannelResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("params") && root.value("params").isObject(), "params field not found");
    const QJsonObject data = root.value("params").toObject();

    return parseChannel(data);
}

Message::Counter parseCountMessagesResponse(const QJsonDocument &response) {
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("cnt_last") && data.value("cnt_last").isString(), "cnt_last field not found");
    return std::stoll(data.value("cnt_last").toString().toStdString());
}

KeyMessageResponse parsePublicKeyMessageResponse(const QJsonDocument &response) {
    KeyMessageResponse result;
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("data") && root.value("data").isObject(), "data field not found");
    const QJsonObject data = root.value("data").toObject();
    CHECK(data.contains("rsa_pub_key") && data.value("rsa_pub_key").isString(), "rsa_pub_key field not found");
    result.publicKey = data.value("rsa_pub_key").toString();
    CHECK(data.contains("addr") && data.value("addr").isString(), "addr field not found");
    result.addr = data.value("addr").toString();
    CHECK(data.contains("fee") && data.value("fee").isString(), "fee field not found");
    result.fee = std::stoull(data.value("fee").toString().toStdString());
    CHECK(data.contains("tx_hash") && data.value("tx_hash").isString(), "tx_hash field not found");
    result.txHash = data.value("tx_hash").toString();
    CHECK(data.contains("blockchain_name") && data.value("blockchain_name").isString(), "blockchain_name field not found");
    result.blockchain_name = data.value("blockchain_name").toString();

    return result;
}

RequiresPubkeyResponse parseRequiresPubkeyResponse(const QJsonDocument &response) {
    RequiresPubkeyResponse result;
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("params") && root.value("params").isObject(), "params field not found");
    const QJsonObject data = root.value("params").toObject();
    CHECK(data.contains("from") && data.value("from").isString(), "from field not found");
    result.collocutor = data.value("from").toString();
    CHECK(data.contains("to") && data.value("to").isString(), "to field not found");
    result.address = data.value("to").toString();

    return result;
}

CollocutorAddedPubkeyResponse parseCollocutorAddedPubkeyResponse(const QJsonDocument &response) {
    CollocutorAddedPubkeyResponse result;
    CHECK(response.isObject(), "Response field not found");
    const QJsonObject root = response.object();
    CHECK(root.contains("params") && root.value("params").isObject(), "params field not found");
    const QJsonObject data = root.value("params").toObject();
    CHECK(data.contains("addr_req") && data.value("addr_req").isString(), "addr_req field not found");
    result.collocutor = data.value("addr_req").toString();
    CHECK(data.contains("addr") && data.value("addr").isString(), "addr field not found");
    result.address = data.value("addr").toString();
    CHECK(data.contains("rsa_pub_key") && data.value("rsa_pub_key").isString(), "rsa_pub_key field not found");
    result.pubkey = data.value("rsa_pub_key").toString();

    return result;
}

}
