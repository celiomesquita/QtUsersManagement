#ifndef AUTHUTILS_H
#define AUTHUTILS_H

#include <QString>
#include <QSqlDatabase>

bool authenticateUser(QSqlDatabase& db, const QString& username, const QString& password, bool& isAdmin);

#endif // AUTHUTILS_H
