#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class UserManagementDialog;
}

class UserManagementDialog : public QDialog
{
    Q_OBJECT

public:
    enum class UserRole { Admin, User };

    explicit UserManagementDialog(QSqlDatabase& db, QString loggedInUser, UserRole role, QWidget* parent = nullptr);
    ~UserManagementDialog();

private slots:
    void onEditUser();
    void loadUsers();
    void resetFields();
    void onAddOrUpdateUser();
    void onDeleteUser();
    void onShowPasswordHashToggled(bool checked);

private:
    void initializeUserTable(); // Declare the method for initializing the table

    Ui::UserManagementDialog *ui;  // Declare the UI pointer
    QSqlDatabase& db;
    QString loggedInUser;
    UserRole role;
    QString userIDBeingEdited;
};

#endif // USERMANAGER_H
