#include "UserManagementDialog.h"
#include "../Database/UserDB.h"  // Include the existing service class
#include "../Hash/HashUtils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "../build/ui_UserManagementDialog.h"  // Include the generated UI header

enum UserTableColumns {
    UsernameColumn,
    PasswordColumn,
    AdminColumn,
    EditColumn,
    DeleteColumn,
    ColumnCount // Keep track of the total number of columns
};

UserManagementDialog::UserManagementDialog(QSqlDatabase& db, QString loggedInUser, UserRole role, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::UserManagementDialog),
    db(db),
    loggedInUser(loggedInUser),
    role(role)  // Initialize the role field
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file
    initializeUserTable(); // Initialize the table

    QString adminStatus = (role == UserRole::Admin) ? "Admin" : "User";
    ui->statusLabel->setText(loggedInUser + " (" + adminStatus + ")");   

    setWindowTitle(tr("Gestão de usuários"));
    setModal(true);  // Blocks interaction with other windows until the user closes it

    if (role == UserRole::Admin){
        connect(ui->showPasswordHashCheckBox, &QCheckBox::toggled, this, &UserManagementDialog::onShowPasswordHashToggled);
    }

    connect(ui->addOrUpdateBtn, &QPushButton::clicked, this, &UserManagementDialog::onAddOrUpdateUser);    

    loadUsers();  // Load the initial list of users
}

UserManagementDialog::~UserManagementDialog()
{
    delete ui;  // Clean up the UI
}

void UserManagementDialog::initializeUserTable() {
    ui->userTable->setColumnCount(UserTableColumns::ColumnCount);
    ui->userTable->setHorizontalHeaderLabels(QStringList() << tr("Usuário") << tr("Senha") << tr("Admin") << tr("Editar") << tr("Excluir"));
    
    // Set column widths (adjust values as needed)
    ui->userTable->setColumnWidth(UserTableColumns::UsernameColumn, 118);
    ui->userTable->setColumnWidth(UserTableColumns::PasswordColumn, 118);
    ui->userTable->setColumnWidth(UserTableColumns::AdminColumn, 118);
    ui->userTable->setColumnWidth(UserTableColumns::EditColumn, 118);
    ui->userTable->setColumnWidth(UserTableColumns::DeleteColumn, 118);
}

void UserManagementDialog::onEditUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int userId = ui->userTable->item(row, UserTableColumns::UsernameColumn)->data(Qt::UserRole).toInt();  // Retrieve the user ID
    QString username = ui->userTable->item(row, UserTableColumns::UsernameColumn)->text();
    QString password = ""; // Do not load the password
    bool userIsAdmin = (ui->userTable->item(row, UserTableColumns::AdminColumn)->text() == tr("Sim"));

    ui->usernameEdit->setText(username);
    ui->passwordEdit->setText(password);
    ui->adminCheckBox->setChecked(userIsAdmin);

    // only Admin can modify usernames and admin status
    ui->adminCheckBox->setEnabled(role == UserRole::Admin);
    ui->usernameEdit->setEnabled(role == UserRole::Admin);

    userIDBeingEdited = QString::number(userId);  // Store the user ID as a string

    ui->addOrUpdateBtn->setText(tr("Salvar"));
}

void UserManagementDialog::loadUsers() {

    ui->userTable->setRowCount(0); // Clear existing rows

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

        int row = ui->userTable->rowCount();
        ui->userTable->insertRow(row);

        // Store the user ID in a custom property of the row
        QTableWidgetItem *usernameItem = new QTableWidgetItem(dbUsername);
        usernameItem->setData(Qt::UserRole, userId);  // Store user ID in the item
        ui->userTable->setItem(row, UserTableColumns::UsernameColumn, usernameItem);

        ui->userTable->setItem(row, UserTableColumns::PasswordColumn, new QTableWidgetItem(ui->showPasswordHashCheckBox->isChecked() ? dbPassword : "???"));
        ui->userTable->setItem(row, UserTableColumns::AdminColumn, new QTableWidgetItem(dbAdmin ? tr("Sim") : tr("Não")));

        if (role == UserRole::Admin){
            // Add Edit button
            QPushButton* editButton = new QPushButton(tr("Editar"), this);
            editButton->setProperty("row", row);
            connect(editButton, &QPushButton::clicked, this, &UserManagementDialog::onEditUser);
            ui->userTable->setCellWidget(row, UserTableColumns::EditColumn, editButton);
        }else{
            if(dbUsername == loggedInUser){
                QPushButton* editButton = new QPushButton(tr("Editar"), this);
                editButton->setProperty("row", row);
                connect(editButton, &QPushButton::clicked, this, &UserManagementDialog::onEditUser);
                ui->userTable->setCellWidget(row, UserTableColumns::EditColumn, editButton);            
            }else{
                QLabel* placeholderLabel = new QLabel("-", this);
                placeholderLabel->setAlignment(Qt::AlignCenter);
                ui->userTable->setCellWidget(row, UserTableColumns::EditColumn, placeholderLabel);                  
            }
        }

        if (role == UserRole::Admin){
            if(dbUsername == loggedInUser){
                QLabel* placeholderLabel = new QLabel("-", this);
                placeholderLabel->setAlignment(Qt::AlignCenter);
                ui->userTable->setCellWidget(row, UserTableColumns::DeleteColumn, placeholderLabel); 
            }else{
                // Add Delete button
                QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
                deleteButton->setProperty("row", row);
                connect(deleteButton, &QPushButton::clicked, this, &UserManagementDialog::onDeleteUser);
                ui->userTable->setCellWidget(row, UserTableColumns::DeleteColumn, deleteButton);                
            }
        }else{
            QLabel* placeholderLabel = new QLabel("-", this);
            placeholderLabel->setAlignment(Qt::AlignCenter);
            ui->userTable->setCellWidget(row, UserTableColumns::DeleteColumn, placeholderLabel);
        }
    }
}

void UserManagementDialog::resetFields() {
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->adminCheckBox->setChecked(false);
    userIDBeingEdited.clear();
    ui->addOrUpdateBtn->setText(tr("Adicionar usuário"));
}

void UserManagementDialog::onAddOrUpdateUser() {
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    bool userIsAdmin = ui->adminCheckBox->isChecked();

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

void UserManagementDialog::onDeleteUser() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int userId = ui->userTable->item(row, UserTableColumns::UsernameColumn)->data(Qt::UserRole).toInt();  // Retrieve the user ID

    if (DeleteUser(userId)) {
        QMessageBox::information(this, tr("Success"), tr("Usuário excluído com sucesso."));
        loadUsers();
    }
}

void UserManagementDialog::onShowPasswordHashToggled(bool checked) {
    Q_UNUSED(checked);  // Suppress the unused parameter warning
    loadUsers(); // Reload users with or without showing password hashes
}
