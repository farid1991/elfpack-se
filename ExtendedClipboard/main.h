#ifndef _MAIN_H_
  #define _MAIN_H_

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"

#define GUI_DESTROY(a) if(a){a = GUIObject_Destroy(a);};

#define BOOKNAME "ExtendedClipboard"

#define CLIPBOARD_TEXTCOPIED_EVENT 0x33C

#define FILE_HANDLE_INVALID   -1
#define RESULT_FAIL           -1
#define RESULT_OK              0

#define CLIPBOARD_CURRENT_TEXT 0

typedef struct
{
  BOOK book;
  LIST *clipboard_texts;
  GUI *list_menu;
  DISP_OBJ_ONKEY_METHOD old_on_key;
  int selected_item;
  int called_from_string_input;
  int hotkeys_state;
}ExtClipboardBook;

#define FILE_NAME_SET L"/tpa/cb_temp/hotkeys.bin"
#define FILE_NAME L"/tpa/cb_temp/clipboard_text_%02d.txt"
#define PATH L"/tpa/cb_temp"

int onExit(void *msg, BOOK *book);
int onAbout(void *msg, BOOK *book);
int CreateViewer(void *msg, BOOK *book);
ExtClipboardBook *Find_ExtClipboardBook(void);

#endif
