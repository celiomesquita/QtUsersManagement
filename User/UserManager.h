#ifndef UserManager_H
#define UserManager_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class UserManager;
}

class UserManager : public QDialog
{
    Q_OBJECT

public:
    enum class UserRole { Admin, User };

    explicit UserManager(QSqlDatabase& db, QString loggedInUser, UserRole role, QWidget* parent = nullptr);
    ~UserManager();

private slots:
    void loadUsers();
    void resetFields();

    void onEditUser();
    void onAddOrUpdateUser();
    void onDeleteUser();
    void onShowPasswordHashToggled(bool checked);

    bool AddUser(const std::string& username, const std::string& hashedPassword, int isAdmin);
    bool UpdateUser(int id, const std::string& hashedPassword, int isAdmin, bool updatingPassword);
    bool DeleteUser(int id);

private:
    void initializeUserTable(); // Declare the method for initializing the table

    Ui::UserManager *ui;  // Declare the UI pointer
    QSqlDatabase& db;
    QString loggedInUser;
    UserRole role;
    QString userIDBeingEdited;
};

#endif // UserManager_H
