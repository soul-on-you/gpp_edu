//---------------------------------------------------------------------------

#pragma hdrstop

#include "BackEnd.h"
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

TStatusCode LoadMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep)
{
    ifstream file;
    file.open(AnsiString(FileName).c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;
    char signature[4] = "APC";
    /*int  signature = 0    , */ int hash = 0;        ///ПОКА ДЛЯ ТЕСТА!
    file.read((char *)&signature, sizeof(signature)); //сигнатура
    file.read((char *)&hash, sizeof(hash));           //хэш

    int studentCount;
    file.read((char *)&studentCount, sizeof(studentCount)); // кол-во студентов и карта имен студентов
    Table->RowCount = studentCount + 1;
    //	(*Table).ColCount = studentCount + 1;
    for (int i = 0, curStrSize; i < studentCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //		String T1;
        //		file.read((char *)T1.c_str(), curStrSize);
        //		Table->Cells[1][1 + i] = T1.c_str();
        //		Table->Cells[1][1 + i] =L"";
        file.read((char *)Table->Cells[1][1 + i].c_str(), curStrSize);
        Table->Cells[1][1 + i] = Table->Cells[1][1 + i].c_str();
        Table->Cells[0][i + 1] = IntToStr(i + 1);
        //		file.read((char *)&Table->Cells[1][1+i], curStrSize);  //////////
    }
    //	Table->ColCount = studentCount + 1;

    int subjectCount;
    file.read((char *)&subjectCount, sizeof(subjectCount)); // кол-во предметов и карта названий предметов
    Table->ColCount = subjectCount + 2;
    for (int i = 0, curStrSize; i < subjectCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //		String T1;
        //		file.read((char *)T1.c_str(), curStrSize);
        //		Table->Cells[2 + i][0] = T1.c_str();
        //		Table->Cells[2 + i][0] = L"";
        file.read((char *)Table->Cells[2 + i][0].c_str(), curStrSize /*Table->Cells[2 + i][0].Length()*2*/);
        //        Table->Cells[2 + i][0].SetLength(curStrSize/2);
        Table->Cells[2 + i][0] = Table->Cells[2 + i][0].c_str();
        //		int i1 = Table->Cells[2 + i][0].Length();
        //		Table->Cells[2 + i][0] = IntToStr(i1);      //Table->Cells[2 + i][0].ElementSize()

        //		file.read((char *)AnsiString(Table->Cells[2+i][0]).c_str(), curStrSize);  //////////
    }
    //	Table->ColCount = subjectCount + 2;

    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0, tmp; j < subjectCount; j++)
        {
            file.read((char *)&tmp, sizeof(tmp));
            Table->Cells[2 + j][1 + i] = IntToStr(tmp);
        }
    file.close();
    return EGood;
}

TStatusCode SaveMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep)
{
    // Добавить проверку файла на существование
    fstream file;
    file.open(AnsiString(FileName).c_str(), std::ios_base::in);
    if (file.is_open())
    {
    }
    file.open(AnsiString(FileName).c_str(), std::ios_base::trunc | std::ios_base::out | std::ios_base::binary); //std::ios_base::out | std::ios_base::binary
    if (!file.is_open())
        return EOpenFile;
    //	int  signature = 0    , hash  = 0 ;
    char signature[4] = "APC";
    int size = 0, hash = 0;
    //	file.write((char *)&signature, sizeof(signature)); //сигнатура
    //	file.write((char *)&hash, sizeof(hash));           //хэш
    file.seekp(8, std::ios_base::cur);

    int studentCount = Table->RowCount - 1; // кол-во студентов и карта имен студентов
    file.write((char *)&studentCount, sizeof(studentCount));
    for (int i = 0; i < studentCount; i++)
    {
        file.write((char *)&(const int &)(sizeof(Table->Cells[1][1 + i][1]) * Table->Cells[1][1 + i].Length() + 2), sizeof(int));
        file.write((char *)Table->Cells[1][1 + i].c_str(), sizeof(Table->Cells[1][1 + i][1]) * Table->Cells[1][1 + i].Length() + 2);
    }

    //if (file.fail()) ДОБАВИТЬ К КАЖДОМУ WRITE МОЖЕТ НЕ ХВАТИТЬ ПАМЯТИ
    int subjectCount = Table->ColCount - 2; // кол-во предметов и карта названий предметов
    file.write((char *)&subjectCount, sizeof(subjectCount));
    for (int i = 0; i < subjectCount; i++)
    {
        file.write((char *)&(const int &)(sizeof(Table->Cells[2 + i][0][1]) * Table->Cells[2 + i][0].Length() + 2), sizeof(int));
        file.write((char *)Table->Cells[2 + i][0].c_str(), sizeof(Table->Cells[2 + i][0][1]) * Table->Cells[2 + i][0].Length() + 2);
    }
    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0; j < subjectCount; j++)
            file.write((char *)&(const int &)StrToInt(Table->Cells[j + 2][i + 1]), sizeof(int));

    file.close();
    return EGood;
}