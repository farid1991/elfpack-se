#ifndef _PAGES_H_
#define _PAGES_H_


const PAGE_MSG bk_base_msglst[] @ "DYN_PAGE" =
{
  ELF_TERMINATE_EVENT , TerminateElf,
  ELF_SHOW_INFO_EVENT , ShowAuthorInfo,
  NIL_EVENT_TAG, 0
};

const PAGE_DESC LogToFSBook_Base_Page = { "LogToFSBook_Base_Page", 0, bk_base_msglst };


const PAGE_MSG bk_main_msglst[] @ "DYN_PAGE" =
{
  PAGE_ENTER_EVENT_TAG , LogToFSBook_MainPage_Enter,
  NIL_EVENT_TAG , 0
};

const PAGE_DESC LogToFSBook_Main_Page = { "LogToFSBook_Main_Page", 0, bk_main_msglst };


#endif
