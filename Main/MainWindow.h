#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// #include "../Project/ProjectManager.h"
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
    void onUsersManBtnClicked();
    void onProjManBtnClicked();

private:
    Ui::MainWindow *ui;
    QString loggedInUser;
    bool isAdmin;
    QSqlDatabase& db; 
    // ProjectManager *projectManager;
};

#endif // MAINWINDOW_H
