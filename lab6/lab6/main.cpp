#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

// === Tree Node ===
struct Tree {
    int info;
    std::string text;
    Tree* left = nullptr;
    Tree* right = nullptr;
    Tree(int i, std::string t) : info(i), text(std::move(t)) {}
};

// === Tree Manager ===
class TreeManager {
    Tree* root = nullptr;

    static Tree* addNode(Tree* node, int key, std::string txt) {
        if (!node) return new Tree(key, std::move(txt));
        if (key == node->info) return node;
        if (key < node->info)
            node->left = addNode(node->left, key, std::move(txt));
        else
            node->right = addNode(node->right, key, std::move(txt));
        return node;
    }

    static void deleteTree(Tree* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    static void buildTreeView(Tree* node, QTextEdit* view, int level = 0) {
        if (!node) return;
        buildTreeView(node->right, view, level + 1);
        view->append(QString("    ").repeated(level) + QString::number(node->info) + " (" + QString::fromStdString(node->text) + ")");
        buildTreeView(node->left, view, level + 1);
    }

public:
    ~TreeManager() { deleteTree(root); }

    void buildFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            qWarning() << "Файл не найден:" << QString::fromStdString(filename);
            return;
        }
        int key;
        std::string line, text;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> key) {
                std::getline(iss, text);
                if (!text.empty() && text[0] == ' ') text.erase(0, 1);
                root = addNode(root, key, text);
            }
        }
    }

    void addFromDialog(QWidget* parent) {
        bool ok1, ok2;
        int key = QInputDialog::getInt(parent, "Добавить узел", "Ключ:", 0, -1000, 1000, 1, &ok1);
        if (!ok1) return;
        QString qtext = QInputDialog::getText(parent, "Добавить узел", "Текст:", QLineEdit::Normal, "", &ok2);
        if (!ok2) return;
        root = addNode(root, key, qtext.toStdString());
    }

    Tree* getRoot() const { return root; }

    void displayTree(QTextEdit* view) const {
        view->clear();
        if (!root) {
            view->setText("Дерево пусто");
            return;
        }
        buildTreeView(root, view);
    }
};

// === Database Manager ===
class Database : public QObject {
    Q_OBJECT
    QSqlDatabase db;

    bool open() { return db.open(); }
    void close() { db.close(); }

public:
    explicit Database(QObject* parent = nullptr) : QObject(parent) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("tree.db");
    }

    void createTable() {
        if (!open()) return;
        QSqlQuery q(db);
        q.exec("CREATE TABLE IF NOT EXISTS Derevo (id INTEGER PRIMARY KEY, name TEXT);");
        close();
    }

    void insertTree(Tree* node) {
        if (!node || !open()) return;
        QSqlQuery q(db);
        q.exec("DELETE FROM Derevo;"); // Очистить перед вставкой
        q.prepare("INSERT INTO Derevo (id, name) VALUES (?, ?);");
        std::function<void(Tree*)> rec = [&](Tree* n) {
            if (!n) return;
            q.addBindValue(n->info);
            q.addBindValue(QString::fromStdString(n->text));
            q.exec();
            rec(n->left);
            rec(n->right);
        };
        rec(node);
        close();
    }

    void loadToModel(QStandardItemModel* model) {
        if (!open()) return;
        model->removeRows(0, model->rowCount());
        QSqlQuery q("SELECT id, name FROM Derevo ORDER BY id", db);
        int row = 0;
        while (q.next()) {
            model->setItem(row, 0, new QStandardItem(q.value(0).toString()));
            model->setItem(row, 1, new QStandardItem(q.value(1).toString()));
            ++row;
        }
        close();
    }
};

// === Main Window ===
class MainWindow : public QMainWindow {
    Q_OBJECT
    TreeManager tree;
    Database db;
    QTableView* table;
    QStandardItemModel* model;
    QTextEdit* treeView;
    QPushButton *btnAdd, *btnSave, *btnLoad, *btnRefresh;

public:
    explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent), db(this) {
        // === UI ===
        auto* central = new QWidget(this);
        auto* layout = new QVBoxLayout(central);

        // Кнопки
        auto* btnLayout = new QHBoxLayout();
        btnAdd = new QPushButton("Добавить узел");
        btnSave = new QPushButton("Сохранить в БД");
        btnLoad = new QPushButton("Загрузить из файла");
        btnRefresh = new QPushButton("Обновить");
        btnLayout->addWidget(btnAdd);
        btnLayout->addWidget(btnSave);
        btnLayout->addWidget(btnLoad);
        btnLayout->addWidget(btnRefresh);
        layout->addLayout(btnLayout);

        // Визуализация дерева
        treeView = new QTextEdit(this);
        treeView->setReadOnly(true);
        treeView->setFont(QFont("Courier", 10));
        layout->addWidget(treeView);

        // Таблица
        table = new QTableView(this);
        model = new QStandardItemModel(this);
        model->setHorizontalHeaderLabels({"id", "name"});
        table->setModel(model);
        layout->addWidget(table);

        setCentralWidget(central);
        resize(700, 600);
        setWindowTitle("ЛР6: Дерево + SQLite + Визуализация");

        // === Инициализация ===
        tree.buildFromFile("input_tree.txt");
        db.createTable();
        db.insertTree(tree.getRoot());
        updateAll();

        // === Сигналы ===
        connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddNode);
        connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveToDb);
        connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
        connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::updateAll);
    }

private slots:
    void onAddNode() {
        tree.addFromDialog(this);
        updateAll();
    }

    void onSaveToDb() {
        db.insertTree(tree.getRoot());
        QMessageBox::information(this, "Успех", "Дерево сохранено в БД");
        updateAll();
    }

    void onLoadFromFile() {
        tree.buildFromFile("input_tree.txt");
        updateAll();
    }

    void updateAll() {
        tree.displayTree(treeView);
        db.loadToModel(model);
        table->resizeColumnsToContents();
    }
};

// === Main ===
int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
