#include "ProjectManager.h"
#include "ui_ProjectManager.h"  // Include the UI header generated from the .ui file

ProjectManager::ProjectManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectManager)
{
    ui->setupUi(this);

    // Connect to the SQLite database
    if (connectDatabase()) {
        loadProjects();  // Load projects from the database if connected
    } else {
        qDebug() << "Failed to connect to the database.";
    }
}

ProjectManager::~ProjectManager()
{
    delete ui;
}

bool ProjectManager::connectDatabase()
{
    // Setup the SQLite database connection (assuming users.db is in the same directory)
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qWarning() << "Error: Unable to open database.";
        return false;
    }

    return true;
}

void ProjectManager::loadProjects()
{
    QSqlQuery query("SELECT name, config FROM Projects");

    while (query.next()) {
        QString name = query.value(0).toString();
        QString configString = query.value(1).toString();

        // Parse the JSON config
        QJsonDocument doc = QJsonDocument::fromJson(configString.toUtf8());
        QJsonObject config = doc.object();

        // Display the project name and config in the UI (assuming you have a QListWidget or similar)
        QListWidgetItem *item = new QListWidgetItem(name, ui->projectListWidget);
        item->setData(Qt::UserRole, configString);  // Store config as item data
    }
}

void ProjectManager::addProject(const QString &name, const QJsonObject &config)
{
    // Prepare the SQL query
    QSqlQuery query;
    query.prepare("INSERT INTO Projects (name, config) VALUES (:name, :config)");

    // Convert JSON object to string
    QJsonDocument doc(config);
    QString configString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));

    // Bind the values
    query.bindValue(":name", name);
    query.bindValue(":config", configString);

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to insert project:" << query.lastError();
    } else {
        // Add the new project to the UI list
        QListWidgetItem *item = new QListWidgetItem(name, ui->projectListWidget);
        item->setData(Qt::UserRole, configString);  // Store config as item data
    }
}
