#ifndef USERDB_H
#define USERDB_H

#include <string>

bool AddUser(const std::string& username, const std::string& hashedPassword, int isAdmin);
bool UpdateUser(int id, const std::string& hashedPassword, int isAdmin, bool updatingPassword);
bool DeleteUser(int id);

#endif // USERDB_H
