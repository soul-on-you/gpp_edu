#pragma once
#ifndef _MATRIX_FUNCTION_MODUL
#define _MATRIX_FUNCTION_MODUL

#include <ostream>
#include <string>
#include <functional>

#define PREPARETORY_STEP -1

enum StatusCode
{
    Exit = -1,
    Good,
    FailbitErr,
    EmptyFileErr,
    FileOpenErr,
    NotSquareMatrixErr,
    NotRectangularMatrixErr,
    FailAllocMemmoryErr,
    InvalidCastMatrixToDinamic,
    InvalidCastDinamicToMatrix,
    InvalidMultiplier
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
    std::string *InvalidMultiplier;
};

typedef int __matrix_elem_t;
typedef __matrix_elem_t *__matrix_str_t;
typedef __matrix_str_t *__matrix_t;

StatusCode LoadMatrix(const std::string &FileAdress, __matrix_t &matrix, int &matrix_str, int &matrix_column,
                      std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck = nullptr, ErrInfo *err = nullptr);
__matrix_t CopyMatrix(__matrix_elem_t const *const *matr, int matr_str, int matr_column, StatusCode &status_code, ErrInfo *err = nullptr);
void DeleteMatrix(__matrix_t &matrix, int matrix_str);
__matrix_t ResultFunc(__matrix_t matrix, int matrix_str, int matrix_column);
std::ostream &OutputMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column,
                           std::streamsize width = 0, std::streamsize precision = 6, std::ios::fmtflags flags = (std::ios::fmtflags)0);
std::ostream &CoutPerfectMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column);
StatusCode CreateMatrix(__matrix_elem_t **&matr, int matr_str, int matr_column, int *MemAllocStepID = nullptr);

#endif