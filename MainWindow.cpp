#include "MainWindow.h"
#include "build/ui_MainWindow.h"
#include "UserManager.h"
#include "ProjectManager.h"

MainWindow::MainWindow(QString loggedInUser, bool isAdmin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loggedInUser(loggedInUser),
    isAdmin(isAdmin)    
{
    ui->setupUi(this);

    QString adminStatus = isAdmin ? "Admin" : "User";
    ui->statusLabel->setText(loggedInUser + " (" + adminStatus + ")");

    connect(ui->users_btn,    &QPushButton::clicked, this, &MainWindow::onusers_btnClicked);
    connect(ui->projects_btn, &QPushButton::clicked, this, &MainWindow::onprojects_btnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onusers_btnClicked()
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

void MainWindow::onprojects_btnClicked()
{
    ProjectManagementDialog* projectManagementDialog = nullptr;

    projectManagementDialog = new ProjectManagementDialog(db, this);

    projectManagementDialog->exec();

    delete projectManagementDialog;
}
