#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include <QString>

class Database : public QObject {
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool initialize(); // Initialize the database
    bool addTask(const QString &task); // Add a task to the database
    bool removeTask(int id); // Remove a task by ID
    QList<QPair<int, QString>> getTasks(); // Retrieve all tasks

private:
    QSqlDatabase db; // SQLite database connection
};

#endif // DATABASE_H