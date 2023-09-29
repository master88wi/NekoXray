#ifndef DIALOG_PROTOCOL_SETTINGS_H
#define DIALOG_PROTOCOL_SETTINGS_H

#include <QDialog>

namespace Ui {
    class DialogProtocolSettings;
}

class DialogProtocolSettings : public QDialog {
    Q_OBJECT

public:
    explicit DialogProtocolSettings(QWidget *parent = nullptr);

    ~DialogProtocolSettings();

public slots:

    void accept();

private:
    Ui::DialogProtocolSettings *ui;

    // private slots:
};

#endif // DIALOG_PROTOCOL_SETTINGS_H
