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

TStatusCode LoadMatrix(const String &FileName, TStudentsData &data, TStringGrid *Table, int *EMemAllocStep)
{
    fstream file;
    file.open(AnsiString(FileName).c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
        return EOpenFile;
    file.read((char *)&data.signature, sizeof(data.signature)); //сигнатура
    file.read((char *)&data.hash, sizeof(data.hash));           //хэш

    int studentCount;
    //	file.read((char*)&Table->ColCount, sizeof(Table->ColCount));
    file.read((char *)&studentCount, sizeof(studentCount));
    Table->ColCount = studentCount + 1; // ..............
    for (int i = 0, curStrSize; i < studentCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //
        //		file.read((char *)AnsiString(T1).c_str(), curStrSize);
        //		Table->Cells[1][1+i] = AnsiString(T1).c_str();
        //		Table->Cells[0][i+1] = IntToStr(i+1);

        String T1; //////////////
        file.read((char *)T1.c_str(), curStrSize);
        Table->Cells[1][1 + i] = T1.c_str();
        Table->Cells[0][i + 1] = IntToStr(i + 1);

        //		file.read((char *)&Table->Cells[1][1+i], curStrSize);  //////////
        //		file.read((char *)AnsiString((String &)Table->Cells[1][1+i]).c_str(), curStrSize);

        //		file.read((char *)AnsiString(Table->Cells[1][1+i]).c_str(), curStrSize);
    }

    int subjectCount;
    //	file.read((char*)&Table->ColCount, sizeof(Table->ColCount));
    file.read((char *)&subjectCount, sizeof(subjectCount));
    Table->ColCount = subjectCount + 2; //  ...............
    for (int i = 0, curStrSize; i < subjectCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        String T1;
        file.read((char *)T1.c_str(), curStrSize);
        Table->Cells[2 + i][0] = T1.c_str();
        //		file.read((char *)AnsiString(Table->Cells[2+i][0]).c_str(), curStrSize);
    }

    for (int i = 0; i < studentCount; i++)
        for (int j = 0, tmp; j < subjectCount; j++)
        {
            file.read((char *)&tmp, sizeof(tmp));
            Table->Cells[2 + j][1 + i] = IntToStr(tmp);
        }

    //	try
    //	{                                                  // кол-во студентов
    //		int count;
    //		file.read((char*)&count, sizeof(count));
    //		data.Students.Length = count;
    //	}
    //	catch(...)
    //	{
    //		data.Students.Length = 0; ///////////??????????
    //		return EMemAlloc;
    //
    //
    //
    ////		for (int i = 0; i < data.Students.Length; i++)
    ////			file.read((char *) AnsiString(data.Students[i]).c_str() );
    ////			students[i] = AnsiString(data.Students[i]).Length() + 1;
    ////		file.write((char *)students, sizeof(*students) * data.Students.Length);
    ////		for (int i = 0; i < data.Students.Length; i++)
    ////			file.write((char *)AnsiString(data.Students[i]).c_str(), students[i]);
    //
    //	}
    //    __matrix_str_t students = new (std::nothrow) __matrix_elem_t[data.Students.Length];   //карта имен студентов
    //	if (!students)
    //		return EMemAlloc;
    //	file.read((char*) students, sizeof(__matrix_elem_t) * data.Students.Length);
    //	for(int i = 0; i < data.Students.Length; i++)
    //		file.read((char*)AnsiString(data.Students[i]).c_str(), students[i]);
    //	delete[] students;

    //	try
    //	{                                                  // кол-во предметов
    //		int count;
    //		file.read((char*)&count, sizeof(count));
    //		data.Subjects.Length = count;
    //	}
    //	catch(...)
    //	{
    //		data.Students.Length = 0; ///////////??????????
    //		return EMemAlloc;
    //	}
    //	__matrix_str_t subjects = new (std::nothrow) __matrix_elem_t[data.Subjects.Length];   //карта названий предметов
    //	if (!subjects)
    //		return EMemAlloc;
    //	file.read((char*) subjects, sizeof(__matrix_elem_t) * data.Subjects.Length);
    //	for(int i = 0; i < data.Subjects.Length; i++)
    //		file.read((char*)AnsiString(data.Subjects[i]).c_str(), subjects[i]);
    //	delete[] subjects;
    //
    //
    //
    //	__matrix_t matrix;
    //	CreateMatrix(matrix, data.Students.Length, data.Subjects.Length, EMemAllocStep);
    ////	__matrix_t matrix = castDinamicToMatrix(data.marks, false, EMemAllocStep);   // оценки по [студенту] по [предмету]
    //	if (!matrix)
    //		return EMemAlloc;
    //	for (int i = 0; i < data.Students.Length; i++)
    //		file.read((char *)matrix[i], sizeof(int) * data.Subjects.Length);
    //	data.marks = castMatrixToDynamic(matrix, data.Students.Length, data.Subjects.Length, true, EMemAllocStep);
    return EGood;
}

TStatusCode SaveMatrix(const String &FileName, TStudentsData &data, TStringGrid *Table, int *EMemAllocStep)
{
    fstream file;
    file.open(AnsiString(FileName).c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
    if (!file.is_open())
        return EOpenFile;

    file.write((char *)&data.signature, sizeof(data.signature)); //сигнатура
    file.write((char *)&data.hash, sizeof(data.hash));           //хэш

    //	String S1 = AnsiString(typeid(*(const int&)(data.Students.Length)).name()).c_str();

    //	file.write((char*)(int&)(const int&)(data.Students.Length), sizeof(data.Students.Length));    // кол-во студентов   //без &
    //	{
    //		int count =  data.Students.Length;
    //		file.write((char*)&count, sizeof(count));
    //	}
    int studentCount = Table->RowCount - 1;
    file.write((char *)&studentCount, sizeof(studentCount));
    for (int i = 0; i < studentCount; i++)
    {
        //		file.write((char *)&(const int&)(Table->Cells[1][1+i].Length() +1), sizeof(int));
        //		file.write((char *)AnsiString(Table->Cells[1][1+i]).c_str(), Table->Cells[1][1+i].Length() +1);
        file.write((char *)&(const int &)(sizeof(Table->Cells[1][1 + i][1]) * Table->Cells[1][1 + i].Length() + 2), sizeof(int));
        file.write((char *)Table->Cells[1][1 + i].c_str(), sizeof(Table->Cells[1][1 + i][1]) * Table->Cells[1][1 + i].Length() + 2);
    }

    //	__matrix_str_t students = new (std::nothrow) __matrix_elem_t[data.Students.Length];   //карта имен студентов
    //	if (!students)
    //		return EMemAlloc;
    //	for (int i = 0; i < data.Students.Length; i++)
    //		students[i] = AnsiString(data.Students[i]).Length() + 1;
    //	file.write((char *)students, sizeof(*students) * data.Students.Length);
    //	for (int i = 0; i < data.Students.Length; i++)
    //		file.write((char *)AnsiString(data.Students[i]).c_str(), students[i]);
    //	delete[] students;

    //		file.write((char*)(int(data.Students.Length)), sizeof(data.Subjects.Length));      // кол-во предметов       //без &
    int subjectCount = Table->ColCount - 2;
    //	{
    //		int count =  data.Subjects.Length;
    //		file.write((char*)&count, sizeof(count));
    //	}

    file.write((char *)&subjectCount, sizeof(subjectCount));
    for (int i = 0; i < subjectCount; i++)
    {
        //		file.write((char *)&(const int&)(Table->Cells[2+i][0].Length() +1), sizeof(int));
        //		file.write((char *)AnsiString(Table->Cells[2+i][0]).c_str(), Table->Cells[2+i][0].Length() +1);
        file.write((char *)&(const int &)(sizeof(Table->Cells[2 + i][0][1]) * Table->Cells[2 + i][0].Length() + 2), sizeof(int));
        file.write((char *)Table->Cells[2 + i][0].c_str(), sizeof(Table->Cells[2 + i][0][1]) * Table->Cells[2 + i][0].Length() + 2);
        //		file.write((char *)Table->Cells[1][1+i].c_str(), sizeof(Table->Cells[1][1+i][1]) * Table->Cells[1][1+i].Length() +2);
    }

    //	__matrix_str_t subjects = new (std::nothrow) __matrix_elem_t[data.Subjects.Length]; //карта названий предметов
    //	if (!subjects)
    //		return EMemAlloc;
    //	for (int i = 0; i < data.Subjects.Length; i++)
    //		subjects[i] = AnsiString(data.Subjects[i]).Length() + 1;
    //	file.write((char *)subjects, sizeof(*subjects) * data.Subjects.Length);
    //	for (int i = 0; i < data.Subjects.Length; i++)
    //		file.write((char *)AnsiString(data.Subjects[i]).c_str(), subjects[i]);
    //	delete[] subjects;

    for (int i = 0; i < studentCount; i++)
        for (int j = 0; j < subjectCount; j++)
            file.write((char *)&(const int &)StrToInt(Table->Cells[j + 2][i + 1]), sizeof(int));

    //	__matrix_t matrix = castDinamicToMatrix(data.marks, false, EMemAllocStep);   // оценки по [студенту] по [предмету]
    //	if (!matrix)
    //		return EMemAlloc;
    //	for (int i = 0; i < data.Students.Length; i++)
    //		file.write((char *)matrix[i], sizeof(int) * data.Subjects.Length);
    //	DeleteMatrix(matrix, data.marks.Length);

    file.close();
}