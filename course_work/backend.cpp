//---------------------------------------------------------------------------

#pragma hdrstop

#include "BackEnd.h"
#include "matrix_functions_lib.h"
#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include <fstream>

//---------------------------------------------------------------------------
#pragma package(smart_init)

int HashCheck(void const *buffer, size_t BUFFER_SIZE)
{
    boost::crc_basic<32> crc(0x27809EA7, 0u, 0u, true, true);
    crc.process_bytes(buffer, BUFFER_SIZE);
    return crc.checksum();
}

TStatusCode LoadMatrix(const String &FileName, TStudentsData &data, int *EMemAllocStep)
{
    fstream file;
    file.open(AnsiString(FileName).c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        return EOpenFile;
    file.read((char *)&data.signature, sizeof(data.signature)); //сигнатура
    file.read((char *)&data.hash, sizeof(data.hash));           //хэш

    try
    { // кол-во студентов
        int count;
        file.read((char *)&count, sizeof(count));
        data.Students.Length = count;
    }
    catch (...)
    {
        data.Students.Length = 0; ///////////??????????
        return EMemAlloc;

        //		for (int i = 0; i < data.Students.Length; i++)
        //			file.read((char *) AnsiString(data.Students[i]).c_str() );
        //			students[i] = AnsiString(data.Students[i]).Length() + 1;
        //		file.write((char *)students, sizeof(*students) * data.Students.Length);
        //		for (int i = 0; i < data.Students.Length; i++)
        //			file.write((char *)AnsiString(data.Students[i]).c_str(), students[i]);
    }
    __matrix_str_t students = new (std::nothrow) __matrix_elem_t[data.Students.Length]; //карта имен студентов
    if (!students)
        return EMemAlloc;
    file.read((char *)students, sizeof(__matrix_elem_t) * data.Students.Length);
    for (int i = 0; i < data.Students.Length; i++)
        file.read((char *)AnsiString(data.Students[i]).c_str(), students[i]);
    delete[] students;

    try
    { // кол-во предметов
        int count;
        file.read((char *)&count, sizeof(count));
        data.Subjects.Length = count;
    }
    catch (...)
    {
        data.Students.Length = 0; ///////////??????????
        return EMemAlloc;
    }
    __matrix_str_t subjects = new (std::nothrow) __matrix_elem_t[data.Subjects.Length]; //карта названий предметов
    if (!subjects)
        return EMemAlloc;
    file.read((char *)subjects, sizeof(__matrix_elem_t) * data.Subjects.Length);
    for (int i = 0; i < data.Subjects.Length; i++)
        file.read((char *)AnsiString(data.Subjects[i]).c_str(), subjects[i]);
    delete[] subjects;

    __matrix_t matrix;
    CreateMatrix(matrix, data.Students.Length, data.Subjects.Length, EMemAllocStep);
    //	__matrix_t matrix = castDinamicToMatrix(data.marks, false, EMemAllocStep);   // оценки по [студенту] по [предмету]
    if (!matrix)
        return EMemAlloc;
    for (int i = 0; i < data.Students.Length; i++)
        file.read((char *)matrix[i], sizeof(int) * data.Subjects.Length);
    data.marks = castMatrixToDynamic(matrix, data.Students.Length, data.Subjects.Length, true, EMemAllocStep);
    return EGood;
}

TStatusCode SaveMatrix(const String &FileName, TStudentsData &data, int *EMemAllocStep)
{
    fstream file;
    file.open(AnsiString(FileName).c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
    if (!file.is_open())
        return EOpenFile;

    file.write((char *)&data.signature, sizeof(data.signature)); //сигнатура
    file.write((char *)&data.hash, sizeof(data.hash));           //хэш

    //	String S1 = AnsiString(typeid(*(const int&)(data.Students.Length)).name()).c_str();

    //	file.write((char*)(int&)(const int&)(data.Students.Length), sizeof(data.Students.Length));    // кол-во студентов   //без &
    {
        int count = data.Students.Length;
        file.write((char *)&count, sizeof(count));
    }
    __matrix_str_t students = new (std::nothrow) __matrix_elem_t[data.Students.Length]; //карта имен студентов
    if (!students)
        return EMemAlloc;
    for (int i = 0; i < data.Students.Length; i++)
        students[i] = AnsiString(data.Students[i]).Length() + 1;
    file.write((char *)students, sizeof(*students) * data.Students.Length);
    for (int i = 0; i < data.Students.Length; i++)
        file.write((char *)AnsiString(data.Students[i]).c_str(), students[i]);
    delete[] students;

    //		file.write((char*)(int(data.Students.Length)), sizeof(data.Subjects.Length));      // кол-во предметов       //без &
    {
        int count = data.Subjects.Length;
        file.write((char *)&count, sizeof(count));
    }
    __matrix_str_t subjects = new (std::nothrow) __matrix_elem_t[data.Subjects.Length]; //карта названий предметов
    if (!subjects)
        return EMemAlloc;
    for (int i = 0; i < data.Subjects.Length; i++)
        subjects[i] = AnsiString(data.Subjects[i]).Length() + 1;
    file.write((char *)subjects, sizeof(*subjects) * data.Subjects.Length);
    for (int i = 0; i < data.Subjects.Length; i++)
        file.write((char *)AnsiString(data.Subjects[i]).c_str(), subjects[i]);
    delete[] subjects;

    __matrix_t matrix = castDinamicToMatrix(data.marks, false, EMemAllocStep); // оценки по [студенту] по [предмету]
    if (!matrix)
        return EMemAlloc;
    for (int i = 0; i < data.Students.Length; i++)
        file.write((char *)matrix[i], sizeof(int) * data.Subjects.Length);
    DeleteMatrix(matrix, data.marks.Length);

    file.close();
}