#include <QApplication>
#include <QListWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
class EditDialog : public QDialog {
public:
    EditDialog(QWidget *parent = nullptr) : QDialog(parent) {
        layout = new QVBoxLayout(this);
        contentEdit = new QTextEdit(this);
        contentEdit->setAcceptRichText(false);
        contentEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        contextEdit = new QLineEdit(this);
        buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

        layout->addWidget(contentEdit);
        layout->addWidget(contextEdit);
        layout->addWidget(buttons);

        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        setMinimumSize(800, 600);
    }

    void setValues(const QString &content, const QString &meta) {
        contentEdit->setPlainText(content);
        contextEdit->setText(meta);
    }

    QString getContent() const { return contentEdit->toPlainText(); }
    QString getMeta() const { return contextEdit->text(); }

private:
    QVBoxLayout *layout;
    QTextEdit *contentEdit;
    QLineEdit *contextEdit;
    QDialogButtonBox *buttons;
};

class MainWindow : public QListWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QListWidget(parent) {
        setupDatabase();
        loadData();
        
        connect(this, &QListWidget::itemDoubleClicked, this, &MainWindow::editItem);
    }

private:
    void setupDatabase() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("bookmark");
        db.setUserName("root");
        db.setPassword("yangbo");

        if (!db.open()) {
            QMessageBox::critical(nullptr, "Database Error", db.lastError().text());
            exit(1);
        }
    }

    void loadData() {
        clear();
        QSqlQuery query("SELECT id, content, context FROM bookmarks");
        while (query.next()) {
            QListWidgetItem *item = new QListWidgetItem(query.value(1).toString());
            item->setData(Qt::UserRole, query.value(0));   // Store ID
            item->setData(Qt::UserRole + 1, query.value(2)); // Store context
            addItem(item);
        }
    }

    void editItem(QListWidgetItem *item) {
        EditDialog dialog;
        dialog.setValues(item->text(), item->data(Qt::UserRole + 1).toString());
        
        if (dialog.exec() == QDialog::Accepted) {
            QSqlQuery query;
            query.prepare("UPDATE bookmarks SET content = :content, context = :context WHERE id = :id");
            query.bindValue(":content", dialog.getContent());
            query.bindValue(":context", dialog.getMeta());
            query.bindValue(":id", item->data(Qt::UserRole));
            
            if (!query.exec()) {
                QMessageBox::critical(this, "Error", query.lastError().text());
            } else {
                item->setText(dialog.getContent());
                item->setData(Qt::UserRole + 1, dialog.getMeta());
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication::addLibraryPath("/opt/homebrew/Cellar/qt-mysql/6.7.3/share/qt/plugins/sqldrivers");
    QApplication app(argc, argv);
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
    // Verify MySQL driver is available
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        QMessageBox::critical(nullptr, "Error", "MySQL driver not available!");
        return 1;
    }

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Database Editor");
    mainWindow.resize(800, 600);
    mainWindow.show();
    
    return app.exec();
}

#include "qt_mysql.moc"
//brew install qt-mysql