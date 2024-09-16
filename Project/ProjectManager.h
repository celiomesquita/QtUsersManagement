#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QTableWidgetItem>

namespace Ui {
class ProjectManagementDialog;
}

class ProjectManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectManagementDialog(QSqlDatabase& db, QWidget* parent = nullptr);  // Constructor
    ~ProjectManagementDialog();  // Destructor

private slots:
    void onAddOrUpdateProject();  // Slot to handle adding or updating a project
    void onEditProject();         // Slot to handle editing a project
    void onDeleteProject();       // Slot to handle deleting a project

private:
    Ui::ProjectManagementDialog* ui;  // Pointer to the UI
    QSqlDatabase& db;                 // Reference to the SQLite database
    QString projectIDBeingEdited;     // Variable to track the project ID being edited

    void initializeProjectTable();    // Function to initialize the project table in the UI
    void loadProjects();              // Function to load projects from the database
    void resetFields();               // Function to clear input fields after add/update
};

#endif // PROJECTMANAGER_H
