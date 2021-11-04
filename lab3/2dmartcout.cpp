#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

template <typename Xtype>
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}

int main()
{
    const int m = 3, n = 3;
    double matr[m][n];
    fstream file;
    file.open("matr");
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int i = 0; i < n; i++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - ElemSize(i) / 2 - ElemSize(i) % 2) << "" << (i + 1) << setw(6 - ElemSize(i) / 2) << '|';
    }
    cout << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < m; i++)
    {
        cout << "| " << (i + 1) << " |";
        for (int j = 0; j < n; j++)
        {
            file >> matr[i][j];
            cout << setw(10) << right << scientific << setprecision(2) << matr[i][j] << " |";
        }
        cout << endl
             << skipstr.str() << '\n'
    }
    return 0;
}