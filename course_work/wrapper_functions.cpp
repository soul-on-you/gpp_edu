#include "wrapper_functions.h"
#include <sstream>
#include <iomanip>
#include <cmath>

template <typename input>
size_t TextVievSize(input a, std::ios::fmtflags flags = (std::ios::fmtflags)0, std::streamsize prec = 6)
{
    std::ios::fmtflags resetflags = (std::ios::fmtflags)0;
    if (flags)
    {
        if (flags & std::ios::basefield)
            resetflags = resetflags | std::ios::basefield;
        if (flags & std::ios::floatfield)
            resetflags = resetflags | std::ios::floatfield;
    }
    return ((std::ostringstream &)(std::ostringstream() << std::resetiosflags(resetflags) << std::setiosflags(flags) << std::setprecision(prec) << a)).str().size();
}

__dmatrix_t castMatrixToDynamic(__matrix_t &oldMatrix, int oldMatrixStr, int oldMatrixColumn, bool deleteOldMatrix,
                                int *MemAllocStepID)
{
    __dmatrix_t newMatrix;
    if (oldMatrix && oldMatrixStr > 0 && oldMatrixColumn > 0)
    {
        int matrixStr = PREPARETORY_STEP;
        try
        {
            newMatrix.Length = oldMatrixStr;
            for (matrixStr = 0; matrixStr < oldMatrixStr; matrixStr++)
            {
                newMatrix[matrixStr].Length = oldMatrixColumn;
            }
        }
        catch (...)
        {
            newMatrix.Length = 0;
            if (MemAllocStepID)
                *MemAllocStepID = matrixStr;
        }
        if (newMatrix.Length != 0)
        {
            for (int matrixStr = 0; matrixStr < oldMatrixStr; matrixStr++)
            {
                for (int matrixColumn = 0; matrixColumn < oldMatrixColumn; matrixColumn++)
                {
                    newMatrix[matrixStr][matrixColumn] = oldMatrix[matrixStr][matrixColumn];
                }
            }
        }
        if (deleteOldMatrix)
            DeleteMatrix(oldMatrix, oldMatrixStr);
    }
    return newMatrix;
}

__matrix_t castDinamicToMatrix(__dmatrix_t &oldMatrix, bool deleteOldMatrix, int *MemAllocStepID)
{
    __matrix_t newMatrix;
    if (oldMatrix.Length != 0)
    {
        if (CreateMatrix(newMatrix, oldMatrix.Length, oldMatrix[0].Length, MemAllocStepID) == Good)
        {
            for (int matrixStr = 0; matrixStr != oldMatrix.Length; matrixStr++)
            {
                for (int matrixColumn = 0; matrixColumn != oldMatrix[matrixStr].Length; matrixColumn++)
                {
                    newMatrix[matrixStr][matrixColumn] = oldMatrix[matrixStr][matrixColumn];
                }
            }
        }
        if (deleteOldMatrix)
            oldMatrix.Length = 0;
    }
    return newMatrix;
}

StatusCode eLoadMatrix(const String &FileAdress, __dmatrix_t &dmatrix,
                       std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck, ErrInfo *err)
{
    int matrix_str = 0, matrix_column = 0;
    __matrix_t pMatrix;
    StatusCode status = LoadMatrix(AnsiString(FileAdress).c_str(), pMatrix, matrix_str, matrix_column, fAdditionalCheck, err);
    if (status == Good)
        dmatrix = castMatrixToDynamic(pMatrix, matrix_str, matrix_column, true, &err->MemAllocStepID);
    return status;
}

//String eCoutPerfectMatrix(__dmatrix_t  matrix, int matrix_str, int matrix_column)
//{
//	std::ostringstream ostr("", std::ostringstream::ate);
//	std::ostringstream numberstr("|   |", std::ostringstream::ate);
//	std::ostringstream skipstr("+---+", std::ostringstream::ate);
//	for (int j = 0; j < matrix_column; j++)
//	{
//		skipstr << std::setw(12) << std::setfill('-') << '+';
//		numberstr << std::setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << std::setw(6 - TextVievSize(j) / 2) << '|';
//	}
//	ostr // << '\r' << '\n'
//		 << skipstr.str() << '\r' << '\n'
//		 << numberstr.str() << '\r' << '\n'
//		 << skipstr.str() << '\r' << '\n';
//	for (int i = 0; i < matrix_str; i++)
//	{
//		ostr << "| " << (i + 1) << " |";
//		for (int j = 0; j < matrix_column; j++)
//		{
//			ostr << std::setw(10) << std::right << std::scientific << std::setprecision(2) << matrix[i][j]  << " |"; // *(*(matrix + i) + j)
//		}
//		ostr << '\r' << '\n'
//			 << skipstr.str() << '\r' << '\n';
//	}
//	return ostr.str().c_str();
//}

