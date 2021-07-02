#define allocmemory_try_catch_method
//#define checkMatr_v1
//#define SScheckforPos_v2

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

#define PreparatoryStep -1

streamoff GetOffSet()
{
    ofstream tmp("tafsfgqts;№№443fd4$dfg42tr3214sdsdff32423", ios::out);
    tmp << '\n'
        << "";
    streamoff PlatformOffSet = (tmp.seekp(0, ios::end)).tellp();
    tmp.close();
    remove("tafsfgqts;№№443fd4$dfg42tr3214sdsdff32423");
    return PlatformOffSet;
}
const streamoff PlatformOffSet = GetOffSet();

enum StatusCode
{
    Exit = -1,
    Good,
    FailbitErr,
    EmptyFileErr,
    FileOpenErr,
    NotSquareMatrixErr,
    NotRectangularMatrixErr,
    FailAllocMemmoryErr
};

union ErrInfo
{
    int NotRectangular_str_in_Matrix;
    int MemAllocStepID;
    struct FbitInfo
    {
        int matr_str;
        int matr_column;
        streamoff pos;
    } fbit;
};

bool GettingNameOrExit(string &tmp, char SymForExit)
{
    std::cin.sync_with_stdio(false);
    if (cin.peek() == SymForExit && cin.rdbuf()->in_avail() == 2)
    {
        std::cin.sync_with_stdio(true);
        return false;
    }
    std::cin.sync_with_stdio(true);
    getline(cin, tmp);
    return true;
}

template <typename N>
size_t TextVievSize(N a, ios::fmtflags flags = (ios::fmtflags)0, streamsize prec = 6)
{
    ios::fmtflags resetflags = (ios::fmtflags)0;
    if (flags)
    {
        if (flags & ios::basefield)
            resetflags = resetflags | ios::basefield;
        if (flags & ios::floatfield)
            resetflags = resetflags | ios::floatfield;
    }
    return ((ostringstream &)(ostringstream() << resetiosflags(resetflags) << setiosflags(flags) << setprecision(prec) << a)).str().size();
}

bool IstreamElementCorCheck(istream &istr, ErrInfo *err = nullptr)
{
    Xtype Xtemp;
    streamoff pos = istr.tellg();
    char StreamCurElem;
    if (!(istr >> Xtemp) || ((StreamCurElem = istr.peek()) != ' ' && StreamCurElem != '\n' && StreamCurElem != '\t' && StreamCurElem != EOF))
    {
        if (err)
#if defined(checkMatr_v1) || defined(SScheckforPos_v2)
            err->fbit.pos = pos;
#else
        {
            istr.clear();
            istr.seekg(0, ios::end);
            err->fbit.pos = istr.tellg() - pos;
        }
#endif
        return false;
    }
    return true;
}

#ifdef checkMatr_v1
StatusCode CheckMatr(ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    int size_for_check = 0;
    file >> ws;
    while (!file.eof())
    {
        if (!IstreamElementCorCheck(file, err))
        {
            if (err)
            {
                err->fbit.matr_str = matr_str;
                err->fbit.matr_column = matr_column;
            }
            return FailbitErr;
        }
        matr_column++;
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
                tmp = file.peek();
            case EOF:
                if (size_for_check != matr_str * matr_column)
                {
                    if (err)
                        err->NotRectangular_str_in_Matrix = matr_str;
                    return NotRectangularMatrixErr;
                }
                matr_str++;

                if (tmp != EOF)
                {
                    size_for_check += matr_column;
                    matr_column = 0;
                    tmp = EOF;
                }
                break;
            default:
                file.unget();
                tmp = EOF;
            }
        }
    }
    if (!matr_str)
    {
        return EmptyFileErr;
    }
    return Good;
}

#else
StatusCode SSCheckMatr(ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    int size_for_check = 0;
    while (!(file >> ws).eof())
    {
        string tmp;
        matr_column = 0;
        getline(file, tmp);
        istringstream istr(tmp);
        while (!istr.eof())
        {
            if (!(IstreamElementCorCheck(istr, err)))
            {
                if (err)
                {
#ifdef SScheckforPos_v2
                    istr.clear();
                    /*istr.seekg(err->fbit.pos, ios::beg);
                    istr >> tmp;
                    streamoff SS_pos;
                    if ((SS_pos = istr.tellg()) == EOF)
                    {
                        istr.clear();
                        istr.seekg(0, ios::end);
                        SS_pos = istr.tellg();
                    }*/
                    //err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - (istr.tellg() - (streamoff)tmp.size())) - (streamoff)1; ///(istr.tellg() - (streamoff)tmp.size()))->ERR.POS
                    err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - err->fbit.pos) - PlatformOffSet;
#else
                    err->fbit.pos = file.tellg() - err->fbit.pos - PlatformOffSet;
#endif
                    err->fbit.matr_str = matr_str;
                    err->fbit.matr_column = matr_column;
                }
                return FailbitErr;
            }
            matr_column++;
            istr >> ws;
        }
        matr_str++;
        size_for_check += matr_column;
        if (size_for_check != matr_str * matr_column)
        {
            if (err)
                err->NotRectangular_str_in_Matrix = matr_str;
            return NotRectangularMatrixErr;
        }
    }
    if (!matr_str)
        return EmptyFileErr;
    return Good;
}
#endif

