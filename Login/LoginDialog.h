#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getUsername() const;
    bool isAdmin() const;

private slots:
    void onLoginBtnClicked();

private:
    Ui::LoginDialog *ui;  // UI elements managed by the Ui::LoginDialog class

    QString m_username;
    bool m_isAdmin;
};

#endif // LOGINDIALOG_H