String eCoutPerfectMatrixV2(__dmatrix_t &dmatrix, int *MemAllocStepID)
{
    std::ostringstream ostr("", std::ostringstream::ate);
    if (dmatrix.Length > 0)
    {
        //__matrix_t castDinamicToMatrix(__dmatrix_t &oldMatrix, bool deleteOldMatrix, int* MemAllocStepID)
        __matrix_t matrix = castDinamicToMatrix(dmatrix, false, MemAllocStepID);
        if (matrix)
        {
            CoutPerfectMatrix(ostr, matrix, dmatrix.Length, dmatrix[0].Length);
            DeleteMatrix(matrix, dmatrix.Length);
        }
    }
    return ostr.str().c_str();
}

//std::ostream &OutputMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column,
//						   std::streamsize width = 0, std::streamsize precision = 6, std::ios::fmtflags flags = (std::ios::fmtflags)0);

String eOutputMatrix(__dmatrix_t &dmatrix, std::streamoff width, std::streamsize precision,
                     std::ios::fmtflags flags, int *MemAllocStepID)
{
    std::ostringstream ostr("", std::ostringstream::ate);
    if (dmatrix.Length > 0)
    {
        __matrix_t matrix = castDinamicToMatrix(dmatrix, false, MemAllocStepID);
        if (matrix)
        {
            OutputMatrix(ostr, matrix, dmatrix.Length, dmatrix[0].Length, width, precision, flags);
            DeleteMatrix(matrix, dmatrix.Length);
        }
    }
    return ostr.str().c_str();
}
//__dmatrix_t eResultFunc( __dmatrix_t dmatrix, StatusCode& status, ErrInfo *err)

//---------------------------------------------------------------------------------
//__dmatrix_t eResultFunc(const __matrix_elem_t& multiplierNum, __dmatrix_t& dmatrix, int* MemAllocStepID)
//{
//	if(dmatrix.Length != 0)
//	{
//		__matrix_t matrix = castDinamicToMatrix(dmatrix, false, MemAllocStepID);
//		if(matrix)
//		{
////			int matrixStr = dmatrix.Length, matrixColumn = dmatrix[0].Length;
//			matrix = ResultFunc(matrix, dmatrix.Length, dmatrix[0].Length);
//			dmatrix = castMatrixToDynamic(matrix, dmatrix.Length, dmatrix[0].Length, true, MemAllocStepID);
//			for(int MatrixStr = 0; MatrixStr < dmatrix.Length; MatrixStr++)
//				for(int MatrixColumn = 0; MatrixColumn < dmatrix[MatrixStr].Length; MatrixColumn++)
//					dmatrix[MatrixStr][MatrixColumn] *= multiplierNum;
//		}
//	}
//   return dmatrix;
//---------------------------------------------------------------------------------

bool eResultFunc(const __matrix_elem_t &multiplierNum, __dmatrix_t &dmatrix, int *MemAllocStepID)
{
    if (dmatrix.Length != 0)
    {
        __matrix_t matrix = castDinamicToMatrix(dmatrix, false, MemAllocStepID);
        if (matrix)
        {
            matrix = ResultFunc(matrix, dmatrix.Length, dmatrix[0].Length);
            dmatrix = castMatrixToDynamic(matrix, dmatrix.Length, dmatrix[0].Length, true, MemAllocStepID);
            for (int MatrixStr = 0; MatrixStr < dmatrix.Length; MatrixStr++)
                for (int MatrixColumn = 0; MatrixColumn < dmatrix[MatrixStr].Length; MatrixColumn++)
                    dmatrix[MatrixStr][MatrixColumn] *= multiplierNum;
            return true;
        }
    }
    return false;
}