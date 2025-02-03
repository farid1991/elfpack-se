#ifndef _EXTERNS_
#define _EXTERNS_

#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\dll.h"
#include "..\\include\var_arg.h"

#define AUTHOR "Elf-Menu\nv.2.1\n(c)arban\n(r)UltraShot,\nHussein,\nIronMaster,\nSlawwan\n\nThx for test to A-R-T\n and other"

typedef struct
{
  int menu_style;       //����� ����
  int y_offset;      //������ ���� ������ �� y
  int x_spacing;     //���. ������ ����� ��������
  int y_spacing;     //������������ ������ ����� ��������
  int x_tab;            //�������
  int x_size;          //������ ��������
  int y_size;          //������
  int x_size_s;          //������ ���������� ��������
  int y_size_s;          //������
  int is_selected;          //���� �� ���������
  int cursor_speed;    //�������� �������
  int icon_y_speed;        //�������� ����. ������ �� y
  int icon_max_y_offset;  //����. ���������� �� y
  int icon_min_y_offset;   //��� ���������� �� y
  int icon_x_speed;        //�������� ����. ������ �� x
  int icon_max_x_offset;  //����. ���������� �� x
  int icon_min_x_offset;   //��� ���������� �� x
  int lable_style;         //����� ��������
  int lable_font;          //�����
  int lable_color;   //����
  int lable_stroke_color;  //���� �������
  int lable_x;              //x ���������� ������ ���������
  int lable_y;              //y ���������� ������ ���������
  int min_scale;          //���. ������ ����������
  int max_scale;            //����. ������ ����������
  int scale_speed;          //�������� ����������
  int selected_type;        //��� �������� ���������� ������
  int appear_speed;        //�������� ���������

  int lable_offset;  //������ ������ �� ������
  int cursor_offset;  //������������ �������� �������
  int cursor_color;  //���� �������
  int back_color;   //���� ����
  int eventab_offset;  //������ �� y ������� ��������
  int scroll_speed;  //�������� ���������

  int is_cursor_img;     //��� ������� (0-���������� �������, 1-��������)
  int cursor_w;     //������ �������
  int cursor_h;     //������ �������
}STYLE;

typedef struct
{
  LIST*list;         //������ �������
  int style;         //�����(���� �� �����������)
  int style_type;    //��� �����(����������� ��� ����)
  int gui_style;     //����� ���
  int is_sicon;     //���� �� ������ ������
  wchar_t * cstyle;  //���� �����(��� �����)
  STYLE st;        //���� �����(���������)
  int last;          //��������� �����
  wchar_t*cursor_img;      //���� �� ������ �������
  IMAGEID cursorID;         //ID ������ �������
}MENU;                    //���������, ���������� �������� � ���� � ���� ���� ����������

typedef struct
{
  wchar_t*name;      //�������� ������
  wchar_t*img;       //���� ������
  wchar_t*simg;      //���� ���������� ������
  wchar_t*link;      //������
  IMAGEID image;         //ID ������
  IMAGEID simage;        //ID ���������� ������
  int type;          //��� ������
  MENU menu;         //�������
}ITEM;                    //���������, ���������� �������� � ������ ����

typedef struct
{
  int list;
  int is_second_icon;
  wchar_t * name;
}STYLE_ITEM;

typedef struct
{
  BOOK book;         //�����
  GUI_LIST*menu;     //GUI ����
  GUI_LIST*edit;     //GUI ���������
  GUI_LIST*type;     //GUI ������ ���� ������
  GUI_LIST*set;      //GUI ���� ��������
  GUI_TABMENUBAR * style_tab;
  GUI_LIST*gstyle;     //GUI ������ ����� ����
  GUI_LIST*cstyle;   //GUI ������ ������ �����
  GUI_LIST*style;    //GUI ������ �����
  GUI_LIST*skin;     //GUI �����
  GUI_LIST*selectf;  //GUI ������ �����/�����
  GUI*str_input;     //GUI ����� �����
  GUI*sn_input;      //GUI ����� ����� �����
  GUI*ev_input;      //GUI ����� ������
  GUI*vc_input;      //GUI ����� ������
  GUI*skn_input;     //GUI ����� �������� ������ �����
  GUI*g_del;         //GUI ��������
  GUI*gmenu;         //GUI ������������ ����
  //MENU cur_menu;     //������� ����
  void * oldOnKey;   //������ OnKey ��� ����
  void * oldOnKey2;  //������ OnKey ��� ���������
  LIST * style_list; //������ ������
}MyBOOK;                  //���������, ���������� �������� � �����, GUI � �.�.

