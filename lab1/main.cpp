#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

struct Node
{
    std::string id;
    Node *next;
    Node(const std::string &val) : id(val), next(nullptr) {}
};

Node *createListFromInput(int n)
{
    if (n <= 0)
        return nullptr;

    std::cout << "Введите " << n << " идентификаторов (длина <= 8 символов): ";
    std::string val;
    std::cin >> val;

    Node *head = new Node(val);
    Node *temp = head;

    for (int i = 1; i < n; i++)
    {
        std::cin >> val;
        temp->next = new Node(val);
        temp = temp->next;
    }
    return head;
}

int countStartingWith(Node *head, char letter)
{
    int count = 0;
    while (head)
    {
        if (!head->id.empty() && head->id[0] == letter)
            count++;
        head = head->next;
    }
    return count;
}

int countAfter(Node *head, const std::string &target)
{
    int count = 0;
    bool found = false;
    while (head)
    {
        if (found)
            count++;
        if (head->id == target)
            found = true;
        head = head->next;
    }
    return found ? count : -1;
}

int countBefore(Node *head, const std::string &target)
{
    int count = 0;
    while (head)
    {
        if (head->id == target)
            return count;
        count++;
        head = head->next;
    }
    return -1;
}

void printList(Node *head)
{
    while (head)
    {
        std::cout << head->id << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

void deleteList(Node *head)
{
    while (head)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    int n;
    std::cout << "Введите количество идентификаторов: ";
    std::cin >> n;
    Node *head = createListFromInput(n);

    std::cout << "Исходный список: ";
    printList(head);

    // 3а: Количество идентификаторов, начинающихся с заданной буквы
    char letter;
    std::cout << "Введите букву для подсчета начинающихся с неё: ";
    std::cin >> letter;
    int countA = countStartingWith(head, letter);
    std::cout << "Количество идентификаторов, начинающихся с '" << letter << "': " << countA << std::endl;

    // 3б: Количество идентификаторов, следующих после заданного
    std::string targetAfter;
    std::cout << "Введите идентификатор для подсчета после него: ";
    std::cin >> targetAfter;
    int countB = countAfter(head, targetAfter);
    if (countB == -1)
        std::cout << "Идентификатор '" << targetAfter << "' не найден в списке." << std::endl;
    else
        std::cout << "Количество идентификаторов после '" << targetAfter << "': " << countB << std::endl;

    // 3в: Количество идентификаторов, следующих до заданного
    std::string targetBefore;
    std::cout << "Введите идентификатор для подсчета до него: ";
    std::cin >> targetBefore;
    int countC = countBefore(head, targetBefore);
    if (countC == -1)
        std::cout << "Идентификатор '" << targetBefore << "' не найден в списке." << std::endl;
    else
        std::cout << "Количество идентификаторов до '" << targetBefore << "': " << countC << std::endl;

    deleteList(head);
    system("pause");
    return 0;
}