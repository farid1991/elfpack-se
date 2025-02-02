#ifndef PAGE_DESC_H
#define PAGE_DESC_H

#include "main.h"

#define ELF_BCFG_CONFIG_EVENT 994

int HexEditor_OnExit(void *mess, BOOK *book);
int HexEditor_OnAbout(void *mess, BOOK *book);
int HexEditor_Reconfig(void *mess, BOOK *book);
int HexEditor_OnBcfgConfig(void *mess, BOOK *book);
int HexEditor_DB_OnAccept(void *mess, BOOK *book);
int HexEditor_DB_OnCancel(void *mess, BOOK *book);

int CreateHexEditorGUI(void *mess, BOOK *book);


//------------------------------------------------------
const PAGE_MSG HexEditor_base_evlist[] @ "DYN_PAGE"  = 
{
  ELF_TERMINATE_EVENT,    HexEditor_OnExit,
  ELF_SHOW_INFO_EVENT,    HexEditor_OnAbout,
  ELF_RECONFIG_EVENT,     HexEditor_Reconfig,
  ELF_BCFG_CONFIG_EVENT,  HexEditor_OnBcfgConfig,
  ACCEPT_EVENT_TAG,       HexEditor_DB_OnAccept,
  CANCEL_EVENT_TAG,       HexEditor_DB_OnCancel,
  NIL_EVENT_TAG,          NULL
};

const PAGE_MSG HexEditor_main_evlist[] @ "DYN_PAGE"  = 
{
  PAGE_ENTER_EVENT_TAG,   CreateHexEditorGUI,
  NIL_EVENT_TAG,          NULL
};

const PAGE_DESC HexEditor_BaseDesc = {"HexEditor_BasePage", 0, HexEditor_base_evlist};
const PAGE_DESC HexEditor_MainDesc = {"HexEditor_MainPage", 0, HexEditor_main_evlist};

#undef PAGE_DESC_H
#endif
