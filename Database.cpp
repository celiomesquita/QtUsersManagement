#include "Database.h"
#include <QSqlError>
#include <QDebug>

QSqlDatabase Database::db;

bool Database::initialize(const std::string& db_name) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(db_name));

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";

    return true;
}

// ensuring that only one database connection exists throughout the application
QSqlDatabase& Database::getDatabase() {
    return db;
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}
