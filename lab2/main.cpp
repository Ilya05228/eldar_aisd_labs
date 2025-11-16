#include <iostream>
#include <vector>
#include <string>

struct Stack
{
    std::vector<int> data;
    int top;

    Stack() : top(-1) {}

    void push(int pos)
    {
        ++top;
        if (top >= data.size())
            data.push_back(pos);
        else
            data[top] = pos;
    }

    int pop()
    {
        if (top < 0)
            return -1;
        return data[top--];
    }

    bool empty() const { return top < 0; }
};

void printBracketPairs()
{
    Stack stk;
    std::string expr;
    std::getline(std::cin, expr);
    int pos = 1;
    for (char ch : expr)
    {
        if (ch == '(')
            stk.push(pos);
        else if (ch == ')')
        {
            int open = stk.pop();
            if (open == -1)
            {
                std::cout << "Ошибка: лишняя закрывающая скобка\n";
                return;
            }
            std::cout << open << " " << pos << '\n';
        }
        ++pos;
    }
    if (!stk.empty())
        std::cout << "Ошибка: не закрыты скобки\n";
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    std::cout << "Выражение (пробел в конце): ";
    printBracketPairs();
}
// (a+(b+c)/(a-b)*(x+(y+2)*3))/2