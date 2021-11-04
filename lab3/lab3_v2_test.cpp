#define LINUX
#define v1

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>

#ifdef LINUX
#include <stdlib.h>
#include <termios.h>
#else
#include <windows.h>
#include <conio.h>
#include <sstream>
#endif

typedef double d_arr;
using namespace std;

template <typename Xtype>
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
bool ReadFileName(string &FileAdress, ifstream &file)
{
    cout << "\nВведите имя файла(чтобы выйти *): ";
    getline(cin, FileAdress);
    if (FileAdress == "*")
        return true;
    file.open(FileAdress, ios_base::in);
    if (!(file.is_open()))
    {
        perror(("\nОшибка открытия файла с именем " + FileAdress).c_str());
        return ReadFileName(FileAdress, file);
    }
    return false;
}
void ErrCheck(const char &err, ifstream &file, const int pos, const int dsize_s, const int dsize_c)
{
    if (err == 'f')
    {
        string tmp;
        file.clear();
        file.seekg(pos);
        file >> tmp;
        file.close();
        cout << "Найдена ошибка в элементе номер " << (dsize_c + 1) << " на строке " << (dsize_s + 1)
             << "\nАбсолютная позиция в файле " << pos
             << "\nНеверное значение: \"" << tmp << '\"' << '\n';
        cout << "\nФайл содержит некорректные значения\n";
    }
    else if (err == 'e')
    {
        file.close();
        cout << "\nВ файле не оказалось значений, которые можно считать\n";
    }
    if (err == 'c')
    {
        file.close();
        cout << "\nНе правильный формат, количество элементов строки " << dsize_s << " не совпадает со стокой " << (dsize_s - 1) << '\n';
    }
    if (err == 'q')
    {
        file.close();
        cout << "\nНе правильный формат, матрица не кваратная\n";
    }
}
void MatrConditionCheck(ifstream &file, char &err, int &dsize_s, int &dsize_c, int &dsize_all_sim)
{
    int pos;
    d_arr dtemp;
    while (!(file >> ws).eof() && !err)
    {
        char tmp(0);
        pos = file.tellg();
        if (!(file >> dtemp) || (file.peek() != ' ' && file.peek() != '\n' && file.peek() != '\t' && file.peek() != EOF))
        {
            err = 'f';
            break;
        }
        dsize_c++;

        while (tmp != EOF)
        {
            tmp = file.get();
            switch (tmp)
            {
            case '\t':
            case ' ':
                break;
            case EOF:
            case '\n':
                if (dsize_c)
                {
                    dsize_s++;
                    dsize_all_sim += dsize_c;
                    if (dsize_all_sim % dsize_c)
                        err = 'c';
                    if (tmp != EOF)
                        dsize_c = 0;
                }
                break;
            default:
                file.unget();
                tmp = EOF;
            }
        }
    }
    if (!dsize_all_sim)
        err = 'e';
    if (dsize_s != dsize_c)
        err = 'q';
    if (err)
    {
        ErrCheck(err, file, pos, dsize_s, dsize_c);
    }
}
void CreateDynamicArr(ifstream &file, d_arr **&p_buf_double, const int dsize_s, const int dsize_c)
{
    p_buf_double = new (nothrow) d_arr *[dsize_s];
    if (p_buf_double == nullptr)
    {
        file.close();
        cout << "\nОшибка выделения оперативной памяти\n";
    }
    for (int i = 0; i < dsize_c; i++)
    {
        *(p_buf_double + i) = new (nothrow) d_arr[dsize_c];
        if (*(p_buf_double + i) == nullptr)
        {
            file.close();
            cout << "\nОшибка выделения оперативной памяти\n";
            for (i--; 0 <= i; i--)
            {
                delete[] p_buf_double[i];
                *(p_buf_double + i) = nullptr;
            }
            delete[] p_buf_double;
            p_buf_double = nullptr;
            break;
        }
    }
}
void MatrCout(d_arr **buf_d_arr, const int &bufstr, const int &bufcolomn)
{
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int j = 0; j < bufcolomn; j++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - ElemSize(j) / 2 - ElemSize(j) % 2) << "" << (j + 1) << setw(6 - ElemSize(j) / 2) << '|';
    }
    cout << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < bufstr; i++)
    {
        cout << "| " << (i + 1) << " |";
        for (int j = 0; j < bufcolomn; j++)
        {
            cout << setw(10) << right << scientific << setprecision(2) << *(*(buf_d_arr + i) + j) << " |";
        }
        cout << endl
             << skipstr.str() << '\n';
    }
}
void FReadMatr(ifstream &file, d_arr **buf_a_arr, const int &bufstr, const int &bufcolomn)
{
    file.clear();
    int pos = file.tellg();
    file.seekg(0, ios::beg);
    for (int i = 0; i < bufstr; i++)
        for (int j = 0; j < bufcolomn; j++)
            file >> *(*(buf_a_arr + i) + j);
    file.clear();
    file.seekg(pos);
}
void ResultFunc(d_arr **p_buf_double, const int dsize_s, const int dsize_c)
{
    for (int i = 0, j = 0; i < dsize_s; i++, j++)
        if (*(*(p_buf_double + i) + j) < 0)
        {
            for (int k = 0; k < dsize_c; k++)
            {
                if (k == j)
                    continue;
                if (!(*(*p_buf_double + k)) && (*(*(p_buf_double + i) + k) > 0))
                {
                    cout << "\nЗамена столбца " << (i + 1) << " на строку " << (k + 1) << '\n';
                    d_arr dtemp;
                    for (int l = 0; l < dsize_c; l++)
                    {
                        dtemp = *(*(p_buf_double + i) + l);
                        *(*(p_buf_double + i) + l) = *(*(p_buf_double + l) + k);
                        *(*(p_buf_double + l) + k) = dtemp;
                    }
                    break;
                }
                if (dsize_c - 1 == k)
                {
                    cout << "\nНеудалось найти замену для " << (i + 1) << " строки\n";
                }
            }
        }
}
void MatrDel(d_arr **&p_buf_double, const int dsize_s)
{
    for (int i = 0; i < dsize_s; i++)
        delete[] * (p_buf_double + i);
    delete[] p_buf_double;
    p_buf_double = nullptr;
}

int main()
{
    d_arr **p_buf_double = nullptr;
    setlocale(LC_ALL, "ru");
    while (true)
    {
        ifstream file;
        string FileAdress, tempstr;
        if (ReadFileName(FileAdress, file))
            break;
        char err(0);
        cout << '\n';
        int dsize_s = 0, dsize_all_sim = 0, dsize_c = 0;
        d_arr dtemp = 0;
        bool power = true;
        cout << '+';
        MatrConditionCheck(file, err, dsize_s, dsize_c, dsize_all_sim);
        cout << '+';
        CreateDynamicArr(file, p_buf_double, dsize_s, dsize_c);
        cout << '+';
        FReadMatr(file, p_buf_double, dsize_s, dsize_c);
        cout << '+';
        MatrCout(p_buf_double, dsize_s, dsize_c);
        cout << '+';
        ResultFunc(p_buf_double, dsize_s, dsize_c);
        cout << '+';
        MatrCout(p_buf_double, dsize_s, dsize_c);
        cout << '+';
        MatrDel(p_buf_double, dsize_s);
        cout << '+';
    }
    return 0;
}