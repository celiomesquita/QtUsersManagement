#include "../Hash/HashUtils.h"     // Include the file where sha256 is declared
#include "../build/ui_LoginDialog.h"  // Include the generated header
#include "../Login/LoginDialog.h"
#include "../Database/Database.h"

#include <QSqlDatabase>    // Include for QSqlDatabase
#include <QSqlQuery>       // Include for QSqlQuery
#include <QSqlError>       // Include for QSqlError
#include <QMessageBox>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog), m_isAdmin(false)
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file

    setWindowTitle("Acesso ao Sistema");
    setModal(true);

    // Connect the login button click signal to the appropriate slot
    connect(ui->login_btn, &QPushButton::clicked, this, &LoginDialog::onLoginBtnClicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;  // Clean up the UI
}

QString LoginDialog::getUsername() const
{
    return m_username;
}

bool LoginDialog::isAdmin() const
{
    return m_isAdmin;
}

void LoginDialog::onLoginBtnClicked()
{
    QString username = ui->username_edit->text();
    QString password = ui->password_edit->text();    

    if (username.isEmpty() || password.isEmpty()) {
        ui->status_label->setText("Entre com o usuário e a senha!");
        return;
    }

    QString hashedPassword = sha256(password.trimmed());

    QSqlDatabase db = Database::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        ui->status_label->setText("Erro ao tentar acessar o sistema.");
        return;
    }

    // Example: Check user credentials
    QSqlQuery query(db);
    query.prepare("SELECT password, admin FROM users WHERE username = :username");
    query.bindValue(":username", username);

    QString storedPassword;  // Declare storedPassword in the correct scope

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        ui->status_label->setText("Erro ao tentar acessar o sistema.");
        return;
    }

    if (query.next()) {
        storedPassword = query.value(0).toString();  // Assign the value to storedPassword
        bool admin = query.value(1).toBool();

        // Compare the hashed input password with the stored hashed password
        if (hashedPassword == storedPassword) {
            m_username = username;
            m_isAdmin = admin;
            accept();  // Close the dialog with Accepted status
            return;
        }
    }

    qDebug() << "Input Password (hashed):" << hashedPassword;
    qDebug() << "Stored Password:" << storedPassword;  // Use storedPassword for debugging

    ui->status_label->setText("Senha ou usuário inválidos.");
}
