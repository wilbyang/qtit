#include "db.h"
#include <QDebug>
#include <QFile>
#include <QDir>

Database::Database(QObject *parent) : QObject(parent) {
    // Set up the SQLite database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("todo.db"); // Database file name
}

Database::~Database() {
    db.close(); // Close the database connection
}

bool Database::initialize() {
    if (!db.open()) {
        qWarning() << "Error: Could not open database.";
        return false;
    }

    // Create the tasks table if it doesn't exist
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS tasks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "task TEXT NOT NULL)")) {
        qWarning() << "Error: Could not create table.";
        return false;
    }

    return true;
}

bool Database::addTask(const QString &task) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (task) VALUES (:task)");
    query.bindValue(":task", task);

    if (!query.exec()) {
        qWarning() << "Error: Could not add task.";
        return false;
    }

    return true;
}

bool Database::removeTask(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Error: Could not remove task.";
        return false;
    }

    return true;
}

QList<QPair<int, QString>> Database::getTasks() {
    QList<QPair<int, QString>> tasks;
    QSqlQuery query("SELECT id, task FROM tasks");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString task = query.value(1).toString();
        tasks.append(qMakePair(id, task));
    }

    return tasks;
}