#pragma once

#include <QWidget>
#include "profile_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EditWireGuard;
}
QT_END_NAMESPACE

class EditWireGuard : public QWidget, public ProfileEditor {
    Q_OBJECT

public:
    explicit EditWireGuard(QWidget *parent = nullptr);

    ~EditWireGuard() override;

    void onStart(std::shared_ptr<NekoGui::ProxyEntity> _ent) override;

    bool onEnd() override;

private:
    Ui::EditWireGuard *ui;
    std::shared_ptr<NekoGui::ProxyEntity> ent;
};
