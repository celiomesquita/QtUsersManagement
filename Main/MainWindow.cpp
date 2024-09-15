#include "../build/ui_MainWindow.h"
#include "../Project/ProjectManager.h"
#include "../User/UserManager.h"
#include "MainWindow.h"

MainWindow::MainWindow(QString loggedInUser, bool isAdmin, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loggedInUser(loggedInUser),
    isAdmin(isAdmin)  
{
    ui->setupUi(this);

    QString adminStatus = isAdmin ? "Admin" : "User";
    ui->statusLabel->setText(loggedInUser + " (" + adminStatus + ")");

    connect(ui->UsersMan_btn, &QPushButton::clicked, this, &MainWindow::onUsersMan_btnClicked);
    connect(ui->ProjMan_btn,  &QPushButton::clicked, this, &MainWindow::onProjMan_btnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (projectManager) {
        delete projectManager;
    } 
    if (userManager) {
        delete userManager;
    }   
}

void MainWindow::onUsersManBtnClicked()
{
    UserManager* userManager = nullptr;

    if (isAdmin) {
        userManager = new UserManager(db, loggedInUser, UserManager::UserRole::Admin, this);
    } else {
        userManager = new UserManager(db, loggedInUser, UserManager::UserRole::User, this);
    }

    userManager->exec();  // Show the UserManager modally

    delete userManager;  // Clean up the dialog after it is closed
}

void MainWindow::onProjManBtnClicked()
{
    ProjectManager* projectManager = nullptr;

    projectManager = new ProjectManager(db, this);

    projectManager->exec();

    delete projectManager;  // Clean up the dialog after it is closed
}