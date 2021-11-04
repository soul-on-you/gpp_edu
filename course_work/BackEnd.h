#ifndef _BACKEND_H
#define _BACKEND_H

#include <System.hpp>
#include "wrapper_functions.h"
typedef DynamicArray<DynamicArray<__matrix_elem_t>> __dmatrix_t;

enum TStatusCode
{
	EGood = 0,
	EEmptyFile,
	EMemAlloc,
	EFileIntegrity,
	EOpenFile,
	ESaveFile
};

union TErrInfo
{
	int MemAllocStep;
	int Other;
};

struct TStudentsData
{
	int signature;
	int hash;
	__dmatrix_t marks;
	DynamicArray<String> Subjects;
	DynamicArray<String> Students;
};

TStatusCode LoadMatrix(const String &FileName, TStudentsData &data, int *EMemAllocStep = nullptr);
TStatusCode SaveMatrix(const String &FileName, TStudentsData &data, int *EMemAllocStep = nullptr);
#endif
