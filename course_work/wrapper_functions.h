#pragma once
#ifndef _MATRIX_CAST_FUNCTIONS
#define _MATRIX_CAST_FUNCTIONS
#include <System.Classes.hpp>
#include "matrix_functions_lib.h"

typedef DynamicArray<DynamicArray<__matrix_elem_t>> __dmatrix_t;

//__dmatrix_t castMatrixToDynamic(__matrix_t &oldMatrix, int &oldMatrixStr, int &oldMatrixColumn, StatusCode& status,
//	 bool deleteOldMatrix = true, ErrInfo *err = nullptr);

__dmatrix_t castMatrixToDynamic(__matrix_t &oldMatrix, int oldMatrixStr, int oldMatrixColumn, bool deleteOldMatrix = true,
								int *MemAllocStepID = nullptr);
__matrix_t castDinamicToMatrix(__dmatrix_t &oldMatrix, bool deleteOldMatrix = true, int *MemAllocStepID = nullptr);
//StatusCode eLoadMatrix(__dmatrix_t &dmatrix, int &matrix_str, int &matrix_column, const String &FileAdress,
//					   std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck = nullptr, ErrInfo *err = nullptr);
StatusCode eLoadMatrix(const String &FileAdress, __dmatrix_t &dmatrix,
					   std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck = nullptr, ErrInfo *err = nullptr);

//String eCoutPerfectMatrix(__dmatrix_t matrix, int matrix_str, int matrix_column);
//String eCoutPerfectMatrixV2(__dmatrix_t dmatrix, StatusCode& status, ErrInfo *err = nullptr);
String eCoutPerfectMatrixV2(__dmatrix_t &dmatrix, int *MemAllocStepID = nullptr);
String eOutputMatrix(__dmatrix_t &dmatrix, std::streamoff width = 0, std::streamsize precision = 6,
					 std::ios::fmtflags flags = (std::ios::fmtflags)0, int *MemAllocStepID = nullptr);
//__dmatrix_t eResultFunc(const __matrix_elem_t& multiplierNum, __dmatrix_t dmatrix, StatusCode& status, ErrInfo *err = nullptr);
///////////////////////////////
//__dmatrix_t eResultFunc(const __matrix_elem_t& multiplierNum, __dmatrix_t& dmatrix, int* MemAllocStepID = nullptr);   ///++++++
///////////////////////////////
bool eResultFunc(const __matrix_elem_t &multiplierNum, __dmatrix_t &dmatrix, int *MemAllocStepID = nullptr);
#endif
