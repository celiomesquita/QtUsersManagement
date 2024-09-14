#include "ProjectManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "ui_ProjectManager.h"  // Include the generated UI header

enum projectTableColumns {
    NameColumn,
    ConfigColumn,
    EditColumn,
    DeleteColumn,
    ColumnCount // Keep track of the total number of columns
};

ProjectManagerDialog::ProjectManagerDialog(QSqlDatabase& db, QString Name, QString Config, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ProjectManagerDialog),
    db(db)
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file
    initializeprojectTable(); // Initialize the table

    ui->statusLabel->setText(Name);  // Example of setting some data on the UI

    setWindowTitle(tr("Gestão de projetos"));
    setModal(true);  // Blocks interaction with other windows until the project manager is closed

    connect(ui->addOrUpdateBtn, &QPushButton::clicked, this, &ProjectManagerDialog::onAddOrUpdateProject);    

    loadProjects();  // Load the initial list of projects
}

ProjectManagerDialog::~ProjectManagerDialog()
{
    delete ui;  // Clean up the UI
}

void ProjectManagerDialog::initializeprojectTable() {
    ui->projectTable->setColumnCount(projectTableColumns::ColumnCount);
    ui->projectTable->setHorizontalHeaderLabels(QStringList() << tr("Projeto") << tr("Config") << tr("Editar") << tr("Excluir"));
    
    // Set column widths (adjust values as needed)
    ui->projectTable->setColumnWidth(projectTableColumns::NameColumn, 118);
    ui->projectTable->setColumnWidth(projectTableColumns::ConfigColumn, 118);
    ui->projectTable->setColumnWidth(projectTableColumns::EditColumn, 118);
    ui->projectTable->setColumnWidth(projectTableColumns::DeleteColumn, 118);
}

void ProjectManagerDialog::onEditProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int projectId = ui->projectTable->item(row, projectTableColumns::NameColumn)->data(Qt::UserRole).toInt();  // Retrieve the project ID

    QString projectName   = ui->projectTable->item(row, projectTableColumns::NameColumn)->text();
    QString projectConfig = ui->projectTable->item(row, projectTableColumns::ConfigColumn)->text();

    ui->projectNameEdit->setText(projectName);
    ui->projectConfigEdit->setText(projectConfig);

    ProjectIDBeingEdited = QString::number(projectId);  // Store the project ID as a string

    ui->addOrUpdateBtn->setText(tr("Salvar"));
}

void ProjectManagerDialog::loadProjects() {
    ui->projectTable->setRowCount(0);  // Clear existing rows

    QString queryString = "SELECT id, name, config FROM Projects";
    QSqlQuery query(db);
    if (!query.exec(queryString)) {
        qCritical() << tr("Falha ao carregar projetos:") << query.lastError().text();
        return;
    }

    while (query.next()) {
        int projectId = query.value(0).toInt();  // Retrieve the project ID
        QString projectName = query.value(1).toString();
        QString projectConfig = query.value(2).toString();

        int row = ui->projectTable->rowCount();
        ui->projectTable->insertRow(row);

        QTableWidgetItem *projectNameItem = new QTableWidgetItem(projectName);
        projectNameItem->setData(Qt::UserRole, projectId);  // Store the project ID in the item
        ui->projectTable->setItem(row, projectTableColumns::NameColumn, projectNameItem);

        QTableWidgetItem *projectConfigItem = new QTableWidgetItem(projectConfig);
        ui->projectTable->setItem(row, projectTableColumns::ConfigColumn, projectConfigItem);

        QPushButton* editButton = new QPushButton(tr("Editar"), this);
        editButton->setProperty("row", row);
        connect(editButton, &QPushButton::clicked, this, &ProjectManagerDialog::onEditProject);
        ui->projectTable->setCellWidget(row, projectTableColumns::EditColumn, editButton);

        QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
        deleteButton->setProperty("row", row);
        connect(deleteButton, &QPushButton::clicked, this, &ProjectManagerDialog::onDeleteProject);
        ui->projectTable->setCellWidget(row, projectTableColumns::DeleteColumn, deleteButton);
    }
}

void ProjectManagerDialog::resetFields() {
    ui->projectNameEdit->clear();
    ui->projectConfigEdit->clear();
    ProjectIDBeingEdited.clear();
    ui->addOrUpdateBtn->setText(tr("Adicionar projeto"));
}

void ProjectManagerDialog::onAddOrUpdateProject() {
    QString projectName = ui->projectNameEdit->text();
    QString projectConfig = ui->projectConfigEdit->text();

    if (!ProjectIDBeingEdited.isEmpty()) {  // Edit existing project
        int projectId = ProjectIDBeingEdited.toInt();  // Convert ID back to integer
        if (UpdateProject(projectId, projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto atualizado com sucesso."));
            resetFields();
            loadProjects();
        }
    } else {  // Add new project
        if (AddProject(projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto incluído com sucesso."));
            resetFields();
            loadProjects();
        }
    }
}

void ProjectManagerDialog::onDeleteProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int projectId = ui->projectTable->item(row, projectTableColumns::NameColumn)->data(Qt::UserRole).toInt();

    if (DeleteProject(projectId)) {
        QMessageBox::information(this, tr("Success"), tr("Projeto excluído com sucesso."));
        loadProjects();
    }
}

bool ProjectManagerDialog::AddProject(const std::string& projectName, const std::string& projectConfig) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO Projects (name, config) VALUES (:projectName, :projectConfig)");
    query.bindValue(":projectName", QString::fromStdString(projectName));
    query.bindValue(":projectConfig", QString::fromStdString(projectConfig));

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to add project:" << query.lastError().text();
        return false;
    }
}

bool ProjectManagerDialog::UpdateProject(int id, const std::string& projectName, const std::string& projectConfig) {
    QSqlQuery query(db);
    query.prepare("UPDATE Projects SET name=:name, config=:config WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":name", QString::fromStdString(projectName));
    query.bindValue(":config", QString::fromStdString(projectConfig));

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to update project:" << query.lastError().text();
        return false;
    }
}

bool ProjectManagerDialog::DeleteProject(int id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM Projects WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qCritical() << "Failed to delete project:" << query.lastError().text();
        return false;
    }
}
