#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTranslator>

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
    void onusers_btnClicked();  // Slot to handle the button click
    void onprojects_btnClicked();
    void switchToEnglish();
    void switchToPortuguese();

private:
    Ui::MainWindow *ui;
    QString loggedInUser;
    bool isAdmin;
    QSqlDatabase db;  // Assuming you have a database connection here
    QTranslator translator;
    void changeLanguage(const QString &language);
};

#endif // MAINWINDOW_H
