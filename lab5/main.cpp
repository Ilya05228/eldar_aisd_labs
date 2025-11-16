#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct Tree
{
    int info;
    string text;
    Tree *Left;
    Tree *Right;
};

Tree *List(int i, string txt)
{
    Tree *t = new Tree;
    t->info = i;
    t->text = txt;
    t->Left = t->Right = NULL;
    return t;
}

Tree *Make(Tree *Root)
{
    Tree *Prev, *t;
    int b, find;
    string txt;

    if (Root == NULL)
    {
        cout << "\nВведите значение корня: ";
        cin >> b;
        cout << "Введите текст для корня: ";
        cin >> txt;
        Root = List(b, txt);
    }

    while (1)
    {
        cout << "\nВведите значение (отрицательное для выхода): ";
        cin >> b;
        if (b < 0)
            break;
        cout << "Введите текст: ";
        cin >> txt;

        t = Root;
        find = 0;
        while (t && !find)
        {
            Prev = t;
            if (b == t->info)
                find = 1;
            else if (b < t->info)
                t = t->Left;
            else
                t = t->Right;
        }
        if (!find)
        {
            t = List(b, txt);
            if (b < Prev->info)
                Prev->Left = t;
            else
                Prev->Right = t;
        }
    }
    return Root;
}

Tree *AddNode(Tree *Root, int key, string txt)
{
    if (Root == NULL)
        return List(key, txt);

    Tree *t = Root, *Prev;
    while (t)
    {
        Prev = t;
        if (key == t->info)
        {
            cout << "Ключ уже существует!\n";
            return Root;
        }
        else if (key < t->info)
            t = t->Left;
        else
            t = t->Right;
    }
    t = List(key, txt);
    if (key < Prev->info)
        Prev->Left = t;
    else
        Prev->Right = t;
    return Root;
}

Tree *Del(Tree *Root, int key)
{
    Tree *Del, *Prev_Del, *R, *Prev_R;
    Del = Root;
    Prev_Del = NULL;

    while (Del != NULL && Del->info != key)
    {
        Prev_Del = Del;
        if (Del->info > key)
            Del = Del->Left;
        else
            Del = Del->Right;
    }
    if (Del == NULL)
    {
        cout << "\n NO Key!";
        return Root;
    }

    if (Del->Right == NULL)
        R = Del->Left;
    else if (Del->Left == NULL)
        R = Del->Right;
    else
    {
        Prev_R = Del;
        R = Del->Left;
        while (R->Right != NULL)
        {
            Prev_R = R;
            R = R->Right;
        }
        if (Prev_R == Del)
            R->Right = Del->Right;
        else
        {
            R->Right = Del->Right;
            Prev_R->Right = R->Left;
            R->Left = Prev_R;
        }
    }
    if (Del == Root)
        Root = R;
    else if (Del->info < Prev_Del->info)
        Prev_Del->Left = R;
    else
        Prev_Del->Right = R;

    cout << "\n Delete element \n"
         << Del->info;
    delete Del;
    return Root;
}

void View_LRR(Tree *t, int level)
{
    if (t)
    {
        View_LRR(t->Right, level + 1);
        cout << t->info << " (" << t->text << ")" << endl;
        View_LRR(t->Left, level + 1);
    }
}

void View_RLR(Tree *t)
{
    if (t)
    {
        cout << t->info << " (" << t->text << ") ";
        View_RLR(t->Left);
        View_RLR(t->Right);
    }
}

void View_LRR_Post(Tree *t)
{
    if (t)
    {
        View_LRR_Post(t->Left);
        View_LRR_Post(t->Right);
        cout << t->info << " (" << t->text << ") ";
    }
}

void View_Even(Tree *t)
{
    if (t)
    {
        View_Even(t->Left);
        if (t->info % 2 == 0)
            cout << "Четный ключ: " << t->info << " (" << t->text << ")" << endl;
        View_Even(t->Right);
    }
}

void Del_All(Tree *t)
{
    if (t != NULL)
    {
        Del_All(t->Left);
        Del_All(t->Right);
        delete t;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    Tree *Root = NULL;
    int choice, key;
    string txt;

    while (true)
    {
        cout << "\nДоступные команды:\n";
        cout << "1) Создать дерево\n";
        cout << "2) Добавить узел\n";
        cout << "3) Удалить узел\n";
        cout << "4) Показать дерево (слева-корень-справа)\n";
        cout << "5) Показать дерево (корень-слева-справа)\n";
        cout << "6) Показать дерево (слева-справа-корень)\n";
        cout << "7) Показать четные ключи\n";
        cout << "8) Очистить дерево\n";
        cout << "9) Выход\n";
        cout << "Выберите команду (1-9): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (Root != NULL)
            {
                Del_All(Root);
                Root = NULL;
            }
            Root = Make(Root);
            break;

        case 2:
            cout << "Введите ключ: ";
            cin >> key;
            cout << "Введите текст: ";
            cin >> txt;
            Root = AddNode(Root, key, txt);
            break;

        case 3:
            cout << "Введите ключ для удаления: ";
            cin >> key;
            Root = Del(Root, key);
            break;

        case 4:
            if (Root == NULL)
                cout << "\nДерево пустое!\n";
            else
            {
                cout << "\nОбход слева-корень-справа:\n";
                View_LRR(Root, 0);
            }
            break;

        case 5:
            if (Root == NULL)
                cout << "\nДерево пустое!\n";
            else
            {
                cout << "\nОбход корень-слева-справа: ";
                View_RLR(Root);
                cout << endl;
            }
            break;

        case 6:
            if (Root == NULL)
                cout << "\nДерево пустое!\n";
            else
            {
                cout << "\nОбход слева-справа-корень: ";
                View_LRR_Post(Root);
                cout << endl;
            }
            break;

        case 7:
            if (Root == NULL)
                cout << "\nДерево пустое!\n";
            else
            {
                cout << "\nЧетные ключи:\n";
                View_Even(Root);
            }
            break;

        case 8:
            if (Root != NULL)
            {
                Del_All(Root);
                Root = NULL;
                cout << "Дерево очищено!\n";
            }
            else
                cout << "Дерево уже пустое!\n";
            break;

        case 9:
            if (Root != NULL)
                Del_All(Root);
            cout << "Программа завершена.\n";
            return 0;

        default:
            cout << "Неверная команда! Выберите число от 1 до 9.\n";
        }
    }
    return 0;
}
