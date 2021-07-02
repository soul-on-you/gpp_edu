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

#define Good 0
#define Exit -1
#define FailbitErr 'f'
#define EmptyFileErr 'e'
#define FileOpenErr 'o'
#define NotSquareMatrixErr 'q'
#define NotRectangularMatrixErr 'r'
#define FailAllocMemmoryErr 'a'

union ErrInfo
{
    int dsize_s;
    int MemAllocStepID;
    struct FbitInfo
    {
        int dsize_s;
        int dsize_c;
        streamoff pos;
    } fbit;
};

bool GetFileName(string &tmp, string *str = nullptr)
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
    char StreamCurElem;
    if (!(istr >> Xtemp) || ((StreamCurElem = istr.peek()) != ' ' && StreamCurElem != '\n' && StreamCurElem != '\t' && StreamCurElem != EOF))
    {
        if (err)
            err->fbit.pos = pos;
        return false;
    }
    return true;
}

char CheckMatr(ifstream &file, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    int dsize_for_check = 0;
    file >> ws;
    while (!file.eof())
    {
        if (!IstreamElementCorCheck(file, err))
        {
            if (err)
            {
                err->fbit.dsize_s = dsize_s;
                err->fbit.dsize_c = dsize_c;
            }
            return FailbitErr;
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
            case '\n':
                file >> ws;
                tmp = file.get();
            case EOF:
                dsize_s++;

#ifdef checkRectangular_v1
                if (!dsize_for_check)
                    dsize_for_check = dsize_c;
                else if (dsize_for_check != dsize_c)
#else
                dsize_for_check += dsize_c;
                if (dsize_for_check % dsize_c || dsize_for_check / dsize_c != dsize_s)
#endif
                {
                    if (err)
                        err->dsize_s = dsize_s;
                    return NotRectangularMatrixErr;
                }
                if (dsize_s > dsize_c)
                {
                    return NotSquareMatrixErr;
                }
                if (tmp != EOF)
                {
                    dsize_c = 0;
                    tmp = EOF;
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
        return EmptyFileErr;
    }
    dsize_c = dsize_for_check / dsize_s; ///////////////////////!!!!!!!!!!ПРОВЕРИТЬ!!!!!!!!Скорее всего удалить надо
    if (dsize_s != dsize_c)
    {
        return NotSquareMatrixErr;
    }
    return Good;
}

char SSCheckMatr(ifstream &file, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    int dsize_for_check = 0;
    streamoff pos_s = 0;
    while (!(file >> ws).eof())
    {
        string tmp;
        dsize_c = 0;
        pos_s = file.tellg();
        getline(file, tmp);
        istringstream istr(tmp);
        istr.clear(); /////////////////?
        while (!istr.eof())
        {
            if (!(IstreamElementCorCheck(istr, err)))
            {
                if (err)
                {
                    cout << "file pos = " << file.tellg() << " , str size = " << (streamoff)istr.str().size() << " , pos in str = " << err->fbit.pos << " , pos_s = " << pos_s << '\n';
                    err->fbit.pos += pos_s;
                    /////убрать 152!!!!!!!!!!
                    cout << "file pos = " << file.tellg() << " , str size = " << (streamoff)istr.str().size() << " , pos in str = " << err->fbit.pos << '\n';
                    err->fbit.dsize_s = dsize_s;
                    err->fbit.dsize_c = dsize_c;
                }
                return FailbitErr;
            }
            dsize_c++;
            istr >> ws;
        }
        dsize_s++;
#ifdef checkRectangular_v1
        if (!dsize_for_check)
            dsize_for_check = dsize_c;
        else if (dsize_for_check != dsize_c)
#else
        dsize_for_check += dsize_c;
        if (dsize_for_check % dsize_c || dsize_for_check / dsize_c != dsize_s)
#endif
        {
            if (err)
                err->dsize_s = dsize_s;
            return NotRectangularMatrixErr;
        }
        if (dsize_s > dsize_c)
            return NotSquareMatrixErr;
    }
    if (!dsize_s)
        return EmptyFileErr;
    if (dsize_s != dsize_c)
    {
        return NotSquareMatrixErr;
    }
    return Good;
}

void FReadMatr(ifstream &file, Xtype **matr, const int &dsize_s, const int &dsize_c)
{
    streamoff pos = file.tellg(); ///для чтения из случайного места: 1) сохраняем позицию
    file.clear();
    file.seekg(0);
    for (int i = 0; i < dsize_s; i++) ///или можно считать до pos
        for (int j = 0; j < dsize_c; j++)
            file >> *(*(matr + i) + j);
    file.clear();
    file.seekg(pos); ///для чтения из случайного места: 2) возвращаем позицию
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
            err->MemAllocStepID = -1;
        return FailAllocMemmoryErr;
    }
    for (int i = 0; i < dsize_s; i++)
    {
        *(matr + i) = new (nothrow) Xtype[dsize_c];
        if (*(matr + i) == nullptr)
        {
            DeleteMatr(matr, i);
            if (err)
                err->MemAllocStepID = i;
            return FailAllocMemmoryErr;
        }
    }
    return Good;
}

char LoadMatr(Xtype **&matr, string &FileAdress, int &dsize_s, int &dsize_c, ErrInfo *err = nullptr)
{
    dsize_s = 0, dsize_c = 0;
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
        return FileOpenErr;
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
        case FailbitErr:
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
        case EmptyFileErr:
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case NotRectangularMatrixErr:
            cout << "\nНе правильный формат, количество элементов строки " << err->dsize_s << " не совпадает со стокой " << (err->dsize_s - 1) << '\n';
            break;
        case NotSquareMatrixErr:
            cout << "\nНе правильный формат, матрица не кваратная\n";
            break;
        case FileOpenErr:
            perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
            break;
        case FailAllocMemmoryErr:
            if (err->MemAllocStepID == -1)
                cout << "\nОшибка, не удалось выделить память под массив указателей\n";
            else
                cout << "\nОшибка, не удалось выделить память под " << err->MemAllocStepID << " строку\n";
            break;
        default:
            cout << "\nОтсутствует обработка данного типа ошибки\n";
        }
    }
    status_code = Good;
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

