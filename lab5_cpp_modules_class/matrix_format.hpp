#ifndef _MATRIX_FORMAT
#define _MATRIX_FORMAT

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <functional>
#include <sstream>

typedef double __matrix_elem_t;
typedef __matrix_elem_t *__matrix_str_t;
typedef __matrix_str_t *__matrix_t;

class Matrix
{
    __matrix_t _matrix;
    int _str_count;
    int _column_count;

public:
    StatusCode LoadMatrix(const std::string &FileAdress, ErrInfo * = nullptr);
    std::ostream OutputMatrix(std::ostream &out, std::streamsize width = 0,
                              std::streamsize precision = 6, std::ios::fmtflags flags = (std::ios::fmtflags)0);
};

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
        std::streamoff pos;
    } fbit;
};

#endif