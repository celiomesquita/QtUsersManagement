#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database {
public:
    static bool initialize(const std::string& db_name);
    static QSqlDatabase& getDatabase();  // Add this method
    ~Database();

private:
    static QSqlDatabase db;
};

#endif // DATABASE_H
