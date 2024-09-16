#include "../build/ui_MainWindow.h"
#include "../Project/ProjectManager.h"
#include "../User/UserManager.h"
#include "MainWindow.h"

MainWindow::MainWindow(QString loggedInUser, bool isAdmin, QSqlDatabase& database, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loggedInUser(loggedInUser),
    isAdmin(isAdmin),
    db(database)  // Initialize the database reference
{
    ui->setupUi(this);

    QString adminStatus = isAdmin ? "Admin" : "User";
    ui->status_label->setText(loggedInUser + " (" + adminStatus + ")");

    // Connect button signals to their respective slots
    connect(ui->users_man_btn, &QPushButton::clicked, this, &MainWindow::onUsersManBtnClicked);
    connect(ui->proj_man_btn,  &QPushButton::clicked, this, &MainWindow::onProjManBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;  // Clean up the UI
}

void MainWindow::onUsersManBtnClicked()
{
    // Create UserManager dialog and execute it modally
    UserManager userManager(db, loggedInUser, isAdmin ? UserManager::UserRole::Admin : UserManager::UserRole::User, this);
    userManager.exec();  // Show the UserManager dialog modally
}

void MainWindow::onProjManBtnClicked()
{
    // Create ProjectManager dialog and execute it modally
    ProjectManager projectManager(db, projectName, projectConfig, this);
    projectManager.exec();  // Show the ProjectManager dialog modally
}
