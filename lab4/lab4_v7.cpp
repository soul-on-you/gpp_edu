#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <functional>
#include <sstream>

using namespace std;
typedef double Xtype;

//char status_code{ 0 }; //0-good //-1-exit //'f'-failbit  //'e'-empty file  //'o'-file isn't open //'q'-non-quadr_matr //'s'-string difference //'a'-fail alloc memory

union ErrInfo
{
    int dsize_s;
    int dyn_elem;
    struct fbit
    {
        int dsize_s;
        int dsize_c;
        streamoff pos;
    } fbit;
};

bool GetFileName(string &tmp, const string &str)
{
    std::cin.sync_with_stdio(false);
    cout << str << "\nВведите имя файла (чтобы завершить работу программы, введите *): ";
    std::cin.sync_with_stdio(true);
    if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
        return false;
    getline(cin, tmp);
    return true;
}

bool IstreamElementCorCheck(istream &istr, ErrInfo *err = nullptr)
{
    Xtype Xtemp;
    streamoff pos = istr.tellg();
    if (!(istr >> Xtemp) || (istr.peek() != ' ' && istr.peek() != '\n' && istr.peek() != '\t' && istr.peek() != EOF))
    {
        if (err)
            err->fbit.pos = pos;
        return false;
    }
    return true;
}

char CheckMatr(ifstream &file, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    int dsize_all = 0;
    while (!(file >> ws).eof())
    {
        if (!IstreamElementCorCheck(file, err))
        {
            if (err)
            {
                err->fbit.dsize_s = dsize_s;
                err->fbit.dsize_c = dsize_c;
            }
            return 'f';
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
                        return 'q';
                    }
                    dsize_all += dsize_c;
                    if (dsize_all % dsize_c)
                    {
                        if (err)
                            err->dsize_s = dsize_s;
                        return 's';
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
        return 'e';
    }
    dsize_c = dsize_all / dsize_s;
    if (dsize_s != dsize_c)
    {
        return 'q';
    }
    return 0;
}

char SSCheckMatr(ifstream &file, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    int dsize_all = 0, pos_s = 0;
    while (!(file >> ws).eof())
    {
        string tmp;
        dsize_c = 0;
        pos_s = file.tellg();
        getline(file, tmp);
        istringstream istr(tmp);
        istr.clear();
        while (!(istr >> ws).eof())
        {
            if (!(IstreamElementCorCheck(istr, err)))
            {
                if (err)
                {
                    err->fbit.pos += pos_s;
                    err->fbit.dsize_s = dsize_s;
                    err->fbit.dsize_c = dsize_c;
                }
                return 'f';
            }
            dsize_c++;
        }
        dsize_s++;
        if (dsize_s > dsize_c)
            return 'q';
        dsize_all += dsize_c;
        if (dsize_all % dsize_c)
        {
            if (err)
                err->dsize_s = dsize_s;
            return 's';
        }
    }
    if (!dsize_s)
        return 'e';
    if (dsize_s != dsize_c)
    {
        return 'q';
    }
    return 0;
}

void FReadMatr(ifstream &file, Xtype **matr, const int &dsize_s, const int &dsize_c)
{
    streamoff pos = file.tellg();           ///для чтения из случайного места: 1) сохраняем позицию
    file.clear();
    file.seekg(0);
    for (int i = 0; i < dsize_s; i++)       ///или можно считать до pos
        for (int j = 0; j < dsize_c; j++)
            file >> *(*(matr + i) + j);
    file.clear();
    file.seekg(pos);                        ///для чтения из случайного места: 2) возвращаем позицию
}

void DeleteMatr(Xtype **&matr, const int &dsize_s)
{
    for (int i = 0; i < dsize_s; i++)
        delete[] * (matr + i);
    delete[] matr;
    matr = nullptr;
}
char CreateMatr(Xtype **&matr, const int &dsize_s, const int &dsize_c, ErrInfo *err = nullptr)
{
    matr = new (nothrow) Xtype *[dsize_s];
    if (matr == nullptr)
    {
        if (err)
            err->dyn_elem = 0;
        return 'a';
    }
    for (int i = 0; i < dsize_s; i++)
    {
        *(matr + i) = new (nothrow) Xtype[dsize_c];
        if (*(matr + i) == nullptr)
        {
            DeleteMatr(matr, i);
            if (err)
                err->dyn_elem = (i + 1);
            return 'a';
        }
    }
    return 0;
}

char LoadMatr(function<bool(string &, const string &)> f_get_adress, Xtype **&matr, string &FileAdress, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    dsize_s = 0, dsize_c = 0;
    if (!f_get_adress(FileAdress, "\nСчитывание из файла:"))
    {
        return -1;
    }
    ifstream file(FileAdress, ios_base::in);
    if (file.is_open())
    {
        char status_code = 0;
        //if (!(status_code = CheckMatr(file, dsize_s, dsize_c, err)))
        if (!(status_code = SSCheckMatr(file, dsize_s, dsize_c, err)))
            if (!(status_code = CreateMatr(matr, dsize_s, dsize_c, err)))
            {
                FReadMatr(file, matr, dsize_s, dsize_c);
                file.close();
                return status_code;
            }
        file.close();
        dsize_s = 0, dsize_c = 0;
        return status_code;
    }
    else
    {
        return 'o';
    }
}

/*при вызове функции нельзя передавать аргументом параметр err, если до этого он не определялся, в другой функции,
 которую мы хотим проверить на наличие ошибок, иначе мы не получим корректную обработку*/
void ErrCheck(const string &FileAdress, char &status_code, ErrInfo *err = nullptr)
{
    if (err)
    {
        switch (status_code)
        {
        case 'f':
        {
            string tmp;
            ifstream file(FileAdress);
            file.seekg(err->fbit.pos);
            file >> tmp;
            file.close();
            cout << "Найдена ошибка в элементе номер " << (err->fbit.dsize_c + 1) << " на строке " << (err->fbit.dsize_s + 1)
                 << "\nАбсолютная позиция в файле " << err->fbit.pos
                 << "\nНеверное значение: \"" << tmp << '\"' << '\n'
                 << "\nФайл содержит некорректные значения\n";
        }
        break;
        case 'e':
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case 's':
            cout << "\nНе правильный формат, количество элементов строки " << err->dsize_s << " не совпадает со стокой " << (err->dsize_s - 1) << '\n';
            break;
        case 'q':
            cout << "\nНе правильный формат, матрица не кваратная\n";
            break;
        case 'o':
            perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
            break;
        case 'a':
            if (!err->dyn_elem)
                cout << "\nОшибка, не удалось выделить память под массив указателей\n";
            else
                cout << "\nОшибка, не удалось выделить память под " << err->dyn_elem << " строку\n";
            break;
        default:
            cout << "\nОтсутствует обработка данного типа ошибки\n";
        }
    }
    status_code = 0;
}

Xtype **CopyMatr(Xtype const *const *matr1, const int &dsize_s, const int &dsize_c, char &status_code, ErrInfo *err = nullptr)
{
    Xtype **matr2;
    if (!(status_code = CreateMatr(matr2, dsize_s, dsize_c, err)))
        for (int i = 0; i < dsize_s; i++)
            for (int j = 0; j < dsize_c; j++)
                *(*(matr2 + i) + j) = *(*(matr1 + i) + j);
    return matr2;
}

char WriteMatr(function<bool(string &, const string &)> f_get_name, Xtype const *const *matr, string &FileAdress, const int &dsize_s, const int &dsize_c, ErrInfo *err = nullptr)
{
    if (f_get_name(FileAdress, "\nЗапись в файл:"))
    {
        ofstream file(FileAdress);
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
            return 0;
        }
        return 'o';
    }
    return -1;
}

