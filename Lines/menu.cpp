#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "cfg_items.h"
#include "menu.h"
#include "gui.h"
#include "game.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

typedef struct
{
  wchar_t *ru;
  wchar_t *en;
}menu_item_text_t;

#define MENU_ITEMS_NUM 6

void doPlay(BOOK *book);
void doHighScores(BOOK *book);
void doHelp(BOOK *book);
void doSettings(BOOK *book);
void doAbout(BOOK *book);
void doExit(BOOK *book);

DISP_OBJ_ONKEY_METHOD MenuOldOnKey;

static menu_item_text_t menutexts[MENU_ITEMS_NUM]=
{
  {L"Играть",    L"Грати"},
  {L"Рекорды",   L"Рекорди"},
  {L"Помощь",    L"Допомога"},
  {L"Настройки", L"Налаштування"},
  {L"Об игре",   L"Про гру"},
  {L"Выход",     L"Вихід"}
};

static void (*smenuprocs[MENU_ITEMS_NUM])(BOOK *) =
{
  doPlay,
  doHighScores,
  doHelp,
  doSettings,
  doAbout,
  doExit
};

void doPlay(BOOK *book)
{
  BookObj_CallPage(book,&game_page);
}

void doHighScores(BOOK *book)
{
  int header = Language ? STR("Найкращі гравці") : STR("Лучшие игроки");
  int text = BestResults();
  MessageBox(header, text, 0, 2, 0, 0);
}

void doHelp(BOOK *book)
{
  int header = Language ? STR("Правила гри") : STR("Правила игры");
  int text;
  if(Language)
  {
    text = STR("На квадратному полі, розміром 9х9, з`являються різнокольорові кульки,7-ми кольорів.\nПеретягуючи одну кульку за один крок,потрібно вистроїти їх в одну лінію одного кольору. Вони зникають коли їх набереться більше 5-ти, або 5. Гра продовжується,поки є місце,куди можливо пересунути кульку.\nЗа кожну  'згорівшу'  лінію отримуємо очки. Чим більшу кількість кульок було вистроєно,тим більше очків.\nЦіль Lines - набрати максимальну кількість очків");
  }
  else
  {
    text = STR("На квадратном поле размером 9Х9 появляются разноцветные шарики. Всего 7 цветов.\nПередвигая один шарик за один ход, выстраиваем в линию шары одного цвета. Они исчезают, когда их набирается 5 (или больше). Игра продолжается, пока есть место, куда можно передвинуть шарик.\nЗа каждую 'сгоревшую' линию получаем очки. Чем больше количество шаров выстроили, тем больше очков.\nЦель Lines - набрать наибольшее число очков.");
  }
  MessageBox(header,text, 0, 2, 0, 0);
}

void doSettings(BOOK *book)
{
  onBcfgConfig(0,book);
}

void doAbout(BOOK *book)
{
  if(Language)
  {
    MessageBox(0x6fFFFFFF,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO _T("\n") LTRANSLATED,ENC_UCS2,TEXTID_ANY_LEN), 0, 1, 5000, book);
  }
  else
  {
    MessageBox(0x6fFFFFFF,TextID_Create( LELFNAME _T("\n") LELFVERSION _T("\n") LAUTHORINFO,ENC_UCS2,TEXTID_ANY_LEN), 0, 1, 5000, book);
  }
}

void doExit(BOOK *book)
{
  FreeBook(book);
}

static void OnSelect1Gui(BOOK *book, GUI *)
{
  MyBOOK *bk = (MyBOOK *)book;
  int item = ListMenu_GetSelectedItem(bk->menu_gui);
  smenuprocs[item](book);
};

static void OnBackGui(BOOK *book, GUI *)
{
  if(isGameActivated())
    doPlay(book);
  else
    FreeBook(book);
};

static int onLBMessage(GUI_MESSAGE * msg)
{
  switch(GUIonMessage_GetMsg( msg ))
  {
  case 1:
    int item=GUIonMessage_GetCreatedItemIndex(msg);
    if((item >= 0) && (item < MENU_ITEMS_NUM))
      GUIonMessage_SetMenuItemText(msg,TextID_Create(Language ? menutexts[item].en : menutexts[item].ru , ENC_UCS2, TEXTID_ANY_LEN));
    break;
  }
  return 1;
};

void MenuOnKey(DISP_OBJ *p, int keyID, int i2, int i3, int press_mode)
{
  MyBOOK * bk = (MyBOOK *) FindBook(isLinesBook);

  MenuOldOnKey( p, keyID, i2, i3, press_mode );

  if(press_mode==KBD_SHORT_RELEASE)
  {
    int num = keyID - KEY_DIGITAL_0;
    if(num == 2)
    {
      int item = ListMenu_GetSelectedItem(bk->menu_gui) - 1;
      if(item < 0) item = MENU_ITEMS_NUM - 1;
      ListMenu_SetCursorToItem(bk->menu_gui,item);
    }
    else if((num == 8) || (num == 0))
    {
      int item = ListMenu_GetSelectedItem(bk->menu_gui) + 1;
      if(item >= MENU_ITEMS_NUM) item = 0;
      ListMenu_SetCursorToItem(bk->menu_gui,item);
    }
    else if(num == 5)
    {
      OnSelect1Gui((BOOK*)bk,0);
    }
  }
};

static GUI_LIST *CreateMainMenu(BOOK *book)
{
  GUI_LIST * lo=0;
  if (lo=CreateListMenu(book,0))
  {
    MyBOOK *mbk = (MyBOOK *)book;
    ListMenu_SetItemCount(lo,MENU_ITEMS_NUM);
    ListMenu_SetCursorToItem(lo,0);
    ListMenu_SetOnMessage(lo,onLBMessage);
    GUIObject_SetTitleIcon( lo, mbk->logo );
    GUIObject_SetTitleText(lo,TextID_Create(L"Lines",ENC_UCS2,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(lo,ACTION_BACK, OnBackGui);
    GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,OnSelect1Gui);

    MenuOldOnKey = DispObject_GetOnKey(GUIObject_GetDispObject( lo ));
    DISP_DESC_SetOnKey( DispObject_GetDESC ( GUIObject_GetDispObject( lo ) ), MenuOnKey );
  }
  return(lo);
}

extern int cur_resource;
static int MenuPageOnEnter(void *, BOOK *book)
{
  MyBOOK *mbk = (MyBOOK *)book;
  mbk->menu_gui = CreateMainMenu(book);
  GUIObject_Show(mbk->menu_gui);
  if(cur_resource != resource)
  {
    if(Language)
      MessageBox(0x6FFFFFFF, STR("Помилка при завантаженні зовнішніх рксурсів!"), 0, 1 ,5000, 0);
    else
      MessageBox(0x6FFFFFFF, STR("Ошибка при загрузке внешних ресурсов!"), 0, 1 ,5000, 0);
  }
  return (1);
}
/*
static int MenuPageOnExit(void *, BOOK *book)
{
  MyBOOK *mbk = (MyBOOK *)book;
  FREE_GUI(mbk->menu_gui);
  return (1);
}
*/
const PAGE_MSG Lines_MenuPageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,  MenuPageOnEnter,
//  PAGE_EXIT_EVENT_TAG,   MenuPageOnExit,
  NIL_EVENT_TAG,         NULL
};

const PAGE_DESC menu_page ={"Lines_MenuPage",0,Lines_MenuPageEvents};

