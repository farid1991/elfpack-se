#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "..\include\var_arg.h"
#include "cfg_items.h"
#include "gui.h"
#include "main.h"
#include "game.h"
#include "random.h"

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

volatile int RedrawGUI = 0; // флаг необходимости перерисовки гуя

int GameGuiOnCreate(DISP_OBJ_GAME *db)
{
  randomize();
  InitGame();
  DispObject_SetRefreshTimer(&db->dsp_obj,100);
  return (1);
}

void GameGuiOnClose(DISP_OBJ_GAME *db)
{
  DispObject_KillRefreshTimer(&db->dsp_obj);
  FreeGame();
}

void GameGuiOnRedraw(DISP_OBJ_GAME *db,int ,RECT *cur_rc,int)
{
  int font_old, gc_xx;
  int font=FONT_E_20R;
  void *gc=get_DisplayGC();
  gc_xx=GC_GetXX((GC*)gc);
  GC_SetXX((GC*)gc,1);
  font_old=SetFont(font);
  ShowGame(gc, cur_rc);
  SetFont(font_old);
  GC_SetXX((GC*)gc,gc_xx);
}

void GameGuiOnKey(DISP_OBJ_GAME *db,int key,int,int repeat,int type)
{
  if((type == KBD_SHORT_PRESS) || (type == KBD_REPEAT))
  {
    if (key==KEY_LEFT_SOFT)
    {
      onEndGame();
    }
    else if(key==KEY_RIGHT_SOFT)
    {
      BookObj_ReturnPage(FindBook(isLinesBook),NIL_EVENT);
    }
    else if((key == KEY_UP) || (key == (KEY_DIGITAL_0 + 2)))
    {
      UpdateCursor(0,-1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if((key == KEY_DOWN) || (key == (KEY_DIGITAL_0 + 8)))
    {
      UpdateCursor(0,1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if((key == KEY_LEFT) || (key == (KEY_DIGITAL_0 + 4)))
    {
      UpdateCursor(-1,0);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if((key == KEY_RIGHT) || (key == (KEY_DIGITAL_0 + 6)))
    {
      UpdateCursor(1,0);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if(key == (KEY_DIGITAL_0 + 1))
    {
      UpdateCursor(-1,-1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if(key == (KEY_DIGITAL_0 + 3))
    {
      UpdateCursor(1,-1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if(key == (KEY_DIGITAL_0 + 7))
    {
      UpdateCursor(-1,1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if(key == (KEY_DIGITAL_0 + 9))
    {
      UpdateCursor(1,1);
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
    else if((key == KEY_ENTER) || (key == (KEY_DIGITAL_0 + 5)))
    {
      onClickCursor();
      DispObject_InvalidateRect(&db->dsp_obj,0);
    }
  }
}

void GameGuiOnRefresh(DISP_OBJ_GAME *db)
{
  if (RedrawGUI){
    DispObject_InvalidateRect(&db->dsp_obj,0);
    RedrawGUI=0;
  }
  DispObject_SetRefreshTimer(&db->dsp_obj,150);
}

static const char GameGuiName[]="Gui_GameLines";
void GameGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,GameGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ_GAME));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)GameGuiOnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)GameGuiOnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)GameGuiOnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)GameGuiOnKey);
  DISP_DESC_SetOnRefresh(desc,(DISP_OBJ_METHOD)GameGuiOnRefresh);
}

void GameGui_destr(GUI*){}

static GUI *CreateGameGui(BOOK *mbk)
{
  GAME_GUI *tmp_gui=new GAME_GUI;
  if (!GUIObject_Create((GUI *)tmp_gui,GameGui_destr,GameGui_constr, mbk,0,0,0))
  {
    delete tmp_gui;
    return 0;
  }
  GUI *gui=(GUI *)tmp_gui;
  if (mbk) BookObj_AddGUIObject(mbk,gui);
  GUIObject_SetStyle(gui,UI_OverlayStyle_TrueFullScreen);
  GUIObject_SetTitleType(gui, 1);
  GUIObject_SoftKeys_Hide(gui);
  return gui;
}

static void choice(BOOK* book, int ans)
{
  MyBOOK *mbk = (MyBOOK *)book;
  if(ans)
  {
    SetContinueGameState();
  }
  else
  {
    SetNewGameState();
  }
  mbk->game_gui = CreateGameGui(book);
  GUIObject_Show(mbk->game_gui);
}

static int GamePageOnEnter(void *, BOOK *book)
{

  MyBOOK *mbk = (MyBOOK *)book;
  if(isGameNotEnded())
  {
    wchar_t *en_q = L"Чи бажаєте ви продовжити збережену гру?";
    wchar_t *ru_q = L"Вы хотите продолжить сохраненную игру?";

    MsgBoxYesNo(book, Language ? en_q : ru_q, choice);
  }
  else
  {
    SetNewGameState();
    mbk->game_gui = CreateGameGui(book);
    GUIObject_Show(mbk->game_gui);
  }
  return (1);
}

static int GamePageOnExit(void *, BOOK *book)
{
  MyBOOK *mbk = (MyBOOK *)book;
  FREE_GUI(mbk->game_gui);
  return (1);
}

const PAGE_MSG Lines_GamePageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,  GamePageOnEnter,
  PAGE_EXIT_EVENT_TAG,   GamePageOnExit,
  NIL_EVENT_TAG,         NULL
};

const PAGE_DESC game_page ={"Lines_GamePage",0,Lines_GamePageEvents};

// ----------------------------------------------------------------------------
void OnBackGetNewName(MyBOOK * bk, u16 *string, int len)
{
}

void OnOkGetNewName(MyBOOK * bk, wchar_t *string, int len)
{
  if (len >0)
  {
    wstrncpy(bk->name,string,MAX_NAME_LEN);
    FREE_GUI(bk->text_input);
  }
}

void GetNewName(wchar_t *name)
{
  MyBOOK *bk = (MyBOOK *)FindBook(isLinesBook);
  TEXTID text, header_name;
  bk->name = name;
  header_name = Language ? TextID_Create(L"Введіть ваше ім`я:",ENC_UCS2,TEXTID_ANY_LEN) : TextID_Create(L"Введите Ваше имя:",ENC_UCS2,TEXTID_ANY_LEN);
  text = TextID_Create(name,ENC_UCS2,TEXTID_ANY_LEN);
  FREE_GUI(bk->text_input);
  bk->text_input=(GUI *)CreateStringInputVA(0,
                                          VAR_STRINP_MIN_LEN(1),
                                          VAR_STRINP_MAX_LEN(MAX_NAME_LEN),
                                          VAR_STRINP_MODE(1),
                                          VAR_BOOK(bk),
                                          VAR_STRINP_FIXED_TEXT(header_name),
                                          VAR_STRINP_TEXT(text),
                                          VAR_PREV_ACTION_PROC(OnBackGetNewName),
                                          VAR_OK_PROC(OnOkGetNewName),
                                          VAR_STRINP_IS_PASS_MODE(0),
                                          0);
}

// ----------------------------------------------------------------------------


