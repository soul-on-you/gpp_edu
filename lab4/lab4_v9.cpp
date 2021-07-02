//#define checkRectangular_v1
//#define allocmemory_secure
#define allocmemory_try_catch_method
//#define checkMatr_v1
//#define SScheckforPos_v2
#define LINUX

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cerrno>
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

#ifdef allocmemory_secure
#define MemoryLeakErr 'm'
#endif

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

//функция от 2 исключаемых и неисключаемых флагов
template <typename N>
size_t TextVievSize(N a, ios::fmtflags basefield = ios_base::dec, ios::fmtflags floatfield = ios_base::fixed, ios::fmtflags flags = (ios::fmtflags)0, int prec = 6)
{
    return ((ostringstream &)(ostringstream() << resetiosflags(ios::basefield | ios::floatfield) << setiosflags(basefield | floatfield | flags) << setprecision(prec) << a)).str().size();
}

template <typename N>
size_t TextVievSize_v2(N a, ios::fmtflags flags = (ios::fmtflags)0, int prec = 6)
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
char CheckMatr(ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
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
                matr_str++;

#ifdef checkRectangular_v1
                if (!size_for_check)
                    size_for_check = matr_column;
                else if (size_for_check != matr_column)
#else
                size_for_check += matr_column;
                //if (size_for_check % matr_column || size_for_check / matr_column != matr_str)
                ////новы вариант 141
                if (size_for_check != matr_str * matr_column)
#endif
                {
                    if (err)
                        err->NotRectangular_str_in_Matrix = matr_str;
                    return NotRectangularMatrixErr;
                }
                if (tmp != EOF)
                {
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
char SSCheckMatr(ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    int size_for_check = 0;
    while (!(file >> ws).eof())
    {
        string tmp;
        matr_column = 0;
        //streamoff pos_s=file.tellg(); было, теперь нет лишних действий, если нет ошибки
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
                    istr.seekg(err->fbit.pos, ios::beg);
                    istr >> tmp;
                    streamoff SS_pos;
                    if ((SS_pos = istr.tellg()) == EOF)
                    {
                        istr.clear();
                        istr.seekg(0, ios::end);
                        SS_pos = istr.tellg();
                    }
#ifdef LINUX
                    err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - (istr.tellg() - (streamoff)tmp.size())) - (streamoff)1;
#else
                    err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - (SS_pos - (streamoff)tmp.size())) - (streamoff)2;
#endif
#else
#ifdef LINUX
                    err->fbit.pos = file.tellg() - err->fbit.pos - (streamoff)1;
#else
                    err->fbit.pos = file.tellg() - err->fbit.pos - (streamoff)2;
#endif
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
#ifdef checkRectangular_v1
        if (!size_for_check)
            size_for_check = matr_column;
        else if (size_for_check != matr_column)
#else
        size_for_check += matr_column;
        //if (size_for_check % matr_column || size_for_check / matr_column != matr_str)
        ////новы вариант на 224
        if (size_for_check != matr_str * matr_column)
#endif
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

#ifndef allocmemory_secure
#ifndef allocmemory_try_catch_method
char CreateMatr(Xtype **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
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
char CreateMatr(Xtype **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    int i = -1;
    try
    {
        matr = new Xtype *[matr_str];
        for (int i = 0; i < matr_str; i++)
            *(matr + i) = new Xtype[matr_column];
    }
    catch (bad_alloc)
    {
        DeleteMatr(matr, i);
        if (err)
            err->MemAllocStepID = i;
        return FailAllocMemmoryErr;
    }
    return Good;
}
#endif

#else ////////////убрать из конечной работы
//CreateMatr_s
char CreateMatr(Xtype **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    if (matr)
        return MemoryLeakErr;
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
#endif

char LoadMatr(Xtype **&matr, const string &FileAdress, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    matr_str = 0, matr_column = 0;
    ifstream file(FileAdress, ios_base::in);
    if (file.is_open())
    {
        char status_code = Good;
#ifdef checkMatr_v1
        if (!(status_code = CheckMatr(file, matr_str, matr_column, err)))
#else
        if (!(status_code = SSCheckMatr(file, matr_str, matr_column, err)))
#endif
        {
            if (matr_str != matr_column)
            {
                status_code = NotSquareMatrixErr;
            }
            else if (!(status_code = CreateMatr(matr, matr_str, matr_column, err)))
            {
                file.clear();
                file.seekg(0);
                FReadMatr(file, matr, matr_str, matr_column);
                file.close();
                return status_code;
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
void ErrCheck(const string &FileAdress, char status_code, ErrInfo *err = nullptr) ///прежний вариант char Errcheck
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
        }
        break;
        case EmptyFileErr:
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case NotRectangularMatrixErr:
            cout << "\nНе правильный формат, количество элементов строки " << err->NotRectangular_str_in_Matrix << " не совпадает со стокой " << (err->NotRectangular_str_in_Matrix - 1) << '\n';
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
            break;
        default:
            cout << "\nОтсутствует обработка данного типа ошибки\n";
        }
    }
    else ///////или без else, но, врядли, он нужен, если мы выдаем полную информацию, а код ошибки дает сокращенную, получится какая-то тавтология
        cout << "\nКод ошибки: " << status_code << '\n';
    //////прежний вариант return Good;
}

Xtype **CopyMatr(Xtype const *const *matr, int matr_str, int matr_column, char &status_code, ErrInfo *err = nullptr)
{
    Xtype **matrcopy;
    if (!(status_code = CreateMatr(matrcopy, matr_str, matr_column, err)))
        for (int i = 0; i < matr_str; i++)
            for (int j = 0; j < matr_column; j++)
                *(*(matrcopy + i) + j) = *(*(matr + i) + j);
    return matrcopy;
}

void OutputMatr(ostream &ostr, Xtype const *const *matr, int matr_str, int matr_column, streamsize precition = 6, streamsize width = 0, ios::fmtflags flags = (ios::fmtflags)0)
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
    ostr << setprecision(precition);
    for (int i = 0; i < matr_str; i++)
    {
        for (int j = 0; j < matr_column; j++)
            ostr << setw(width) << *(*(matr + i) + j) << ' ';
        ostr << '\n';
    }
}

void CoutMatr(ostream &ostr, Xtype const *const *matr, int matr_str, int matr_column)
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
}

void ResultFunc(Xtype **matr, int matr_str, int matr_column) /////Xtype ResultFunc (Xtype** matr, int matr_str, int matr_column)
{
    for (int i = 0, j = 0; i < matr_str; i++, j++) //в задании нужно делать замену строки на столбец, если на главной диагонали отрицательное значение
        if (*(*(matr + i) + j) < 0)                //и, кажется, мало смысла в лишних действиях, когда меняем отрицательное число на отриательное
        {
            for (int k = 0; k < matr_column; k++)
            {
                if (k == j)
                    continue;
                if (!(*(*matr + k)) && (*(*(matr + i) + k) > 0))
                {
                    cout << "\nЗамена столбца " << (i + 1) << " на строку " << (k + 1) << '\n'; ///////////удалить
                    Xtype Xtemp;
                    for (int l = 0; l < matr_column; l++)
                    {
                        Xtemp = *(*(matr + i) + l);
                        *(*(matr + i) + l) = *(*(matr + l) + k);
                        *(*(matr + l) + k) = Xtemp;
                    }
                    break;
                }
                if (matr_column - 1 == k)
                {
                    cout << "\nНеудалось найти замену для " << (i + 1) << " строки\n"; ///////////удалить
                }
            }
        }
    /////return matr;
}

int main()
{
    Xtype **matr = nullptr;
    ErrInfo err;
    int matr_column, matr_str;
    char status_code = Good;
    setlocale(LC_ALL, "ru");
    while (status_code == Good)
    {
        string FileAdress;
        cout << "\nСчитывание из файла:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
        if (!GettingNameOrExit(FileAdress, '*'))
        {
            break;
        }

        if ((status_code = LoadMatr(matr, FileAdress, matr_str, matr_column, &err)) != Good) //функция загрузки матрицы
        {
            ErrCheck(FileAdress, status_code, &err); //status_code = ErrCheck(FileAdress, status_code, &err); прежний вариант
            status_code = Good;
            continue;
        }

        CoutMatr(cout, matr, matr_str, matr_column);                                 //фунция форматированного вывода в консоль
        Xtype **copymatr = CopyMatr(matr, matr_str, matr_column, status_code, &err); //создание копии матрицы
        if (status_code != Good)                                                     /////или if(copymatr == nullptr)
        {
            ErrCheck(FileAdress, status_code, &err); //функция обработки ошибок & //status_code = ErrCheck(FileAdress, status_code, &err); прежний вариант
            status_code = Good;
        }
        else
        {
            ResultFunc(copymatr, matr_str, matr_column); //функция по заданию
            //CoutMatr(matr, matr_str, matr_column);       //вывод начальной матрицы  ////////УДАЛИТЬ!!!
            CoutMatr(cout, copymatr, matr_str, matr_column); //вывод копии матрицы, после обработки функцией
            cout << "\nЗапись в файл:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
            while (true)
            {
                if (GettingNameOrExit(FileAdress, '*'))
                {
                    ofstream file(FileAdress);
                    if (file.is_open())
                        OutputMatr(file, matr, matr_str, matr_column);
                    else
                    {
                        status_code = FileOpenErr;
                        ErrCheck(FileAdress, status_code, &err); //status_code = ErrCheck(FileAdress, status_code, &err); прежний вариант
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
    }
    return 0;
}