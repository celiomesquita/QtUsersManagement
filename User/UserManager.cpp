#include "UserManager.h"
#include "../Hash/HashUtils.h"
#include "../build/ui_UserManager.h"  // Include the generated UI header

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

// interface objects in snake_case
enum UserTableColumns {
    username_col,
    password_col,
    admin_col,
    edit_col,
    delete_col,
    ColumnCount // Keep track of the total number of columns
};

UserManager::UserManager(QSqlDatabase& db, QString loggedInUser, UserRole role, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::UserManager),
    db(db),
    loggedInUser(loggedInUser),
    role(role)  // Initialize the role field
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file
    initializeUserTable(); // Initialize the table

    QString adminStatus = (role == UserRole::Admin) ? "Admin" : "User";
    ui->status_label->setText(loggedInUser + " (" + adminStatus + ")");   

    setWindowTitle(tr("Gestão de usuários"));
    setModal(true);  // Blocks interaction with other windows until the user closes it

    if (role == UserRole::Admin){
        connect(ui->showPasswordHash_checkbox, &QCheckBox::toggled, this, &UserManager::onShowPasswordHashToggled);
    }

    connect(ui->add_update_btn, &QPushButton::clicked, this, &UserManager::onAddOrUpdateUser);    

    loadUsers();  // Load the initial list of users
}

UserManager::~UserManager()
{
    delete ui;  // Clean up the UI
}

void UserManager::initializeUserTable() {
    ui->user_table->setColumnCount(UserTableColumns::ColumnCount);
    ui->user_table->setHorizontalHeaderLabels(QStringList() << tr("Usuário") << tr("Senha") << tr("Admin") << tr("Editar") << tr("Excluir"));
    
    // Set column widths (adjust values as needed)
    ui->user_table->setColumnWidth(UserTableColumns::username_col, 118);
    ui->user_table->setColumnWidth(UserTableColumns::password_col, 118);
    ui->user_table->setColumnWidth(UserTableColumns::admin_col, 118);
    ui->user_table->setColumnWidth(UserTableColumns::edit_col, 118);
    ui->user_table->setColumnWidth(UserTableColumns::delete_col, 118);
}

void UserManager::onEditUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int userId = ui->user_table->item(row, UserTableColumns::username_col)->data(Qt::UserRole).toInt();  // Retrieve the user ID
    QString username = ui->user_table->item(row, UserTableColumns::username_col)->text();
    QString password = ""; // Do not load the password
    bool userIsAdmin = (ui->user_table->item(row, UserTableColumns::admin_col)->text() == tr("Sim"));

    ui->username_edit->setText(username);
    ui->password_edit->setText(password);
    ui->admin_check->setChecked(userIsAdmin);

    // only Admin can modify usernames and admin status
    ui->admin_check->setEnabled(role == UserRole::Admin);
    ui->username_edit->setEnabled(role == UserRole::Admin);

    userIDBeingEdited = QString::number(userId);  // Store the user ID as a string

    ui->add_update_btn->setText(tr("Salvar"));
}

