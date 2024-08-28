#include "MainWindow.h"
#include "build/ui_MainWindow.h"
#include "UserManagementDialog.h"

MainWindow::MainWindow(QString loggedInUser, bool isAdmin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loggedInUser(loggedInUser),
    isAdmin(isAdmin)    
{
    ui->setupUi(this);

    QString adminStatus = isAdmin ? "Admin" : "User";
    ui->statusLabel->setText(loggedInUser + " (" + adminStatus + ")");

    connect(ui->pushButtonUsersMan, &QPushButton::clicked, this, &MainWindow::onPushButtonUsersManClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushButtonUsersManClicked()
{
    UserManagementDialog* userManagementDialog = nullptr;

    if (isAdmin) {
        userManagementDialog = new UserManagementDialog(db, loggedInUser, UserManagementDialog::UserRole::Admin, this);
    } else {
        userManagementDialog = new UserManagementDialog(db, loggedInUser, UserManagementDialog::UserRole::User, this);
    }

    userManagementDialog->exec();  // Show the UserManagementDialog modally

    delete userManagementDialog;  // Clean up the dialog after it is closed
}
