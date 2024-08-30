#include "AuthUtils.h"
#include "HashUtils.h"  // Assuming HashUtils contains the sha256 function
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool authenticateUser(QSqlDatabase& db, const QString& username, const QString& password, bool& isAdmin) {
    // Prepare the SQL query to retrieve user information
    QSqlQuery query(db);
    query.prepare("SELECT password, admin FROM users WHERE username = :username");
    query.bindValue(":username", username);

    bool authenticated = false;

    if (query.exec()) {
        if (query.next()) {
            // Retrieve the stored password hash and admin status from the database
            QString storedHash = query.value(0).toString();
            isAdmin = query.value(1).toBool();

            QString providedHash = sha256(password.trimmed());

            if (providedHash == storedHash) {
                authenticated = true;
            } else {
                qDebug() << "Password does not match.";
            }
        } else {
            qDebug() << "Username not found.";
        }
    } else {
        qCritical() << "Failed to execute query:" << query.lastError().text();
    }

    return authenticated;
}
