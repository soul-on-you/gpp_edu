//---------------------------------------------------------------------------

#pragma hdrstop

#include "BackEnd.h"
#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

TStatusCode TMainForm::LoadMatrix(const String &FileName, TStringGrid *curTable, int *EMemAllocStep)
{
    ifstream file;
    file.open(AnsiString(FileName).c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;
    //    char signature[4] = "APC";
    int signature = 0;
    /*int  signature = 0    , */ int hash = 0;        ///ПОКА ДЛЯ ТЕСТА!
    file.read((char *)&signature, sizeof(signature)); //сигнатура
    file.read((char *)&hash, sizeof(hash));           //хэш

    int studentCount;
    file.read((char *)&studentCount, sizeof(studentCount)); // кол-во студентов и карта имен студентов
    curTable->RowCount = studentCount + 1;
    //	(*Table).ColCount = studentCount + 1;
    for (int i = 0, curStrSize; i < studentCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //		String T1;
        //		file.read((char *)T1.c_str(), curStrSize);
        //		Table->Cells[1][1 + i] = T1.c_str();
        curTable->Cells[1][1 + i] = L"";
        file.read((char *)curTable->Cells[1][1 + i].c_str(), curStrSize);
        curTable->Cells[1][1 + i] = curTable->Cells[1][1 + i].c_str();
        curTable->Cells[0][i + 1] = IntToStr(i + 1);
        //		file.read((char *)&Table->Cells[1][1+i], curStrSize);  //////////
    }
    //	Table->ColCount = studentCount + 1;

    int subjectCount;
    file.read((char *)&subjectCount, sizeof(subjectCount)); // кол-во предметов и карта названий предметов
    curTable->ColCount = subjectCount + 2;
    for (int i = 0, curStrSize; i < subjectCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //		String T1;
        //		file.read((char *)T1.c_str(), curStrSize);
        //		Table->Cells[2 + i][0] = T1.c_str();
        curTable->Cells[2 + i][0] = L"";
        file.read((char *)curTable->Cells[2 + i][0].c_str(), curStrSize /*Table->Cells[2 + i][0].Length()*2*/);
        //        Table->Cells[2 + i][0].SetLength(curStrSize/2);
        curTable->Cells[2 + i][0] = curTable->Cells[2 + i][0].c_str();
        //		int i1 = Table->Cells[2 + i][0].Length();
        //		Table->Cells[2 + i][0] = IntToStr(i1);      //Table->Cells[2 + i][0].ElementSize()

        //		file.read((char *)AnsiString(Table->Cells[2+i][0]).c_str(), curStrSize);  //////////
    }
    //	Table->ColCount = subjectCount + 2;

    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0, tmp; j < subjectCount; j++)
        {
            file.read((char *)&tmp, sizeof(tmp));
            curTable->Cells[2 + j][1 + i] = IntToStr(tmp);
        }
    file.close();
    return EGood;
}

TStatusCode TMainForm::SaveMatrix(TStringGrid *curTable, int *EMemAllocStep, String *FileName)
{
    // Добавить проверку файла на существование
    std::fstream file;
    String newFileName;
    if (!FileName)
    {
        while (true)
        {
            if (!DSaveDialog->Execute())
                return EGood;

            newFileName = DSaveDialog->FileName;
            file.open(AnsiString(newFileName).c_str(), std::ios_base::in);
            if (file.is_open())
            { //filename +  существует/n Заменить? ,     ПОДТВЕРДИТЬ СОХРАНЕНИЕ, ОТМЕНА НЕТ ДА
                int chose = Application->MessageBox((SliceAdressToFileName(newFileName) + L" уже существует\r\nЗамениеть?").w_str(), L"Подтвердить сохранение", MB_YESNOCANCEL);
                file.close();
                if (chose == IDCANCEL)
                    return EGood;
                if (chose == IDNO)
                    continue;
            }
            break;
        }
        FileName = &newFileName;
    }

    file.open(AnsiString(*FileName).c_str(), /*std::ios_base::trunc |*/ std::ios_base::out | std::ios_base::binary); //std::ios_base::out | std::ios_base::binary
    if (!file.is_open())
        return EOpenFile;
    //	int  signature = 0    , hash  = 0 ;
    char signature[4] = "APC";
    //	int hash = 0;
    file.write((char *)signature, sizeof(signature));
    //	file.write((char *)&signature, sizeof(signature)); //сигнатура
    //	file.write((char *)&hash, sizeof(hash));           //хэш
    file.seekp(12, std::ios_base::cur); // FileSize 8b + hash 4b

    int studentCount = curTable->RowCount - 1; // кол-во студентов и карта имен студентов
    file.write((char *)&studentCount, sizeof(studentCount));
    for (int i = 0; i < studentCount; i++)
    {
        file.write((char *)&(const int &)(sizeof(curTable->Cells[1][1 + i][1]) * curTable->Cells[1][1 + i].Length() + 2), sizeof(int));
        file.write((char *)curTable->Cells[1][1 + i].c_str(), sizeof(curTable->Cells[1][1 + i][1]) * curTable->Cells[1][1 + i].Length() + 2);
    }

    //if (file.fail()) ДОБАВИТЬ К КАЖДОМУ WRITE МОЖЕТ НЕ ХВАТИТЬ ПАМЯТИ
    int subjectCount = curTable->ColCount - 2; // кол-во предметов и карта названий предметов
    file.write((char *)&subjectCount, sizeof(subjectCount));
    for (int i = 0; i < subjectCount; i++)
    {
        file.write((char *)&(const int &)(sizeof(curTable->Cells[2 + i][0][1]) * curTable->Cells[2 + i][0].Length() + 2), sizeof(int));
        file.write((char *)curTable->Cells[2 + i][0].c_str(), sizeof(curTable->Cells[2 + i][0][1]) * curTable->Cells[2 + i][0].Length() + 2);
    }
    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0; j < subjectCount; j++)
            file.write((char *)&(const int &)StrToInt(curTable->Cells[j + 2][i + 1]), sizeof(int));
    file.close();

    file.open(AnsiString(*FileName).c_str(), std::ios_base::in);
    std::streamsize fileSize /* = file.tellp()*/, buffer_size;
    file.seekg(0, ios::end);
    fileSize = file.tellg();

    if (fileSize > 1036)
        buffer_size = 1024;
    else
        buffer_size = fileSize;
    char *buffer = new (std::nothrow) char[buffer_size];
    if (!buffer)
    {
        //обработка ошибки выделения памяти
    }

    file.seekg(16, std::ios_base::beg);
    boost::crc_basic<32> crc(0x27809EA7, 0u, 0u, true, true);
    while (!file.eof())
    {
        file.read((char *)buffer, buffer_size);
        crc.process_bytes(buffer, (std::size_t)buffer_size);
    }
    file.close();
    delete[] buffer;

    file.open(AnsiString(*FileName).c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    unsigned int hash = crc.checksum();
    file.seekp(4);
    file.write((char *)&fileSize, sizeof(std::streamsize));
    file.write((char *)&hash, sizeof(hash));
    file.flush();
    file.close();

    MenuBSave->Enabled = false;
    return EGood;
}