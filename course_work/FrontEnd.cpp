//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrontEnd.h"
#include "ExternalDependencies.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "perfgrap"
#pragma link "perfgrap"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent *Owner)
    : TForm(Owner)
{
    Table->Cells[0][0] = L"№";
    Table->Cells[1][0] = L"Студент";
    Table->Cells[0][1] = L"1";
    Table->Cells[0][2] = L"2";
    Table->Cells[2][0] = L"Кардиология";
    Table->Cells[3][0] = L"Философия";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N9Click(TObject *Sender)
{
    if (DFileOpen->Execute())
    {
        FileName = DFileOpen->FileName;
        StatusCode = LoadMatrix(FileName, StudentData, &AdditionErrorInformation.MemAllocStep);
        if (StatusCode == EGood)
        {
            Table->ColCount = StudentData.Subjects.Length + 2;
            Table->RowCount = StudentData.Students.Length + 1;
            for (int i = 0; i < StudentData.Subjects.Length; i++)
                Table->Cells[i + 2][0] = AnsiString(StudentData.Subjects[i]).c_str();
            for (int i = 0; i < StudentData.Students.Length; i++)
                Table->Cells[1][i + 1] = AnsiString(StudentData.Students[i]).c_str();
            //			StatusBar->SimpleText= AnsiString(StudentData.Subjects[0]).c_str();
            //			StatusBar->SimpleText= StudentData.Students[0];
        }
    }
    else
    {
        //		ErrHandler(DFileOpen->FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N12Click(TObject *Sender)
{

    String S;
    if (DSaveDialog->Execute())
    {
        FileName = DSaveDialog->FileName;
        StudentData.signature = 0;
        StudentData.hash = 0;

        //      __dmatrix_t marks;
        {
            int Row = PREPARETORY_STEP;
            try
            {
                StudentData.marks.Length = StrToInt(Table->RowCount - 1); /////////
                                                                          //				StatusBar->SimpleText =
                for (Row = 0; Row < StudentData.marks.Length; Row++)
                {
                    StudentData.marks[Row].Length = StrToInt(Table->ColCount - 2); /////////
                }
            }
            catch (...)
            {
                StudentData.marks.Length = 0;
                AdditionErrorInformation.MemAllocStep = Row;
                StatusCode = EMemAlloc;
            }
        }
        if (StudentData.marks.Length == 0)
        {
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation); ///////////////Добавить освобождение всего
        }
        for (int i = 0; i < StudentData.marks.Length; i++)
            for (int j = 0; j < StudentData.marks[i].Length; j++)
                StudentData.marks[i][j] = StrToInt(Table->Cells[j + 2][i + 1]); //[j+2]);
        S += StrToInt(Table->Cells[2][1]);
        //		DynamicArray<String> Subjects;
        try
        {
            StudentData.Subjects.Length = Table->ColCount - 2;
        }
        catch (...)
        {
            StudentData.Subjects.Length = 0;
            AdditionErrorInformation.MemAllocStep = PREPARETORY_STEP;
            StatusCode = EMemAlloc;
        }
        if (StudentData.Subjects.Length == 0)
        {
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation); ///////////////Добавить освобождение всего
        }
        for (int i = 0; i < StudentData.Subjects.Length; i++)
            StudentData.Subjects[i] = Table->Cells[i + 2][0]; // [i+2][0]
        S += Table->Cells[2][0];
        //        DynamicArray<String> Students;
        try
        {
            StudentData.Students.Length = Table->RowCount - 1;
        }
        catch (...)
        {
            StudentData.Students.Length = 0;
            AdditionErrorInformation.MemAllocStep = PREPARETORY_STEP;
            StatusCode = EMemAlloc;
        }
        if (StudentData.Students.Length == 0)
        {
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation); ///////////////Добавить освобождение всего
        }
        for (int i = 0; i < StudentData.Students.Length; i++)
            StudentData.Students[i] = Table->Cells[1][i + 1]; //[2][i+2]
        S += Table->Cells[1][1];
        S += StrToInt(StudentData.Students.Length);
        //		StatusBar->SimpleText = S;
        SaveMatrix(FileName, StudentData, &AdditionErrorInformation.MemAllocStep);
    }
}
//---------------------------------------------------------------------------
