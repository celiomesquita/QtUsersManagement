#include "ProjectDB.h"
#include "../Database/Database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool addProject(const std::string& name, const std::string& config) {
    QSqlQuery query(Database::getDatabase());
    query.prepare("INSERT INTO projects (name, config) VALUES (:name, :config)");

    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":config", QString::fromStdString(config));

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to add project:" << query.lastError().text(); // Correct error message
        return false;
    }
}

bool updateProject(int id, const std::string& name, const std::string& config) {
    QSqlQuery query(Database::getDatabase());
    QString sql = "UPDATE projects SET name=:name, config=:config WHERE id = :id";
    query.prepare(sql);

    // Convert std::string to QString
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":config", QString::fromStdString(config));
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to update project:" << query.lastError().text();
        return false;
    }
}

bool deleteProject(int id) {
    QSqlQuery query(Database::getDatabase());
    query.prepare("DELETE FROM projects WHERE id = :id");

    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to delete project:" << query.lastError().text();
        return false;
    }
}
