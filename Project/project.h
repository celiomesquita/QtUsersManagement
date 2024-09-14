#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

namespace Ui {
class ProjectManager;
}

class ProjectManager : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManager(QWidget *parent = nullptr);
    ~ProjectManager();

    void loadProjects();  // Load projects from the database
    void addProject(const QString &name, const QJsonObject &config);  // Add a new project

private:
    Ui::ProjectManager *ui;
    QSqlDatabase db;  // SQLite database connection

    bool connectDatabase();  // Connect to SQLite database
};

#endif // PROJECTMANAGER_H
