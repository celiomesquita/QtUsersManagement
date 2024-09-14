#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../Project/project.h"
#include <QMainWindow>
#include <QSqlDatabase>

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
    void onUsersManBtnClicked();  // Slot to handle the button click
    void openProjectManager();

private:
    Ui::MainWindow *ui;
    QString loggedInUser;
    bool isAdmin;
    QSqlDatabase db;  // Assuming you have a database connection here
    ProjectManager *projectManager;
};

#endif // MAINWINDOW_H
