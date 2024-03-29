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
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <System.Generics.Collections.hpp>
//---------------------------------------------------------------------------

enum TStatusCode
{
    EGood = 0,
    ECancel,
    EEmptyFile,
    EMemAlloc,
    EFileIntegrity,
    EOpenFile,
    ESaveFile,
    EInvalidInput,
    InvalidGridValue
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
    TMenuItem *N28;
    TMenuItem *N29;
    TMenuItem *N30;
    TMenuItem *N31;
    TMenuItem *N32;
    TMenuItem *N33;
    TMenuItem *N38;
    TMenuItem *N39;
    TMenuItem *N40;
    TMenuItem *N41;
    TMenuItem *N42;
    TSplitter *Splitter1;
    TSplitter *Splitter2;
    TGroupBox *TableBox;
    TOpenDialog *DFileOpen;
    TSaveDialog *DSaveDialog;
    TFontDialog *FontDialog;
    TFindDialog *FindDialog1;
    TReplaceDialog *ReplaceDialog1;
    TMenuItem *N43;
    TEdit *FSubjectName;
    TGroupBox *GroupBox1;
    TPageControl *Pages;
    TSpinButton *SBStudentCount;
    TMenuItem *MenuBFontChange;
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
    TChart *Chart;
    TButton *BFunctionHandle1;
    TButton *BFunctionHandle2;
    TLineSeries *Series1;
    TPanel *PChartManage;
    TComboBox *CBChartMode;
    TComboBox *CBStudentGroup;
    TSpeedButton *BExpandChartToggle;
    TMenuItem *MenuChBAutoWidth;
    TButton *BSort;
    TButton *BInsertStudent;
    TButton *BAddStudent;
    TButton *BDeleteStudent;
    TButton *BInsertSubject;
    TButton *BAddSubject;
    TButton *BDeleteSubject;
    void __fastcall MenuBOpenFileClick(TObject *Sender);
    void __fastcall MenuBSaveClick(TObject *Sender);
    void __fastcall MenuBSaveAsClick(TObject *Sender);
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
    void __fastcall MenuBFontChangeClick(TObject *Sender);
    void __fastcall MenuBNewFileClick(TObject *Sender);
    void __fastcall PagesChange(TObject *Sender);
    void __fastcall BFunctionHandle1Click(TObject *Sender);
    void __fastcall BFunctionHandle2Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall TableSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
    void __fastcall PChartManageResize(TObject *Sender);
    void __fastcall BExpandChartToggleClick(TObject *Sender);
    void __fastcall FontDialogApply(TObject *Sender, HWND Wnd);
    void __fastcall MenuChBAutoWidthClick(TObject *Sender);
    void __fastcall TableDblClick(TObject *Sender);
    void __fastcall TableExit(TObject *Sender);
    void __fastcall TableKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall BSortClick(TObject *Sender);
    void __fastcall BInsertStudentClick(TObject *Sender);
    void __fastcall BAddStudentClick(TObject *Sender);
    void __fastcall BDeleteStudentClick(TObject *Sender);
    void __fastcall TableFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall TableGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
    void __fastcall BInsertSubjectClick(TObject *Sender);
    void __fastcall BAddSubjectClick(TObject *Sender);
    void __fastcall BDeleteSubjectClick(TObject *Sender);
    void __fastcall Splitter2Moved(TObject *Sender);
    void __fastcall Splitter2CanResize(TObject *Sender, int &NewSize, bool &Accept);
    void __fastcall CBChartModeSelect(TObject *Sender);
    void __fastcall CBStudentGroupSelect(TObject *Sender);

private:
    TStatusCode StatusCode;
    TErrInfo AdditionErrorInformation;
    DynamicArray<DynamicArray<TRect>> BlackList;
    DynamicArray<String> DirNames;
    DynamicArray<bool> ModifiedFlag;
    TStringGrid *CurTable;
    String curCellsData;
    bool modifieded;
    bool fixedChanges;
    int SaveCol;
    int SaveRow;
    DynamicArray<int> SortMode;

public: // User declarations
    __fastcall TMainForm(TComponent *Owner);
    String getFileName();
    void ClearSeries(TChart *Chart);
    int CallSaveDialog();
    TStatusCode LoadMatrix(const String &FileName, TStringGrid *Table, int *EMemAllocStep = nullptr);
    TStatusCode SaveMatrix(TStringGrid *Table, int *EMemAllocStep = nullptr, String *FileName = nullptr);
    void ErrHandler(const String &FileAdress, TStatusBar *status_bar, TStatusCode status_code, TErrInfo *err = nullptr);
    template <typename TOwner, typename TParent>
    void NewStudentsTableInit(TOwner *ownerSelector, TParent *parentSelector,
                              /* const TStringGrid *sourse,*/ const String *config = nullptr);
    void NewTabInit(TPageControl *pageSelector, const String *config = nullptr);
    void ChangeTabCaption(const String &newName);
    void renderChart();
    bool checkBlackListEmpty(int pageID);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
void GridManageRow(int rowCount, TStringGrid *gridSelector);
void GridManageCol(const String *subjects, int colCount, TStringGrid *gridSelector);
String SliceAdressToFileName(const String &str);
String SliceFileNameToFileTitle(const String &str);
TStatusCode checkSubjectValue(const String &Str);
unsigned int getFileHash(ifstream &fstr, const std::streamsize &fileSize);
int getOptimalColWidth(TStringGrid *Table, int Col);
void SetMaxSize(TStringGrid *Table, int Col = -1);

#endif