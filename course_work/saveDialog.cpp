int TMainForm::CallSaveDialog()
{
    //	switch (Application->MessageBox((String(L"Сохранить изменения в файле \"") + //вопрос об сохранении изменений
    //								 SaveDataDialog->FileName + "\"?")
    //									.w_str(),
    //								ProgramName, MB_YESNOCANCEL | MB_ICONQUESTION))
    //	{
    //		case mrYes:
    //		{
    //			if (SaveDataDialog->FileName == NewFileName)
    //			{ //сохранение
    //				if (!SaveDataDialog->Execute())
    //				{
    //					SaveDataDialog->FileName = OldFileName;
    //					return;
    //				}
    //			}
    //			if (!SaveSourceData())
    //			{
    //				SaveDataDialog->FileName = OldFileName;
    //				return;
    //			}
    //			SaveSource->Enabled = false; //убирание признака наличия несохраненных изменений
    //			SaveRes->Enabled = false;
    //			break;
    //		}
    //		case mrCancel:
    //			return;
    //	}

    if ((FileName = getFileName()) != String())
    {
        return = Application->MessageBox((String(L"Сохранить изменения в файле \"") + //вопрос об сохранении изменений
                                          FileName + L"\"?")
                                             .w_str(),
                                         Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    }
    else
    {
        return Application->MessageBox((String(L"Сохранить изменения текущей вкладке?")
                                                .w_str(),
                                            Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    }
}

void __fastcall TMainForm::MenuBNewFileClick(TObject *Sender)
{
    if (MenuBSave->Enabled)
    {
        int chose = CallSaveDialog();
        if (chose == IDCANCEL)
            return;
        else if (chose == IDYES)
            MenuBSaveClick(Sender);

        if (StatusCode == EGood)
        {
            int curPageIndex = Pages->ActivePage->Controls[0]->Tag;
            Pages->ActivePage->Controls[0]->Free();
            NewStudentsTableInit(PageTabs[curPageIndex], PageTabs[curPageIndex]);
        }
    }
}
//---------------------------------------------------------------------------

int TMainForm::CallSaveDialog()
{
    if ((FileName = getFileName()) != String())
    {
        return Application->MessageBox((String(L"Сохранить изменения в файле \"") +
                                        FileName + L"\"?")
                                           .w_str(),
                                       Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    }
    else
    {
        return Application->MessageBox(String(L"Сохранить изменения текущей вкладке?").w_str(),
                                       Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    }
}
