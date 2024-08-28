#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include "UserManagementDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString loggedInUser, bool isAdmin, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPushButtonUsersManClicked();  // Slot to handle the button click

private:
    Ui::MainWindow *ui;
    QString loggedInUser;
    bool isAdmin;
    QSqlDatabase db;  // Assuming you have a database connection here
};

#endif // MAINWINDOW_H
