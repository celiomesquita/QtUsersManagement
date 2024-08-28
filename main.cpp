/*
main.cpp

sudo apt-get install qtwayland5

---For SQLite
sudo apt-get install libsqlite3-dev

---For password hashing
sudo apt-get install libssl-dev

sudo apt-get install libqt5sql5
sudo apt-get install g++
sudo apt-get install qtbase5-dev qt5-qmake

sudo apt-get install qttools5-dev-tools

make clean
make -j$(nproc)

*/
#include <QApplication>
#include <QDebug>
#include "Database.h"
#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (!Database::initialize("users.db")) {
        qDebug() << "Failed to initialize the database.";
        return -1;
    }

    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {

        QString loggedInUser = loginDialog.getUsername();
        bool isAdmin         = loginDialog.isAdmin();

        MainWindow mainWindow(loggedInUser, isAdmin);
        mainWindow.show();

        return app.exec();
    } else {
        qDebug() << "Login canceled or failed.";
        return 0;
    }
}