void FReadMatr(ifstream &file, Xtype **matr, int matr_str, int matr_column)
{
    for (int i = 0; i < matr_str; i++)
        for (int j = 0; j < matr_column; j++)
            file >> *(*(matr + i) + j);
}

void DeleteMatr(Xtype **&matr, int matr_str)
{
    if (matr)
    {
        for (int i = 0; i < matr_str; i++)
            delete[] * (matr + i);
        delete[] matr;
        matr = nullptr;
    }
}

#ifndef allocmemory_try_catch_method
StatusCode CreateMatr(Xtype **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    matr = new (nothrow) Xtype *[matr_str];
    if (matr == nullptr)
    {
        if (err)
            err->MemAllocStepID = -1;
        return FailAllocMemmoryErr;
    }
    for (int i = 0; i < matr_str; i++)
    {
        *(matr + i) = new (nothrow) Xtype[matr_column];
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

#else
StatusCode CreateMatr(Xtype **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    int i = PreparatoryStep;
    matr = nullptr;
    try
    {
        matr = new Xtype *[matr_str];
        for (int i = 0; i < matr_str; i++)
            *(matr + i) = new Xtype[matr_column];
    }
    catch (bad_alloc)
    {
        if (i > 0)
            DeleteMatr(matr, i);
        if (err)
            err->MemAllocStepID = i;
        return FailAllocMemmoryErr;
    }
    return Good;
}
#endif

StatusCode AdditionalCheck(Xtype const *const *matr, int matr_str, int matr_column)
{
    if (!matr)
        if (matr_str != matr_column)
        {
            return NotSquareMatrixErr;
        }
    return Good;
}

StatusCode LoadMatr(Xtype **&matr, int &matr_str, int &matr_column, const string &FileAdress, function<StatusCode(Xtype const *const *, int, int)> fAdditionalCheck, ErrInfo *err = nullptr)
{
    matr_str = 0, matr_column = 0;
    ifstream file(FileAdress);
    if (file.is_open())
    {
        StatusCode status_code;
#ifdef checkMatr_v1
        if (!(status_code = CheckMatr(file, matr_str, matr_column, err)))
#else
        if (!(status_code = SSCheckMatr(file, matr_str, matr_column, err)))
#endif
        {
            if (!(status_code = fAdditionalCheck(nullptr, matr_str, matr_column)))
            {
                if (!(status_code = CreateMatr(matr, matr_str, matr_column, err)))
                {
                    file.clear();
                    file.seekg(0);
                    FReadMatr(file, matr, matr_str, matr_column);
                    file.close();
                    if (!(status_code = fAdditionalCheck(matr, matr_str, matr_column)))
                        return status_code;
                }
                DeleteMatr(matr, matr_str);
            }
        }
        file.close();
        matr_str = 0, matr_column = 0;
        return status_code;
    }
    return FileOpenErr;
}

/*при вызове функции нельзя передавать аргументом параметр err, если до этого он не определялся, в другой функции,
 которую мы хотим проверить на наличие ошибок, иначе мы не получим корректную обработку*/
void ErrCheck(const string &FileAdress, StatusCode status_code, ErrInfo *err = nullptr)
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
            cout << "\nНайдена ошибка в элементе номер " << (err->fbit.matr_column + 1) << " на строке " << (err->fbit.matr_str + 1)
                 << "\nАбсолютная позиция в файле " << err->fbit.pos
                 << "\nНеверное значение: \"" << tmp << '\"' << '\n'
                 << "\nФайл содержит некорректные значения\n";
            err->fbit.matr_column = 0, err->fbit.matr_str = 0, err->fbit.pos = 0;
        }
        break;
        case EmptyFileErr:
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case NotRectangularMatrixErr:
            cout << "\nНе правильный формат, количество элементов строки " << err->NotRectangular_str_in_Matrix << " не совпадает со стокой " << (err->NotRectangular_str_in_Matrix - 1) << '\n';
            err->NotRectangular_str_in_Matrix = 0;
            break;
        case NotSquareMatrixErr:
            cout << "\nНе правильный формат, матрица не кваратная\n";
            break;
        case FileOpenErr:
            perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
            break;
#ifdef allocmemory_secure
        case MemoryLeakErr:
            cout << "\nОшибка, попытка инициализации указателя, адрес которого указывает на существующий массив, это вызовет утечку памяти\n";
            ;
            break;
#endif
        case FailAllocMemmoryErr:
            if (err->MemAllocStepID == -1)
                cout << "\nОшибка, не удалось выделить память под массив указателей\n";
            else
                cout << "\nОшибка, не удалось выделить память под " << err->MemAllocStepID << " строку\n";
            err->MemAllocStepID = 0;
            break;
        default:
            cout << "\nОтсутствует обработка данного типа ошибки\n";
        }
    }
    else
        cout << "\nКод ошибки: " << status_code << '\n';
}

