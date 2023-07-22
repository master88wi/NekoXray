#include "db/ProxyEntity.hpp"
#include "fmt/includes.h"

#define MAKE_SETTINGS_STREAM_SETTINGS                         \
    outbound["settings"] = settings;                          \
    auto streamSettings = stream->BuildStreamSettingsV2Ray(); \
    outbound["streamSettings"] = streamSettings;

namespace NekoGui_fmt {
    QJsonObject V2rayStreamSettings::BuildStreamSettingsV2Ray() {
        QJsonObject streamSettings{{"network", network}};

        if (network == "ws") {
            QJsonObject ws;
            if (!host.isEmpty()) ws["headers"] = QJsonObject{{"Host", host}};
            // ws path & ed
            if (!path.isEmpty()) ws["path"] = path;
            streamSettings["wsSettings"] = ws;
        } else if (network == "http") {
            QJsonObject http;
            if (!path.isEmpty()) http["path"] = path;
            if (!host.isEmpty()) http["host"] = QList2QJsonArray(host.split(","));
            streamSettings["httpSettings"] = http;
        } else if (network == "grpc") {
            QJsonObject grpc;
            if (!path.isEmpty()) grpc["serviceName"] = path;
            streamSettings["grpcSettings"] = grpc;
        } else if (network == "quic") {
            QJsonObject quic;
            if (!header_type.isEmpty()) quic["header"] = QJsonObject{{"type", header_type}};
            if (!path.isEmpty()) quic["key"] = path;
            if (!host.isEmpty()) quic["security"] = host;
            streamSettings["quicSettings"] = quic;
        } else if (network == "tcp" && !header_type.isEmpty()) {
            QJsonObject header{{"type", header_type}};
            if (header_type == "http") {
                header["request"] = QJsonObject{
                    {"path", QList2QJsonArray(path.split(","))},
                    {"headers", QJsonObject{{"Host", QList2QJsonArray(host.split(","))}}},
                };
            }
            streamSettings["tcpSettings"] = QJsonObject{{"header", header}};
        }

        if (security == "tls") {
            auto fp = utlsFingerprint.isEmpty() ? NekoGui::dataStore->utlsFingerprint : utlsFingerprint;
            QJsonObject tls;
            if (!fp.isEmpty()) tls["fingerprint"] = fp;
            if (!sni.trimmed().isEmpty()) tls["serverName"] = sni;
            if (reality_pbk.trimmed().isEmpty()) {
                if (allow_insecure || NekoGui::dataStore->skip_cert) tls["allowInsecure"] = true;
                if (!alpn.trimmed().isEmpty()) tls["alpn"] = QList2QJsonArray(alpn.split(","));
                if (!certificate.trimmed().isEmpty()) {
                    tls["disableSystemRoot"] = true;
                    tls["certificates"] = QJsonArray{
                        QJsonObject{
                            {"usage", "verify"},
                            {"certificate", QList2QJsonArray(SplitLines(certificate.trimmed()))},
                        },
                    };
                }
                streamSettings["tlsSettings"] = tls;
                streamSettings["security"] = "tls";
            } else {
                tls["publicKey"] = reality_pbk;
                tls["shortId"] = reality_sid;
                tls["spiderX"] = reality_spx;
                if (fp.isEmpty()) tls["fingerprint"] = "chrome";
                streamSettings["realitySettings"] = tls;
                streamSettings["security"] = "reality";
            }
        }

        return streamSettings;
    }

    CoreObjOutboundBuildResult SocksHttpBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;

        QJsonObject outbound;
        outbound["protocol"] = socks_http_type == type_HTTP ? "http" : "socks";

        QJsonObject settings;
        QJsonArray servers;
        QJsonObject server;

        server["address"] = serverAddress;
        server["port"] = serverPort;

        QJsonArray users;
        QJsonObject user;
        user["user"] = username;
        user["pass"] = password;
        users.push_back(user);
        if (!username.isEmpty() && !password.isEmpty()) server["users"] = users;

        servers.push_back(server);
        settings["servers"] = servers;

        MAKE_SETTINGS_STREAM_SETTINGS

