#include <iostream>
#include <windows.h>

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
    return 0;
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

CRITICAL_SECTION g_cs;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    InitializeCriticalSection(&g_cs);

    int n;
    std::cout << "Введите количество элементов списка: ";
    std::cin >> n;

    Node *head = createListFromInput(n);

    std::cout << "Исходный список: ";
    printList(head);

    head = removeFirstTwo(head);
    std::cout << "После удаления первых двух элементов: ";
    printList(head);

    int givenId;
    std::cout << "Введите новое значение для последнего элемента: ";
    std::cin >> givenId;

    replaceLast(head, givenId);
    std::cout << "После замены последнего элемента на " << givenId << ": ";
    printList(head);

    int numElements = countElements(head);
    std::cout << "Количество элементов в списке: " << numElements << std::endl;

    int k;
    std::cout << "Введите k (сколько элементов скопировать в массив): ";
    std::cin >> k;

    int A[100];
    if (numElements >= k)
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

    int target;
    std::cout << "Введите идентификатор, чтобы узнать количество элементов ДО него: ";
    std::cin >> target;
    std::cout << "Количество элементов до " << target << ": "
              << countBefore(head, target) << std::endl;

    deleteList(head);
    DeleteCriticalSection(&g_cs);
    return 0;
}
