#ifndef _BACKEND_H
#define _BACKEND_H

#include <System.Classes.hpp>
#include <Vcl.Grids.hpp>
//#include "wrapper_functions.h"
typedef int __matrix_elem_t;
typedef __matrix_elem_t *__matrix_str_t;
typedef __matrix_str_t *__matrix_t;
typedef DynamicArray<DynamicArray<__matrix_elem_t>> __dmatrix_t;

enum TStatusCode
{
	EGood = 0,
	EEmptyFile,
	EMemAlloc,
	EFileIntegrity,
	EOpenFile,
	ESaveFile,
	EInvalidInput
};

union TErrInfo
{
	int MemAllocStep;
	String *InvalidInput;
};

//struct TStudentsData
//{
//	int signature;
//	int hash;
//	__dmatrix_t marks;
//	DynamicArray<String> Subjects;
//	DynamicArray<String> Students;
//};

TStatusCode LoadMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep = nullptr);
TStatusCode SaveMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep = nullptr);
#endif