template <typename N>
size_t TextVievSize(N a, ios::fmtflags f = ios_base::dec | ios_base::fixed, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios_base::basefield | ios_base::floatfield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
void CoutMatr(Xtype const *const *matr, const int &dsize_s, const int &dsize_c)
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
    ErrInfo err;
    int dsize_c = 0, dsize_s = 0;
    char status_code;
    setlocale(LC_ALL, "ru");
    while (true)
    {
        string FileAdress;
        status_code = LoadMatr(GetFileName, matr, FileAdress, dsize_s, dsize_c, &err); //функция загрузки матрицы
        if (status_code > 0)
        {
            ErrCheck(FileAdress, status_code, &err);
            continue;
        }
        else if (!status_code)
        {
            CoutMatr(matr, dsize_s, dsize_c);                                       //фунция форматированного вывода в консоль
            Xtype **copymatr = CopyMatr(matr, dsize_s, dsize_c, status_code, &err); //создание копии матрицы
            if (copymatr == nullptr)                                                /////или if(status_code)
            {
                ErrCheck(FileAdress, status_code, &err); //функция обработки ошибок
            }
            ResultFunc(copymatr, dsize_s, dsize_c);                                                       //функция по заданию
            CoutMatr(matr, dsize_s, dsize_c);                                                             //вывод начальной матрицы
            CoutMatr(copymatr, dsize_s, dsize_c);                                                         //вывод копии матрицы, после обработки функцией
            if ((status_code = WriteMatr(GetFileName, copymatr, FileAdress, dsize_s, dsize_c, &err)) > 0) //функция вывода в файл
                ErrCheck(FileAdress, status_code, &err);                                                  //функция обработки ошибок
            DeleteMatr(matr, dsize_s);                                                                    //функция удаления матрицы
            DeleteMatr(copymatr, dsize_s);                                                                //функция удаления копии матрицы
            if (!status_code)
                continue;
        }
        if (status_code == -1)
        {
            break;
        }
    }
    return 0;
}