#include <iostream>
#include <string>
#include <list>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct El
{
    int key;
    int info;
    El *next;
    El(int k, int i) : key(k), info(i), next(nullptr) {}
};

class HashTable
{
private:
    El **table;
    int size;

    int hashFunc(int key)
    {
        return key % size;
    }

public:
    HashTable(int m)
    {
        size = m;
        table = new El *[size];
        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }
    void add(int key, int info)
    {
        int index = hashFunc(key);
        El *newNode = new El(key, info);
        if (table[index] == nullptr)
        {
            table[index] = newNode;
        }
        else
        {
            El *current = table[index];
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newNode;
        }
        cout << "Добавлен элемент с ключом " << key << " в индекс " << index << endl;
    }

    bool search(int key, int &info)
    {
        int index = hashFunc(key);
        El *current = table[index];
        while (current != nullptr)
        {
            if (current->key == key)
            {
                info = current->info;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool remove(int key)
    {
        int index = hashFunc(key);
        El *current = table[index];
        El *prev = nullptr;

        if (current == nullptr)
        {
            return false;
        }

        if (current->key == key)
        {
            table[index] = current->next;
            delete current;
            cout << "Удалён элемент с key " << key << endl;
            return true;
        }

        while (current != nullptr && current->key != key)
        {
            prev = current;
            current = current->next;
        }

        if (current == nullptr)
        {
            return false;
        }
        prev->next = current->next;
        delete current;
        cout << "Удалён элемент с ключом " << key << endl;
        return true;
    }

    void update(int key, int newInfo)
    {
        int index = hashFunc(key);
        El *current = table[index];
        while (current != nullptr)
        {
            if (current->key == key)
            {
                current->info = newInfo;
                cout << "Обновлён элемент с ключом " << key << ", новое значение: " << newInfo << endl;
                return;
            }
            current = current->next;
        }
        cout << "Ключ " << key << " не найден для обновления" << endl;
    }

    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "Индекс " << i << ": ";
            El *current = table[i];
            while (current != nullptr)
            {
                cout << "[" << current->key << ", " << current->info << "] -> ";
                current = current->next;
            }
            cout << "nullptr" << endl;
        }
    }
};

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    int size;
    cout << "Введите размер хеш-таблицы: ";
    cin >> size;
    HashTable ht(size);

    while (true)
    {
        cout << "\nМеню:\n";
        cout << "1. Печать хеш-таблицы\n";
        cout << "2. Добавить элемент\n";
        cout << "3. Удалить элемент\n";
        cout << "4. Изменить элемент\n";
        cout << "5. Поиск элемента\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;

        if (choice == 0)
        {
            cout << "Выход из программы.\n";
            break;
        }

        switch (choice)
        {
        case 1: // Печать
            cout << "\nХеш-таблица:\n";
            ht.display();
            break;

        case 2: // Добавление
            int key, info;
            cout << "Введите ключ: ";
            cin >> key;
            cout << "Введите значение: ";
            cin >> info;
            ht.add(key, info);
            break;

        case 3: // Удаление
            cout << "Введите ключ для удаления: ";
            cin >> key;
            if (!ht.remove(key))
            {
                cout << "Ключ " << key << " не найден\n";
            }
            break;

        case 4: // Изменение
            cout << "Введите ключ для изменения: ";
            cin >> key;
            cout << "Введите новое значение: ";
            cin >> info;
            ht.update(key, info);
            break;

        case 5: // Поиск
            cout << "Введите ключ для поиска: ";
            cin >> key;
            if (ht.search(key, info))
            {
                cout << "Ключ " << key << " найден, значение: " << info << endl;
            }
            else
            {
                cout << "Ключ " << key << " не найден\n";
            }
            break;

        default:
            cout << "Неверный выбор, попробуйте снова\n";
        }
    }

    return 0;
}