#ifndef _MAIN_H
#define _MAIN_H

#define ELFNAME "Lines"
#define LELFNAME L"Lines"
#define LELFVERSION L"v3.1"
#define LAUTHORINFO L"©BigHercules"
#define LTRANSLATED L"(r) Therion"


typedef struct
{
  BOOK  book;
  wchar_t path[128];
  GUI_LIST *menu_gui;
  GUI *game_gui;
  GUI *text_input;
  wchar_t *name;
  GUI *yes_no;
  void (*YesNoFunc)(BOOK*,int); //  1 - yes, 0 - no
  IMAGEID logo;
  IMAGEID cursor;
}MyBOOK;

typedef struct {
  DISP_OBJ dsp_obj;
} DISP_OBJ_GAME;

typedef struct {
  char gui[0x2c];
} GAME_GUI;


typedef GC* pCG;

#define FREE_GUI(a) if (a) a=GUIObject_Destroy(a)

void MsgBoxYesNo(BOOK* book, wchar_t *qv, void(*f)(BOOK*,int));
int isLinesBook(BOOK * book);
int onBcfgConfig(void *mess, BOOK *book);

#endif
