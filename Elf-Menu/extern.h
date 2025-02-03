#ifndef _EXTERNS_
#define _EXTERNS_

#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "..\\include\dll.h"
#include "..\\include\var_arg.h"

#define AUTHOR "Elf-Menu\nv.2.1\n(c)arban\n(r)UltraShot,\nHussein,\nIronMaster,\nSlawwan\n\nThx for test to A-R-T\n and other"

typedef struct
{
  int menu_style;       //стиль меню
  int y_offset;      //отступ всей менюхи по y
  int x_spacing;     //гор. отступ между ячейками
  int y_spacing;     //вертикальный отступ между ячейками
  int x_tab;            //столбцы
  int x_size;          //ширина картинки
  int y_size;          //высота
  int x_size_s;          //ширина выделенной картинки
  int y_size_s;          //высота
  int is_selected;          //есть ли выбранная
  int cursor_speed;    //скорость курсора
  int icon_y_speed;        //скорость аним. иконки по y
  int icon_max_y_offset;  //макс. координата по y
  int icon_min_y_offset;   //мин координата по y
  int icon_x_speed;        //скорость аним. иконки по x
  int icon_max_x_offset;  //макс. координата по x
  int icon_min_x_offset;   //мин координата по x
  int lable_style;         //стиль надписей
  int lable_font;          //шрифт
  int lable_color;   //цвет
  int lable_stroke_color;  //цвет обводки
  int lable_x;              //x координата текста заголовка
  int lable_y;              //y координата текста заголовка
  int min_scale;          //мин. предел увеличения
  int max_scale;            //макс. предел увеличения
  int scale_speed;          //скорость увеличения
  int selected_type;        //тип анимации выделенной иконки
  int appear_speed;        //скорость появления

  int lable_offset;  //отступ текста от иконки
  int cursor_offset;  //вертикальное смещение курсора
  int cursor_color;  //цвет курсора
  int back_color;   //цвет фона
  int eventab_offset;  //отступ по y четного столбика
  int scroll_speed;  //скорость прокрутки

  int is_cursor_img;     //тип курсора (0-рисованный функами, 1-картинка)
  int cursor_w;     //ширина курсора
  int cursor_h;     //высота курсора
}STYLE;

typedef struct
{
  LIST*list;         //список пунктов
  int style;         //стиль(один из стандартных)
  int style_type;    //тип стиля(стандартный или свой)
  int gui_style;     //стиль гуи
  int is_sicon;     //есть ли вторая иконка
  wchar_t * cstyle;  //свой стиль(имя файла)
  STYLE st;        //свой стиль(структура)
  int last;          //последний пункт
  wchar_t*cursor_img;      //путь до иконки курсора
  IMAGEID cursorID;         //ID иконки курсора
}MENU;                    //структура, содержащая сведения о меню и само меню собственно

typedef struct
{
  wchar_t*name;      //название пункта
  wchar_t*img;       //файл иконки
  wchar_t*simg;      //файл выделенной иконки
  wchar_t*link;      //ссылка
  IMAGEID image;         //ID иконки
  IMAGEID simage;        //ID выделенной иконки
  int type;          //тип ссылки
  MENU menu;         //подменю
}ITEM;                    //структура, содержащая сведения о пункте меню

typedef struct
{
  int list;
  int is_second_icon;
  wchar_t * name;
}STYLE_ITEM;

typedef struct
{
  BOOK book;         //книга
  GUI_LIST*menu;     //GUI меню
  GUI_LIST*edit;     //GUI редактора
  GUI_LIST*type;     //GUI выбора типа ссылки
  GUI_LIST*set;      //GUI окна настроек
  GUI_TABMENUBAR * style_tab;
  GUI_LIST*gstyle;     //GUI выбора стиля окна
  GUI_LIST*cstyle;   //GUI выбора своего стиля
  GUI_LIST*style;    //GUI выбора стиля
  GUI_LIST*skin;     //GUI скина
  GUI_LIST*selectf;  //GUI выбора файла/папки
  GUI*str_input;     //GUI ввода имени
  GUI*sn_input;      //GUI ввода имени стиля
  GUI*ev_input;      //GUI ввода ивента
  GUI*vc_input;      //GUI ввода номера
  GUI*skn_input;     //GUI ввода названия нового скина
  GUI*g_del;         //GUI удаления
  GUI*gmenu;         //GUI графического меню
  //MENU cur_menu;     //текущее меню
  void * oldOnKey;   //старый OnKey для меню
  void * oldOnKey2;  //старый OnKey для редактора
  LIST * style_list; //список стилей
}MyBOOK;                  //структура, содержащая сведения о книге, GUI и т.д.

typedef struct {
  char dummy[0x2c];
} GUI_MENU;               //структура, содержащая свой GUI
/*
typedef struct
{
  //MENU * menu;     //меню
  //MENU pre_menu; //предыдущее меню
  ITEM*ci;        //предыдущий пункт, содержащий текущее меню, чтобы не было ребутов и мороки с указателями на меню
}HIS_ITEM;                //структура, содержащая пункт истории
*/
/*typedef struct
{
  LIST*list;         //список открытых подменю
}HISTORY;                //структура, содержащая историю
*/
extern LIST * history;  //история
extern MyBOOK * mbk;     //книга и т.д.
#define LCNT 50          //кол-во языковых меток
//макросы
#define IDFREE(a) if (a!=NOIMAGE && a!=0) {ImageID_Free(a);} a=NOIMAGE
#define TEXT_FREE(a) if(a!=EMPTY_TEXTID && a!=0){TextID_Destroy(a);} a=EMPTY_TEXTID

#define FREE_GUI(a) if (a) {GUIObject_Destroy(a); a = 0;}
#define DELETE(a) if (a) {delete(a);a=0;}
#define LIST_FREE(a) if (a) {List_Destroy(a);a=0;}

#define CONFIG_INT GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL)
#define CONFIG_EXT GetDir(DIR_ELFS_CONFIG|MEM_EXTERNAL)
#define NEWLABLE(a,b) names[a]=new wchar_t[wstrlen(b)+1];wstrcpy(names[a],b);
#define MESSAGEBOX(a)   MessageBox(EMPTY_TEXTID,a,0, 1 ,0,0);
extern void win12512unicode(wchar_t *ws, char *s, int len);  //перевод текста из вин-кодировки в юникод
extern char * unicode2win1251(char *s, wchar_t *ws, int len);//перевод текста из юникода в вин-кодировку
extern wchar_t * get_path(int is_card);                      //функция, возвращающая путь к папке ElfMenu
extern wchar_t * get_path_skin(int is_card);                 //функция, возвращающая путь к папке текущего скина
extern int getint(char *line, int *pos,char end);            //взять число из строки (c)ultrashot
//-------------работа с историей----------//
extern void History_Free();
extern void History_Get();
extern void History_Create();
//-------------работа с меню--------------//
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
//-------------кеширование----------------//
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
//-----------------настройки--------------//
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

//экшены меню
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