Xtype **CopyMatr(Xtype const *const *matr, int matr_str, int matr_column, StatusCode &status_code, ErrInfo *err = nullptr)
{
    Xtype **matrcopy;
    if (!(status_code = CreateMatr(matrcopy, matr_str, matr_column, err)))
        for (int i = 0; i < matr_str; i++)
            for (int j = 0; j < matr_column; j++)
                *(*(matrcopy + i) + j) = *(*(matr + i) + j);
    return matrcopy;
}

ostream &OutputMatr(ostream &ostr, Xtype const *const *matr, int matr_str, int matr_column, streamsize width = 0, streamsize precision = 6, ios::fmtflags flags = (ios::fmtflags)0)
{
    if (flags)
    {
        ios::fmtflags resetflags = (ios::fmtflags)0;
        if (flags & ios::basefield)
            resetflags = resetflags | ios::basefield;
        if (flags & ios::floatfield)
            resetflags = resetflags | ios::floatfield;
        if (flags & ios::adjustfield)
            resetflags = resetflags | ios::adjustfield;
        ostr << resetiosflags(resetflags) << setiosflags(flags);
    }
    ostr << setprecision(precision);
    for (int i = 0; i < matr_str; i++)
    {
        for (int j = 0; j < matr_column; j++)
            ostr << setw(width) << *(*(matr + i) + j) << ' ';
        ostr << '\n';
    }
    return ostr;
}

ostream &CoutPerfectMatr(ostream &ostr, Xtype const *const *matr, int matr_str, int matr_column)
{
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int j = 0; j < matr_column; j++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << setw(6 - TextVievSize(j) / 2) << '|';
    }
    ostr << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < matr_str; i++)
    {
        ostr << "| " << (i + 1) << " |";
        for (int j = 0; j < matr_column; j++)
        {
            ostr << setw(10) << right << scientific << setprecision(2) << *(*(matr + i) + j) << " |";
        }
        ostr << endl
             << skipstr.str() << '\n';
    }
    return ostr;
}

Xtype **ResultFunc(Xtype **matr, int matr_str, int matr_column)
{
    for (int r = 0; r < matr_str; r++)
        if (*(*(matr + r) + r) < 0)
        {
            for (int k = 0; k < matr_column; k++)
            {
                if (k == r)
                    continue;
                if (!(*(*matr + k)) && (*(*(matr + r) + k) > 0))
                {
                    Xtype Xtemp;
                    for (int l = 0; l < matr_column; l++)
                    {
                        Xtemp = *(*(matr + r) + l);
                        *(*(matr + r) + l) = *(*(matr + l) + k);
                        *(*(matr + l) + k) = Xtemp;
                    }
                    break;
                }
            }
        }
    return matr;
}

int main()
{
    Xtype **matr = nullptr;
    ErrInfo err;
    int matr_column, matr_str;
    StatusCode status_code;
    setlocale(LC_ALL, "ru");
    while (true)
    {
        string FileAdress;
        cout << "\nСчитывание из файла:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
        if (!GettingNameOrExit(FileAdress, '*'))
        {
            break;
        }

        if ((status_code = LoadMatr(matr, matr_str, matr_column, FileAdress, AdditionalCheck, &err)) != Good) //функция загрузки матрицы
        {
            ErrCheck(FileAdress, status_code, &err);
            continue;
        }

        CoutPerfectMatr(cout, matr, matr_str, matr_column);                          //фунция форматированного вывода в консоль
        Xtype **copymatr = CopyMatr(matr, matr_str, matr_column, status_code, &err); //создание копии матрицы
        if (status_code != Good)                                                     /////или if(copymatr == nullptr)
            ErrCheck(FileAdress, status_code, &err);                                 //функция обработки ошибок
        else
        {
            //ResultFunc(copymatr, matr_str, matr_column);            //функция по заданию
            CoutPerfectMatr(cout, copymatr = ResultFunc(copymatr, matr_str, matr_column), matr_str, matr_column); //вывод копии матрицы, после обработки функцией
            cout << "\nЗапись в файл:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
            while (true)
            {
                if (GettingNameOrExit(FileAdress, '*'))
                {
                    ofstream file(FileAdress);
                    if (file.is_open())
                        OutputMatr(file, copymatr, matr_str, matr_column);
                    else
                    {
                        status_code = FileOpenErr;
                        ErrCheck(FileAdress, status_code, &err);
                        status_code = Good;
                    }
                }
                else
                    status_code = Exit;
                break;
            }
            DeleteMatr(copymatr, matr_str); //функция удаления копии матрицы
        }
        DeleteMatr(matr, matr_str); //функция удаления матрицы
        if (status_code == Exit)
            break;
    }
    return 0;
}