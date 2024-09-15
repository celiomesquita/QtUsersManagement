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

    ui->status_label->setText(Name);  // Example of setting some data on the UI

    setWindowTitle(tr("Gestão de projetos"));
    setModal(true);  // Blocks interaction with other windows until the project manager is closed

    connect(ui->addOrUpdate_btn, &QPushButton::clicked, this, &ProjectManagerDialog::onAddOrUpdateProject);    

    loadProjects();  // Load the initial list of projects
}

ProjectManagerDialog::~ProjectManagerDialog()
{
    delete ui;  // Clean up the UI
}

void ProjectManagerDialog::initializeprojectTable() {
    ui->project_table->setColumnCount(projectTableColumns::ColumnCount);
    ui->project_table->setHorizontalHeaderLabels(QStringList() << tr("Projeto") << tr("Config") << tr("Editar") << tr("Excluir"));
    
    // Set column widths (adjust values as needed)
    ui->project_table->setColumnWidth(projectTableColumns::Name_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::Config_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::Edit_col, 118);
    ui->project_table->setColumnWidth(projectTableColumns::Delete_col, 118);
}

void ProjectManagerDialog::onEditProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int projectId = ui->project_table->item(row, projectTableColumns::Name_col)->data(Qt::UserRole).toInt();  // Retrieve the project ID

    QString projectName   = ui->project_table->item(row, projectTableColumns::Name_col)->text();
    QString projectConfig = ui->project_table->item(row, projectTableColumns::Config_col)->text();

    ui->projectName_edit->setText(projectName);
    ui->projectConfig_edit->setText(projectConfig);

    ProjectIDBeingEdited = QString::number(projectId);  // Store the project ID as a string

    ui->addOrUpdate_btn->setText(tr("Salvar"));
}

void ProjectManagerDialog::loadProjects() {
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

        QTableWidgetItem *projectNameItem = new QTableWidgetItem(projectName);
        projectNameItem->setData(Qt::UserRole, projectId);  // Store the project ID in the item
        ui->project_table->setItem(row, projectTableColumns::name_col, projectNameItem);

        QTableWidgetItem *projectConfigItem = new QTableWidgetItem(projectConfig);
        ui->project_table->setItem(row, projectTableColumns::config_col, projectConfigItem);

        QPushButton* editButton = new QPushButton(tr("Editar"), this);
        editButton->setProperty("row", row);
        connect(edit_btn, &QPushButton::clicked, this, &ProjectManagerDialog::onEditProject);
        ui->project_table->setCellWidget(row, projectTableColumns::edit_col, edit_btn);

        QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
        deleteButton->setProperty("row", row);
        connect(delete_btn, &QPushButton::clicked, this, &ProjectManagerDialog::onDeleteProject);
        ui->project_table->setCellWidget(row, projectTableColumns::delete_col, delete_btn);
    }
}

void ProjectManagerDialog::resetFields() {
    ui->name_edit->clear();
    ui->config_edit->clear();
    ProjectIDBeingEdited.clear();
    ui->add_update_btn->setText(tr("Adicionar projeto"));
}

void ProjectManagerDialog::onAddOrUpdateProject() {
    QString projectName = ui->name_edit->text();
    QString projectConfig = ui->config_edit->text();

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
    int projectId = ui->project_table->item(row, projectTableColumns::name_col)->data(Qt::UserRole).toInt();

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
