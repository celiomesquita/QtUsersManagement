#include "ProjectManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "ui_ProjectManager.h"  // Include the generated UI header

enum projectTableColumns {
    name_col,
    config_col,
    edit_col,
    delete_col,
    ColumnCount // Keep track of the total number of columns
};

ProjectManager::ProjectManager(QSqlDatabase& db, QString Name, QString Config, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ProjectManager),
    db(db),
    projectName(Name),
    projectConfig(Config)
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file
    initializeprojectTable(); // Initialize the table

    ui->status_label->setText(projectName);  // Example of setting some data on the UI

    setWindowTitle(tr("Gestão de projetos"));
    setModal(true);  // Blocks interaction with other windows until the project manager is closed

    connect(ui->add_update_btn, &QPushButton::clicked, this, &ProjectManager::onAddOrupdateProject);    

    loadProjects();  // Load the initial list of projects
}

ProjectManager::~ProjectManager()
{
    delete ui;  // Clean up the UI
}

void ProjectManager::initializeprojectTable() {
    ui->project_table->setColumnCount(projectTableColumns::ColumnCount);
    ui->project_table->setHorizontalHeaderLabels(QStringList() << tr("Projeto") << tr("Config") << tr("Editar") << tr("Excluir"));
    
    // Set column widths (adjust values as needed)
    ui->project_table->setColumnWidth(projectTableColumns::name_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::config_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::edit_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::delete_col, 118);
}

void ProjectManager::onEditProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int projectId = ui->project_table->item(row, projectTableColumns::Name_col)->data(Qt::UserRole).toInt();  // Retrieve the project ID

    QString projectName   = ui->project_table->item(row, projectTableColumns::Name_col)->text();
    QString projectConfig = ui->project_table->item(row, projectTableColumns::Config_col)->text();

    ui->name_edit->setText(projectName);
    ui->config_edit->setText(projectConfig);

    config_item = QString::number(projectId);  // Store the project ID as a string

    ui->add_update_btn->setText(tr("Salvar"));
}

void ProjectManager::loadProjects() {
    ui->project_table->setRowCount(0);  // Clear existing rows

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

        int row = ui->project_table->rowCount();
        ui->project_table->insertRow(row);

        QTableWidgetItem *name_item = new QTableWidgetItem(projectName);
        name_item->setData(Qt::UserRole, projectId);  // Store the project ID in the item
        ui->project_table->setItem(row, projectTableColumns::name_col, name_item);

        QTableWidgetItem *config_item = new QTableWidgetItem(projectConfig);
        ui->project_table->setItem(row, projectTableColumns::config_col, config_item);

        QPushButton* editButton = new QPushButton(tr("Editar"), this);
        editButton->setProperty("row", row);
        connect(edit_btn, &QPushButton::clicked, this, &ProjectManager::onEditProject);
        ui->project_table->setCellWidget(row, projectTableColumns::edit_col, edit_btn);

        QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
        deleteButton->setProperty("row", row);
        connect(delete_btn, &QPushButton::clicked, this, &ProjectManager::onDeleteProject);
        ui->project_table->setCellWidget(row, projectTableColumns::delete_col, delete_btn);
    }
}

void ProjectManager::resetFields() {
    ui->name_edit->clear();
    ui->config_edit->clear();
    projectIDBeingEdited.clear();
    ui->add_update_btn->setText(tr("Adicionar projeto"));
}

void ProjectManager::onAddOrupdateProject() {
    QString projectName = ui->name_edit->text();
    QString projectConfig = ui->config_edit->text();

    if (!projectIDBeingEdited.isEmpty()) {  // Edit existing project
        int projectId = projectIDBeingEdited.toInt();  // Convert ID back to integer
        if (updateProject(projectId, projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto atualizado com sucesso."));
            resetFields();
            loadProjects();
        }
    } else {  // Add new project
        if (addProject(projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto incluído com sucesso."));
            resetFields();
            loadProjects();
        }
    }
}

void ProjectManager::onDeleteProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int projectId = ui->project_table->item(row, projectTableColumns::name_col)->data(Qt::UserRole).toInt();

    if (deleteProject(projectId)) {
        QMessageBox::information(this, tr("Success"), tr("Projeto excluído com sucesso."));
        loadProjects();
    }
}

bool ProjectManager::addProject(const std::string& projectName, const std::string& projectConfig) {
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

bool ProjectManager::updateProject(int id, const std::string& projectName, const std::string& projectConfig) {
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

bool ProjectManager::deleteProject(int id) {
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