void UserManager::loadUsers() {

    ui->user_table->setRowCount(0); // Clear existing rows

    QString queryString = (role == UserRole::Admin) ? "SELECT id, username, password, admin FROM users" : "SELECT id, username, admin FROM users";
    QSqlQuery query(db); 
    if (!query.exec(queryString)) {
        qCritical() << tr("Falha ao carregar usuários:") << query.lastError().text();
        return;
    }

    if (query.size() == 0) {
        qDebug() << tr("Nenhum usuário encontrado no banco de dados.");
        return;
    }

    while (query.next()) {
        int userId         = query.value(0).toInt();  // Retrieve the user ID
        QString dbUsername = query.value(1).toString();
        QString dbPassword = (role == UserRole::Admin) ? query.value(2).toString() : QString();
        bool dbAdmin       = query.value(3).toBool();

        int row = ui->user_table->rowCount();
        ui->user_table->insertRow(row);

        // Store the user ID in a custom property of the row
        QTableWidgetItem *usernameItem = new QTableWidgetItem(dbUsername);
        usernameItem->setData(Qt::UserRole, userId);  // Store user ID in the item
        ui->user_table->setItem(row, UserTableColumns::username_col, usernameItem);

        ui->user_table->setItem(row, UserTableColumns::password_col, new QTableWidgetItem(ui->showPasswordHashCheckBox->isChecked() ? dbPassword : "???"));
        ui->user_table->setItem(row, UserTableColumns::admin_col, new QTableWidgetItem(dbAdmin ? tr("Sim") : tr("Não")));

        if (role == UserRole::Admin){
            // Add Edit button
            QPushButton* edit_btn = new QPushButton(tr("Editar"), this);
            edit_btn->setProperty("row", row);
            connect(edit_btn, &QPushButton::clicked, this, &UserManager::onEditUser);
            ui->user_table->setCellWidget(row, UserTableColumns::edit_col, edit_btn);
        }else{
            if(dbUsername == loggedInUser){
                QPushButton* edit_btn = new QPushButton(tr("Editar"), this);
                edit_btn->setProperty("row", row);
                connect(edit_btn, &QPushButton::clicked, this, &UserManager::onEditUser);
                ui->user_table->setCellWidget(row, UserTableColumns::edit_col, edit_btn);            
            }else{
                QLabel* placeholderLabel = new QLabel("-", this);
                placeholderLabel->setAlignment(Qt::AlignCenter);
                ui->user_table->setCellWidget(row, UserTableColumns::edit_col, placeholderLabel);                  
            }
        }

        if (role == UserRole::Admin){
            if(dbUsername == loggedInUser){
                QLabel* placeholderLabel = new QLabel("-", this);
                placeholderLabel->setAlignment(Qt::AlignCenter);
                ui->user_table->setCellWidget(row, UserTableColumns::Delete_col, placeholder_label); 
            }else{
                // Add Delete button
                QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
                deleteButton->setProperty("row", row);
                connect(delete_btn, &QPushButton::clicked, this, &UserManager::onDeleteUser);
                ui->user_table->setCellWidget(row, UserTableColumns::delete_col, delete_btn);                
            }
        }else{
            QLabel* placeholderLabel = new QLabel("-", this);
            placeholderLabel->setAlignment(Qt::AlignCenter);
            ui->user_table->setCellWidget(row, UserTableColumns::delete_col, placeholder_label);
        }
    }
}

void UserManager::resetFields() {
    ui->username_edit->clear();
    ui->password_edit->clear();
    ui->admin_check->setChecked(false);
    userIDBeingEdited.clear();
    ui->add_update_btn->setText(tr("Adicionar usuário"));
}

void UserManager::onAddOrUpdateUser() {
    QString username = ui->username_edit->text();
    QString password = ui->password_edit->text();
    bool userIsAdmin = ui->admin_check->isChecked();

    if (username.isEmpty() || (userIDBeingEdited.isEmpty() && password.isEmpty())) {
        QMessageBox::warning(this, tr("Erro na entrada"), tr("Usuário e senha não podem estar vazios!"));
        return;
    }

    QString hashedPassword = sha256(password.trimmed());

    if (!userIDBeingEdited.isEmpty()) {  // Edit existing user
        int userId = userIDBeingEdited.toInt();  // Convert ID back to integer
        bool updatingPassword = !password.isEmpty();
        if (UpdateUser(userId, hashedPassword.toStdString(), userIsAdmin, updatingPassword)) {
            QMessageBox::information(this, tr("Success"), tr("Usuário atualizado com sucesso."));
            resetFields();
            loadUsers();
        }
    } else {  // Add new user
        if (AddUser(username.toStdString(), hashedPassword.toStdString(), userIsAdmin)) {
            QMessageBox::information(this, tr("Success"), tr("Usuário incluído com sucesso."));
            resetFields();
            loadUsers();
        }
    }
}

void UserManager::onDeleteUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int userId = ui->user_table->item(row, UserTableColumns::username_col)->data(Qt::UserRole).toInt();  // Retrieve the user ID

    if (DeleteUser(userId)) {
        QMessageBox::information(this, tr("Success"), tr("Usuário excluído com sucesso."));
        loadUsers();
    }
}

void UserManager::onShowPasswordHashToggled(bool checked) {
    Q_UNUSED(checked);  // Suppress the unused parameter warning
    loadUsers(); // Reload users with or without showing password hashes
}


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
