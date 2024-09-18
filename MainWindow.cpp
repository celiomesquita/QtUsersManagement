#include "MainWindow.h"
#include "build/ui_MainWindow.h"
#include "User/UserManager.h"
#include "Project/ProjectManager.h"

#include <QTranslator>
#include <QLocale>
#include <QApplication>

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
    connect(ui->btn_br,       &QPushButton::clicked, this, &MainWindow::switchToPortuguese);
    connect(ui->btn_uk,       &QPushButton::clicked, this, &MainWindow::switchToEnglish);
}

// Slot to switch to Portuguese
void MainWindow::switchToPortuguese() {
    changeLanguage(":/translations/translation_pt.qm");
}

// Slot to switch to English
void MainWindow::switchToEnglish() {
    changeLanguage(":/translations/translation_en.qm");
}

// Helper function to change language
void MainWindow::changeLanguage(const QString &language) {
    translator.load(language);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);  // Update the UI with the new language
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
