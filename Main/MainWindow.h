#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString loggedInUser, bool isAdmin, QSqlDatabase& db, QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString loggedInUser;   // Store the logged-in user's name
    bool isAdmin;           // Store whether the user is an admin
    QSqlDatabase& db;       // Reference to the SQLite database
    QString projectName;
    QString projectConfig;

private slots:
    void onUsersManBtnClicked();   // Slot for handling user management button click
    void onProjManBtnClicked();    // Slot for handling project management button click
};

#endif // MAINWINDOW_H
