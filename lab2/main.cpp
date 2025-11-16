#include <iostream>
#include <stack>
#include <string>

void printBracketPairs(const std::string &expr)
{
    std::stack<int> stk;
    std::cout << "Результат:\n";
    int pos = 1;
    for (char ch : expr)
    {
        if (ch == '(')
            stk.push(pos);
        else if (ch == ')')
        {
            if (stk.empty())
            {
                std::cout << "Ошибка: лишняя закрывающая скобка\n";
                return;
            }
            int open = stk.top();
            stk.pop();
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
    std::string expr;
    std::cout << "Выражение (пробел в конце): ";
    std::getline(std::cin, expr);
    printBracketPairs(expr);
#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
// (a+(b+c)/(a-b)*(x+(y+2)*3))/2