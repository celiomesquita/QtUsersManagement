#include "Login/LoginDialog.h"
#include "MainWindow.h"
#include "Database/Database.h"

#include <QSqlDatabase>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load the translation file
    QTranslator translator;
    QString locale = QLocale::system().name(); // Get system locale, e.g., "en_US" or "pt_BR"

    if (locale.startsWith("pt")) {
        translator.load(":/translations/translation_pt.qm"); // Portuguese translation
    } else {
        translator.load(":/translations/translation_en.qm"); // Default to English
    }
    app.installTranslator(&translator);

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
