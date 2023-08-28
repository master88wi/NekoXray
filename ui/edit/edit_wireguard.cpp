#include "edit_wireguard.h"
#include "ui_edit_wireguard.h"

#include "fmt/WireGuardBean.hpp"

#include <QUuid>

EditWireGuard::EditWireGuard(QWidget *parent) : QWidget(parent), ui(new Ui::EditWireGuard) {
    ui->setupUi(this);
}

EditWireGuard::~EditWireGuard() {
    delete ui;
}

void EditWireGuard::onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) {
    this->ent = _ent;
    auto bean = this->ent->WireGuardBean();

    ui->local_address->setText(bean->local_address);
    ui->private_key->setText(bean->private_key);
    ui->peer_public_key->setText(bean->peer_public_key);
    ui->pre_shared_key->setText(bean->pre_shared_key);
    ui->reserved->setText(bean->reserved);
    ui->wireguard_mtu->setValue(bean->wireguard_mtu);
    if (bean->wireguard_mtu <= 1000 || bean->wireguard_mtu > 65535) ui->wireguard_mtu->setValue(1408);
}

bool EditWireGuard::onEnd() {
    auto bean = this->ent->WireGuardBean();

    bean->local_address = ui->local_address->text();
    bean->private_key = ui->private_key->text();
    bean->peer_public_key = ui->peer_public_key->text();
    bean->pre_shared_key = ui->pre_shared_key->text();
    bean->reserved = ui->reserved->text();
    bean->wireguard_mtu = ui->wireguard_mtu->value();

    return true;
}
