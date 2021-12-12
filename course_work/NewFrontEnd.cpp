// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <string.h>
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <fstream>
#include <cstring> ///для substr
#include "FrontEnd.h"
#include <winuser.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "perfgrap"
#pragma link "perfgrap"
#pragma link "cspin"
#pragma resource "*.dfm"
TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent *Owner) : TForm(Owner)
{
    NewTabInit(Pages);
    fixedChanges = false;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBOpenFileClick(TObject *Sender)
{
    if (MenuBSave->Enabled)
    {
        int chose = CallSaveDialog();
        if (chose == IDCANCEL)
            return;
        else if (chose == IDYES)
            MenuBSaveClick(Sender);

        if (StatusCode != EGood)
            return;
    }

    if (DFileOpen->Execute())
    {
        StatusCode = LoadMatrix(DFileOpen->FileName, CurTable,
                                &AdditionErrorInformation.MemAllocStep);
        if (StatusCode == EGood)
        {
            ChangeTabCaption(SliceAdressToFileName(DFileOpen->FileName));
            DirNames[Pages->TabIndex] =
                (const String &)(String((((const boost::filesystem::path &)(boost::filesystem::path(DFileOpen->FileName.c_str())))
                                             .parent_path()
                                             .string()
                                             .c_str())));
            EStudentCount->Text = IntToStr(CurTable->RowCount - 1);
        }
        else
            ErrHandler(DFileOpen->FileName, StatusBar, StatusCode,
                       &AdditionErrorInformation);
    }
    if (MenuChBAutoWidth->Checked)
        SetMaxSize(CurTable);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveClick(TObject *Sender)
{
    if (BlackList[Pages->TabIndex].Length == 0)
    {
        DSaveDialog->FileName = getFileName(), String();
        if (DSaveDialog->FileName != String())
        {
            if ((StatusCode = SaveMatrix(CurTable, &AdditionErrorInformation.MemAllocStep,
                                         &(String &)(const String &)(DSaveDialog->FileName))) == EGood)
            {
                MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = false;
                return;
            }
            ErrHandler(DSaveDialog->FileName, StatusBar, StatusCode, &AdditionErrorInformation);
        }
        else
            MenuBSaveAsClick(Sender);
    }
    else
        ErrHandler(DSaveDialog->FileName, StatusBar, StatusCode = InvalidGridValue,
                   &AdditionErrorInformation);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBSaveAsClick(TObject *Sender)
{
    if ((StatusCode = SaveMatrix(CurTable,
                                 &AdditionErrorInformation.MemAllocStep)) == EGood)
    {
        ChangeTabCaption(SliceAdressToFileName(DSaveDialog->FileName));
        MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = false;
    }
    else if (StatusCode != ECancel)
        ErrHandler(DSaveDialog->FileName, StatusBar, StatusCode, &AdditionErrorInformation);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBOpenWindowClick(TObject *Sender)
{
    NewTabInit(Pages);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TableFixedCellClick(TObject *Sender, int ACol, int ARow)

{
    if (KeyboardStateToShiftState() != TShiftState() << ssCtrl << ssLeft)
    {
        if (ACol)
            return; ///Доп.Соритовка, Если нужна будет
        else if (ARow)
            return; ///Доп.Соритовка, Если нужна будет
        else
        {
            CurTable->Rows[CurTable->Row]->Objects[0] = (TObject *)true;
//#define BOOST_SCOPE_PTR_SORT
#ifdef BOOST_SCOPE_PTR_SORT
            boost::scoped_ptr<TStringList> SList(new TStringList);
            for (int i = CurTable->FixedRows; i < CurTable->RowCount; i++)
            {
                boost::scoped_ptr<TStringList> SRow(new TStringList);
                SRow->Assign(CurTable->Rows[i]);
                SList->AddObject(CurTable->Cells[ACol][i], (TStringList *)(SRow.get())); // boost::static_pointer_cast<TStringList*>(SRow));
            }
            SList->Sort();
            if ((SortMode[Pages->TabIndex] = !SortMode[Pages->TabIndex]))
                for (int i = 0; i < SList->Count; i++)
                    CurTable->Rows[i + CurTable->FixedRows]->Assign((TStringList *)(SList->Objects[i]));
            else
                for (int i = 0; i < SList->Count; i++)
                    CurTable->Rows[i + CurTable->FixedRows]->Assign((TStringList *)(SList->Objects[SList->Count - i - 1]));
#else
            TStringList *SRow, *SList = new TStringList;
            for (int i = CurTable->FixedRows; i < CurTable->RowCount; i++)
            {
                SRow = new TStringList;
                SRow->Assign(CurTable->Rows[i]);
                SList->AddObject(CurTable->Cells[ACol][i], SRow);
            }
            SList->Sort();
            if ((SortMode[Pages->TabIndex] = !SortMode[Pages->TabIndex]))
                for (int i = 0; i < SList->Count; i++)
                {
                    SRow = (TStringList *)(SList->Objects[i]);
                    CurTable->Rows[i + CurTable->FixedRows]->Assign(SRow);
                    delete SRow;
                }
            else
                for (int i = 0; i < SList->Count; i++)
                {
                    SRow = (TStringList *)(SList->Objects[SList->Count - i - 1]);
                    CurTable->Rows[i + CurTable->FixedRows]->Assign(SRow);
                    delete SRow;
                }
            delete SList;
#endif
            for (int i = CurTable->FixedRows; i < CurTable->RowCount; i++)
                if (CurTable->Rows[i]->Objects[0])
                {
                    CurTable->Rows[i]->Objects[0] = (TObject *)false;
                    CurTable->Row = i;
                    break;
                }
        }
    }
    else
    {
        if (ACol >= CurTable->FixedCols)
        {
            CurTable->FixedRows = 0;
            CurTable->Row = 0;
            CurTable->Col = ACol;
        }
        else if (ARow >= CurTable->FixedRows)
        {
            CurTable->FixedCols = 0;
            CurTable->Col = 0;
            CurTable->Row = ARow;
        }
        else
            return;
        CurTable->Options = CurTable->Options << goEditing;
        CurTable->EditorMode = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TableGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value)

{
    if (!(modifieded = ModifiedFlag[Pages->TabIndex]))
        curCellsData = CurTable->Cells[ACol][ARow];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TableSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)

{
    //	StatusBar->SimpleText = L"SelectCel " + IntToStr(ACol) + IntToStr(ARow) + L' ';
    if (ACol >= 1 && ARow >= 1)
    {
        CurTable->FixedRows = 1;
        CurTable->FixedCols = 1;
        if (!(modifieded = ModifiedFlag[Pages->TabIndex]) &&
            (SaveCol != CurTable->Col && SaveRow != CurTable->Row))
            curCellsData = CurTable->Cells[ACol][ARow];
        if (!CurTable->EditorMode)
        {
            CurTable->Options = CurTable->Options >> goEditing;
        }
        else
        {
            if (!modifieded && MenuBSave->Enabled)
            {
                SaveCol = CurTable->Col;
                SaveRow = CurTable->Row;
            }
        }
        CurTable->SetFocus();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GridSetEditText(TObject *Sender, int ACol, int ARow,
                                           const UnicodeString Value)
{
    if (CurTable->EditorMode)
    {
        // if (ACol >= 2 && ACol >= 1)
        if (ARow >= 1 && ACol >= 1)
        {
            //		StatusBar->SimpleText += L"SEDIT " + String(CurTable->EditorMode) + L' ' + Value;
            TRect curRect = CurTable->CellRect(ACol, ARow);
            char Sym = *AnsiString(CurTable->Cells[ACol][ARow]).c_str();
            int length = CurTable->Cells[ACol][ARow].Length();
            if (length > 1 || (Sym != '5' && Sym != '4' && Sym != '3' && Sym != '2' && Sym != '0' && Sym != 'Б' && Sym != 'Н' && Sym != '?'))
            {
                bool notContrainsRect = true;
                for (int i = 0;
                     i < BlackList[Pages->TabIndex].Length && notContrainsRect;
                     notContrainsRect = (BlackList[Pages->TabIndex][i++] == curRect) ? false : true)
                    ;
                if (notContrainsRect)
                {
                    BlackList[Pages->TabIndex].Length++;
                    BlackList[Pages->TabIndex][BlackList[Pages->TabIndex].High] =
                        curRect;
                }
            }
            else // смена цвета клетки на белый
            {
                for (int i = 0; i < BlackList[Pages->TabIndex].Length; i++)
                {
                    if (BlackList[Pages->TabIndex][i] == curRect)
                    {
                        BlackList[Pages->TabIndex][i] =
                            BlackList[Pages->TabIndex]
                                     [BlackList[Pages->TabIndex].High];
                        BlackList[Pages->TabIndex].Length--;
                        break;
                    }
                }
            }
        }
        if (!MenuBSave->Enabled)
            MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = true;
        if (!modifieded && curCellsData == Value)
            MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = false;
    }
    else
    {
        if (ARow && ACol)
        {
            if (!modifieded && curCellsData == Value)
                MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = false;
            //		CurTable->EditorMode = true;
            TableKeyPress(Sender, (WideChar &)(const WideChar &)VK_RETURN);
        }
        else
        {
            CurTable->FixedRows = 1;
            CurTable->FixedCols = 1;
            CurTable->Col = ACol ? ACol : 1;
            CurTable->Row = ARow ? ARow : 1;
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::TableDblClick(TObject *Sender)
{
    if (CurTable->Col && CurTable->Row)
    {
        TPoint curPos;
        GetCursorPos(&curPos);
        curPos = CurTable->ScreenToClient(curPos);

        if (GetCursor() == Screen->Cursors[crHSplit])
        {
            TGridCoord gc = CurTable->MouseCoord(curPos.X - 4, curPos.Y);
            if (CurTable->ColWidths[gc.X] > 8)
                CurTable->ColWidths[gc.X] = 8;
            else
                SetMaxSize(CurTable, gc.X);
        }
        else
        {
            TGridCoord gc = CurTable->MouseCoord(curPos.X, curPos.Y);
            if (gc.X >= CurTable->FixedCols && gc.Y >= CurTable->FixedRows)
            {
                CurTable->Options = CurTable->Options << goEditing;
                CurTable->EditorMode = !CurTable->EditorMode;
                CurTable->SetFocus();
            }
            else
                TableFixedCellClick(Sender, gc.X, gc.Y);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TableExit(TObject *Sender)
{
    CurTable->Options = CurTable->Options >> goEditing;
    fixedChanges = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TableKeyPress(TObject *Sender, System::WideChar &Key)
{
    if (CurTable->Col && CurTable->Row)
    {
        if (Key == VK_RETURN && !fixedChanges)
        {
            if (CurTable->EditorMode)
            {
                CurTable->Options = CurTable->Options >> goEditing;
                if (!modifieded && MenuBSave->Enabled)
                {
                    SaveCol = CurTable->Col;
                    SaveRow = CurTable->Row;
                }
            }
            else
            {
                if ((SaveCol != CurTable->Col && SaveRow != CurTable->Row) &&
                    !(modifieded = ModifiedFlag[Pages->TabIndex]))
                    curCellsData = CurTable->Cells[CurTable->Col][CurTable->Row];
                CurTable->Options << goEditing;
                CurTable->SetFocus();
            }
        }
    }
    if (Key == VK_ESCAPE && CurTable->EditorMode)
    {
        CurTable->Cells[CurTable->Col][CurTable->Row] = curCellsData;
        CurTable->EditorMode = false;
        CurTable->Options = CurTable->Options >> goEditing;
        MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex] = false;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GridDrawCell(TObject *Sender, int ACol, int ARow,
                                        const TRect &Rect, TGridDrawState State)
{
    TRect curRect = CurTable->CellRect(ACol, ARow);
    for (int i = 0; i < BlackList[Pages->TabIndex].Length; i++)
    {
        if (curRect == BlackList[Pages->TabIndex][i])
        {
            CurTable->Canvas->Brush->Color = clRed;
            CurTable->Canvas->FillRect(BlackList[Pages->TabIndex][i]);
            int TextWidth = CurTable->Canvas->TextWidth(CurTable->Cells[ACol][ARow]);
            CurTable->Canvas->TextOut(BlackList[Pages->TabIndex][i].Left +
                                          (BlackList[Pages->TabIndex][i].Width() - TextWidth) / 2,
                                      BlackList[Pages->TabIndex][i].Top +
                                          (BlackList[Pages->TabIndex][i].Height() -
                                           CurTable->Canvas->TextHeight(CurTable->Cells[ACol][ARow])) /
                                              2,
                                      CurTable->Cells[ACol][ARow]);
            break;
        }
    }
}
// ---------------------------------------------------------------------------

void TMainForm::NewTabInit(TPageControl *pageSelector, const String *config)
{
    TTabSheet *Tab = new TTabSheet(pageSelector);
    Tab->PageControl = pageSelector;
    NewStudentsTableInit(Tab, Tab);
    BlackList.Length++;
    DirNames.Length++;
    ModifiedFlag.Length++;
    SortMode.Length++;
    ModifiedFlag[ModifiedFlag.High] = false;
    SortMode[SortMode.High] = false;
    Pages->ActivePage = Tab;
    ChangeTabCaption(L"Безымянный");
    PagesChange(MainForm);
}
// ---------------------------------------------------------------------------

template <typename TOwner, typename TParent>
void TMainForm::NewStudentsTableInit(TOwner *ownerSelector,
                                     TParent *parentSelector, const String *config)
{
    TStringGrid *createdTable = new TStringGrid(ownerSelector);
    createdTable->Options << goRowSizing << goColSizing << goRowMoving << goColMoving /*<< goEditing */ << goTabs << goFixedColClick << goFixedRowClick << goFixedHotTrack << goFixedColDefAlign;
    createdTable->Parent = parentSelector;
    createdTable->Align = alClient;
    createdTable->DefaultColAlignment = taCenter;
    createdTable->RowCount = 2;
    createdTable->ColCount = 2;
    //    createdTable->ColCount = 3;
    //    createdTable->Cells[0][0] = String(L"№");              /////////////////!!!!!!!!!!!!!
    //    createdTable->Cells[0][1] = String(L"1");
    //	createdTable->Cells[1][0] = String(L"Студент");
    //	createdTable->Cells[1][1] = String(L"<ФИО>");
    //	createdTable->Cells[2][0] = String(L"Предмет");
    //	createdTable->Cells[2][1] = String(L"<Оценка>");
    createdTable->Cells[0][0] = String(L"Студент");
    createdTable->Cells[0][1] = String(L"<ФИО>");
    createdTable->Cells[1][0] = String(L"Предмет");
    createdTable->Cells[1][1] = String(L"<Оценка>");
    createdTable->OnSetEditText = GridSetEditText;
    createdTable->OnDrawCell = GridDrawCell;
    createdTable->OnSelectCell = TableSelectCell;
    createdTable->OnExit = TableExit;
    createdTable->OnDblClick = TableDblClick;
    createdTable->OnKeyPress = TableKeyPress;
    createdTable->OnFixedCellClick = TableFixedCellClick;
    createdTable->OnGetEditText = TableGetEditText;
}
// --------------------------------------------------------------------------

void __fastcall TMainForm::SBStudentCountDownClick(TObject *Sender)
{
    if (CurTable->RowCount > 2)
    {
        EStudentCount->Text = IntToStr(CurTable->RowCount - 2);
        GridManageRow(CurTable->RowCount - 1, CurTable);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::SBStudentCountUpClick(TObject *Sender)
{
    EStudentCount->Text = IntToStr(CurTable->RowCount);
    GridManageRow(CurTable->RowCount + 1, CurTable);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::EStudentCountKeyDown(TObject *Sender, WORD &Key,
                                                TShiftState Shift)

{
    if (Key == VK_RETURN)
    {
        if (EStudentCount->Text != String())
        {
            int tmp;
            if (TryStrToInt(EStudentCount->Text, tmp) && tmp > 1)
            {
                EStudentCount->Text = IntToStr(tmp);
                GridManageRow(tmp + 1, CurTable);
                return;
            }
            String S1 = EStudentCount->Text.c_str(); /// ТУТ МОЖЕТ ПОЯВИТЬСЯ ОШИБКА
            AdditionErrorInformation.InvalidInput = &S1;
            //				&(String &)(const String &)(EStudentCount->Text.c_str());
            ErrHandler(L"", StatusBar, StatusCode = EInvalidInput,
                       &AdditionErrorInformation);
        }
        else
            ErrHandler(L"", StatusBar, StatusCode = EInvalidInput,
                       &AdditionErrorInformation);
    }
    EStudentCount->SetFocus();
}
// ---------------------------------------------------------------------------

void GridManageRow(int rowCount, TStringGrid *gridSelector)
{
    if (gridSelector->RowCount < rowCount)
    {
        rowCount += gridSelector->RowCount;
        gridSelector->RowCount = rowCount - gridSelector->RowCount;
        rowCount -= gridSelector->RowCount;
        for (int i = rowCount - 1, rowCount = gridSelector->RowCount;
             i < rowCount;)
        {
            for (int j = 1; j < gridSelector->ColCount;)
                gridSelector->Cells[j++][i + 1] = String();
            gridSelector->Cells[0][i] = IntToStr(i++);
        }
    }
    else
        gridSelector->RowCount = rowCount;
}
// --------------------------------------------------------------------------

void GridManageCol(const String *subjects, int colCount,
                   TStringGrid *gridSelector)
{
    if (gridSelector->ColCount < colCount)
    {
        colCount += gridSelector->ColCount;
        gridSelector->ColCount = colCount - gridSelector->ColCount;
        colCount -= gridSelector->ColCount;
        for (int i = colCount, j = 0, k = 0, colCount = gridSelector->ColCount;
             i < colCount; i++, k++)
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
        for (int i = 0, removeCount = gridSelector->ColCount - colCount;
             i < removeCount; i++)
            for (int j = 2, columns = gridSelector->ColCount; j < columns;)
            {
                if (gridSelector->Cells[j][0] == subjects[i])
                {
                    for (int k = 0, last = gridSelector->ColCount - 1;
                         k < gridSelector->RowCount; k++)
                        gridSelector->Cells[j][k] =
                            gridSelector->Cells[last][k];
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
            Application->MessageBox(String(L"Неудалось удалить предметы: " + fallsRemove).w_str(),
                                    L"Ошибка!", MB_OK | MB_ICONERROR);
    }
}
// --------------------------------------------------------------------------

void __fastcall TMainForm::MenuBCloseWindowClick(TObject *Sender)
{
    if (Pages->PageCount > 0)
    {
        if (MenuBSave->Enabled)
        {
            int chose = CallSaveDialog();
            if (chose == IDCANCEL)
                return;
            else if (chose == IDYES)
                MenuBSaveClick(Sender);

            if (StatusCode != EGood)
                return;
        }
        for (int i = Pages->TabIndex; i < Pages->PageCount - 1; i++)
        {
            BlackList[i] = BlackList[i + 1];
            DirNames[i] = DirNames[i + 1];
            ModifiedFlag[i] = ModifiedFlag[i + 1];
        }
        CBStudentGroup->Items->Delete(Pages->TabIndex);
        BlackList.Length--;
        DirNames.Length--;
        ModifiedFlag.Length--;
        Pages->ActivePage->Free();
        if (Pages->PageCount == 0)
            MenuBOpenWindowClick(Sender);
        else
            PagesChange(MainForm);
    }
}
// ---------------------------------------------------------------------------

void substr(char *dest, char *source, int from, int length)
{
    strncpy(dest, source + from, length);
    dest[length] = 0;
}
// ---------------------------------------------------------------------------

String TMainForm::getFileName()
{
    if (Pages->ActivePage->Caption == L"Безымянный")
        return String();
    return DirNames[Pages->TabIndex] + L'\\' +
           Pages->ActivePage->Caption;
}
// ---------------------------------------------------------------------------

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
// ---------------------------------------------------------------------------

String SliceFileNameToFileTitle(const String &str)
{
    if (str != String())
    {
        char *startPoint = AnsiString(str).c_str(), *slicedFileName =
                                                        strrchr(startPoint, '.');
        if (slicedFileName != NULL)
        {
            char slicedFileTitle[slicedFileName - startPoint];
            substr(slicedFileTitle, startPoint, 0, slicedFileName - startPoint);
            return String(slicedFileTitle);
        }
    }
    return str;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BAppendSubjectClick(TObject *Sender)
{
    if ((StatusCode = checkSubjectValue(FSubjectName->Text)) != EGood)
    {
        ErrHandler(L"", StatusBar, StatusCode, &AdditionErrorInformation);
        FSubjectName->SetFocus();
        return;
    }
    GridManageCol(&(const String &)FSubjectName->Text.c_str(),
                  CurTable->ColCount + 1, CurTable);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BRemoveSubjectClick(TObject *Sender)
{
    if ((StatusCode = checkSubjectValue(FSubjectName->Text)) != EGood)
    {
        ErrHandler(L"", StatusBar, StatusCode, &AdditionErrorInformation);
        FSubjectName->SetFocus();
        return;
    }
    GridManageCol(&(const String &)FSubjectName->Text.c_str(),
                  CurTable->ColCount - 1, CurTable);
}
// ---------------------------------------------------------------------------

TStatusCode checkSubjectValue(const String &Str)
{
    if (Str == String())
        return EInvalidInput;
    return EGood;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBExitClick(TObject *Sender)
{
    MainForm->Close();
}
// ---------------------------------------------------------------------------

unsigned int getFileHash(ifstream &fstr, const std::streamsize &fileSize)
{
    char *buffer = new (std::nothrow) char[fileSize];
    boost::crc_basic<32> crc(0x27809EA7, 0u, 0u, true, true);
    fstr.read((char *)buffer, fileSize);
    crc.process_bytes(buffer, (std::size_t)fileSize);
    char a = buffer[0], b = buffer[fileSize - 1], c = buffer[fileSize - 2],
         d = buffer[fileSize - 3], e = buffer[fileSize - 4];
    delete[] buffer;
    return crc.checksum();
}
// ---------------------------------------------------------------------------

/////////////FILE IN/OUT
TStatusCode TMainForm::LoadMatrix(const String &FileName, TStringGrid *curTable,
                                  int *EMemAllocStep)
{
    ifstream file;
    file.open(AnsiString(FileName).c_str(),
              std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;

    char signature[4];                                // = "APC";
    file.read((char *)&signature, sizeof(signature)); // сигнатура
    if (strcmp("APC", signature))
    {
        file.close();
        return EFileIntegrity;
    }

    std::streamsize fileSize;
    file.read((char *)&fileSize, sizeof(fileSize));
    {
        std::streamsize realFileSize = boost::filesystem::file_size(AnsiString(FileName).c_str());
        if (realFileSize != fileSize)
        {
            file.close();
            return EFileIntegrity;
        }
    }

    unsigned int hash;
    file.read((char *)&hash, sizeof(hash)); // хэш
                                            //	std::streamsize bufferSize = fileSize - 16;
    file.seekg(16);
    if (hash != getFileHash(file, fileSize - 16))
    {
        file.close();
        return EFileIntegrity;
    }

    file.seekg(16);
    int studentCount;
    file.read((char *)&studentCount, sizeof(studentCount));
    // кол-во студентов и карта имен студентов
    curTable->RowCount = studentCount + 1;
    for (int i = 0, curStrSize; i < studentCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));

        //		curTable->Cells[1][1 + i] = String();
        //		file.read((char *)curTable->Cells[1][1 + i].c_str(), curStrSize);
        //		curTable->Cells[1][1 + i] = curTable->Cells[1][1 + i].c_str();
        //        curTable->Cells[0][i + 1] = IntToStr(i + 1);            /////////////////
        curTable->Cells[0][1 + i] = String();
        file.read((char *)curTable->Cells[0][1 + i].c_str(), curStrSize);
        curTable->Cells[0][1 + i] = curTable->Cells[0][1 + i].c_str();
    }

    int subjectCount;
    file.read((char *)&subjectCount, sizeof(subjectCount));
    // кол-во предметов и карта названий предметов
    curTable->ColCount = subjectCount + 1;
    for (int i = 0, curStrSize; i < subjectCount; i++)
    {
        file.read((char *)&curStrSize, sizeof(curStrSize));
        //		curTable->Cells[2 + i][0] = String();
        //		file.read((char *)curTable->Cells[2 + i][0].c_str(), curStrSize);
        //		curTable->Cells[2 + i][0] = curTable->Cells[2 + i][0].c_str();
        curTable->Cells[1 + i][0] = String();
        file.read((char *)curTable->Cells[1 + i][0].c_str(), curStrSize);
        curTable->Cells[1 + i][0] = curTable->Cells[1 + i][0].c_str();
    }

    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0, tmp; j < subjectCount; j++)
        {
            //			file.read((char *)&tmp, sizeof(tmp));
            //			curTable->Cells[2 + j][1 + i] = IntToStr(tmp);
            char temp;
            file.read(&temp, sizeof(temp));
            //			curTable->Cells[2 + j][1 + i] = String(temp);
            curTable->Cells[1 + j][1 + i] = String(temp);
        }
    file.close();
    return EGood;
}
// --------------------------------------------------------------------------

TStatusCode TMainForm::SaveMatrix(TStringGrid *curTable, int *EMemAllocStep,
                                  String *FileName)
{
    std::fstream file;
    if (!FileName)
    {

        while (true)
        {
            if (!DSaveDialog->Execute())
                return ECancel;

            //			newFileName = DSaveDialog->FileName;
            file.open(AnsiString(DSaveDialog->FileName).c_str(), std::ios_base::in);
            if (file.is_open())
            {
                int chose =
                    Application->MessageBox((SliceAdressToFileName(DSaveDialog->FileName) +
                                             L" уже существует\r\nЗаменить?")
                                                .w_str(),
                                            L"Подтвердить сохранение", MB_YESNOCANCEL);
                file.close();
                if (chose == IDCANCEL)
                    return EGood;
                if (chose == IDNO)
                    continue;
            }
            break;
        }
        //		*FileName = &(String &)(const String &)(String() = (DSaveDialog->FileName.c_str())); CПРОСИТЬ КАК ТАКОЕ РЕАЛИЗОВЫВАТЬ
        FileName = &(String &)(const String &)(String());
        *FileName = DSaveDialog->FileName;
    }

    file.open(AnsiString(*FileName).c_str(),
              std::ios_base::out | std::ios_base::binary);
    if (!file.is_open())
        return EOpenFile;
    char signature[4] = "APC";
    file.write((char *)signature, sizeof(signature));
    file.seekp(16); // skip FileSize 8b + hash 4b

    int studentCount = curTable->RowCount - 1;
    // кол-во студентов и карта имен студентов
    file.write((char *)&studentCount, sizeof(studentCount));
    for (int i = 0; i < studentCount; i++)
    {
        //		file.write((char *)&(const int &)(sizeof(curTable->Cells[1][1 + i][1]) * curTable->Cells[1][1 + i].Length() + 2), sizeof(int));
        //		file.write((char *)curTable->Cells[1][1 + i].c_str(),
        //				   sizeof(curTable->Cells[1][1 + i][1]) * curTable->Cells[1][1 + i].Length() + 2);
        file.write((char *)&(const int &)(sizeof(curTable->Cells[0][1 + i][1]) * curTable->Cells[0][1 + i].Length() + 2), sizeof(int));
        file.write((char *)curTable->Cells[0][1 + i].c_str(),
                   sizeof(curTable->Cells[0][1 + i][1]) * curTable->Cells[0][1 + i].Length() + 2);
    }

    // if (file.fail()) ДОБАВИТЬ К КАЖДОМУ WRITE МОЖЕТ НЕ ХВАТИТЬ ПАМЯТИ
    int subjectCount = curTable->ColCount - 1;
    //    int subjectCount = curTable->ColCount - 2;
    // кол-во предметов и карта названий предметов
    file.write((char *)&subjectCount, sizeof(subjectCount));
    for (int i = 0; i < subjectCount; i++)
    {
        //		file.write((char *)&(const int &)(sizeof(curTable->Cells[2 + i][0][1]) * curTable->Cells[2 + i][0].Length() + 2), sizeof(int));
        //		file.write((char *)curTable->Cells[2 + i][0].c_str(),
        //				   sizeof(curTable->Cells[2 + i][0][1]) * curTable->Cells[2 + i][0].Length() + 2);
        file.write((char *)&(const int &)(sizeof(curTable->Cells[1 + i][0][1]) * curTable->Cells[1 + i][0].Length() + 2), sizeof(int));
        file.write((char *)curTable->Cells[1 + i][0].c_str(),
                   sizeof(curTable->Cells[1 + i][0][1]) * curTable->Cells[1 + i][0].Length() + 2);
    }
    for (int i = 0; i < studentCount; i++) // оценки по [студенту] по [предмету]
        for (int j = 0; j < subjectCount; j++)
            //			file.write((char *)&(const int &)StrToInt(curTable->Cells[j + 2][i + 1]), sizeof(int));
            //			file.write((char *)AnsiString(curTable->Cells[j + 2][i + 1].c_str()).c_str(), sizeof(char)); // V2
            file.write((char *)AnsiString(curTable->Cells[j + 1][i + 1].c_str()).c_str(), sizeof(char)); // V2
    file.close();

    file.open(AnsiString(*FileName).c_str(), std::ios_base::in | std::ios_base::binary);
    std::streamsize fileSize = boost::filesystem::file_size(AnsiString(*FileName).c_str());
    file.seekg(16);
    unsigned int hash = getFileHash((ifstream &)file, fileSize - 16);

    file.close();

    file.open(AnsiString(*FileName).c_str(),
              std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    //	unsigned int hash = crc.checksum();
    file.seekp(4);
    file.write((char *)&fileSize, sizeof(std::streamsize));
    file.write((char *)&hash, sizeof(hash));
    file.flush();
    file.close();
    return EGood;
}
// ---------------------------------------------------------------------------

//////////////////////////ERROR HANDLER
void TMainForm::ErrHandler(const String &FileAdress, TStatusBar *status_bar,
                           TStatusCode status_code, TErrInfo *err)
{
    if (err)
    {
        switch (status_code)
        {
        case EEmptyFile:
            status_bar->SimpleText =
                L"В файле не оказалось значений, которые можно считать";
            break;
        case EMemAlloc:
            if (err->MemAllocStep == -1)
                status_bar->SimpleText =
                    L"Ошибка, не удалось выделить память под массив указателей";
            else
                status_bar->SimpleText =
                    L"Ошибка, не удалось выделить память под " +
                    IntToStr(err->MemAllocStep) + L" строку";
            err->MemAllocStep = 0;
            break;
        case EFileIntegrity:
            status_bar->SimpleText =
                L"Ошибка считывания, нарушена целостность данных";
            break;
        case EOpenFile:
            status_bar->SimpleText = L"Ошибка открытия файла";
            break;
        case ESaveFile:
            status_bar->SimpleText =
                L"Ошибка, неудалось сохранить данные в файл";
            break;
        case EInvalidInput:
            if (err->InvalidInput)
            {
                status_bar->SimpleText =
                    String(L"Неверное значение множителя \"") +
                    err->InvalidInput->w_str() + L'\"';
                err->InvalidInput = nullptr;
            }
            else
                status_bar->SimpleText = L"Ошибка, в поле ничего не указано";
            break;
        case InvalidGridValue:
            status_bar->SimpleText =
                L"Исправьте неправильные значения элементов";
            break;
        default:
            status_bar->SimpleText =
                L"Отсутствует обработка данного типа ошибки";
        }
        Application->MessageBox(status_bar->SimpleText.w_str(), L"Ошибка!",
                                MB_OK | MB_ICONERROR);
    }
    else
        status_bar->SimpleText = L"Код ошибки: " + IntToStr(status_code);
}
// --------------------------------------------------------------------------

void __fastcall TMainForm::MenuBFontChangeClick(TObject *Sender)
{
    //    if (FontDialog->Execute())
    //        for (int i = 0; i < Pages->PageCount; i++)
    //        {
    //            // Pages->Controls[i]->Controls[0]->Font = FontDialog->Font;
    //        }
    FontDialog->Tag = 0;
    FontDialog->Font->Assign(CurTable->Font);
    TFont *srcFont = new TFont;

    try
    {
        srcFont->Assign(CurTable->Font);
        if (!FontDialog->Execute())
        {
            if (FontDialog->Tag)
            {
                CurTable->Font->Assign(srcFont);
                CurTable->Repaint();
                /// if(autosize)    AutoSizeGrid(CurTable);
                //				SetMaxSize(CurTable);//////////////////////////
                if (MenuChBAutoWidth->Checked)
                    SetMaxSize(CurTable);
            }
            return;
        }
    }
    __finally
    {
        delete srcFont;
    }

    CurTable->Font->Assign(FontDialog->Font);
    CurTable->Repaint();
    /// if(autosize)    AutoSizeGrid(CurTable);  ////////////////////////////
    //	SetMaxSize(CurTable);
    if (MenuChBAutoWidth->Checked)
        SetMaxSize(CurTable);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FontDialogApply(TObject *Sender, HWND Wnd)
{
    FontDialog->Tag = 1;
    CurTable->Font->Assign(FontDialog->Font);
    CurTable->Repaint();
    /// if(autosize)    AutoSizeGrid(CurTable);  //////////////////////////////
    //	SetMaxSize(CurTable);
    if (MenuChBAutoWidth->Checked)
        SetMaxSize(CurTable);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuBNewFileClick(TObject *Sender)
{
    if (MenuBSave->Enabled)
    {
        int chose = CallSaveDialog();
        if (chose == IDCANCEL)
            return;
        else if (chose == IDYES)
            MenuBSaveClick(Sender);

        if (StatusCode != EGood)
            return;
    }
    Pages->ActivePage->Controls[0]->Free();
    NewStudentsTableInit(Pages->ActivePage, Pages->ActivePage);
    ChangeTabCaption(L"Безымянный");
}
// ---------------------------------------------------------------------------

int TMainForm::CallSaveDialog()
{
    DFileOpen->FileName = getFileName(), String();
    if (DFileOpen->FileName != String())
    {
        return Application->MessageBox((String(L"Сохранить изменения в файле \"") + DFileOpen->FileName + L"\"?")
                                           .w_str(),
                                       Application->Title.w_str(),
                                       MB_YESNOCANCEL | MB_ICONQUESTION);
    }
    else
    {
        return Application->MessageBox(String(L"Сохранить изменения текущей вкладке?").w_str(),
                                       Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    }
}

void __fastcall TMainForm::PagesChange(TObject *Sender)
{
    StatusBar->SimpleText = IntToStr(Pages->TabIndex);
    CurTable = (TStringGrid *)Pages->ActivePage->Controls[0];
    MenuBSave->Enabled = ModifiedFlag[Pages->TabIndex];
    CurTable->Font->Assign(FontDialog->Font);
    CurTable->Repaint();
    if (MenuChBAutoWidth->Checked)
        SetMaxSize(CurTable);
    fixedChanges = false;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BFunctionHandle1Click(TObject *Sender)
{
    TStringGrid *curGrid = CurTable;
    ClearSeries(Chart);
    for (int Col = 2; Col < curGrid->ColCount; Col++) // тут кол-во графиков
    {
        TLineSeries *Series = new TLineSeries(Chart);
        Chart->AddSeries(Series);
        for (int Row = 1; Row < curGrid->RowCount;
             Row++) // длинна созданного графика
        {
            Series[0].AddXY(curGrid->Cells[0][Row].ToDouble(), // установка точек
                            curGrid->Cells[Col][Row].ToDouble());
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::BFunctionHandle2Click(TObject *Sender)
{
    Chart->View3DOptions->Orthogonal = false;
    Chart->View3DOptions->Perspective = 0;
    Chart->View3DOptions->Zoom = 100;
    Chart->View3DOptions->Elevation = 360;
    Chart->View3DOptions->Rotation = 360;
    ClearSeries(Chart);
    TBarSeries *Series = new TBarSeries(Chart);
    Chart->AddSeries(Series);
    TStringGrid *curGrid = CurTable;
    for (int Col = 2, validMark = curGrid->RowCount - 1; Col < curGrid->ColCount; Col++)
    {
        int Sum = 0;
        for (int Row = 1; Row < curGrid->RowCount; Row++)
        {
            char Sym = *AnsiString(CurTable->Cells[Col][Row]).c_str();
            if ((Sym == 'Б' || Sym == 'Н' || Sym == '?') && validMark--)
                continue;
            Sum += StrToInt(curGrid->Cells[Col][Row]);
        }
        if (validMark)
            Series->Add(Sum / 1.0 / validMark, curGrid->Cells[Col][0],
                        (TColor)clTeeColor);
        validMark = curGrid->RowCount - 1;
    }
}
// ---------------------------------------------------------------------------

void TMainForm::ClearSeries(TChart *Chart)
{
    for (int i = 0, SeriesCount = Chart->SeriesCount(); i < SeriesCount; i++)
        Chart->SeriesList->Delete(0);
}
// --------------------------------------------------------------------------

void TMainForm::ChangeTabCaption(const String &newName)
{
    CBStudentGroup->Items->Delete(Pages->TabIndex);
    Pages->ActivePage->Caption = newName;
    CBStudentGroup->Items->Insert(Pages->TabIndex <
                                          CBStudentGroup->Items->Count
                                      ? Pages->TabIndex
                                      : CBStudentGroup->Items->Count,
                                  SliceFileNameToFileTitle(Pages->ActivePage->Caption));
}
// --------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    for (int i = Pages->PageCount; i--;)
    {
        Pages->ActivePage = (TTabSheet *)Pages->Controls[i];
        MenuBCloseWindowClick(Sender);
        if (ModifiedFlag.Length == i + 1 && ModifiedFlag[i])
        {
            Action = caNone;
            return;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PChartManageResize(TObject *Sender)
{
    CBStudentGroup->Width = (PChartManage->Width - BExpandChartToggle->Width) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BExpandChartToggleClick(TObject *Sender)
{
    if (RightBox->Width == 24)
        RightBox->Width += TableBox->Width - 4;
    else
        RightBox->Width = 24;
    //	GetMaxColWidth(nullptr, 1);
}
//---------------------------------------------------------------------------

int getOptimalColWidth(TStringGrid *Table, int Col)
{
    //	try{
    int optimal = 0;
    for (int i = 0; i < Table->RowCount; i++)
        optimal = Table->Canvas->TextWidth(Table->Cells[Col][i]) > optimal ? Table->Canvas->TextWidth(Table->Cells[Col][i]) : optimal;
    return optimal;
    //	}
    //	catch(Exception &E)
    //	{
    //		ShowMessage("Read nullptr"+E.Message);
    //	}
}
//---------------------------------------------------------------------------

void SetMaxSize(TStringGrid *Table, int Col)
{
    if (Col < Table->ColCount)
    {
        if (Col >= 0)
            Table->ColWidths[Col] = getOptimalColWidth(Table, Col) + 12;
        else
            for (int i = 0; i < Table->ColCount; i++)
                Table->ColWidths[i] = getOptimalColWidth(Table, i) + 12;
    }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuChBAutoWidthClick(TObject *Sender)
{
    MenuChBAutoWidth->Checked = !MenuChBAutoWidth->Checked;
    if (MenuChBAutoWidth->Checked)
        SetMaxSize(CurTable);
}
//---------------------------------------------------------------------------

class TStringGridExtra : TStringGrid
{
public:
    using TStringGrid::DeleteColumn;
    using TStringGrid::DeleteRow;
    using TStringGrid::InvalidateCell;
    using TStringGrid::MoveColumn;
    using TStringGrid::MoveRow;
    //	std::auto_ptr<TStringList> L (new TStringList);
};

void __fastcall TMainForm::BSortClick(TObject *Sender /* , int Col*/)
{
#ifdef BOOST_SCOPE_PTR_SORT
    int Col = 0;
    boost::scoped_ptr<TStringList> SList(new TStringList);
    for (int i = CurTable->FixedRows; i < CurTable->RowCount; i++)
    {
        boost::scoped_ptr<TStringList> SRow(new TStringList);
        SRow->Assign(CurTable->Rows[i]);
        SList->AddObject(CurTable->Cells[Col][i], (TStringList *)(SRow.get())); // boost::static_pointer_cast<TStringList*>(SRow));
    }
    SList->Sort();
    if ((SortMode[Pages->TabIndex] = !SortMode[Pages->TabIndex]))
        for (int i = 0; i < SList->Count; i++)
            CurTable->Rows[i + CurTable->FixedRows]->Assign((TStringList *)(SList->Objects[i]));
    else
        for (int i = 0; i < SList->Count; i++)
            CurTable->Rows[i + CurTable->FixedRows]->Assign((TStringList *)(SList->Objects[SList->Count - i - 1]));

#else
    int Col = 0;
    TStringList *SRow, *SList = new TStringList;
    for (int i = CurTable->FixedRows; i < CurTable->RowCount; i++)
    {
        SRow = new TStringList;
        SRow->Assign(CurTable->Rows[i]);
        SList->AddObject(CurTable->Cells[Col][i], SRow);
    }
    SList->Sort();
    if ((SortMode[Pages->TabIndex] = !SortMode[Pages->TabIndex]))
        for (int i = 0; i < SList->Count; i++)
        {
            SRow = (TStringList *)(SList->Objects[i]);
            CurTable->Rows[i + CurTable->FixedRows]->Assign(SRow);
            delete SRow;
        }
    else
        for (int i = 0; i < SList->Count; i++)
        {
            SRow = (TStringList *)(SList->Objects[SList->Count - i - 1]);
            CurTable->Rows[i + CurTable->FixedRows]->Assign(SRow);
            delete SRow;
        }
    delete SList;
#endif

    //	delete SList;
    //		SList->Add(CurTable->Rows[i]->CommaText);   V1
    //	SList->Sort();
    //	if((SortMode[Pages->TabIndex] = !SortMode[Pages->TabIndex]))
    //		for(int i = 0; i < SList->Count; i++)
    //			CurTable->Rows[i + CurTable->FixedRows]->CommaText = SList->Strings[i];
    //	else
    //		for(int i = 0; i < SList->Count;)
    //			CurTable->Rows[i + CurTable->FixedRows]->CommaText = SList->Strings[SList->Count - ++i];
    //	delete SList;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BInsertStudentClick(TObject *Sender)
{
    CurTable->RowCount++;
    ((TStringGridExtra *)CurTable)->MoveRow(CurTable->RowCount - 1, CurTable->Row);
    CurTable->Row--;
    CurTable->Rows[CurTable->Row]->Clear();
    MenuBSave->Enabled /*= modified */ = ModifiedFlag[Pages->TabIndex] = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BAddStudentClick(TObject *Sender)
{
    CurTable->RowCount++;
    CurTable->Row = CurTable->RowCount - 1;
    CurTable->Rows[CurTable->Row]->Clear();
    CurTable->SetFocus();
    MenuBSave->Enabled /*= modified */ = ModifiedFlag[Pages->TabIndex] = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BDeleteStudentClick(TObject *Sender)
{
    int DelCount;
    if ((DelCount = CurTable->Selection.Bottom - CurTable->Selection.Top + 1) == 1)
    {
        if (Application->MessageBox((String(L"Вы действительно хотите удалить студента \"") + CurTable->Cells[0][CurTable->Row] + L"\"?").w_str(),
                                    Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != ID_YES)
            return;
    }
    else
    {
        if (Application->MessageBox((String(L"Вы действительно хотите удалить студентов с \"") + CurTable->Cells[0][CurTable->Selection.Top] + L"\" по \"" + CurTable->Cells[1][CurTable->Selection.Bottom] + L"\"?").w_str(),
                                    Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != ID_YES)
            return;
    }
    if (CurTable->RowCount - DelCount >= 1)
    {
        int t = CurTable->Selection.Top, b = CurTable->Selection.Bottom;
        if (CurTable->RowCount - DelCount == 1)
            BAddStudentClick(Sender);
        for (int i = t; i <= b; i++)
            ((TStringGridExtra *)CurTable)->DeleteRow(t);
        if (t < CurTable->RowCount)
            CurTable->Row = t;
    }
}
//---------------------------------------------------------------------------