        result.outbound = outbound;
        return result;
    }

    CoreObjOutboundBuildResult ShadowSocksBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;

        QJsonObject outbound{{"protocol", "shadowsocks"}};

        QJsonObject settings;
        QJsonArray servers;
        QJsonObject server;

        server["address"] = serverAddress;
        server["port"] = serverPort;
        server["method"] = method;
        server["password"] = password;

        if (uot != 0) {
            server["uot"] = true;
            server["UoTVersion"] = uot;
        } else {
            server["uot"] = false;
        }

        servers.push_back(server);
        settings["servers"] = servers;

        if (!plugin.trimmed().isEmpty()) {
            settings["plugin"] = SubStrBefore(plugin, ";");
            settings["pluginOpts"] = SubStrAfter(plugin, ";");
        }

        MAKE_SETTINGS_STREAM_SETTINGS

        result.outbound = outbound;
        return result;
    }

    CoreObjOutboundBuildResult VMessBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;
        QJsonObject outbound{{"protocol", "vmess"}};

        QJsonObject settings{
            {"vnext", QJsonArray{
                          QJsonObject{
                              {"address", serverAddress},
                              {"port", serverPort},
                              {"users", QJsonArray{
                                            QJsonObject{
                                                {"id", uuid.trimmed()},
                                                {"alterId", aid},
                                                {"security", security},
                                            }}},
                          }}}};

        MAKE_SETTINGS_STREAM_SETTINGS

        result.outbound = outbound;
        return result;
    }

    CoreObjOutboundBuildResult TrojanVLESSBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;
        QJsonObject outbound{
            {"protocol", proxy_type == proxy_VLESS ? "vless" : "trojan"},
        };

        QJsonObject settings;
        if (proxy_type == proxy_VLESS) {

            QString configFlow;
            if (flow.right(7) == "-udp443") {
                // 检查末尾是否包含"-udp443"，如果是，则保留
                configFlow = flow;
            } else if (flow == "none" || flow.isEmpty()) {
                // 不使用 flow
                configFlow = "";
            } else {
                // 其余情况加上 -udp443 以放行 QUIC 流量
                configFlow = flow + "-udp443";
            }
            
            settings = QJsonObject{
                {"vnext", QJsonArray{
                              QJsonObject{
                                  {"address", serverAddress},
                                  {"port", serverPort},
                                  {"users", QJsonArray{
                                                QJsonObject{
                                                    {"id", password.trimmed()},
                                                    {"encryption", encryption},
                                                    {"flow", configFlow},
                                                }}},
                              }}}};
        } else {
            settings = QJsonObject{
                {"servers", QJsonArray{
                                QJsonObject{
                                    {"address", serverAddress},
                                    {"port", serverPort},
                                    {"password", password},
                                }}}};
        }

        MAKE_SETTINGS_STREAM_SETTINGS

        result.outbound = outbound;
        return result;
    }

    CoreObjOutboundBuildResult WireGuardBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;
        QString endpoint = serverAddress + ":" + QString::number(serverPort);

        QJsonObject outbound{{"protocol", "wireguard"}};

        // reserved: string to int
        QJsonArray reservedJsonArray;
        if (!reserved.isEmpty()) {
            if (reserved.indexOf(',') != std::string::npos) {
                QStringList reservedList = reserved.split(",");
                QList<int> reservedIntList;

                foreach (const QString& str, reservedList) {
                    int number = str.toInt();
                    reservedIntList.append(number);
                }
                reservedJsonArray = QList2QJsonArray(reservedIntList);
            } else {
                QByteArray decoded = QByteArray::fromBase64(reserved.toUtf8());
                for (int i = 0; i < decoded.size(); i++) {
                    int value = static_cast<unsigned char>(decoded[i]);
                    reservedJsonArray.append(value);
                }
            }
        } else {
            reservedJsonArray = QJsonArray();
        }

        QJsonObject settings{
            {"address", QList2QJsonArray(local_address.split(","))},
            {"secretKey", private_key},
            {"peers", QJsonArray{
                          QJsonObject{
                              {"endpoint", endpoint},
                              {"publicKey", peer_public_key},
                              {"preSharedKey", pre_shared_key},
                          }}},
            {"mtu", wireguard_mtu},
            {"reserved", reservedJsonArray},
        };

        outbound["settings"] = settings;

        result.outbound = outbound;
        return result;
    }

    CoreObjOutboundBuildResult CustomBean::BuildCoreObjV2Ray() {
        CoreObjOutboundBuildResult result;

        if (core == "internal") {
            result.outbound = QString2QJsonObject(config_simple);
        }

        return result;
    }
} // namespace NekoGui_fmt