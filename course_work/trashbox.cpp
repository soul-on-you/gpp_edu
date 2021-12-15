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