#pragma once

#include "fmt/AbstractBean.hpp"

namespace NekoGui_fmt {
    class WireGuardBean : public AbstractBean {
    public:
        QString local_address = "172.16.0.2/32";
        QString private_key = "";
        QString peer_public_key = "";
        QString pre_shared_key = "";
        QString reserved = "";
        int wireguard_mtu = 1402;

        WireGuardBean() : AbstractBean(0) {
            _add(new configItem("local_address", &local_address, itemType::string));
            _add(new configItem("private_key", &private_key, itemType::string));
            _add(new configItem("peer_public_key", &peer_public_key, itemType::string));
            _add(new configItem("pre_shared_key", &pre_shared_key, itemType::string));
            _add(new configItem("reserved", &reserved, itemType::string));
            _add(new configItem("wireguard_mtu", &wireguard_mtu, itemType::integer));
        };

        CoreObjOutboundBuildResult BuildCoreObjV2Ray() override;

        CoreObjOutboundBuildResult BuildCoreObjSingBox() override;

        QString DisplayType() override { return "WireGuard"; };
    };
} // namespace NekoGui_fmt
