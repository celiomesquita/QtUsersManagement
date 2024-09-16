#include "LoginDialog.h"
#include "MainWindow.h"
#include "Database.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (!Database::initialize(DB_PATH)) {
        qDebug() << "Failed to initialize the local SQLite database.";
        return -1;
    }

    // Show the login dialog
    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {
        QString loggedInUser = loginDialog.getUsername();
        bool isAdmin = loginDialog.isAdmin();

        MainWindow mainWindow(loggedInUser, isAdmin, nullptr);

        mainWindow.show();  // Show the main window
        return app.exec();   // Enter the event loop
    } else {
        qDebug() << "Login canceled or failed.";
        return 0;
    }
}
