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

typedef double __matrix_elem_t;
typedef __matrix_elem_t *__matrix_str_t;
typedef __matrix_str_t *__matrix_t;

StatusCode LoadMatrix(__matrix_t &matrix, int &matrix_str, int &matrix_column, const std::string &FileAdress,
                      std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck = nullptr, ErrInfo *err = nullptr);
__matrix_t CopyMatrix(__matrix_elem_t const *const *matr, int matr_str, int matr_column, StatusCode &status_code, ErrInfo *err = nullptr);
void DeleteMatrix(__matrix_t &matrix, int matrix_str);
__matrix_t ResultFunc(__matrix_t matrix, int matrix_str, int matrix_column);
std::ostream &OutputMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column,
                           std::streamsize width = 0, std::streamsize precision = 6, std::ios::fmtflags flags = (std::ios::fmtflags)0);
std::ostream &CoutPerfectMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column);

#endif