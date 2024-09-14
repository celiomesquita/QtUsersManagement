#include "MainWindow.h"
#include "../build/ui_MainWindow.h"
#include "../Management/UserManagementDialog.h"


MainWindow::MainWindow(QString loggedInUser, bool isAdmin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loggedInUser(loggedInUser),
    isAdmin(isAdmin),
    projectManager(nullptr)  
{
    ui->setupUi(this);

    QString adminStatus = isAdmin ? "Admin" : "User";
    ui->statusLabel->setText(loggedInUser + " (" + adminStatus + ")");

    connect(ui->UsersManBtn, &QPushButton::clicked, this, &MainWindow::onUsersManBtnClicked);

    connect(ui->openProjectButton, &QPushButton::clicked, this, &MainWindow::openProjectManager);

}

MainWindow::~MainWindow()
{
    delete ui;
    if (projectManager) {
        delete projectManager;
    }    
}

void MainWindow::onUsersManBtnClicked()
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

void MainWindow::openProjectManager()
{
    // Instantiate and show the ProjectManager when the button is clicked
    if (!projectManager) {
        projectManager = new ProjectManager(this);
    }
    projectManager->show();
}