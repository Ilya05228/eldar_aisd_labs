#include <iostream>
#define NMAX 10
using namespace std;
#ifdef _WIN32
#include <windows.h>
#endif
void pechMatr(int g1[NMAX][NMAX], int n)
{
    cout << "Введите матрицу смежности (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> g1[i][j];
        }
    }
}

bool pred(int g1[NMAX][NMAX], int n, int i)
{
    for (int j = 0; j < n; j++)
    {
        if (g1[j][i] == 1)
        {
            return true;
        }
    }
    return false;
}

bool preem(int g1[NMAX][NMAX], int n, int i)
{
    for (int j = 0; j < n; j++)
    {
        if (g1[i][j] == 1)
        {
            return true;
        }
    }
    return false;
}

void poisk(int g1[NMAX][NMAX], int n)
{
    cout << "Вершины с предшественниками и преемниками: ";
    bool nashli = false;
    for (int i = 0; i < n; i++)
    {
        if (pred(g1, n, i) && preem(g1, n, i))
        {
            cout << i << " ";
            nashli = true;
        }
    }
    if (!nashli)
    {
        cout << "Нет таких вершин";
    }
    cout << endl;
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    int n;
    int g1[NMAX][NMAX];

    cout << "Введите количество вершин (n <= " << NMAX << "): ";
    cin >> n;

    if (n > NMAX || n < 1)
    {
        cout << "Недопустимое количество вершин!" << endl;
        return 1;
    }

    pechMatr(g1, n);

    poisk(g1, n);

    return 0;
}
