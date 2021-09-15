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
#include "matrix_format.hpp"

bool GettingNameOrExit(std::string &tmp, char SymForExit);
StatusCode AdditionalCheck(__matrix_elem_t const *const *matr, int matr_str, int matr_column);
void ErrCheck(const std::string &FileAdress, StatusCode status_code, ErrInfo *err = nullptr);

int main()
{
    __matrix_t matr = nullptr;
    ErrInfo err;
    int matr_column, matr_str;
    StatusCode status_code;
    setlocale(LC_ALL, "ru");
    while (true)
    {
        std::string FileAdress;
        std::cout << "\nСчитывание из файла:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
        if (!GettingNameOrExit(FileAdress, '*'))
        {
            break;
        }

        if ((status_code = LoadMatrix(matr, matr_str, matr_column, FileAdress, AdditionalCheck, &err)) != Good) //функция загрузки матрицы
        {
            ErrCheck(FileAdress, status_code, &err);
            continue;
        }

        CoutPerfectMatrix(std::cout, matr, matr_str, matr_column);                        //фунция форматированного вывода в консоль
        __matrix_t copymatr = CopyMatrix(matr, matr_str, matr_column, status_code, &err); //создание копии матрицы
        if (status_code != Good)                                                          /////или if(copymatr == nullptr)
            ErrCheck(FileAdress, status_code, &err);                                      //функция обработки ошибок
        else
        {
            //ResultFunc(copymatr, matr_str, matr_column);            //функция по заданию
            CoutPerfectMatrix(std::cout, copymatr = ResultFunc(copymatr, matr_str, matr_column), matr_str, matr_column); //вывод копии матрицы, после обработки функцией
            std::cout << "\nЗапись в файл:\nВведите имя файла (чтобы завершить работу программы, введите *): ";
            while (true)
            {
                if (GettingNameOrExit(FileAdress, '*'))
                {
                    std::ofstream file(FileAdress);
                    if (file.is_open())
                        OutputMatrix(file, copymatr, matr_str, matr_column);
                    else
                    {
                        status_code = FileOpenErr;
                        ErrCheck(FileAdress, status_code, &err);
                    }
                }
                else
                    status_code = Exit;
                break;
            }
            DeleteMatrix(copymatr, matr_str); //функция удаления копии матрицы
        }
        DeleteMatrix(matr, matr_str); //функция удаления матрицы
        if (status_code == Exit)
            break;
    }
    return 0;
}

bool GettingNameOrExit(std::string &tmp, char SymForExit)
{
    std::cin.sync_with_stdio(false);
    if (std::cin.peek() == SymForExit && std::cin.rdbuf()->in_avail() == 2)
    {
        std::cin.sync_with_stdio(true);
        return false;
    }
    std::cin.sync_with_stdio(true);
    getline(std::cin, tmp);
    return true;
}
StatusCode AdditionalCheck(__matrix_elem_t const *const *matr, int matr_str, int matr_column)
{
    if (!matr)
        if (matr_str != matr_column)
        {
            return NotSquareMatrixErr;
        }
    return Good;
}
void ErrCheck(const std::string &FileAdress, StatusCode status_code, ErrInfo *err)
{
    if (err)
    {
        switch (status_code)
        {
        case FailbitErr:
        {
            std::string tmp;
            std::ifstream file(FileAdress);
            file.seekg(err->fbit.pos);
            file >> tmp;
            file.close();
            std::cout << "\nНайдена ошибка в элементе номер " << (err->fbit.matr_column + 1) << " на строке " << (err->fbit.matr_str + 1)
                      << "\nАбсолютная позиция в файле " << err->fbit.pos
                      << "\nНеверное значение: \"" << tmp << '\"' << '\n'
                      << "\nФайл содержит некорректные значения\n";
            err->fbit.matr_column = 0, err->fbit.matr_str = 0, err->fbit.pos = 0;
        }
        break;
        case EmptyFileErr:
            std::cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case NotRectangularMatrixErr:
            std::cout << "\nНе правильный формат, количество элементов строки " << err->NotRectangular_str_in_Matrix
                      << " не совпадает со стокой " << (err->NotRectangular_str_in_Matrix - 1) << '\n';
            err->NotRectangular_str_in_Matrix = 0;
            break;
        case NotSquareMatrixErr:
            std::cout << "\nНе правильный формат, матрица не кваратная\n";
            break;
        case FileOpenErr:
            perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
            break;
#ifdef allocmemory_secure
        case MemoryLeakErr:
            cout << "\nОшибка, попытка инициализации указателя, адрес которого указывает на существующий массив, это вызовет утечку памяти\n";
            break;
#endif
        case FailAllocMemmoryErr:
            if (err->MemAllocStepID == -1)
                std::cout << "\nОшибка, не удалось выделить память под массив указателей\n";
            else
                std::cout << "\nОшибка, не удалось выделить память под " << err->MemAllocStepID << " строку\n";
            err->MemAllocStepID = 0;
            break;
        default:
            std::cout << "\nОтсутствует обработка данного типа ошибки\n";
        }
    }
    else
        std::cout << "\nКод ошибки: " << status_code << '\n';
}