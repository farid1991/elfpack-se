#include "..\\classlib\cl.h"
#include "..\\include\var_arg.h"
#include "..\\include\Dir.h"

#define NAME_ELF "FindFile"
#define GUIFREE(x) if(x)((CGuiBase*)x)->Free();x=0;
#define MESSAGE(x) MessageBox(EMPTY_TEXTID,Str2ID(x,ENC_UCS2,TEXTID_ANY_LEN),NOIMAGE,1,5000,0)
#define SLASH L"/"
#define MYBOOK(x) CMyBook*mbk=(CMyBook*)x
#ifdef NDEBUG
#define FUNCTION
#else
#define FUNCTION CDbgPrint __function((char*)__FILE__,(char*)__FUNCTION__);
#endif

typedef struct
{
  wchar_t name[256];
  wchar_t path[256];
  bool isDir;
}ELEM_;

bool isDir(const wchar_t*dir,const wchar_t*sub_dir);//����� �� ���
void elf_exit(void);
bool equ(wchar_t*str,wchar_t*sub_str);//���� �� � ������ ��������� ��� ����� ��������
wchar_t GetIcon(wchar_t *name);//������ ������ �� ��
int wstrncmp2(wchar_t*str,wchar_t*sub_str);//������� ��������� � ������ (���������� ������� ������� � ������, ��� ���������� ���������)

class CDbgPrint
{
  char* fl;
  char* fn;
public:
  CDbgPrint(char* file,char* func){debug_printf("\nfunction enter: %s/%s\n",fl=file,fn=func);}
  ~CDbgPrint(){debug_printf("\nfunction leave: %s/%s\n",fl,fn);}
};

class CGuiFindList:public CGuiListMenu
{
  LIST*list;
  virtual int OnMessage(CGUIMessage*msg);
public:
  CGuiFindList(CBook*bk,int x,LIST*_list);
};

class CGuiFolderList:public CGuiListMenu
{
  virtual int OnMessage(CGUIMessage*msg);
public:
  CGuiFolderList(CBook*bk);
};

class CGuiAddFolderList:public CGuiListMenu
{
  LIST*list;
  virtual int OnMessage(CGUIMessage*msg);
public:
  CGuiAddFolderList(CBook*bk,LIST*_list);
};

class CGuiDeleteFolders: public CGuiListMenu
{
  LIST*list;
  virtual int OnMessage(CGUIMessage*msg);
public:
  CGuiDeleteFolders(CBook*bk,LIST*_list);
};

class CMyBook:public CBook
{
  wchar_t*findstr;
  wchar_t*open_folder;
  GUI*gui_input;
  CGuiListMenu*gui_find;
  CGuiListMenu*gui_folder;
  CGuiListMenu*gui_addfolder;
  CGuiListMenu*gui_del;
  bool state;
  LIST*find_list;
  LIST*folder_list;
  void*DB;
  u16 TimerID;
  //base_page
  static int TerminateElf(CBook**bm_book,CMyBook*mbk);
  static int ShowAuthorInfo(CBook**bm_book,CMyBook*mbk);
  DECLARE_PAGE_DESC_MEMBER(base_page);
  //input_page
  static int OnInputPage(void*,CMyBook*mbk);//������ ����� ������
  static void OnEnterPress_GuiInput(CMyBook*mbk,wchar_t*string,int len);//��
  static void OnBackPress_GuiInput(CMyBook*mbk,wchar_t*string,int len);//�����
  DECLARE_PAGE_DESC_MEMBER(input_page);
  //find_page
  static int OnFindShowPage(void*,CMyBook*mbk);//������ ����������� ���� � ���������� ������� ����� ������
  static int OnFindPage(void*,CMyBook*mbk);//������ ����������� ���� � ���������� ������� �� ����� ������
  static void OnFindElseWherePress_GuiFind(CBookBase*bk,CGuiBase*gui);//����� � ������ �����
  static void OnFindNewPress_GuiFind(CBookBase*bk,CGuiBase*gui);//����� �����
  static void OnExitPress_GuiFind(CBookBase*bk,CGuiBase*gui);//�����
  static void OnFindStopPress_GuiFind(CBookBase*bk,CGuiBase*gui);//���������� �����
  static void OnFolderOpenPress_GuiFind(CBookBase*bk,CGuiBase*gui);//������� �����
  DECLARE_PAGE_DESC_MEMBER(find_page);
  DECLARE_PAGE_DESC_MEMBER(findshow_page);
  //folder_page
  static int OnFolderPage(void*,CMyBook*mbk);//������ "����� �����"
  static void OnEnterPress_GuiFolder(CBookBase*bk,CGuiBase*gui);//�������
  static void OnBackPress_GuiFolder(CBookBase*bk,CGuiBase*gui);//�����
  DECLARE_PAGE_DESC_MEMBER(folder_page);
  //folderopen_page
  static int OnFolderOpenPage(void*,CMyBook*mbk);//������ �����
  static int OnExitPress_Folder(void*,CMyBook*mbk);//�����
  DECLARE_PAGE_DESC_MEMBER(folderopen_page);
  //timer
  static void OnTimer(u16 unk,LPARAM data);//������� �������
  //other_function
  int FolderInList(LIST*list,wchar_t*dir);//���� �� ��� ����� � �����
  wchar_t*GetFindStr();//�������� ������ ��� ������
  wchar_t*GetFindFolder();//�������� ����� ��� ������ � ��������� �� �� ������
  void AddFolderItem(wchar_t*dir,wchar_t*sub_dir);//�������� ����� ��� ������ � ����
  void AddFindItem(wchar_t*dir,wchar_t*name);//�������� ��������� ���� � ����
  void SetFindStr(wchar_t*str);//���������� ������ ��� ������
  LIST*GetFolderList();//�������� ���� � ������� ��� ������
  LIST*GetFindList();//�������� ���� � ���������� �������
  void QuickRefresh();//���������� ���������
protected:
  ~CMyBook();
public:
  CMyBook(wchar_t*_findstr,wchar_t**_findfolders,int nfolder);
};