char WriteMatr(Xtype const *const *matr, string &FileAdress, const int &dsize_s, const int &dsize_c, ErrInfo *err = nullptr)
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
        return Good;
    }
    return FileOpenErr;
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
    for (int r = 0; r < dsize_s; r++)
        if (*(*(matr + r) + r) < 0)
        {
            for (int k = 0; k < dsize_c; k++)
            {
                if (k == r)
                    continue;
                if (!(*(*matr + k)) && (*(*(matr + r) + k) > 0))
                {
                    cout << "\nЗамена столбца " << (r + 1) << " на строку " << (k + 1) << '\n';
                    Xtype Xtemp;
                    for (int l = 0; l < dsize_c; l++)
                    {
                        Xtemp = *(*(matr + r) + l);
                        *(*(matr + r) + l) = *(*(matr + l) + k);
                        *(*(matr + l) + k) = Xtemp;
                    }
                    break;
                }
                if (dsize_c - 1 == k)
                {
                    cout << "\nНеудалось найти замену для " << (r + 1) << " строки\n";
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
        cout << "\nСчитывание из файла:";
        if (!GetFileName(FileAdress))
            break;
        status_code = LoadMatr(matr, FileAdress, dsize_s, dsize_c, &err); //функция загрузки матрицы
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
            ResultFunc(copymatr, dsize_s, dsize_c); //функция по заданию
            CoutMatr(matr, dsize_s, dsize_c);       //вывод начальной матрицы
            CoutMatr(copymatr, dsize_s, dsize_c);   //вывод копии матрицы, после обработки функцией
            cout << "\nЗапись в файл:";
            if (!GetFileName(FileAdress))
                break;
            if ((status_code = WriteMatr(copymatr, FileAdress, dsize_s, dsize_c, &err)) > 0) //функция вывода в файл
                ErrCheck(FileAdress, status_code, &err);                                     //функция обработки ошибок
            DeleteMatr(matr, dsize_s);                                                       //функция удаления матрицы
            DeleteMatr(copymatr, dsize_s);                                                   //функция удаления копии матрицы
            if (!status_code)
                continue;
        }
        if (status_code == Exit)
        {
            break;
        }
    }
    return 0;
}