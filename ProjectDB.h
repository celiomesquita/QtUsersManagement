#ifndef PROJECTDB_H
#define PROJECTDB_H

#include <string>

// Function to add a new project to the database
bool addProject(const std::string& name, const std::string& config);

// Function to update an existing project in the database by ID
bool updateProject(int id, const std::string& name, const std::string& config);

// Function to delete a project from the database by ID
bool deleteProject(int id);

#endif // PROJECTDB_H