typedef struct {
  char dummy[0x2c];
} GUI_MENU;               //���������, ���������� ���� GUI
/*
typedef struct
{
  //MENU * menu;     //����
  //MENU pre_menu; //���������� ����
  ITEM*ci;        //���������� �����, ���������� ������� ����, ����� �� ���� ������� � ������ � ����������� �� ����
}HIS_ITEM;                //���������, ���������� ����� �������
*/
/*typedef struct
{
  LIST*list;         //������ �������� �������
}HISTORY;                //���������, ���������� �������
*/
extern LIST * history;  //�������
extern MyBOOK * mbk;     //����� � �.�.
#define LCNT 50          //���-�� �������� �����
//�������
#define IDFREE(a) if (a!=NOIMAGE && a!=0) {ImageID_Free(a);} a=NOIMAGE
#define TEXT_FREE(a) if(a!=EMPTY_TEXTID && a!=0){TextID_Destroy(a);} a=EMPTY_TEXTID

#define FREE_GUI(a) if (a) {GUIObject_Destroy(a); a = 0;}
#define DELETE(a) if (a) {delete(a);a=0;}
#define LIST_FREE(a) if (a) {List_Destroy(a);a=0;}

#define CONFIG_INT GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL)
#define CONFIG_EXT GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL)
#define NEWLABLE(a,b) names[a]=new wchar_t[wstrlen(b)+1];wstrcpy(names[a],b);
#define MESSAGEBOX(a)   MessageBox(EMPTY_TEXTID,a,0, 1 ,0,0);
extern void win12512unicode(wchar_t *ws, char *s, int len);  //������� ������ �� ���-��������� � ������
extern char * unicode2win1251(char *s, wchar_t *ws, int len);//������� ������ �� ������� � ���-���������
extern wchar_t * get_path(int is_card);                      //�������, ������������ ���� � ����� ElfMenu
extern wchar_t * get_path_skin(int is_card);                 //�������, ������������ ���� � ����� �������� �����
extern int getint(char *line, int *pos,char end);            //����� ����� �� ������ (c)ultrashot
//-------------������ � ��������----------//
extern void History_Free();
extern void History_Get();
extern void History_Create();
//-------------������ � ����--------------//
extern void Menu_Read();
extern void Menu_Free(MENU*menu);
extern void Menu_Create();
extern void Menu_Copy(MENU*dest,MENU src);
extern void Menu_Save();
extern void Menu_TitleRefresh();
extern void Menu_Refresh();
extern void Menu_Check();
extern MENU * Menu_Get();
extern int Menu_CheckStyle();
extern ITEM * Menu_GetSelected();
extern void Item_Init(ITEM * it, int type);
extern void Menu_Init(MENU * menu);
//----------------------------------------//
extern wchar_t * GetPath_Skin(int is_card);
extern wchar_t * GetPath_Menu();
extern wchar_t * get_style_path(int is_card);
extern void Book_Refresh();
extern void Config_Read();
extern void Config_Save();
//-------------�����������----------------//
extern void Cache_SkinPath();
extern void Cache_SkinPath_Del();
extern void Cache_Lables();
extern void Cache_Lables_Del();
//----------------------------------------//

extern void Link_Execute();
//extern void History_Refresh();
extern void Reg_Image(IMAGEID*img,wchar_t*path,wchar_t*name);
extern void Icon_Reg(ITEM*it,wchar_t*name,int is_sel);
extern void Icon_Unreg(ITEM*it,int is_sel);
extern void Softkey_SetAction(int mode);

//extern void MenuCreateGuiList(void * r0, BOOK * bk);


//extern void SubOnEnterPressed(BOOK *bk, GUI *lt);
extern void EditOnEnterPressed(BOOK *bk, GUI *lt);
extern void myOnKey(void *p, int i1, int i2, int i3, int i4);
extern void myOnKey2(void *p, int i1, int i2, int i3, int i4);
extern void myOnKey3(void *p, int i1, int i2, int i3, int i4);

