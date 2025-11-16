#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#endif

// === Дерево ===
struct Tree
{
    int info;
    std::string text;
    Tree *Left = nullptr;
    Tree *Right = nullptr;
};

Tree *List(int i, std::string txt)
{
    Tree *t = new Tree;
    t->info = i;
    t->text = txt;
    return t;
}

Tree *AddNode(Tree *Root, int key, std::string txt)
{
    if (!Root)
        return List(key, txt);
    Tree *t = Root, *Prev = nullptr;
    while (t)
    {
        Prev = t;
        if (key == t->info)
        {
            std::cout << "Ключ уже существует!\n";
            return Root;
        }
        t = (key < t->info) ? t->Left : t->Right;
    }
    t = List(key, txt);
    (key < Prev->info) ? Prev->Left = t : Prev->Right = t;
    return Root;
}

void Del_All(Tree *t)
{
    if (t)
    {
        Del_All(t->Left);
        Del_All(t->Right);
        delete t;
    }
}

// === TreeManager ===
class TreeManager
{
public:
    Tree *root = nullptr;
    ~TreeManager() { Del_All(root); }

    void buildFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Файл не найден: " << filename << std::endl;
            return;
        }
        int key;
        std::string txt;
        while (file >> key >> txt)
        {
            root = AddNode(root, key, txt);
        }
        file.close();
    }
};

// === Database ===
class Database : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase db;
    QSqlQuery query;

    Database(QObject *parent = nullptr) : QObject(parent)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("tree.db");
        query = QSqlQuery(db);
    }

    void createTable()
    {
        if (db.open())
        {
            query.exec("CREATE TABLE IF NOT EXISTS Derevo (id INTEGER PRIMARY KEY, name TEXT);");
            db.close();
        }
    }

    void insertTree(Tree *node)
    {
        if (!node)
            return;
        if (db.open())
        {
            query.prepare("INSERT OR IGNORE INTO Derevo (id, name) VALUES (?, ?);");
            query.addBindValue(node->info);
            query.addBindValue(QString::fromStdString(node->text));
            query.exec();
            insertTree(node->Left);
            insertTree(node->Right);
            db.close();
        }
    }
};

// === MainWindow ===
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    TreeManager *tree;
    Database *db;
    QTableView *view;
    QStandardItemModel *model;

    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        tree = new TreeManager();
        tree->buildFromFile("input_tree.txt");

        db = new Database();
        db->createTable();
        db->insertTree(tree->root);

        view = new QTableView(this);
        setCentralWidget(view);

        model = new QStandardItemModel(this);
        model->setHorizontalHeaderLabels({"id", "name"});

        if (db->db.open())
        {
            QSqlQuery q("SELECT id, name FROM Derevo ORDER BY id;", db->db);
            int row = 0;
            while (q.next())
            {
                model->setItem(row, 0, new QStandardItem(q.value(0).toString()));
                model->setItem(row, 1, new QStandardItem(q.value(1).toString()));
                ++row;
            }
            view->setModel(model);
            view->resizeColumnsToContents();
            db->db.close();
        }

        resize(500, 400);
        setWindowTitle("ЛР6: Дерево + SQLite");
    }

    ~MainWindow()
    {
        delete db;
        delete tree;
    }
};

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
