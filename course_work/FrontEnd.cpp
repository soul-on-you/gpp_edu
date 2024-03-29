//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <string.h>
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include <fstream>
#include "FrontEnd.h"
//#include "ExternalDependencies.h"
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
    NewTabInit(Pages, PageTabs);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBOpenFileClick(TObject *Sender)
{
    if (DFileOpen->Execute())
    {
        FileName = DFileOpen->FileName;
        StatusCode = LoadMatrix(FileName, getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep);
        if (StatusCode == EGood)
        {
            boost::filesystem::path path_to_file(FileName.c_str());
            //            StatusBar->SimpleText = L"as is: " + String(path_to_file.parent_path().string().c_str());
            //			StatusBar->SimpleText += L"\tnormalized: " + String(path_to_file.parent_path().normalize().string().c_str());
            Pages->ActivePage->Caption = SliceAdressToFileName(FileName);
            int tag = getCurrentTable()->Tag;
            int l2 = DirNames.Length;
            DirNames[tag] = (const String &)(String(
                (
                    (
                        (const boost::filesystem::path &)(boost::filesystem::path(FileName.c_str())))
                        .parent_path()
                        .string()
                        .c_str())));
            int l = DirNames[tag].Length();
            StatusBar->SimpleText = DirNames[getCurrentTable()->Tag];
        }
        else
            ErrHandler(DFileOpen->FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveClick(TObject *Sender)
{
    if ((FileName = getFileName()) != String())
    {
        if ((StatusCode = SaveMatrix(getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep, &FileName)) != EGood)
            ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
    }
    else
        MenuBSaveAsClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveAsClick(TObject *Sender)
{
    if ((StatusCode = SaveMatrix(getCurrentTable() /*Table*/, &AdditionErrorInformation.MemAllocStep)) == EGood)
    {
        //		boost::filesystem::path path_to_file(FileName.c_str());
        //		StatusBar->SimpleText = L"as is: " + String(path_to_file.parent_path().string().c_str());
        //		StatusBar->SimpleText += L"\tnormalized: " + String(path_to_file.parent_path().normalize().string().c_str());
        if (FileName != String())
            Pages->ActivePage->Caption = SliceAdressToFileName(FileName);
    }
    else
        ErrHandler(FileName, StatusBar, StatusCode, &AdditionErrorInformation);
}
//---------------------------------------------------------------------------

//void __fastcall TForm1::TableKeyPress(TObject *Sender, System::WideChar &Key)
//{
//	MenuBSave->Enabled = true;
//}

//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBOpenWindowClick(TObject *Sender)
{
    NewTabInit(Pages, PageTabs);
    Pages->ActivePage = PageTabs[PageTabs.High];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value)
{
    TStringGrid *curTable = (TStringGrid *)Sender;
    //	int tmp = curTable->Tag;
    if (ACol >= 2 && ACol >= 1)
    {
        TRect curRect = curTable->CellRect(ACol, ARow);
        //		DynamicArray<TRect> curBlackList = BlackList[curTable->Tag];
        char Sym = *AnsiString(curTable->Cells[ACol][ARow]).c_str();
        if (Sym != '5' && Sym != '4' && Sym != '3' && Sym != '2' && Sym != '1')
        {
            //			curTable->Cells[ACol][ARow].
            // смена цвета клетки на красный
            //			GridDrawCell(curTable, ACol, ARow, (TRect &)(const TRect&)curTable->CellRect(ACol, ARow), TGridDrawState());

            bool notContrainsRect = true;
            for (int i = 0; i < /*curBlackList*/ BlackList[curTable->Tag].Length && notContrainsRect;
                 notContrainsRect = (/*curBlackList*/ BlackList[curTable->Tag][i++] == curRect) ? false : true)
                ;
            if (notContrainsRect)
            {
                /*curBlackList*/ BlackList[curTable->Tag].Length++;
                //				int l = /*curBlackList*/BlackList[curTable->Tag].Length;  DEBUG
                /*curBlackList*/ BlackList[curTable->Tag][/*curBlackList*/ BlackList[curTable->Tag].High] = curRect;
            }
        }
        else // смена цвета клетки на белый
        {
            for (int i = 0; i < /*curBlackList*/ BlackList[curTable->Tag].Length; i++)
            {
                if (/*curBlackList*/ BlackList[curTable->Tag][i] == curRect)
                {
                    /*curBlackList*/ BlackList[curTable->Tag][i] = /*curBlackList*/ BlackList[curTable->Tag][/*curBlackList*/ BlackList[curTable->Tag].High];
                    /*curBlackList*/ BlackList[curTable->Tag].Length--;
                    //					int l = /*curBlackList*/BlackList[curTable->Tag].Length;  DEBUG
                    break;
                }
            }
        }
    }
    MenuBSave->Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::GridDrawCell(TObject *Sender, int ACol, int ARow, const TRect &Rect,
                                        TGridDrawState State)
{
    TStringGrid *curTable = (TStringGrid *)Sender;
    TRect curRect = curTable->CellRect(ACol, ARow);
#define curBlackList (BlackList[curTable->Tag])
    //	DynamicArray<TRect> curBlackList = BlackList[curTable->Tag];
    int l = curBlackList.Length;
    for (int i = 0; i < curBlackList.Length; i++)
    {
        if (curRect == curBlackList[i])
        {
            curTable->Canvas->Brush->Color = clRed;
            curTable->Canvas->FillRect(curBlackList[i]);
            int TextWidth = curTable->Canvas->TextWidth(curTable->Cells[ACol][ARow]);
            curTable->Canvas->TextOut(curBlackList[i].Left + (curBlackList[i].Width() - TextWidth) / 2,
                                      curBlackList[i].Top + (curBlackList[i].Height() - curTable->Canvas->TextHeight(curTable->Cells[ACol][ARow])) / 2,
                                      curTable->Cells[ACol][ARow]);
            break;
        }
    }
}

//---------------------------------------------------------------------------
void TMainForm::NewTabInit(TPageControl *pageSelector, DynamicArray<TTabSheet *> &Tabs, const String *config)
{
    Tabs.Length = Tabs.Length + 1;
    Tabs[Tabs.High] = new TTabSheet(pageSelector);
    Tabs[Tabs.High]->PageControl = pageSelector;
    Tabs[Tabs.High]->Caption = L"Безымянный";
    String Check = Tabs[Tabs.High]->Caption;
    NewStudentsTableInit(Tabs[Tabs.High], Tabs[Tabs.High]);
    BlackList.Length++;
    Tabs[Tabs.High]->Controls[0]->Tag = BlackList.High;
    DirNames.Length++;
    //	StatusBar->SimpleText =  Tabs[Tabs.High]->Tag;
}

//---------------------------------------------------------------------------

template <typename TOwner, typename TParent>
void TMainForm::NewStudentsTableInit(TOwner *ownerSelector, TParent *parentSelector, const String *config)
{
    TStringGrid *createdTable = new TStringGrid(ownerSelector);
    createdTable->Options << goRowSizing << goColSizing << goRowMoving
                          << goColMoving << goEditing << goTabs << goFixedColClick
                          << goFixedRowClick << goFixedHotTrack << goFixedColDefAlign;
    createdTable->Parent = parentSelector;
    createdTable->Align = alClient;
    createdTable->DefaultColAlignment = taCenter;
    createdTable->RowCount = 2;
    createdTable->ColCount = 3;
    createdTable->Cells[0][0] = String(L"№");
    createdTable->Cells[0][1] = String(L"1");
    createdTable->Cells[1][0] = String(L"Студент");
    createdTable->Cells[1][1] = String(L"<ФИО>");
    createdTable->Cells[2][0] = String(L"Предмет");
    createdTable->Cells[2][1] = String(L"<Оценка>");
    createdTable->OnSetEditText = GridSetEditText;
    createdTable->OnDrawCell = GridDrawCell;
}

void __fastcall TMainForm::SBStudentCountDownClick(TObject *Sender)
{
    TStringGrid *curTable = getCurrentTable();
    if (curTable->RowCount > 2)
    {
        EStudentCount->Text = curTable->RowCount - 2;
        GridManageRow(curTable->RowCount - 1, curTable);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SBStudentCountUpClick(TObject *Sender)
{
    EStudentCount->Text = getCurrentTable()->RowCount;
    GridManageRow(getCurrentTable()->RowCount + 1, getCurrentTable());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::EStudentCountKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
    if (Key == VK_RETURN)
    {
        if (EStudentCount->Text != String())
        {
            int tmp;
            if (TryStrToInt(EStudentCount->Text, tmp) && tmp > 1)
            {
                EStudentCount->Text = tmp;
                GridManageRow(tmp + 1, getCurrentTable());
                return;
            }
            String S1 = EStudentCount->Text.c_str();
            AdditionErrorInformation.InvalidInput = &S1;
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
            for (int j = 1; j < gridSelector->ColCount;)
                gridSelector->Cells[j++][i + 1] = String(); //String();
            gridSelector->Cells[0][i] = IntToStr(i++);
        }
    }
    else
    {
        gridSelector->RowCount = rowCount;
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
            for (j = 1; j < gridSelector->RowCount; j++)
                gridSelector->Cells[i][j] = String();
        }
    }
    else
    {
        String fallsRemove;
        bool falls = false;
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
                    if (!falls)
                    {
                        fallsRemove += L"\"" + subjects[i] + L"\"";
                        falls = true;
                    }
                    else
                        fallsRemove += L", \"" + subjects[i] + L"\"";
                    colCount++;
                }
            }
        gridSelector->ColCount = colCount;
        if (falls)
            Application->MessageBox(String(L"Неудалось удалить предметы: " + fallsRemove).w_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
    }
}
void __fastcall TMainForm::MenuBCloseWindowClick(TObject *Sender)
{
    //  ДОБАВИТЬ ЧТОБЫ ПРИ ЗАКРЫТИИ ВКЛАДКИ УМЕНЬШАЛИСЬ ДИНАМИЧЕСКИЕ МАССИВЫ ВКЛАДОК И БЛЭКЛИСТА
    //	for(int rmIndex = Pages->ActivePage->Controls[0]->Tag; rmIndex< BlackList.High; )
    //	{
    //		PageTabs[rmIndex]->Controls[0]->Tag
    //		BlackList[rmIndex] = BlackList[++rmIndex];
    //	}
    //	BlackList.Length--;
    if (Pages->PageCount > 0)
    {
        PageTabs[Pages->ActivePage->Controls[0]->Tag] = nullptr;
        BlackList[Pages->ActivePage->Controls[0]->Tag].Length = 0;
        DirNames[Pages->ActivePage->Controls[0]->Tag] = String();
        Pages->ActivePage->Free();
        if (Pages->PageCount == 0)
            int i = 0; // затычка, вызвать констуктор новой пустой вкладки
        return;
    }
}
//---------------------------------------------------------------------------
String TMainForm::getFileName()
{
    if (Pages->ActivePage->Caption == L"Безымянный")
        return String();
    return DirNames[Pages->ActivePage->Controls[0]->Tag] + L'\\' + Pages->ActivePage->Caption;
}

String SliceAdressToFileName(const String &str)
{
    if (str != String())
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
    if (Str == String())
        return EInvalidInput;
    return EGood;
}

void __fastcall TMainForm::MenuBExitClick(TObject *Sender)
{
    MainForm->Close();
}
//---------------------------------------------------------------------------

/////////////FILE IN/OUT

TStatusCode TMainForm::LoadMatrix(const String &FileName, TStringGrid *curTable, int *EMemAllocStep)
{
    ifstream file;
    file.open(AnsiString(FileName).c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;

    char signature[4];                                //= "APC";
    file.read((char *)&signature, sizeof(signature)); //сигнатура
    if (strcmp("APC", signature))
    {
        file.close();
        return EFileIntegrity;
    }
    //
    std::streamsize fileSize, bufferSize;
    file.read((char *)&fileSize, sizeof(fileSize));
    {
        file.seekg(0, std::ios_base::end);
        std::streamsize realFileSize = file.tellg();
        //      std::streamsize realFileSize = boost::filesystem::file_size(AnsiString(FileName).c_str());
        if (realFileSize != fileSize)
        {
            file.close();
            return EFileIntegrity;
        }
    }
    file.seekg(12);
    unsigned int hash;
    file.read((char *)&hash, sizeof(hash)); //хэш
                                            //	if(fileSize > 1036)
                                            //		bufferSize = 1024;
                                            //	else
    bufferSize = fileSize - 16;
    std::streamsize cur = file.tellg();
    {
        char *buffer = new (std::nothrow) char[bufferSize];
        //	boost::crc_basic<32> *crc = new(std::nothrow) boost::crc_basic<32>(0x27809EA7, 0u, 0u, true, true);
        boost::crc_basic<32> crc(0x27809EA7, 0u, 0u, true, true);
        file.seekg(16);
        //		while(!file.eof())
        //		{
        file.read((char *)buffer, bufferSize); //buffer_size
        crc.process_bytes(buffer, (std::size_t)bufferSize);

        //		}
        delete[] buffer;
        //		unsigned int check = crc->checksum();
        if (hash != crc.checksum())
        {
            return EFileIntegrity;
            file.close();
        }
        //        delete crc;
    }

    file.seekg(16);
    int studentCount;
    file.read((char *)&studentCount, sizeof(studentCount)); // кол-во студентов и карта имен студентов
    curTable->RowCount = studentCount + 1;
    for (int i = 0, curStrSize; i < studentCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));

        curTable->Cells[1][1 + i] = String();
        file.read((char *)curTable->Cells[1][1 + i].c_str(), curStrSize);
        curTable->Cells[1][1 + i] = curTable->Cells[1][1 + i].c_str();
        curTable->Cells[0][i + 1] = IntToStr(i + 1);
    }

    int subjectCount;
    file.read((char *)&subjectCount, sizeof(subjectCount)); // кол-во предметов и карта названий предметов
    curTable->ColCount = subjectCount + 2;
    for (int i = 0, curStrSize; i < subjectCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        curTable->Cells[2 + i][0] = String();
        file.read((char *)curTable->Cells[2 + i][0].c_str(), curStrSize);
        curTable->Cells[2 + i][0] = curTable->Cells[2 + i][0].c_str();
    }

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
            {
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

    file.open(AnsiString(*FileName).c_str(), std::ios_base::out | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;
    char signature[4] = "APC";
    file.write((char *)signature, sizeof(signature));
    file.seekp(16); // skip FileSize 8b + hash 4b

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
    std::streamsize fileSize, bufferSize;
    file.seekg(0, ios::end);
    fileSize = file.tellg();

    boost::crc_basic<32> crc(0x27809EA7, 0u, 0u, true, true);
    {
        //		if(fileSize > 1036)
        //			bufferSize = 1024;
        //		else
        bufferSize = fileSize - 16;
        char buffer[bufferSize];
        //
        file.seekg(16, std::ios_base::beg);
        //		while(!file.eof())
        //		{
        file.read((char *)buffer, bufferSize);
        crc.process_bytes(buffer, (std::size_t)bufferSize);
        //		}
        std::streamsize cur = file.tellg();
        file.seekg(-1, std::ios_base::cur);
        cur = file.tellg();
        int i = cur;
    }
    file.close();
    //	delete[] buffer;

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

//---------------------------------------------------------------------------

//////////////////////////ERROR HANDLER
void TMainForm::ErrHandler(const String &FileAdress, TStatusBar *status_bar, TStatusCode status_code, TErrInfo *err)
{
    if (err)
    {
        switch (status_code)
        {
        case EEmptyFile:
            status_bar->SimpleText = L"В файле не оказалось значений, которые можно считать";
            break;
        case EMemAlloc:
            if (err->MemAllocStep == -1)
                status_bar->SimpleText = L"Ошибка, не удалось выделить память под массив указателей";
            else
                status_bar->SimpleText = L"Ошибка, не удалось выделить память под " + IntToStr(err->MemAllocStep) + L" строку";
            err->MemAllocStep = 0;
            break;
        case EFileIntegrity:
            status_bar->SimpleText = L"Ошибка считывания, нарушена целостность данных";
            break;
        case EOpenFile:
            status_bar->SimpleText = L"Ошибка открытия файла";
            break;
        case ESaveFile:
            status_bar->SimpleText = L"Ошибка, неудалось сохранить данные в файл";
            break;
        case EInvalidInput:
            if (err->InvalidInput)
            {
                status_bar->SimpleText = String(L"Неверное значение множителя \"") + err->InvalidInput->w_str() + L'\"';
                err->InvalidInput = nullptr;
            }
            else
                status_bar->SimpleText = L"Ошибка, в поле ничего не указано";
            break;
        default:
            status_bar->SimpleText = L"Отсутствует обработка данного типа ошибки";
        }
        Application->MessageBox(status_bar->SimpleText.w_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
    }
    else
        status_bar->SimpleText = L"Код ошибки: " + IntToStr(status_code);
}
