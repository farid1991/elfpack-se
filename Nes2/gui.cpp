#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "book.h"
#include "gui.h"

#include "nes.h"

#include <math.h>

#define CD_HEADER_MODE
#include "config_data.c"
#undef CD_HEADER_MODE

#define	JOY_A	1
#define	JOY_B	2
#define	JOY_SELECT	4
#define	JOY_START	8
#define	JOY_UP	0x10
#define	JOY_DOWN	0x20
#define	JOY_LEFT	0x40
#define	JOY_RIGHT	0x80

typedef unsigned char byte;

extern byte joy_1,joy_2;

extern int spal[32];

MyGUI *MyGui;

unsigned char *XBuf;

int *screenbuf;

int DWidth;
int DHeight;

void InitVars()
{
  DWidth = Display_GetWidth(0);
  DHeight = Display_GetHeight(0);
}

void DrawStr(wchar_t *str, int x, int y, int color)
{
  SetFont(0xC);
  int sid = TextID_Create( str, ENC_UCS2, TEXTID_ANY_LEN );
  DrawString(sid, 0, x, y, 0xFFFF, y+0xC, 60, 0, color, 0 );
  TextID_Destroy(sid);
}

void video2screen()
{
  unsigned char *buf = XBuf;//+8*(256+16);
  int x_ratio = (int)((256<<16)/DWidth) +1;
  int y_ratio = (int)((240<<16)/DHeight)+1;
  int x2, y2;
  for (int i=0;i<DHeight;i++)
    for (int j=0;j<DWidth;j++)
    {
      x2 = ((j*x_ratio)>>16)+8;
      y2 = ((i*y_ratio)>>16);
      screenbuf[(i*DWidth)+j] = spal[buf[(y2*(256+16))+x2]];
    }
}

void GUI_Redraw(DISP_OBJ *dd, int a, int b, int c)
{
  GC *gc = get_DisplayGC();
  int gc_xx = GC_GetXX( gc );
  GC_SetXX( gc, 1 );

  GC_WritePixels( gc, 0, 0, DWidth, DHeight, screenbuf );

  /*wchar_t buf[0x100];
  snwprintf( buf, MAXELEMS(buf), L"fps %d", timing_fps);

  DrawStr( buf, 0, 0, 0xFFFFFFFF );*/

  GC_SetXX( gc, gc_xx );
}

void GUI_OnKey(DISP_OBJ *db, int key, int, int repeat, int type)
{
  if( type == KBD_SHORT_PRESS || type == KBD_LONG_PRESS )
  {
    if( key == key_a )
      joy_1 |= JOY_A;
    else if( key == key_b )
      joy_1 |= JOY_B;
    else if( key == key_select )
      joy_1 |= JOY_SELECT;
    else if( key == key_start )
      joy_1 |= JOY_START;
    else if( key == key_up )
      joy_1 |= JOY_UP;
    else if( key == key_down )
      joy_1 |= JOY_DOWN;
    else if( key == key_left )
      joy_1 |= JOY_LEFT;
    else if( key == key_right )
      joy_1 |= JOY_RIGHT;
  }
  else if( type == KBD_SHORT_RELEASE || type == KBD_LONG_RELEASE )
  {
    if( key == key_a )
      joy_1 &= ~JOY_A;
    else if( key == key_b )
      joy_1 &= ~JOY_B;
    else if( key == key_select )
      joy_1 &= ~JOY_SELECT;
    else if( key == key_start )
      joy_1 &= ~JOY_START;
    else if( key == key_up )
      joy_1 &= ~JOY_UP;
    else if( key == key_down )
      joy_1 &= ~JOY_DOWN;
    else if( key == key_left )
      joy_1 &= ~JOY_LEFT;
    else if( key == key_right )
      joy_1 &= ~JOY_RIGHT;
  }
  else
    return;
}

int GUI_OnCreate(DISP_OBJ *)
{
  return 1;
}

void GUI_OnClose(DISP_OBJ *)
{
}


void GUI_Exit(BOOK *bk,GUI*)
{
  UI_Event_toBookID( ELF_TERMINATE_EVENT , BookObj_GetBookID(MyBook) );
}


void GUI_Construct(DISP_DESC *dd)
{
  DISP_DESC_SetName( dd, "TEST_DD" );
  DISP_DESC_SetSize( dd, sizeof(DISP_OBJ) );

  DISP_DESC_SetOnCreate( dd, GUI_OnCreate);
  DISP_DESC_SetOnClose( dd, GUI_OnClose);

  DISP_DESC_SetOnRedraw( dd, GUI_Redraw );
  DISP_DESC_SetOnKey( dd, GUI_OnKey );
}

void GUI_Destruct(GUI *dd)
{
  return;
}

GUI *GUI_Create()
{
  MyGui = new MyGUI;

  if ( !GUIObject_Create( MyGui, GUI_Destruct, GUI_Construct, MyBook, 0, 0, 0) )
  {
    delete MyGui;
    return 0;
  }

  InitVars();

  screenbuf = (int*)malloc(DWidth*DHeight*sizeof(int));
  XBuf = (unsigned char*)malloc((256+16)*262/*240*/);

  memset(screenbuf, 0, DWidth*DHeight*sizeof(int));
  memset(XBuf, 0, (256+16)*262/*240*/ );

  BookObj_AddGUIObject( MyBook, MyGui );
  GUIObject_SetStyle( MyGui, UI_OverlayStyle_TrueFullScreen );
  GUIObject_SetTitleType( MyGui , UI_TitleMode_None );
  GUIObject_SoftKeys_Hide( MyGui );
  GUIObject_Show(MyGui);

  GUIObject_SoftKeys_SetAction( MyGui, ACTION_BACK, GUI_Exit );

  if( !start_emu() )
  {
    GUI_Destroy();
    return 0;
  }

  return MyGui;
}

void GUI_Destroy()
{
  //stop_emu();

  if( MyGui )
    MyGui = (MyGUI *)GUIObject_Destroy(MyGui);

  mfree(screenbuf);
  mfree(XBuf);
}
