#include <iostream>
#include <string>
#include <stack>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
    string expr;
    cout << "Введите скобочное выражение (окончание пробел): ";
    getline(cin, expr);

    stack<int> stk;
    for (int i = 0; i < expr.length(); ++i)
    {
        if (expr[i] == '(')
        {
            stk.push(i + 1);
        }
        else if (expr[i] == ')')
        {
            if (stk.empty())
            {
                cout << "Ошибка: лишняя закрывающая скобка на " << (i + 1) << endl;
                return 1;
            }
            int open = stk.top();
            stk.pop();
            cout << open << " " << (i + 1) << endl;
        }
    }

    if (!stk.empty())
    {
        cout << "Ошибка: лишняя открывающая скобка" << endl;
    }

    system("pause");
    return 0;
}