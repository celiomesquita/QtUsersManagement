#include <QApplication>
#include <QDebug>
#include "../Database/Database.h"
#include "../Login/LoginDialog.h"
#include "MainWindow.h"
#include "ProjectManager.h"  // Include the ProjectManager header

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize SQLite for local database (Assuming DB_PATH is already defined)
    if (!Database::initialize(DB_PATH)) {
        qDebug() << "Failed to initialize the local SQLite database.";
        return -1;
    }

    // Show the login dialog
    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {
        QString loggedInUser = loginDialog.getUsername();
        bool isAdmin = loginDialog.isAdmin();

        // Pass the user info and the projects they have access to into the main window
        MainWindow mainWindow(loggedInUser, isAdmin);
        
        // Initialize ProjectManager and add it to the main window or show it directly
        ProjectManager projectManager;  // Create the ProjectManager widget
        projectManager.show();  // Show the project manager after login

        return app.exec();
    } else {
        qDebug() << "Login canceled or failed.";
        return 0;
    }
}
