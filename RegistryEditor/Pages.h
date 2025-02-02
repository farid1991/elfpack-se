#ifndef _PAGES_H_
#define _PAGES_H_


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT  , ShowAuthorInfo,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC RegistryEditorBook_Base_Page = { "RegistryEditorBook_Base_Page", 0, bk_base_msglst };


const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG, RegistryEditorBook_MainPage_Enter,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC RegistryEditorBook_Main_Page = { "RegistryEditorBook_Main_Page", 0, bk_main_msglst };


const PAGE_MSG bk_select_backup_msglst[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG, RegistryEditorBook_SelectBackup_Page_Enter,
  ACCEPT_EVENT_TAG, RegistryEditorBook_SelectBackup_Page_Accept,
  PREVIOUS_EVENT_TAG, RegistryEditorBook_SelectBackup_Page_Previous,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC RegistryEditorBook_SelectBackup_Page = { "RegistryEditorBook_SelectBackup_Page", 0, bk_select_backup_msglst };



#endif
