#include "ProjectManager.h"
#include "ProjectDB.h"  // Include the existing service class

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "../build/ui_ProjectManager.h"

enum ProjectTableColumns {
    idColumn,  // Column for project ID (will be hidden)
    nameColumn,
    configColumn,
    editColumn,
    deleteColumn,
    ColumnCount // Keep track of the total number of columns
};

ProjectManagementDialog::ProjectManagementDialog(QSqlDatabase& db, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ProjectManagementDialog),
    db(db)
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file
    initializeProjectTable(); // Initialize the table

    ui->statusLabel->setText("---");   

    setWindowTitle(tr("Configurações de projetos"));
    setModal(true);  // Blocks interaction with other windows until the Project closes it

    connect(ui->addOrUpdateButton, &QPushButton::clicked, this, &ProjectManagementDialog::onAddOrUpdateProject);    

    loadProjects();  // Load the initial list of Projects
}

ProjectManagementDialog::~ProjectManagementDialog()
{
    delete ui;  // Clean up the UI
}

void ProjectManagementDialog::initializeProjectTable() {
    ui->projectTable->setColumnCount(ProjectTableColumns::ColumnCount);
    ui->projectTable->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("Projeto") << tr("Config") << tr("Editar") << tr("Excluir"));

    ui->projectTable->setColumnWidth(ProjectTableColumns::nameColumn, 100);
    ui->projectTable->setColumnWidth(ProjectTableColumns::configColumn, 400);
    ui->projectTable->setColumnWidth(ProjectTableColumns::editColumn, 100);
    ui->projectTable->setColumnWidth(ProjectTableColumns::deleteColumn, 100);

    // Hide the idColumn (column 0)
    ui->projectTable->hideColumn(ProjectTableColumns::idColumn);
}


void ProjectManagementDialog::onEditProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();  // Get the row number from the button's property

    // Retrieve the Project ID from the hidden idColumn
    int projectId = ui->projectTable->item(row, ProjectTableColumns::idColumn)->text().toInt();

    // Retrieve the project name and config from the table
    QString projectName = ui->projectTable->item(row, ProjectTableColumns::nameColumn)->text();
    QString projectConfig = ui->projectTable->item(row, ProjectTableColumns::configColumn)->text();

    // Pre-fill the input fields for editing
    ui->nameEdit->setText(projectName);
    ui->configEdit->setPlainText(projectConfig);

    // Store the project ID for later use
    projectIDBeingEdited = QString::number(projectId);

    ui->addOrUpdateButton->setText(tr("Salvar"));
}


void ProjectManagementDialog::loadProjects() {
    ui->projectTable->setRowCount(0); // Clear existing rows

    QString queryString = "SELECT id, name, config FROM Projects";
    QSqlQuery query(db); 
    if (!query.exec(queryString)) {
        qCritical() << tr("Falha ao carregar projetos:") << query.lastError().text();
        return;
    }

    while (query.next()) {
        int projectId           = query.value(0).toInt();  // Retrieve the Project ID from the database
        QString dbProjectName   = query.value(1).toString();
        QString dbProjectConfig = query.value(2).toString();

        int row = ui->projectTable->rowCount();
        ui->projectTable->insertRow(row);

        // Store the project ID in the hidden column (idColumn)
        QTableWidgetItem *projectIdItem = new QTableWidgetItem(QString::number(projectId));
        ui->projectTable->setItem(row, ProjectTableColumns::idColumn, projectIdItem);

        // Store the project name and config in their respective columns
        QTableWidgetItem *projectNameItem = new QTableWidgetItem(dbProjectName);
        QTableWidgetItem *projectConfigItem = new QTableWidgetItem(dbProjectConfig);

        ui->projectTable->setItem(row, ProjectTableColumns::nameColumn, projectNameItem);
        ui->projectTable->setItem(row, ProjectTableColumns::configColumn, projectConfigItem);

        // Add Edit button
        QPushButton* editButton = new QPushButton(tr("Editar"), this);
        editButton->setProperty("row", row);
        connect(editButton, &QPushButton::clicked, this, &ProjectManagementDialog::onEditProject);
        ui->projectTable->setCellWidget(row, ProjectTableColumns::editColumn, editButton);

        // Add Delete button
        QPushButton* deleteButton = new QPushButton(tr("Excluir"), this);
        deleteButton->setProperty("row", row);
        connect(deleteButton, &QPushButton::clicked, this, &ProjectManagementDialog::onDeleteProject);
        ui->projectTable->setCellWidget(row, ProjectTableColumns::deleteColumn, deleteButton);                
    }
}


void ProjectManagementDialog::resetFields() {
    ui->nameEdit->clear();
    ui->configEdit->clear();
    projectIDBeingEdited.clear();
    ui->addOrUpdateButton->setText(tr("Adicionar projeto"));
}

void ProjectManagementDialog::onAddOrUpdateProject() {
    QString projectName   = ui->nameEdit->text();
    QString projectConfig = ui->configEdit->toPlainText();

    if (projectName.isEmpty() || projectConfig.isEmpty()) {
        QMessageBox::warning(this, tr("Erro na entrada"), tr("Nome ou config não podem estar vazios!"));
        return;
    }

    if (!projectIDBeingEdited.isEmpty()) {  // Edit existing Project
        int projectId = projectIDBeingEdited.toInt();  // Convert ID back to integer
        if (updateProject(projectId, projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto atualizado com sucesso."));
            resetFields();
            loadProjects();
        }
    } else {  // Add new Project
        if (addProject(projectName.toStdString(), projectConfig.toStdString())) {
            QMessageBox::information(this, tr("Success"), tr("Projeto incluído com sucesso."));
            resetFields();
            loadProjects();
        }
    }
}


void ProjectManagementDialog::onDeleteProject() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();

    int projectId = ui->projectTable->item(row, ProjectTableColumns::idColumn)->text().toInt();

    if (deleteProject(projectId)) {
        QMessageBox::information(this, tr("Success"), tr("Projeto excluído com sucesso."));
        loadProjects();
    }
}
