#include "dialog_protocol_settings.h"
#include "ui_dialog_protocol_settings.h"

#include "fmt/Preset.hpp"

#include "main/GuiUtils.hpp"
#include "main/NekoGui.hpp"

DialogProtocolSettings::DialogProtocolSettings(QWidget* parent)
    : QDialog(parent), ui(new Ui::DialogProtocolSettings) {
    ui->setupUi(this);
    ADD_ASTERISK(this);

    using namespace NekoGui;
    auto ds = dataStore;

    if (!IS_NEKO_BOX) {
        ui->enabled_ech->hide();
        ui->global_padding->hide();
        ui->authenticated_length->hide();
        ui->packet_encoding_l->hide();
        ui->packet_encoding->hide();
    }

    // TLS
    ui->utlsFingerprint->addItems(IS_NEKO_BOX ? Preset::SingBox::UtlsFingerPrint : Preset::Xray::UtlsFingerPrint);
    ui->utlsFingerprint->setCurrentText(NekoGui::dataStore->utlsFingerprint);
    D_LOAD_BOOL(skip_cert)
    D_LOAD_BOOL(enabled_ech)

    // QUIC
    ui->up->setText(Int2String(ds->protocol_quic_up));
    ui->down->setText(Int2String(ds->protocol_quic_down));
    ui->hy2speed->setChecked(ds->protocol_quic_hy2_speed);

    // VMess
    ui->vmess_security->setCurrentText(ds->protocol_vmess_security);
    ui->packet_encoding->setCurrentText(ds->protocol_vmess_packet_encoding);
    ui->global_padding->setChecked(ds->protocol_vmess_global_padding);
    ui->authenticated_length->setChecked(ds->protocol_vmess_authenticated_length);
}

DialogProtocolSettings::~DialogProtocolSettings() {
    delete ui;
}

void DialogProtocolSettings::accept() {
    using namespace NekoGui;
    auto ds = dataStore;

    // TLS
    D_SAVE_BOOL(skip_cert)
    D_SAVE_BOOL(enabled_ech)
    NekoGui::dataStore->utlsFingerprint = ui->utlsFingerprint->currentText();

    // QUIC
    ds->protocol_quic_up = ui->up->text().toInt();
    ds->protocol_quic_down = ui->down->text().toInt();
    ds->protocol_quic_hy2_speed = ui->hy2speed->isChecked();

    // VMess
    ds->protocol_vmess_security = ui->vmess_security->currentText();
    ds->protocol_vmess_packet_encoding = ui->packet_encoding->currentText();
    ds->protocol_vmess_global_padding = ui->global_padding->isChecked();
    ds->protocol_vmess_authenticated_length = ui->authenticated_length->isChecked();

    QDialog::accept();
}
