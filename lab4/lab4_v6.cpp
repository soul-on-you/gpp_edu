#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>
#include <functional>
//#include <windows.h>
//#include <conio.h>
#include <sstream>

using namespace std;
typedef double Xtype;

struct Err
{
    char errtype{0}; //'f'-failbit  //'e'-empty file  //'o'-file isn't open //'q'-non-quadr_matr //'s'-string difference //'a'-fail alloc memory
    union ErrInfo
    {
        int dsize_s = 0;
        int dyn_elem;
        struct fbit
        {
            int dsize_s;
            int dsize_c;
            streamoff pos;
        } fbit;
    } errinfo;
} err;

bool GetFileName(string &tmp)
{
    std::cin.sync_with_stdio(false);
    cout << "\nВведите имя файла (чтобы выйти, введите *): ";
    std::cin.sync_with_stdio(true);
    if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
        return false;
    getline(cin, tmp);
    return true;
}
bool FileElementCorCheck(ifstream &file, Err *err = nullptr) ////ErrInfo* err
{
    char tmp(0);
    Xtype Xtemp;
    streamoff pos = file.tellg();
    if (!(file >> Xtemp) || (file.peek() != ' ' && file.peek() != '\n' && file.peek() != '\t' && file.peek() != EOF))
    {
        //m_data.err.errinfo.fbit.pos = pos;
        //m_data.err.errtype = 'f';
        err->errtype = 'f';
        err->errinfo.fbit.pos = pos;
        //m_data.err.errinfo.fbit.dsize_s = m_data.dsize_s;  ///////// В ОШИБКАХ МОЖНО ИСПОЛЬЗОВАТЬ САМ МАТР
        //m_data.err.errinfo.fbit.dsize_c = m_data.dsize_c;  ///////// m_data.dsize_s и m_data.dsize_c
        return false;
    }
    return true;
}
bool CheckMatr(ifstream &file, int &dsize_s, int &dsize_c, Err *err = nullptr) ////int &dsize_s, int &dsize_c,  ErrInfo* err=nullptr
{
    int dsize_all = 0;
    while (!(file >> ws).eof()) //////&& !err
    {
        if (!FileElementCorCheck(file, err)) //m_data.err.fbit.dsize_c=dsize_c; //m_data.err.fbit.dsize_s=dsize_s;
        {
            err->errinfo.fbit.dsize_s = dsize_s;
            err->errinfo.fbit.dsize_c = dsize_c;
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
                    if (dsize_s > dsize_c)
                    {
                        err->errtype = 'q';
                        return false;
                    }
                    dsize_all += dsize_c;
                    if (dsize_all % dsize_c)
                    {
                        err->errinfo.dsize_s = dsize_s;
                        err->errtype = 's';
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
    if (!dsize_s)
    {
        err->errtype = 'e';
        return false;
    }
    dsize_c = dsize_all / dsize_s;
    if (dsize_s != dsize_c)
    {
        err->errtype = 'q';
        return false;
    }
    return true;
}

void FReadMatr(ifstream &file, Xtype **matr, const int &dsize_s, const int &dsize_c) /////ifstream& file, Xtype** buf_a_arr, const int &bufstr, const int &bufcolomn
{
    streamoff pos = file.tellg(); ///для чтения из случайного места: 1) сохраняем позицию
    file.clear();
    file.seekg(0);
    for (int i = 0; i < dsize_s; i++)
        for (int j = 0; j < dsize_c; j++)
            file >> *(*(matr + i) + j);
    file.clear();
    file.seekg(pos); ///для чтения из случайного места: 2) возвращаем позицию
}

void DeleteMatr(Xtype **&matr, const int &dsize_s) ////Xtype** (&matr), const int &str_in_matr
{
    for (int i = 0; i < dsize_s; i++) ///////str_in_matr
        delete[] * (matr + i);
    delete[] matr;
    matr = nullptr;
}
bool CreateMatr(Xtype **&matr, const int &dsize_s, const int &dsize_c, Err *err = nullptr) //////Xtype** matr, const int &str_in_matr, const int &column_in_matr, ErrInfo &err
{
    matr = new (nothrow) Xtype *[dsize_s];
    if (matr == nullptr)
    {
        err->errtype = 'a';
        err->errinfo.dyn_elem = 0; /////под указатели но протестить закоментив
        return false;
    }
    for (int i = 0; i < dsize_s; i++)
    {
        *(matr + i) = new (nothrow) Xtype[dsize_c];
        if (*(matr + i) == nullptr)
        {
            DeleteMatr(matr, i - 1);
            err->errtype = 'a';
            err->errinfo.dsize_s = (i + 1);
            return false;
        }
    }
    return true;
}

//template <typename StreamType>
bool LoadMatr(function<bool(string &)> f_get_adress, Xtype **&matr, string &FileAdress, int &dsize_s, int &dsize_c, Err *err = nullptr) ///function <void(StreamType&, Xtype**, const int&, const int&)>f_read
{                                                                                                                                       ///function <string()> f_get_adress, Xtype** matr, ifstream &file, ErrInfo* &err
    //string FileAdress;                      ///function <bool(string&)> f_get_adress, MatrInfo& m_data , string &FileAdress
    cout << "\nСчитывание из файла:";
    if (!f_get_adress(FileAdress))
    {
        return false;
    }
    ifstream file(FileAdress, ios_base::in);
    if (file.is_open())
    {
        if (CheckMatr(file, dsize_s, dsize_c, err))      //////v2//m_data.dsize_s,m_data.dsize_c, &m_data.err
            if (CreateMatr(matr, dsize_s, dsize_c, err)) /////m_data.matr, m_data.dsize_s, m_data.dsize_c, m_data.err
            {
                FReadMatr(file, matr, dsize_s, dsize_c); //////
                file.close();
                return true;
            }
        //if (m_data.err.errtype == 'f')                        //////////// или по указателю fileadress
        //m_data.err.errinfo.fbit.filename = &FileAdress;   ////////////
        file.close();
    }
    else
    {
        //m_data.err.errinfo.filename = &FileAdress;
        err->errtype = 'o'; ///////// m_data.err.errtype = 'o';
    }
    return false;
}
bool ErrCheck(const string &FileAdress, const int &dsize_s, const int &dsize_c, Err *err = nullptr) ///////const MatrInfo& m_data   string& FileAdress
{
    switch (err->errtype)
    {
    case 'f':
    {
        string tmp;
        ifstream file(FileAdress);
        file.seekg(err->errinfo.fbit.pos);
        file >> tmp;
        file.close();
        cout << "Найдена ошибка в элементе номер " << (err->errinfo.fbit.dsize_c + 1) << " на строке " << (err->errinfo.fbit.dsize_s + 1)
             << "\nАбсолютная позиция в файле " << err->errinfo.fbit.pos
             << "\nНеверное значение: \"" << tmp << '\"' << '\n'
             << "\nФайл содержит некорректные значения\n";
    }
    break;
    case 'e':
        cout << "\nВ файле не оказалось значений, которые можно считать\n";
        break;
    case 's':
        cout << "\nНе правильный формат, количество элементов строки " << err->errinfo.dsize_s << " не совпадает со стокой " << (err->errinfo.dsize_s - 1) << '\n';
        break;
    case 'q':
        cout << "\nНе правильный формат, матрица не кваратная\n";
        break;
    case 'o':
        perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
        break;
    case 'a':
        if (!err->errinfo.dsize_s)
            cout << "\nОшибка, не удалось выделить память под массив указателей\n";
        else
            cout << "\nОшибка, не удалось выделить память под " << err->errinfo.dsize_s << " строку\n";
        break;
    default:
        return false;
    }
    err->errtype = 0;
    return true;
}

Xtype **CopyMatr(const Xtype **matr1, const int &dsize_s, const int &dsize_c, Err *err = nullptr)
{
    Xtype **matr2 = nullptr;
    CreateMatr(matr2, dsize_s, dsize_c, err);
    for (int i = 0; i < dsize_s; i++)
        for (int j = 0; j < dsize_c; j++)
            *(*(matr2 + i) + j) = *(*(matr1 + i) + j);
    return matr2;
}

bool WriteMatr(function<bool(string &)> f_get_name, Xtype **matr, const int &dsize_s, const int &dsize_c, Err *err = nullptr)
{
    string FileName;
    cout << "\nЗапись в файл:";
    if (f_get_name(FileName))
    {
        ofstream file(FileName);
        if (file.is_open())
        {
            for (int i = 0; i < dsize_s; i++)
            {
                for (int j = 0; j < dsize_c; j++)
                {
                    file << *(*(matr + i) + j) << ' ';
                }
                file << '\n';
            }
            file.close();
            return true;
        }
        err->errtype = 'o';
    }
    return false;
}

template <typename N>
size_t TextVievSize(N a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
void CoutMatr(Xtype **matr, const int &dsize_s, const int &dsize_c) ////////Xtype** matr, const int& bufstr, const int& bufcolomn
{
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int j = 0; j < dsize_c; j++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << setw(6 - TextVievSize(j) / 2) << '|';
    }
    cout << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < dsize_s; i++)
    {
        cout << "| " << (i + 1) << " |";
        for (int j = 0; j < dsize_c; j++)
        {
            cout << setw(10) << right << scientific << setprecision(2) << *(*(matr + i) + j) << " |";
        }
        cout << endl
             << skipstr.str() << '\n';
    }
}

void ResultFunc(Xtype **matr, const int &dsize_s, const int &dsize_c)
{
    for (int i = 0, j = 0; i < dsize_s; i++, j++)
        if (*(*(matr + i) + j) < 0)
        {
            for (int k = 0; k < dsize_c; k++)
            {
                if (k == j)
                    continue;
                if (!(*(*matr + k)) && (*(*(matr + i) + k) > 0))
                {
                    cout << "\nЗамена столбца " << (i + 1) << " на строку " << (k + 1) << '\n';
                    Xtype Xtemp;
                    for (int l = 0; l < dsize_c; l++)
                    {
                        Xtemp = *(*(matr + i) + l);
                        *(*(matr + i) + l) = *(*(matr + l) + k);
                        *(*(matr + l) + k) = Xtemp;
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

int main()
{
    Xtype **matr = nullptr;
    Err err;
    setlocale(LC_ALL, "ru");
    while (true)
    {
        string FileAdress;
        int dsize_c = 0, dsize_s = 0;
        if (!LoadMatr(GetFileName, matr, FileAdress, dsize_s, dsize_c, &err))
        {
            if (ErrCheck(FileAdress, dsize_s, dsize_c, &err))
                continue;
        }
        else
        {
            CoutMatr(matr, dsize_s, dsize_c);
            ResultFunc(matr, dsize_s, dsize_c);
            CoutMatr(matr, dsize_s, dsize_c);
            //WriteMatr(GetFileName, matr, dsize_s, dsize_c, err);
            DeleteMatr(matr, dsize_s);
            continue;
        }
        if (matr != nullptr)
        {
            DeleteMatr(matr, dsize_s); ////p_buf_Xtype, pArrSIZE
        }
        break;
    }
    return 0;
}