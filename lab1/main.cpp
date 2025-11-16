#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

struct Node
{
    int id;
    Node *next;
    Node(int val) : id(val), next(nullptr) {}
};

Node *createListFromInput(int n)
{
    if (n <= 0)
        return nullptr;

    std::cout << "Введите " << n << " значений для списка: ";
    int val;
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

Node *removeFirstTwo(Node *head)
{
    for (int i = 0; i < 2 && head != nullptr; i++)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
    return head;
}

void replaceLast(Node *head, int newId)
{
    if (!head)
        return;
    while (head->next)
        head = head->next;
    head->id = newId;
}

int countElements(Node *head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return count;
}

int countBefore(Node *head, int target)
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
void copyFirstK(Node *head, int *A, int k)
{
    for (int i = 0; i < k && head; i++)
    {
        A[i] = head->id;
        head = head->next;
    }
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

    // 1. Создание списка
    int n;
    std::cout << "Введите количество элементов списка: ";
    std::cin >> n;
    Node *head = createListFromInput(n);

    std::cout << "Исходный список: ";
    printList(head);

    // 2. Удаление первых двух элементов
    head = removeFirstTwo(head);
    std::cout << "После удаления первых двух элементов: ";
    printList(head);

    // 3 Количество элементов до заданного идентификатора
    int target;
    std::cout << "Введите идентификатор, чтобы узнать количество элементов до него: ";
    std::cin >> target;
    int before = countBefore(head, target);
    if (before == -1)
        std::cout << "Элемент не найден в списке." << std::endl;
    else
        std::cout << "Количество элементов до " << target << ": " << before << std::endl;

    // 4. Замена последнего элемента
    int newId;
    std::cout << "Введите новое значение для последнего элемента: ";
    std::cin >> newId;
    replaceLast(head, newId);
    std::cout << "После замены последнего элемента: ";
    printList(head);

    // 5. Копирование первых k элементов в массив
    int k;
    std::cout << "Введите k (сколько элементов скопировать в массив): ";
    std::cin >> k;
    int A[100];
    int total = countElements(head);
    if (total >= k)
    {
        copyFirstK(head, A, k);
        std::cout << "Первые " << k << " элементов массива A: ";
        for (int i = 0; i < k; i++)
            std::cout << A[i] << " ";
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Список содержит меньше чем " << k << " элементов." << std::endl;
    }

    deleteList(head);
    return 0;
}
