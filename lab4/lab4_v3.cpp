#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>
#include <functional>

using namespace std;
typedef double Xtype;

union ErrInfo
{
    int dsize_s;
    int dyn_elem;
    struct fbit
    {
        int dsize_s;
        int dsize_c;
        int pos;
    } fbit;
    char errtype;
};

struct MatrInfo
{
    Xtype **matr;
    int dsize_s, dsize_c;
    //ifstream file;
    ErrInfo err;
    //string FileAdress;
};

void FReadMatr(ifstream &file, Xtype **(&buf_a_arr), const int &bufstr, const int &bufcolomn)
{
    int pos = file.tellg();
    file.seekg(0, ios::beg);
    for (int i = 0; i < bufstr; i++)
        for (int j = 0; j < bufcolomn; j++)
            file >> *(*(buf_a_arr + i) + j);
    file.clear();
    file.seekg(pos);
}

string GetFileName()
{
    cout << "\nВведите имя файла(чтобы выйти *): ";
    string tmp;
    getline(cin, tmp);
    return tmp;
}

bool FileElementCorCheck(ifstream &file, ErrInfo *err)
{
    char tmp(0);
    Xtype Xtemp;
    size_t pos = file.tellg();
    if (!(file >> Xtemp) || (file.peek() != ' ' && file.peek() != '\n' && file.peek() != '\t' && file.peek() != EOF))
    {
        err->fbit.pos = pos;
        return false;
    }
    return true;
}
bool CheckMatr(ifstream &file, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    int dsize_s = 0, dsize_c = 0, dsize_all = 0;
    while (!(file >> ws).eof()) //////&& !err
    {
        if (!FileElementCorCheck(file, err))
        {
            err->fbit.dsize_c = dsize_c;
            err->fbit.dsize_s = dsize_s;
            return false;
        }
        dsize_c++;
        char tmp(0);
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
                    dsize_all += dsize_c;
                    if (dsize_all % dsize_c)
                    {
                        err->dsize_s = dsize_s;
                        return false;
                    }
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
    return true;
}
//template <typename StreamType>
bool LoadMatr(function<string()> f_get_adress, MatrInfo &m_data) ///function <void(StreamType&, Xtype**, const int&, const int&)>f_read
{                                                                ///function <string()> f_get_adress, Xtype** matr, ifstream &file, ErrInfo* &err
    string FileAdress = f_get_adress();
    ifstream file(FileAdress, ios_base::in);
    if (file.is_open())
    {
        if (CheckMatr(file, m_data.dsize_s, m_data.dsize_c, &m_data.err))
            if (CreateMatr(m_data)) /////m_data.matr, m_data.dsize_s, m_data.dsize_c, m_data.err
            {
                FReadMatr(file, m_data.matr, m_data.dsize_s, m_data.dsize_c);
                file.close();
                return true;
            }
        file.close();
    }
    else
        m_data.err.errtype = 'o';
    return false;
}

bool CreateMatr(MatrInfo &m_data) //////Xtype** matr, const int &str_in_matr, const int &column_in_matr, ErrInfo &err
{
    m_data.matr = new (nothrow) Xtype *[m_data.dsize_s];
    if (m_data.matr == nullptr)
    {
        m_data.err.errtype = 's';
        return false;
    }
    for (int i = 0; i < m_data.dsize_s; i++)
    {
        *(m_data.matr + i) = new (nothrow) Xtype[m_data.dsize_c];
        if (*(m_data.matr + i) == nullptr)
        {
            m_data.err.dsize_s = (i + 1);
            return false;
        }
    }
    return true;
}
void DeleteMatr(Xtype **(&matr), const int &str_in_matr)
{
    for (int i = 0; i < str_in_matr; i++)
        delete[] * (matr + i);
    delete[] matr;
    matr = nullptr;
}

template <typename N>
size_t TextVievSize(N a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
void MatrCout(MatrInfo m_data) ////////Xtype** matr, const int& bufstr, const int& bufcolomn
{
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int j = 0; j < m_data.dsize_c; j++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << setw(6 - TextVievSize(j) / 2) << '|';
    }
    cout << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < m_data.dsize_s; i++)
    {
        cout << "| " << (i + 1) << " |";
        for (int j = 0; j < m_data.dsize_c; j++)
        {
            cout << setw(10) << right << scientific << setprecision(2) << *(*(m_data.matr + i) + j) << " |";
        }
        cout << endl
             << skipstr.str() << '\n';
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    string FileAdress;
    Xtype **p_buf_Xtype;
    int pArrSIZE = 0;
    while (true)
    {
        cout << "Введите имя файла (чтобы вернуться в главное меню, введите \"*\")\n";
        getline(cin, FileAdress);
        if (FileAdress == "*")
            break;
    }
    if (p_buf_Xtype != nullptr)
    {
        DeleteMatr(p_buf_Xtype, pArrSIZE);
    }
}