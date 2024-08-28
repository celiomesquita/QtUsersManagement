/********************************************************************************
** Form generated from reading UI file 'UserManagementDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANAGEMENTDIALOG_H
#define UI_USERMANAGEMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManagementDialog
{
public:
    QLineEdit *usernameEdit;
    QLabel *label_2;
    QLineEdit *passwordEdit;
    QLabel *label;
    QCheckBox *adminCheckBox;
    QTableWidget *userTable;
    QCheckBox *showPasswordHashCheckBox;
    QPushButton *addOrUpdateButton;
    QLabel *statusLabel;

    void setupUi(QDialog *UserManagementDialog)
    {
        if (UserManagementDialog->objectName().isEmpty())
            UserManagementDialog->setObjectName(QString::fromUtf8("UserManagementDialog"));
        UserManagementDialog->resize(697, 721);
        usernameEdit = new QLineEdit(UserManagementDialog);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));
        usernameEdit->setGeometry(QRect(132, 90, 191, 30));
        QFont font;
        font.setPointSize(15);
        usernameEdit->setFont(font);
        label_2 = new QLabel(UserManagementDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 140, 101, 22));
        label_2->setFont(font);
        passwordEdit = new QLineEdit(UserManagementDialog);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setGeometry(QRect(132, 140, 191, 30));
        passwordEdit->setFont(font);
        label = new QLabel(UserManagementDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 90, 101, 22));
        label->setFont(font);
        adminCheckBox = new QCheckBox(UserManagementDialog);
        adminCheckBox->setObjectName(QString::fromUtf8("adminCheckBox"));
        adminCheckBox->setGeometry(QRect(130, 190, 109, 28));
        adminCheckBox->setFont(font);
        userTable = new QTableWidget(UserManagementDialog);
        userTable->setObjectName(QString::fromUtf8("userTable"));
        userTable->setGeometry(QRect(20, 270, 651, 431));
        showPasswordHashCheckBox = new QCheckBox(UserManagementDialog);
        showPasswordHashCheckBox->setObjectName(QString::fromUtf8("showPasswordHashCheckBox"));
        showPasswordHashCheckBox->setGeometry(QRect(490, 80, 181, 28));
        showPasswordHashCheckBox->setFont(font);
        addOrUpdateButton = new QPushButton(UserManagementDialog);
        addOrUpdateButton->setObjectName(QString::fromUtf8("addOrUpdateButton"));
        addOrUpdateButton->setGeometry(QRect(390, 150, 281, 71));
        addOrUpdateButton->setFont(font);
        statusLabel = new QLabel(UserManagementDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(280, 20, 391, 31));
        QFont font1;
        font1.setPointSize(16);
        statusLabel->setFont(font1);

        retranslateUi(UserManagementDialog);

        QMetaObject::connectSlotsByName(UserManagementDialog);
    } // setupUi

    void retranslateUi(QDialog *UserManagementDialog)
    {
        UserManagementDialog->setWindowTitle(QCoreApplication::translate("UserManagementDialog", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("UserManagementDialog", "Senha", nullptr));
        label->setText(QCoreApplication::translate("UserManagementDialog", "Usu\303\241rio", nullptr));
        adminCheckBox->setText(QCoreApplication::translate("UserManagementDialog", "Admin", nullptr));
        showPasswordHashCheckBox->setText(QCoreApplication::translate("UserManagementDialog", "Exibir o Hash", nullptr));
        addOrUpdateButton->setText(QCoreApplication::translate("UserManagementDialog", "Adicionar / Atualizar", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class UserManagementDialog: public Ui_UserManagementDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANAGEMENTDIALOG_H
