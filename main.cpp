#include <QApplication>
#include <QDebug>
#include "Database.h"
#include "LoginDialog.h"
#include "MainWindow.h"

// MongoDB C++ driver includes
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>

// Define the connection string, database, and collection
const std::string MONGO_CONN_STR = "sirerc_conn";  // Use your actual connection string here
const std::string MONGO_DB_NAME = "sirerc_db";
const std::string MONGO_COLLECTION_NAME = "sirerc_collection";

// Function to retrieve project data for a user
std::vector<std::string> getUserProjects(mongocxx::collection& collection, const QString& username) {
    std::vector<std::string> userProjects;

    // Query the entire document that holds the "projects" array
    auto query = bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize;
    auto projectDoc = collection.find_one(query.view());

    if (projectDoc) {
        auto projectsArray = projectDoc->view()["projects"].get_array().value;
        
        // Iterate over the projects array to find projects accessible by the user
        for (auto&& projectElement : projectsArray) {
            auto project = projectElement.get_document().view();
            auto allowedUsers = project["allowed_users"].get_array().value;

            // Check if the username exists in the allowed_users list
            for (auto&& userElement : allowedUsers) {
                auto user = userElement.get_document().view();
                if (user["username"].get_utf8().value == username.toStdString()) {
                    // If the user is found, add the project to the user's project list
                    userProjects.push_back(project["project_name"].get_utf8().value.to_string());
                    break;
                }
            }
        }
    }

    return userProjects;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize SQLite for local database (Assuming DB_PATH is already defined)
    if (!Database::initialize(DB_PATH)) {
        qDebug() << "Failed to initialize the local SQLite database.";
        return -1;
    }

    // Initialize MongoDB client
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{MONGO_CONN_STR}};  // Use the MongoDB connection string you set up

    // Access the MongoDB database and collection
    auto db = client[MONGO_DB_NAME];  // Use your MongoDB database name
    auto collection = db[MONGO_COLLECTION_NAME];  // Use your MongoDB collection name

    // Show the login dialog
    LoginDialog loginDialog;

    if (loginDialog.exec() == QDialog::Accepted) {
        QString loggedInUser = loginDialog.getUsername();
        bool isAdmin = loginDialog.isAdmin();

        // Fetch the list of projects the user has access to from MongoDB
        std::vector<std::string> userProjects = getUserProjects(collection, loggedInUser);

        // Convert the std::vector to QStringList to pass it to the MainWindow
        QStringList userProjectsList;
        for (const auto& project : userProjects) {
            userProjectsList << QString::fromStdString(project);
        }

        // Pass the user info and the projects they have access to into the main window
        MainWindow mainWindow(loggedInUser, isAdmin, userProjectsList);
        mainWindow.show();

        return app.exec();
    } else {
        qDebug() << "Login canceled or failed.";
        return 0;
    }
}
