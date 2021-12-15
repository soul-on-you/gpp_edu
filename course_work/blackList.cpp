void TMainForm::addBlackListRect(int pageID, TRect rect)
{
    bool notContrainsRect = true;
    for (int i = 0; i < BlackList[pageID].Length && notContrainsRect;
         notContrainsRect = (BlackList[pageID][i++] == rect) ? false : true)
        ;
    if (notContrainsRect)
    {
        BlackList[pageID].Length++;
        BlackList[pageID][BlackList[pageID].High] = rect;
    }
}

void TMainForm::deleteBlackListRect(int pageID, TRect rect)
{
    for (int i = 0; i < BlackList[pageID].Length; i++)
    {
        if (BlackList[pageID][i] == rect)
        {
            BlackList[pageID][i] = BlackList[pageID][BlackList[pageID].High];
            BlackList[pageID].Length--;
            break;
        }
    }
}