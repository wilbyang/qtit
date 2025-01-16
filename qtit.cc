#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "db.h"

class TodoApp : public QWidget {
    Q_OBJECT

public:
    TodoApp(QWidget *parent = nullptr) : QWidget(parent) {
        // Initialize the database
        db = new Database(this);
        if (!db->initialize()) {
            QMessageBox::critical(this, "Error", "Could not initialize database!");
            return;
        }

        // Set up the main layout
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Create a list widget to display tasks
        taskList = new QListWidget(this);
        layout->addWidget(taskList);

        // Create a line edit for adding new tasks
        taskInput = new QLineEdit(this);
        taskInput->setPlaceholderText("Enter a new task...");
        layout->addWidget(taskInput);

        // Create buttons for adding and removing tasks
        QPushButton *addButton = new QPushButton("Add Task", this);
        QPushButton *removeButton = new QPushButton("Remove Selected Task", this);
        layout->addWidget(addButton);
        layout->addWidget(removeButton);

        // Connect buttons to their respective slots
        connect(addButton, &QPushButton::clicked, this, &TodoApp::addTask);
        connect(removeButton, &QPushButton::clicked, this, &TodoApp::removeTask);

        // Load tasks from the database
        loadTasks();

        // Set the layout for the main window
        setLayout(layout);
        setWindowTitle("Todo List App");
    }

private slots:
    void addTask() {
        QString task = taskInput->text().trimmed();
        if (!task.isEmpty()) {
            if (db->addTask(task)) {
                taskList->addItem(task); // Add the task to the list
                taskInput->clear();      // Clear the input field
            } else {
                QMessageBox::warning(this, "Warning", "Could not add task to database!");
            }
        } else {
            QMessageBox::warning(this, "Warning", "Task cannot be empty!");
        }
    }

    void removeTask() {
        QListWidgetItem *selectedItem = taskList->currentItem();
        if (selectedItem) {
            int id = taskList->currentRow() + 1; // Assuming IDs start from 1
            if (db->removeTask(id)) {
                delete selectedItem; // Remove the selected task
            } else {
                QMessageBox::warning(this, "Warning", "Could not remove task from database!");
            }
        } else {
            QMessageBox::warning(this, "Warning", "No task selected!");
        }
    }

private:
    void loadTasks() {
        QList<QPair<int, QString>> tasks = db->getTasks();
        for (const auto &task : tasks) {
            taskList->addItem(task.second); // Add each task to the list
        }
    }

    QListWidget *taskList;
    QLineEdit *taskInput;
    Database *db;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TodoApp window;
    window.show();

    return app.exec();
}



#include "qtit.moc" // Required for Q_OBJECT macro to work with CMake