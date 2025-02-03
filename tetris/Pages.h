#include "ActionList.h"
#include "Menu.h"
#include "DataBrowser.h"
#include "Game.h"
#include "NewRecord.h"
#include "Save.h"
#include "MyFUNC.h"
#include "erroriar.h"

#ifndef _PAGES_H_
  #define _PAGES_H_

const PAGE_MSG bk_msglst_save[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    CreateAsk,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bsave = {"Game_GuiMenu_Page",0,bk_msglst_save};

const PAGE_MSG bk_msglst_go[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    CreateGO,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_go = {"Game_GuiMenu_Page",0,bk_msglst_go};

const PAGE_MSG bk_msglst_db[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,     CreateDB,
  ACCEPT_EVENT_TAG,         onAccept_DB,
  PREVIOUS_EVENT_TAG,       onCancelDB,
  CANCEL_EVENT_TAG,         onCancelDB,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,            NULL
};

const PAGE_DESC bk_db = {"OpenSaveFile",0,bk_msglst_db};

const PAGE_MSG bk_msglst_gm[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    Create,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_gm = {"Game_GuiMenu_Page",0,bk_msglst_gm};

const PAGE_MSG bk_msglst_gs[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    OpenGame,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_gs = {"Game_OpenGame_Page",0,bk_msglst_gs};

const PAGE_MSG bk_msglst_base[] @ "DYN_PAGE"  =
{
  ELF_TERMINATE_EVENT,     FreeElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_base = {"Game_Base_Page",0,bk_msglst_base};



const PAGE_MSG bk_msglst_ggm[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    CreateGame,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_game = {"Game_GuiGame_Page",0,bk_msglst_ggm};

const PAGE_MSG bk_al_ggm[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,    ActionListCreate,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG,           NULL
};

const PAGE_DESC bk_actionlist = {"Game_ActionList_Page",0,bk_al_ggm};

#endif
