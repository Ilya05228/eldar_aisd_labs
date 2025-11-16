#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <functional>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

struct Tree {
    int info;
    std::string text;
    Tree* left = nullptr;
    Tree* right = nullptr;
    Tree(int i, std::string t) : info(i), text(std::move(t)) {}
};

class TreeManager {
    Tree* root = nullptr;

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

    void clear() { deleteTree(root); root = nullptr; }

    Tree* addNode(int key, std::string txt) {
        if (!root) {
            root = new Tree(key, std::move(txt));
            return root;
        }
        Tree* cur = root;
        while (true) {
            if (key == cur->info) return cur;
            if (key < cur->info) {
                if (!cur->left) {
                    cur->left = new Tree(key, std::move(txt));
                    return cur->left;
                }
                cur = cur->left;
            } else {
                if (!cur->right) {
                    cur->right = new Tree(key, std::move(txt));
                    return cur->right;
                }
                cur = cur->right;
            }
        }
    }

    void readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;
        clear();
        int key;
        std::string line, text;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> key) {
                std::getline(iss, text);
                if (!text.empty() && text[0] == ' ') text.erase(0, 1);
                addNode(key, text);
            }
        }
    }

    void writeToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return;
        std::function<void(Tree*)> rec = [&](Tree* n) {
            if (!n) return;
            rec(n->left);
            rec(n->right);
            file << n->info << " " << n->text << "\n";
        };
        rec(root);
    }

    void addFromDialog(QWidget* parent) {
        bool ok1, ok2;
        int key = QInputDialog::getInt(parent, "Добавить узел", "Ключ:", 0, -1000, 1000, 1, &ok1);
        if (!ok1) return;
        QString qtext = QInputDialog::getText(parent, "Добавить узел", "Текст:", QLineEdit::Normal, "", &ok2);
        if (!ok2) return;
        addNode(key, qtext.toStdString());
    }

    Tree* getRoot() const { return root; }

    void displayTree(QTextEdit* view) const {
        view->clear();
        if (!root) { view->setText("Дерево пусто"); return; }
        buildTreeView(root, view);
    }
};

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

    void saveTree(Tree* node) {
        if (!node || !open()) return;
        QSqlQuery q(db);
        q.exec("DELETE FROM Derevo;");
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

    void loadTree(TreeManager* tm) {
        if (!open()) return;
        tm->clear();
        QSqlQuery q("SELECT id, name FROM Derevo ORDER BY id", db);
        while (q.next()) {
            int key = q.value(0).toInt();
            std::string txt = q.value(1).toString().toStdString();
            tm->addNode(key, txt);
        }
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

class MainWindow : public QMainWindow {
    Q_OBJECT
    TreeManager tree;
    Database db;
    QTableView* table;
    QStandardItemModel* model;
    QTextEdit* treeView;

    QPushButton *btnReadFile, *btnWriteFile, *btnReadDb, *btnWriteDb, *btnAdd, *btnRefresh;

public:
    explicit MainWindow(QWidget* parent = nullptr) : QMainWindow(parent), db(this) {
        auto* central = new QWidget(this);
        auto* layout = new QVBoxLayout(central);

        auto* btnLayout = new QHBoxLayout();
        btnReadFile = new QPushButton("Читать из файла");
        btnWriteFile = new QPushButton("Записать в файл");
        btnReadDb = new QPushButton("Читать из БД");
        btnWriteDb = new QPushButton("Записать в БД");
        btnAdd = new QPushButton("Добавить узел");
        btnRefresh = new QPushButton("Обновить");
        btnLayout->addWidget(btnReadFile);
        btnLayout->addWidget(btnWriteFile);
        btnLayout->addWidget(btnReadDb);
        btnLayout->addWidget(btnWriteDb);
        btnLayout->addWidget(btnAdd);
        btnLayout->addWidget(btnRefresh);
        layout->addLayout(btnLayout);

        treeView = new QTextEdit(this);
        treeView->setReadOnly(true);
        treeView->setFont(QFont("Courier", 10));
        layout->addWidget(treeView);

        table = new QTableView(this);
        model = new QStandardItemModel(this);
        model->setHorizontalHeaderLabels({"id", "name"});
        table->setModel(model);
        layout->addWidget(table);

        setCentralWidget(central);
        resize(800, 600);
        setWindowTitle("ЛР6");

        db.createTable();
        tree.readFromFile("input_tree.txt");
        db.saveTree(tree.getRoot());
        updateAll();

        connect(btnReadFile, &QPushButton::clicked, this, &MainWindow::onReadFile);
        connect(btnWriteFile, &QPushButton::clicked, this, &MainWindow::onWriteFile);
        connect(btnReadDb, &QPushButton::clicked, this, &MainWindow::onReadDb);
        connect(btnWriteDb, &QPushButton::clicked, this, &MainWindow::onWriteDb);
        connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddNode);
        connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::updateAll);
    }

private slots:
    void onReadFile() {
        QString fn = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Текстовые файлы (*.txt)");
        if (fn.isEmpty()) return;
        tree.readFromFile(fn.toStdString());
        updateAll();
    }

    void onWriteFile() {
        QString fn = QFileDialog::getSaveFileName(this, "Сохранить файл", "output_tree.txt", "Текстовые файлы (*.txt)");
        if (fn.isEmpty()) return;
        tree.writeToFile(fn.toStdString());
        QMessageBox::information(this, "Успех", "Дерево сохранено в файл");
    }

    void onReadDb() {
        db.loadTree(&tree);
        updateAll();
    }

    void onWriteDb() {
        db.saveTree(tree.getRoot());
        QMessageBox::information(this, "Успех", "Дерево сохранено в БД");
        updateAll();
    }

    void onAddNode() {
        tree.addFromDialog(this);
        updateAll();
    }

    void updateAll() {
        tree.displayTree(treeView);
        db.loadToModel(model);
        table->resizeColumnsToContents();
    }
};

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
