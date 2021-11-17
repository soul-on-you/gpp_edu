//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <string.h>
//#include <boost/filesystem.hpp>
#include "FrontEnd.h"
#include "ExternalDependencies.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "perfgrap"
#pragma link "perfgrap"
#pragma link "cspin"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent *Owner)
    : TForm(Owner)
{
    //	StudentData.signature = 0;
    //	StudentData.hash = 0;
    //	Table->Cells[0][0] = L"№";
    //	Table->Cells[1][0] = L"Студент";
    //	Table->Cells[0][1] = L"1";
    //	Table->Cells[0][2] = L"2";
    //	Table->Cells[2][0] = L"Мат.Анализ";
    //	Table->Cells[3][0] = L"Программирование";
    //	NewStudentsTableInit(Tabs[Tabs.Length-1], Tabs[Tabs.Length-1], Table);
    //		PageTabs.Length = 1;
    //		PageTabs[0] =  TabSheet0;
    //		NewStudentsTableInit(TabSheet0, TabSheet0, Table);
    NewTabInit(Table, Pages, PageTabs); // сделать вклатку hide и брать из нее пропертис
    //	PageTabs[1]->
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N9Click(TObject *Sender)
{
    if (DFileOpen->Execute())
    {
        FileName = DFileOpen->FileName;
        StatusCode = LoadMatrix(FileName, getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep);
        if (StatusCode == EGood)
        {
            Pages->ActivePage->Caption = SliceAdressToFileName(FileName);
        }
        else
            ErrHandler(DFileOpen->FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveClick(TObject *Sender)
{
    //	if(Changed)
    //	{
    if ((FileName = getFileName()) != L"")
    {
        if ((StatusCode = SaveMatrix(FileName, getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep)) != EGood)
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
    else
        MenuBSaveAsClick(Sender);
    MenuBSave->Enabled = false;
    //	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveAsClick(TObject *Sender)
{
    if (DSaveDialog->Execute())
    {
        FileName = DSaveDialog->FileName;
        if ((StatusCode = SaveMatrix(FileName, getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep)) == EGood)
        {
            //			boost::filesystem::path path_to_file(FileName.c_str());
            //			StatusBar->SimpleText = L"as is: " + String(path_to_file.parent_path().string().c_str());
            //			StatusBar->SimpleText += L"\tnormalized: " + String(path_to_file.parent_path().normalize().string().c_str());
            Pages->ActivePage->Caption = SliceAdressToFileName(FileName);
        }
        else
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
}
//---------------------------------------------------------------------------

//void __fastcall TForm1::TableKeyPress(TObject *Sender, System::WideChar &Key)
//{
//	MenuBSave->Enabled = true;
//}

//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBOpenWindowClick(TObject *Sender)
{
    NewTabInit(Table /*getCurrentTable()*/, Pages, PageTabs);
    Pages->ActivePage = PageTabs[PageTabs.High];
    //	PageTabs.Length = PageTabs.Length +1;
    //	PageTabs[PageTabs.Length-1] = new TTabSheet(Pages);
    //	PageTabs[PageTabs.Length-1]->PageControl = Pages;
    //	PageTabs[PageTabs.Length-1]->Caption = L"New";
    //	TStringGrid* createdTable = new TStringGrid(PageTabs[PageTabs.Length-1]);
    //	createdTable->Parent = PageTabs[PageTabs.Length-1];
    //	createdTable->RowCount = 3;
    //	createdTable->ColCount = 3;
    //	createdTable->Align = alClient;
}
//---------------------------------------------------------------------------

void NewTabInit(const TStringGrid *sourse, TPageControl *pageSelector, DynamicArray<TTabSheet *> &Tabs, const String *config)
{
    Tabs.Length = Tabs.Length + 1;
    Tabs[Tabs.Length - 1] = new TTabSheet(pageSelector);
    Tabs[Tabs.Length - 1]->PageControl = pageSelector;
    Tabs[Tabs.Length - 1]->Caption = L"Безымянный";
    NewStudentsTableInit(Tabs[Tabs.Length - 1], Tabs[Tabs.Length - 1], sourse);
    //	TStringGrid* createdTable = new TStringGrid(Tabs[Tabs.Length-1]);
    //	createdTable->Parent = Tabs[Tabs.Length-1];
    //	createdTable->RowCount = 3;
    //	createdTable->ColCount = 3;
    //	createdTable->Align = alClient;
}
template <typename TOwner, typename TParent>
void NewStudentsTableInit(TOwner *ownerSelector, TParent *parentSelector, const TStringGrid *sourse, const String *config)
{
    TStringGrid *createdTable = new TStringGrid(ownerSelector);
    createdTable->Options = sourse->Options;
    createdTable->Parent = parentSelector;
    createdTable->Align = alClient;
    createdTable->RowCount = 2;
    createdTable->ColCount = 3;
    createdTable->Cells[0][0] = L"№";
    createdTable->Cells[0][1] = L"1";
    createdTable->Cells[1][0] = L"Студент";
    createdTable->Cells[1][1] = L"<ФИО>";
    createdTable->Cells[2][0] = L"Предмет";
    createdTable->Cells[2][1] = L"<Оценка>";
    //
    //	if(config)
    //	{
    //		//загрузка настроек по дефолту
    //	}
    //	else
    //	{
    ////		createdTable->Options = [ goFixedVertLine, goFixedHorzLine, goVertLine,goHorzLine, goRangeSelect, goRowSizing, goColSizing,goRowMoving, goColMoving, goEditing, goTabs, goFixedColClick,goFixedRowClick, goFixedHotTrack, goFixedColDefAlign, goFixedRowDefAlign ];
    ////		createdTable->DefaultColAlignment = taCenter;
    //	}
}

void __fastcall TMainForm::SBStudentCountDownClick(TObject *Sender)
{
    if (Table->RowCount > 2)
    {
        EStudentCount->Text = getCurrentTable() /*Table*/->RowCount - 2;
        GridManageRow(getCurrentTable() /*Table*/->RowCount - 1, getCurrentTable() /*Table*/);
        //		Table->RowCount =  Table->RowCount-1;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SBStudentCountUpClick(TObject *Sender)
{
    EStudentCount->Text = getCurrentTable() /*Table*/->RowCount;
    GridManageRow(getCurrentTable() /*Table*/->RowCount + 1, getCurrentTable() /*Table*/);
    //		Table->RowCount =  Table->RowCount+1;
    //		Table->Cells[0][Table->RowCount] = Table->RowCount;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EStudentCountKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == VK_RETURN)
    {
        if (EStudentCount->Text != L"")
        {
            int tmp;
            if (TryStrToInt(EStudentCount->Text, tmp) && tmp > 1)
            {
                EStudentCount->Text = tmp;
                //				Table->RowCount =  tmp + 1;
                GridManageRow(tmp + 1, getCurrentTable() /*Table*/);
                return;
            }
            String S1 = EStudentCount->Text.c_str();
            AdditionErrorInformation.InvalidInput = &S1;
            //			String S1 = *(&(String&)(const String &)(String() = EStudentCount->Text.c_str()));
            //          AdditionErrorInformation.InvalidInput = &S1;
            //			AdditionErrorInformation.InvalidInput = &(String&)(const String &)(String() = EStudentCount->Text.c_str());        //&(String&)(const String &)
            ErrHandler(FileName, StatusBar, StatusCode = EInvalidInput, &AdditionErrorInformation);
        }
        else
            ErrHandler(FileName, StatusBar, StatusCode = EInvalidInput, &AdditionErrorInformation);
    }
    EStudentCount->SetFocus();
}
//---------------------------------------------------------------------------

void GridManageRow(int rowCount, TStringGrid *gridSelector)
{
    if (gridSelector->RowCount < rowCount)
    {
        rowCount += gridSelector->RowCount;
        gridSelector->RowCount = rowCount - gridSelector->RowCount;
        rowCount -= gridSelector->RowCount;
        for (int i = rowCount - 1, rowCount = gridSelector->RowCount; i < rowCount;)
        {
            for (int j = 1 /*gridSelector->ColCount-3*/; j < gridSelector->ColCount;)
                gridSelector->Cells[j++][i + 1] = L"";
            gridSelector->Cells[0][i] = IntToStr(i++);
        }
    }
    else
    {
        gridSelector->RowCount = rowCount;
        //		String tmp = gridSelector->Cells[gridSelector->RowCount+1][0];
        //		gridSelector->Cells[1][1] = tmp;
    }
}

void GridManageCol(const String *subjects, int colCount, TStringGrid *gridSelector)
{
    if (gridSelector->ColCount < colCount)
    {
        colCount += gridSelector->ColCount;
        gridSelector->ColCount = colCount - gridSelector->ColCount;
        colCount -= gridSelector->ColCount;
        for (int i = colCount, j = 0, k = 0, colCount = gridSelector->ColCount; i < colCount; i++, k++)
        {
            gridSelector->Cells[i][j] = subjects[k];
            for (j = 1 /*gridSelector->RowCount-3*/; j < gridSelector->RowCount; j++)
                gridSelector->Cells[i][j] = L"";
        }
    }
    else
    {
        for (int i = 0, removeCount = gridSelector->ColCount - colCount; i < removeCount; i++)
            for (int j = 2, columns = gridSelector->ColCount; j < columns;)
            {
                if (gridSelector->Cells[j][0] == subjects[i])
                {
                    for (int k = 0, last = gridSelector->ColCount - 1; k < gridSelector->RowCount; k++)
                        gridSelector->Cells[j][k] = gridSelector->Cells[last][k];
                    break;
                }
                if (++j == columns)
                {
                    ////СООБЩЕНИЕ ЧТО НЕ УДАЛОСЬ УДАЛИТЬ
                    colCount++;
                }
            }
        gridSelector->ColCount = colCount;
        //		String tmp = gridSelector->Cells[gridSelector->RowCount+1][0];
        //		gridSelector->Cells[1][1] = tmp;
    }
}
void __fastcall TMainForm::MenuBCloseWindowClick(TObject *Sender)
{
    Pages->ActivePage->Free();
}
//---------------------------------------------------------------------------
String TMainForm::getFileName()
{
    if (Pages->ActivePage->Caption == L"Безымянный")
        return L"";
    return Pages->ActivePage->Caption;
}
void __fastcall TMainForm::TableSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value)
{
    MenuBSave->Enabled = true;
}

String SliceAdressToFileName(const String &str)
{
    if (str != L"")
    {
        char *slicedFileName = strrchr(AnsiString(str).c_str(), '\\');
        if (slicedFileName != NULL)
            return String(++slicedFileName);
    }
    return str;
}

TStringGrid *TMainForm::getCurrentTable()
{
    return (TStringGrid *)Pages->ActivePage->Controls[0];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BAppendSubjectClick(TObject *Sender)
{
    if ((StatusCode = checkSubjectValue(FSubjectName->Text)) != EGood)
    {
        ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
        FSubjectName->SetFocus();
        return;
    }
    GridManageCol(&(const String &)FSubjectName->Text.c_str(), getCurrentTable() /*Table*/->ColCount + 1, getCurrentTable() /*Table*/);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BRemoveSubjectClick(TObject *Sender)
{
    if ((StatusCode = checkSubjectValue(FSubjectName->Text)) != EGood)
    {
        ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
        FSubjectName->SetFocus();
        return;
    }
    GridManageCol(&(const String &)FSubjectName->Text.c_str(), getCurrentTable() /*Table*/->ColCount - 1, getCurrentTable() /*Table*/);
}
//---------------------------------------------------------------------------

TStatusCode checkSubjectValue(const String &Str)
{
    if (Str == L"")
        return EInvalidInput;
    return EGood;
}

void __fastcall TMainForm::MenuBExitClick(TObject *Sender)
{
    MainForm->Close();
}
//---------------------------------------------------------------------------
