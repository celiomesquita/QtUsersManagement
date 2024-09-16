#include "../Login/LoginDialog.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize SQLite for local database (Assuming DB_PATH is already defined in CMake)
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_PATH);

    if (!db.open()) {
        qDebug() << "Failed to initialize the local SQLite database.";
        return -1;
    }

    // Show the login dialog
    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {
        QString loggedInUser = loginDialog.getUsername();
        bool isAdmin = loginDialog.isAdmin();

        // Pass the database, user info, and admin status into the main window
        MainWindow mainWindow(loggedInUser, isAdmin, db, nullptr);  // Pass db to MainWindow constructor

        mainWindow.show();  // Show the main window
        return app.exec();   // Enter the event loop
    } else {
        qDebug() << "Login canceled or failed.";
        return 0;
    }
}
