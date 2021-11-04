#define LINUX
//#define v1

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
typedef ifstream TreaD;
using namespace std;

template <typename Xtype>
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
char correct_input(TreaD &input_stream)
{
    d_arr dtmp;
    int pos = input_stream.tellg();
    if (!(input_stream >> dtmp) || (input_stream.peek() != ' ' && input_stream.peek() != '\n' && input_stream.peek() != '\t' && input_stream.peek() != EOF))
    {
        input_stream.seekg(pos);
        return 'f';
    }
    return 0;
}

string GetFileName()
{
    cout << "\nВведите имя файла(чтобы выйти *): ";
    string tmp;
    getline(cin, tmp);
    return tmp;
}
ifstream open_ifstream(string getname())
{
    while (true)
    {
        ifstream file(getname(), ios_base::in);
        if (!(file.is_open()))
        {
            perror("\nОшибка открытия файла");
            continue;
        }
        return file;
    }
}
void failbit_err(ifstream &file, const int pos, const int dsize_s, const int dsize_c)
{
    string tmp;
    file.clear();
    file.seekg(pos);
    file >> tmp;
    //file.close();
    cout << "Найдена ошибка в элементе номер " << (dsize_c + 1) << " на строке " << (dsize_s + 1)
         << "\nАбсолютная позиция в файле " << pos
         << "\nНеверное значение: \"" << tmp << '\"' << '\n'
         << "\nФайл содержит некорректные значения\n";
}
void empty_file_err(ifstream &file)
{
    file.close();
    cout << "\nВ файле не оказалось значений, которые можно считать\n";
}
void strings_no_equality_err(ifstream &file, const int dsize_s)
{
    //file.close();
    cout << "\nНе правильный формат, количество элементов строки " << dsize_s << " не совпадает со стокой " << (dsize_s - 1) << '\n';
}
void not_square_matr_err(ifstream &file)
{
    //file.close();
    cout << "\nНе правильный формат, матрица не кваратная\n";
}
/*
void failbit_err(const char& err, ifstream& file, const int pos, const int dsize_s, const int dsize_c)
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
void empty_file_err(const char& err, ifstream& file, const int pos, const int dsize_s, const int dsize_c)
{
    file.close();
    cout << "\nВ файле не оказалось значений, которые можно считать\n";
}
void strings_no_equality_err(const char& err, ifstream& file, const int pos, const int dsize_s, const int dsize_c)
{
    file.close();
    cout << "\nНе правильный формат, количество элементов строки " << dsize_s << " не совпадает со стокой " << (dsize_s - 1) << '\n';
}
void not_square_matr_err(const char& err, ifstream& file, const int pos, const int dsize_s, const int dsize_c)
{
    file.close();
    cout << "\nНе правильный формат, матрица не кваратная\n";
}
*/
bool ErrCheck(const char &err, ifstream &file, const int pos, const int dsize_s, const int dsize_c)
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
    else if (err == 'c')
    {
        file.close();
        cout << "\nНе правильный формат, количество элементов строки " << dsize_s << " не совпадает со стокой " << (dsize_s - 1) << '\n';
    }
    else if (err == 'q')
    {
        file.close();
        cout << "\nНе правильный формат, матрица не кваратная\n";
    }
    else
        return false;
    return true;
}
template <typename func, typename arg>
void GetMatr(func input(arg), char &err, int &dsize_s, int &dsize_c, int &dsize_all_sim, TreaD &input_stream)
{

    while (!(Stream >> ws).eof() && !err)
    {
        if (input(err))
            break;
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
    int pos = file.tellg();
    file.seekg(0, ios::beg);
    for (int i = 0; i < bufstr; i++)
        for (int j = 0; j < bufcolomn; j++)
            file >> *(*(buf_a_arr + i) + j);
    file.clear();
    file.seekg(pos);
}

int main()
{
    while (true)
    {
        struct matrdata
        {
            d_arr **p_buf_double = nullptr;
            TreaD file;
            int dsize_s = 0, dsize_c = 0, dsize_all = 0;
            char err{0};
        } mdata;
        int pos = 0;
        mdata.file = (open_ifstream(&GetFileName));
        GetMatr(&correct_input, mdata.err, mdata.dsize_s, mdata.dsize_c, mdata.dsize_all, mdata.file);
        if (ErrCheck(mdata.err, mdata.file, pos, mdata.dsize_s, mdata.dsize_c))
            continue;
    }
    return 0;
}

/*
d_arr** p_buf_double=nullptr;
        setlocale(LC_ALL, "ru");
    while (true)
    {
        ifstream file;
        string FileAdress, tempstr;
        cout << "\nВведите имя файла(чтобы выйти *): ";
        getline(cin, FileAdress);
        if (FileAdress == "*")
            break;
        file.open(FileAdress, ios_base::in);
        if (!(file.is_open()))
        {
            perror(("\nОшибка открытия файла с именем " + FileAdress).c_str());
            continue;
        }
        char err(0);
        cout << '\n';
        int pos = 0, dsize_s = 0, dsize_all_sim = 0, dsize_c = 0;
        d_arr dtemp = 0;

        while (!(file >> ws).eof() && !err)
        {
            char tmp(0);
            pos = file.tellg();
            if (!(file >> dtemp) || (file.peek() != ' ' && file.peek() != '\n'
                && file.peek() != '\t' && file.peek() != EOF))
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
                case '\t':case ' ':
                    break;
                case EOF: case '\n':
                    if (dsize_c)
                    {
                        dsize_s++;
                        dsize_all_sim += dsize_c;
                        if (dsize_all_sim % dsize_c)
                            err = 'c';
                        if(tmp!=EOF)
                            dsize_c = 0;
                    }
                    break;
                default:
                    file.unget();
                    tmp = EOF;
                }
            }
        }
        if (err == 'f')
        {
            file.clear();
            file.seekg(pos);
            file >> tempstr;
            file.close();
            cout << "\nНайдена ошибка в элементе номер " << (dsize_c + 1) << " на строке " << (dsize_s + 1)
                << "\nАбсолютная позиция в файле " << pos
                << "\nНеверное значение: \"" << tempstr << '\"' << '\n';
            cout << "\nФайл содержит некорректные значения\n";
            continue;
        }
        if (!dsize_all_sim)
        {
            file.close();
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            continue;
        }
        if (err == 'c')
        {
            file.close();
            cout << "\nНе правильный формат, количество элементов строки "<<dsize_s<<" не совпадает со стокой " <<(dsize_s-1)<<'\n';
            continue;
        }
        if (dsize_s != dsize_c)
        {
            file.close();
            cout << "\nНе правильный формат, матрица не кваратная\n";
            continue;
        }

        p_buf_double = new(nothrow) d_arr* [dsize_s];
        if (p_buf_double == nullptr)
        {
            file.close();
            cout << "\nОшибка выделения оперативной памяти\n";
            continue;
        }
        for (int i = 0; i < dsize_c ; i++)
        {
            *(p_buf_double+i) = new(nothrow) d_arr[dsize_c];
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

        file.clear();
        FReadMatr(file, p_buf_double, dsize_s, dsize_c);
        MatrCout(p_buf_double, dsize_s, dsize_c);

        for (int i = 0, j = 0; i < dsize_s; i++, j++)
            if (*(*(p_buf_double + i) + j) < 0)
            {
                for (int k = 0; k < dsize_c; k++)
                {
                    if(k==j)
                        continue;
                    if (!(*(*p_buf_double + k)) && (*(*(p_buf_double + i) + k) > 0))
                    {
                        cout << "\nЗамена столбца " << (i + 1) << " на строку " << (k+1) << '\n';
                        for (int l = 0; l < dsize_c; l++) //не квадратная тогда l<min(dsize_s, dsize_c)
                        {
                            //*(*(p_buf_double + i) + l) += *(*(p_buf_double + l) + k);
                            //*(*(p_buf_double + l) + k) = *(*(p_buf_double + i) + l) - *(*(p_buf_double + l) + k);
                            //*(*(p_buf_double + i) + l) -= *(*(p_buf_double + l) + k);
                            //не совсем правильно будет работать с типом double если будет большая разница значений мантисс
                            dtemp = *(*(p_buf_double + i) + l);
                            *(*(p_buf_double + i) + l) = *(*(p_buf_double + l) + k);
                            *(*(p_buf_double + l) + k) = dtemp;
                        }
                        break;
                    }
                    if (dsize_c - 1 == k)
                    {
                        cout << "\nНеудалось найти замену для " << (i+1) << " строки\n";
                    }
                }

            }
        MatrCout(p_buf_double, dsize_s, dsize_c);
        for(int i=0; i<dsize_s; i++)
            delete[] *(p_buf_double+i);
        delete[] p_buf_double;
        p_buf_double=nullptr;
    }
    return 0;
*/
