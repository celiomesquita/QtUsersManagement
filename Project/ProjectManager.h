#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QPushButton>

namespace Ui {
class ProjectManagerDialog;
}

class ProjectManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectManagerDialog(QSqlDatabase& db, QString Name, QString Config, QWidget *parent = nullptr);
    ~ProjectManagerDialog();

private slots:
    void onAddOrUpdateProject();  // Slot for adding or updating a project
    void onDeleteProject();       // Slot for deleting a project
    void onEditProject();         // Slot for editing an existing project

private:
    Ui::ProjectManagerDialog *ui;
    QSqlDatabase& db;              // Reference to the SQLite database
    QString ProjectIDBeingEdited;  // Stores the ID of the project being edited

    void initializeprojectTable(); // Initialize the project table
    void loadProjects();           // Load projects from the database
    void resetFields();            // Reset input fields for adding/editing projects

    // Helper functions for database operations
    bool AddProject(const std::string &ProjectName, const std::string &ProjectConfig);
    bool UpdateProject(int id, const std::string &ProjectName, const std::string &ProjectConfig);
    bool DeleteProject(int id);
};

#endif // PROJECTMANAGER_H