extern int DB_Filter(const wchar_t * ext_table,const wchar_t * path ,const wchar_t * name);
extern int GetIconID(wchar_t *txt);
extern int onPrevious_MainMenu_DB(void *data, BOOK * book);
extern int onCancel_MainMenu_DB(void *data, BOOK * book);
extern wchar_t * names[LCNT];
//-----------------���������--------------//
extern wchar_t *SKIN;
extern int VIBRA;
extern int IS_HOOK;
extern int IS_FULLSCREEN;
extern int IS_NOSOFTKEYS;
extern int IS_DYNTITLE;
extern int IS_EDITOR;
extern int IS_JOY;
extern int IS_MOVE;
extern int IS_EXIT;
extern int IS_MAIN_DYNTITLE;
//----------------------------------------//
extern int CreateMainMenu(void *data, BOOK * book);
extern int CreateDB(void *data, BOOK * book);
extern int CreateImgDB(void *data, BOOK * book);
extern PAGE_DESC SelectElf_page;
/*
extern PAGE_DESC SelectImg_page;
extern PAGE_DESC SelectSImg_page;
extern PAGE_DESC SelectCursorImg_page;*/
extern PAGE_DESC bk_GMenu;
extern PAGE_DESC bk_splash;
extern int CreateJavaList(void *data, BOOK * book);
extern void SetCreateGuiList(void * r0, BOOK * bk);
extern void StyleCreateGuiList(void * r0, BOOK * bk);
extern void EditCreateGuiList(void * r0, BOOK * bk);
extern void kickbadsymbols(wchar_t *string, int len);
extern PAudioControl pAC;
extern int isMyBook(BOOK * book);

extern void SkinCreateGuiList(void * r0, BOOK * bk);
extern int Platform;
extern int intget(char *line, char end);
extern wchar_t *getwchr(char *line, int *pos, char end);
extern char *getparam(char *buf, char *mask, char end, int fsize);


extern void DrawIcn(void*GC,RECT rct,int pos,short icn);

extern int h2i(char * h);
extern wchar_t Slash[];
extern int move;
extern int move_index;
extern wchar_t * cur_title;

extern wchar_t*skin_path_int;
extern wchar_t*skin_path_ext;
extern wchar_t*settings_path;

extern GUI_LIST *CreateFileFolderSelect(MyBOOK * myBook, wchar_t *str);
extern void CreateVcInput();
extern void CreateEvInput();
extern int wstr2id(wchar_t*str);

extern void Menu_Item_Move();
extern void Return_ToStandby();

//������ ����
extern void MenuOnEnterPressed(BOOK *bk, GUI*lt);
extern void MenuAdd(BOOK * book,GUI* lt);
extern void SubMenuAdd(BOOK * book,GUI * lt);
extern void Move(BOOK * book,GUI* lt);
extern void Skin_(BOOK * book,GUI* lt);
extern void OnDelGui(BOOK *bk, GUI *);
extern void Style_(BOOK * book,GUI* lt);
extern void Edit_(BOOK * book,GUI * lt);
extern void Options_(BOOK * book,GUI * lt);
extern void MenuBack(BOOK *bk, GUI *lt);

extern u16 timer;
extern int cursor;
extern int GET_SELECTED();
extern void MENU_CREATE_GUI();
extern void Style_Read(wchar_t*path, MENU * mn);
extern void HotKey(int x);

extern int GetInt(wchar_t * buf,int * x, wchar_t end);
extern wchar_t * GetWchar(wchar_t * buf,int * x, wchar_t end);
extern void Read_Lang();
extern void Create_SelectImageDB(int mode);
extern int JavaName_GetID(wchar_t * name);
extern void set_nav();
wchar_t * mstr(wchar_t * w);
extern void RefreshEdList(GUI_LIST * g,int itemcount);
extern void SetRefresh();
extern wchar_t separator;
extern void Styles_List_Destroy();
extern void CustomStyles_List_Destroy();
extern void (*action_std)(BOOK *,GUI*);
#endif
