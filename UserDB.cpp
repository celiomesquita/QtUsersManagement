#include "UserDB.h"
#include "Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool AddUser(const std::string& username, const std::string& hashedPassword, int isAdmin) {
    QSqlQuery query(Database::getDatabase());
    query.prepare("INSERT INTO users (username, password, admin) VALUES (:username, :password, :admin)");

    query.bindValue(":username", QString::fromStdString(username));
    query.bindValue(":password", QString::fromStdString(hashedPassword));
    query.bindValue(":admin", isAdmin);

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to add user:" << query.lastError().text();
        return false;
    }
}

bool UpdateUser(int id, const std::string& hashedPassword, int isAdmin, bool updatingPassword) {
    QSqlQuery query(Database::getDatabase());
    QString sql = "UPDATE users SET admin = :admin";

    if (updatingPassword) {
        sql += ", password = :password";
    }

    sql += " WHERE id = :id";
    query.prepare(sql);

    query.bindValue(":admin", isAdmin);
    query.bindValue(":id", id);

    if (updatingPassword) {
        query.bindValue(":password", QString::fromStdString(hashedPassword));
    }

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to update user:" << query.lastError().text();
        return false;
    }
}

bool DeleteUser(int id) {
    QSqlQuery query(Database::getDatabase());
    query.prepare("DELETE FROM users WHERE id = :id");

    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to delete user:" << query.lastError().text();
        return false;
    }
}
