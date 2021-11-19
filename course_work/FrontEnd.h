//---------------------------------------------------------------------------

#ifndef FrontEndH
#define FrontEndH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "perfgrap.h"
#include <Vcl.Dialogs.hpp>
//#include "BackEnd.h"
//#include "supportTypes.h"
#include <System.Win.TaskbarCore.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Samples.Spin.hpp>
#include <Vcl.Taskbar.hpp>
#include "cspin.h"
#include <Vcl.Samples.Spin.hpp>
#include <System.Win.TaskbarCore.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Taskbar.hpp>
//---------------------------------------------------------------------------

enum TStatusCode
{
    EGood = 0,
    EEmptyFile,
    EMemAlloc,
    EFileIntegrity,
    EOpenFile,
    ESaveFile,
    EInvalidInput
};

union TErrInfo
{
    int MemAllocStep;
    String *InvalidInput;
};

class TMainForm : public TForm
{
    __published : // IDE-managed Components
                  TMainMenu *MainMenu1;
    TGroupBox *LeftBox;
    TGroupBox *RightBox;
    TStatusBar *StatusBar;
    TMenuItem *N1;
    TMenuItem *MenuBNewFile;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *MenuBOpenWindow;
    TMenuItem *N7;
    TMenuItem *MenuBCreateNewFile;
    TMenuItem *MenuBOpenFile;
    TMenuItem *N10;
    TMenuItem *N11;
    TMenuItem *MenuBSave;
    TMenuItem *MenuBSaveAs;
    TMenuItem *N14;
    TMenuItem *MenuBOptions;
    TMenuItem *MenuBCloseWindow;
    TMenuItem *N18;
    TMenuItem *N19;
    TMenuItem *N20;
    TMenuItem *N21;
    TMenuItem *N22;
    TMenuItem *N23;
    TMenuItem *N24;
    TMenuItem *N25;
    TMenuItem *N26;
    TMenuItem *N27;
    TMenuItem *N28;
    TMenuItem *N29;
    TMenuItem *N30;
    TMenuItem *N31;
    TMenuItem *N32;
    TMenuItem *N33;
    TMenuItem *N34;
    TMenuItem *N35;
    TMenuItem *N36;
    TMenuItem *N37;
    TMenuItem *N38;
    TMenuItem *N39;
    TMenuItem *N40;
    TMenuItem *N41;
    TMenuItem *N42;
    TSplitter *Splitter1;
    TSplitter *Splitter2;
    TPerformanceGraph *PerformanceGraph1;
    TPerformanceGraph *PerformanceGraph2;
    TGroupBox *TableBox;
    TOpenDialog *DFileOpen;
    TSaveDialog *DSaveDialog;
    TFontDialog *FontDialog1;
    TFindDialog *FindDialog1;
    TReplaceDialog *ReplaceDialog1;
    TMenuItem *N43;
    TEdit *FSubjectName;
    TGroupBox *GroupBox1;
    TPageControl *Pages;
    TTabSheet *TabSheet0;
    TSpinButton *SBStudentCount;
    TMenuItem *N44;
    TMenuItem *N45;
    TMenuItem *N46;
    TMenuItem *N47;
    TMenuItem *N48;
    TSpeedButton *BRemoveSubject;
    TSpeedButton *BAppendSubject;
    TTaskbar *Taskbar1;
    TLabeledEdit *EStudentCount;
    TMenuItem *MenuBExit;
    TStringGrid *Table;
    void __fastcall MenuBOpenFileClick(TObject *Sender);
    void __fastcall MenuBSaveClick(TObject *Sender);
    void __fastcall MenuBSaveAsClick(TObject *Sender);
    //	void __fastcall TableKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall MenuBOpenWindowClick(TObject *Sender);
    void __fastcall SBStudentCountDownClick(TObject *Sender);
    void __fastcall SBStudentCountUpClick(TObject *Sender);
    void __fastcall EStudentCountKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall MenuBCloseWindowClick(TObject *Sender);
    void __fastcall GridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
    void __fastcall BAppendSubjectClick(TObject *Sender);
    void __fastcall BRemoveSubjectClick(TObject *Sender);
    void __fastcall MenuBExitClick(TObject *Sender);
    void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow, const TRect &Rect,
                                 TGridDrawState State);

private:
    bool Changed;
    String FileName;
    TStatusCode StatusCode;
    TErrInfo AdditionErrorInformation;
    //	TStudentsData StudentData;
    DynamicArray<TTabSheet *> PageTabs;
    DynamicArray<TRect> BlackList;
    TStringGrid *CurTable;

public: // User declarations
    __fastcall TMainForm(TComponent *Owner);
    String getFileName();
    TStringGrid *getCurrentTable();
    TStatusCode LoadMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep = nullptr);
    TStatusCode SaveMatrix(TStringGrid *Table, int *EMemAllocStep = nullptr, String *FileName = nullptr);
    void ErrHandler(const String &FileAdress, TStatusBar *status_bar, TStatusCode status_code, TErrInfo *err = nullptr);
    template <typename TOwner, typename TParent>
    void NewStudentsTableInit(TOwner *ownerSelector, TParent *parentSelector,
                              /* const TStringGrid *sourse,*/ const String *config = nullptr);
    void NewTabInit(const TStringGrid *sourse, TPageControl *pageSelector,
                    DynamicArray<TTabSheet *> &Tabs, const String *config = nullptr);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
void GridManageRow(int rowCount, TStringGrid *gridSelector);
void GridManageCol(const String *subjects, int colCount, TStringGrid *gridSelector);
String SliceAdressToFileName(const String &str);
TStatusCode checkSubjectValue(const String &Str);

#endif