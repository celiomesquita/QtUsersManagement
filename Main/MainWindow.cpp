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

    connect(ui->UsersManBtn, &QPushButton::clicked, this, &MainWindow::onUsersManBtnClicked);
    connect(ui->ProjManBtn,  &QPushButton::clicked, this, &MainWindow::onProjManBtnClicked);
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
    UserManager* UserManager = nullptr;

    if (isAdmin) {
        UserManager = new UserManager(db, loggedInUser, UserManager::UserRole::Admin, this);
    } else {
        UserManager = new UserManager(db, loggedInUser, UserManager::UserRole::User, this);
    }

    UserManager->exec();  // Show the UserManager modally

    delete UserManager;  // Clean up the dialog after it is closed
}

void MainWindow::onProjManBtnClicked()
{
    ProjectManager* ProjectManager = nullptr;

    ProjectManager = new ProjectManager(db, this);

    ProjectManager->exec();  // Show the UserManager modally

    delete ProjectManager;  // Clean up the dialog after it is closed
}