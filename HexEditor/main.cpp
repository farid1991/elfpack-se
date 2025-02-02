#include "main.h"

DLL_DATA *dll_data;

wchar_t *SPLASH = L"/";
wchar_t *HEXEDITOR = L"HexEditor";

LIST *corrections_list;

wchar_t *elf_name;

//wchar_t *CFGfile;
wchar_t *file_o;
wchar_t *find_text;

LIST *menu_items;

int CELLS_COUNT;

int is_menu;
int menu_item;

int about_visible;

int page;
int offset;
int file_size;

char *buffer; // один раз выделили пам€ть дл€ CELLS_COUNT и всЄ!
              // и реаллокать только если изменилась CELLS_COUNT

int w, h;

OPEN_MODE_T open_mode; // тип открываемого ресурса

FILE_HANDLE f;

VIEW_MODES_T view_mode = HEX_MODE;

GUI *gui_list[NUM_OF_GUI];

int item_enable[MENU_MAX+1];
int item_visible[MENU_MAX+1];

char *book_name;

void elf_exit(void)
{
  trace_done();
  kill_data(&ELF_BEGIN, (void(*)(void *))mfree_adr());
}

int isHexEditorBook(BOOK *book)
{
  if(!strcmp(book->xbook->name, book_name)) return(1);
  return(0);
}

BOOK *Find_HexEditorBook(void)
{
  return(FindBook(isHexEditorBook));
}

void HexEditorMenu_Item_SetEnable(int item, int enable)
{
  item_enable[item] = enable;
}

int HexEditorMenu_Item_GetEnable(int item)
{
  return(item_enable[item]);
}

void HexEditorMenu_Item_SetVisible(int item, int enable)
{
  item_visible[item] = enable;
}

int HexEditorMenu_Item_GetVisible(int item)
{
  return(item_visible[item]);
}

int HexEditor_OnAbout(void *mess, BOOK *book)
{
  char *info = BOOKNAME ENTER VERSION ENTER DATE ENTER ENTER AUTHOR ENTER ENTER PORTED ENTER;
  TEXTID id = TextID_Create(info, ENC_LAT1, strlen(info));
  MessageBox(EMPTY_TEXTID, id, NOIMAGE, 1, 0, 0);
  return(1);
}

GUI *BookObj_GetGUIObject(BOOK *book, int num)
{
  GUI *gui = (GUI *)List_Get(book->xguilist->guilist, num);
  if((int)gui == LIST_ERROR) return(NULL);
  else return(gui);
}

int TextColor;

int MyGuiOnCreate_0(DISP_OBJ_HE *db)
{
  TextColor = clBlack;
  about_visible = true;
  w = Display_GetWidth(0);
  h = Display_GetHeight(0);
  return(1);
}

int MyGuiOnCreate_1(DISP_OBJ_HE *db)
{
  return(1);
}

int MyGuiOnCreate_5(DISP_OBJ_HE *db)
{
  is_menu = false;
  menu_item = MENU_FILE_OPEN;
  
  HexEditorMenu_Item_SetEnable(MENU_FILE_OPEN, true);
  //HexEditorMenu_Item_SetEnable(MENU_RAM_OPEN, true);
  HexEditorMenu_Item_SetEnable(MENU_RAM_OPEN, false);
  HexEditorMenu_Item_SetEnable(MENU_FIND, true);
  HexEditorMenu_Item_SetEnable(MENU_GOTO_OFFSET, true);
  HexEditorMenu_Item_SetEnable(MENU_GOTO_PAGE, true); //отключено, тк ф-и€ не написана
  //HexEditorMenu_Item_SetEnable(MENU_SETTINGS, true);
  HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false); //отключено, тк файл не открыт или нечего сохран€ть
  HexEditorMenu_Item_SetEnable(MENU_CLOSE_FILE, true);
  HexEditorMenu_Item_SetEnable(MENU_CLOSE_VIEW_RAM, true);
  HexEditorMenu_Item_SetEnable(MENU_EXIT, true);
  
  HexEditorMenu_Item_SetVisible(MENU_FILE_OPEN, true);
  HexEditorMenu_Item_SetVisible(MENU_RAM_OPEN, true);
  HexEditorMenu_Item_SetVisible(MENU_FIND, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, false);
  HexEditorMenu_Item_SetVisible(MENU_SETTINGS, true);
  HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_VIEW_RAM, false);
  HexEditorMenu_Item_SetVisible(MENU_EXIT, true);
  
  if(open_mode == OPEN_FILE)
  {
    HexEditorMenu_Item_SetVisible(MENU_FIND, true);
    HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, true);
    HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, true);
    HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, true);
    HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
    HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, true);
    
    Redraw_Update(true, true, true, true, true, false);
  }
  
  return(1);
}

void MyGuiOnClose(DISP_OBJ_HE *db)
{
}

wchar_t* SplitPath( wchar_t* path, wchar_t* spliter ) 
{
  int j = wstrlen(path)-2;
  while( j > 0 ) 
  {
    if( !wstrncmp(path+j, spliter, 1) ) return(path+j+1);
    j--;
  }
  return(path);
}

/*
void u_ltou(int intr, wchar_t *ws)
{
  snwprintf(ws, 0xFF, L"%d", intr);
}
*/

char *GetMode(void)
{
  switch(view_mode)
  {
  case HEX_MODE:
    return("HEX");

  case DEC_MODE:
    return("DEC");

  case TEXT_MODE:
    return("TEXT");
  }
  return(NULL);
}

int UCS2toCP1251(wchar_t *src, char *trg)
{
    int  i;

    for(i=0; i <= wstrlen(src); i++) {
        if (src[i] < 0xA0) { // до 128
            trg[i] = src[i];
        } else if (src[i] == 0x451){ // Є
            trg[i] = 0xB8;
        } else if (src[i] == 0x401){ // ®
            trg[i] = 0xA8;
        } else if (src[i] > 0x40F) { // кирилица
            trg[i] = src[i] - 0x350;
        } else {
            trg[i] = 0x3F; // ? - нет символа
        }
    }

    return RESULT_OK;
}

int IllegalSymbol(wchar_t *wstr, int len)
{
  for(;;)
  {
    wchar_t s = *wstr++;
    if(((s>='0') && (s<='9'))||((s>='a') && (s<='f'))||((s>='A') && (s<='F'))||(s=='?'))
      len--;
    else break;
  }
  return(len);
}

int stoi( char *str, int len )
{
  char s;
  int res=0;
  while(len--)
  {
    s=*str++;
    if((s>='0') && (s<='9')) s -= '0';
    res *= 10;
    res += s*10;
  }
  return(res/10);
}

int stoh( char *str, int len )
{
  char s;
  int res=0;
  while(len--)
  {
    s=*str++;
    if ((s>='0') && (s<='9')) s -= '0';
    else if((s>='A') && (s<='F')) s -= '7';
    else if((s>='a') && (s<='f')) s -= ('7' + ' ');
    res<<=4;
    res|=s;
  }
  return(res);
}

int Data2Pattern(wchar_t *data, char *pattern)
{
    char hex[3];
    hex[2] = 0;
    int j=0, s;

    switch(view_mode)
    {
    case DEC_MODE:
    case HEX_MODE:
        s = 2;
        wstr2strn(pattern, data, wstrlen(data));
        
        if(view_mode == DEC_MODE) s = 3;
        
        for(int i=0; i < strlen(pattern); i+=s)
        {
            strncpy(hex, pattern+i, s);
            if(hex[0] == '?' || hex[1] == '?')
              pattern[j] = '?';
            else
              if(s == 2) pattern[j] = stoh( hex, 2 ); // hex
              else if(s == 3) pattern[j] = stoi( hex, 3 ); // dec
            j++;
        }
        pattern[j] = 0;
        break;

    case TEXT_MODE:
        UCS2toCP1251(data, pattern);
        break;
    }

    return(RESULT_OK);
}

int FindPattern(char *pattern, int off_set)
{
    int cur_offset = off_set;
    int begin_offset = RelativeOffset(off_set);  
    int offset_pattern = -1;
    int i=0, j=0;

    //debug_printf("\nFind pattern = '%s'\n", pattern);
    
    //debug_printf("\n!!! cur_offset = 0x%X\n", cur_offset);
    //debug_printf("\n!!! begin_offset = 0x%X\n", begin_offset);

    while(cur_offset+i < file_size+CELLS_COUNT)
    {
        LoadBuffer(cur_offset);

        for(i=begin_offset; (i < CELLS_COUNT) && (i <= file_size); i++)
        {
            //debug_printf("\nsym buffer = 0x%x\n", buffer[i]);
            //debug_printf("\nsym pattern = 0x%x\n", pattern[j]);
            //debug_printf("\ni = 0x%x\n", i);
            if(pattern[j] == buffer[i] || pattern[j] == '?')
            {
                if (j < strlen(pattern)-1)
                {
                    //debug_printf("\nSymbol %d found\n", pattern[j]);
                    //debug_printf("\ni = 0x%X, j = 0x%X\n", i, j);
                    j++;
                    if(offset_pattern == -1)
                      offset_pattern = (cur_offset/CELLS_COUNT)*CELLS_COUNT+i;
                }
                else
                {
                    if(strlen(pattern)==1)
                      offset_pattern = (cur_offset/CELLS_COUNT)*CELLS_COUNT+i; // поправка дл€ односимвольной паттерны
                    //debug_printf("\nPattern found. offset = 0x%x\n", offset_pattern);
                    GoTo(offset_pattern); // паттерна найдена!
                    return(offset_pattern);
                }
            }
            else
            {
                j=0;
                offset_pattern = -1;
            }
        }

        begin_offset = 0;
        cur_offset += CELLS_COUNT;
    }

    LoadBuffer(off_set);

    return(offset_pattern);
}

int RelativeOffset(int off_set)
{
   int I;
   if(off_set >= CELLS_COUNT) I = off_set % (CELLS_COUNT);
   else I = off_set;
   return(I);
}

POINT XY(int sItem)
{
    POINT item;
    int I=0;
    int n_line, n_column;

    I = RelativeOffset(offset);

    n_line = I/COLUMNS_COUNT;
    n_column = I - COLUMNS_COUNT*n_line;
    
    item.y = GRID_Y + n_line*LINE_H;
    item.x = GRID_X + n_column*COLUMN_W;

    return(item);
}

void DispObject_OnRefresh(DISP_OBJ *db)
{
  DispObject_InvalidateRect(db, 0);
}

int fade(int color, int speed)
{
  int New_AlphaChannel = (((color & 0xFF000000)>>24) - speed);
  if(New_AlphaChannel <= 0) return(0);
  int New_Color = (New_AlphaChannel<<24) | ((color<<8)>>8);
  return(New_Color);
}

void DrawAbout()
{
  int offs_text = GetImageHeight(' ')+4;
  DrawTextS(BOOKNAME, 0, 20, ALIGN_CENTER);
  DrawTextS(AUTHOR, 0, 20+offs_text, ALIGN_CENTER);
  DrawTextS(VERSION, 0, 20+offs_text*2, ALIGN_CENTER);
}

int ColorInversion(int color)
{
  int A = color>>24;
  int R = 0xFF - ((color<<8)>>24);
  int G = 0xFF - ((color<<16)>>24);
  int B = 0xFF - ((color<<24)>>24);
  return((A<<24)|(R<<16)|(G<<8)|B);
}

// рисуем фон, сетку, им€ файла и адреса сверху (смещени€)
void MyGuiOnRedraw_0(DISP_OBJ_HE *db, int, int, int)
{
  SetFont(HEADER_FONT);
  
  // фон
  SetFillColor(BACKGROUND_COLOR); //цвет заливки пр€моугольника
  FillRect(0, 0, w, h);
  
  if(open_mode != NOT_OPEN)
  {
    int adr_minor=0;
    int x=0, y=0;
    wchar_t hex[16];
    
    // сетка
    SetForegroundColor(GRID_COLOR);
    
    for(y=GRID_Y-LINE_H; y < h-LINE_H; y += LINE_H)
      DrawLine(0, y, w, y);
    
    for(x=GRID_X; x < w; x += COLUMN_W)
      DrawLine(x, GRID_Y-LINE_H, x, y-LINE_H);
    
    // им€ открытого файла
    SetForegroundColor(HEADER_COLOR);
    
    switch(open_mode)
    {
    case OPEN_FILE:
      DrawText( SplitPath(file_o, SPLASH), HEADER_X, HEADER_Y, ALIGN_LEFT );
      break;
      
    case OPEN_RAM:
      DrawText( L"RAM", HEADER_X, HEADER_Y, ALIGN_LEFT );
      break;
      
    default:
      break;
    }
    
    SetFont(COLUMN_FONT);
    
    // адреса сверху
    SetForegroundColor(COLUMN_COLOR);
    for(x=0; x < COLUMNS_COUNT; x++)
    {
        //dec2hex( adr_minor, hex, 2 );
        snwprintf(hex, 2, L"%02X", adr_minor);
        DrawText(hex, GRID_X + (COLUMN_W*x)+1, GRID_Y-(LINE_H-1), ALIGN_LEFT);
        adr_minor++;
    }
  }
  else
  {
    SetFont(DATA_FONT);
    if(about_visible)
    {
      SetForegroundColor(ColorInversion(BACKGROUND_COLOR)); //цвет текста about
      DrawAbout();
      about_visible = false;
      DISP_DESC_SetOnRefresh(DispObject_GetDESC((DISP_OBJ *)db), DispObject_OnRefresh);
      DispObject_SetRefreshTimer((DISP_OBJ *)db, 2000);
    }
    else if(DispObject_GetonRefresh((DISP_OBJ *)db))
    {
      int New_ForegroundColor = fade(TextColor, 80);
      
      if(!New_ForegroundColor )
      {
        TextColor = clBlack;
        DISP_DESC_SetOnRefresh(DispObject_GetDESC((DISP_OBJ *)db), NULL);
        DispObject_KillRefreshTimer((DISP_OBJ *)db);
        return;
      }
      SetForegroundColor(New_ForegroundColor); //цвет текста
      DrawAbout();
      TextColor = New_ForegroundColor;
      DispObject_SetRefreshTimer((DISP_OBJ *)db, 1);
    }
  }
}

//рисуем информацию вида: L"s=%db; page=%d/%d; %s"
void MyGuiOnRedraw_1(DISP_OBJ_HE *db, int, int, int)
{
  if(open_mode != NOT_OPEN)
  {
    int adr_major=0;
    wchar_t info[64];
    
    SetFont(INFO_FONT);
    
    // информаци€
    SetForegroundColor(INFO_COLOR);
    //SetBackgroundColor(0xFFFFFFFF);
    info[0]=0;
    
    adr_major = (offset/CELLS_COUNT)*CELLS_COUNT;
    
    char *mode = GetMode();
    
    if(open_mode == OPEN_FILE)
    {
      // инфо
      snwprintf(info,
                0xFF,
                L"s=%db; page=%d/%d; %s",
                file_size,
                adr_major/CELLS_COUNT+1,
                (file_size%CELLS_COUNT > 0)?(file_size/CELLS_COUNT+1):(file_size/CELLS_COUNT),
                mode);
    }
    else snwprintf(info, 0xFF, L"%s", mode);
    
    DrawText(info, 1, h-LINE_H-1, ALIGN_LEFT);
  }
}

// рисуем текцщий оффсет и выделенную €чейку
void MyGuiOnRedraw_2(DISP_OBJ_HE *db, int, int, int)
{
  if(open_mode != NOT_OPEN)
  {
    POINT item;
    wchar_t hex[16];
    
    SetFont(OFFSET_FONT);
    
    // текущий оффсет
    SetForegroundColor(OFFSET_COLOR);
    //dec2hex( offset, hex, 8 );
    snwprintf(hex, 8, L"%08X", offset);
    DrawText(hex, 1, GRID_Y - (LINE_H-1), ALIGN_LEFT);
    
    // выделенна€ €чейка
    SetFillColor(HL_CELL_COLOR);
    item = XY(offset);
    FillRect(item.x+1, item.y+1, COLUMN_W-1, LINE_H-1);
  }
}

// адреса слева
void MyGuiOnRedraw_3(DISP_OBJ_HE *db, int, int, int)
{
  if(open_mode != NOT_OPEN)
  {
    wchar_t hex[16];
    int adr_major = (offset/CELLS_COUNT)*CELLS_COUNT;
    
    SetFont(ADDRESS_FONT);
    
    // адреса слева
    SetForegroundColor(ADDRESS_COLOR);
    //SetBackgroundColor(0xFFFFFFFF); 
    for(int y=0; y < LINES_COUNT; y++ )
    {
      snwprintf(hex, 8, L"%08X", adr_major);
      DrawText(hex, 1, GRID_Y + (LINE_H*y)+1, ALIGN_LEFT);
      adr_major += COLUMNS_COUNT;
    }
  }
}

// сами данные €чеек
void MyGuiOnRedraw_4(DISP_OBJ_HE *db, int, int, int)
{
  if(open_mode != NOT_OPEN)
  {
    int addres=0;
    int x=0, y=0;
    wchar_t hex[16];
    
    SetFont(DATA_FONT);
    
    // заполн€ем данными
    SetForegroundColor(DATA_COLOR);
    //SetBackgroundColor(0xFFFFFFFF);
        
    for(int i=0; i < CELLS_COUNT; i++)
    {
      if(open_mode == OPEN_FILE && addres+i >= file_size) break;
      
      char value = buffer[i];
      
      for(int j=0; j<corrections_list->FirstFree; j++)
      {
        HE_CORRECTION *c = (HE_CORRECTION *)List_Get(corrections_list, j);
        if(c->c_offset == (((page-1)*CELLS_COUNT)+i))
        {
          value = c->correction;
          SetForegroundColor(DATA_CORRECT_COLOR);
          break;
        }
      }
      
      switch(view_mode)
      {
      case HEX_MODE:
        snwprintf(hex, 2, L"%02X", value);
        break;
        
      case DEC_MODE:
        snwprintf(hex, 0xFF, L"%03d", value);
        break;
      
      case TEXT_MODE:
        hex[0] = value;
        hex[1] = 0;
        break;
      }
            
      TEXTID id = TextID_Create(hex, ENC_UCS2, TEXTID_ANY_LEN);
      int coord_x = (COLUMN_W - Disp_GetTextIDWidth(id, wstrlen(hex)))/2;
      TextID_Destroy(id);
      
      DrawText(hex, GRID_X+(COLUMN_W*x)+coord_x-1 , GRID_Y + (LINE_H*y)+1, ALIGN_LEFT);
      
      SetForegroundColor(DATA_COLOR);
      
      if(x < COLUMNS_COUNT-1) x++;
      else
      {
        x = 0;
        y++;
      }
    }
  }
}

int Y_ITEMMENU(int item)
{
  int mi_visible = HexEditorMenu_GetNumItemsViseble();
  
  int text_h = GetImageHeight(' ') + INDENT_H;
  int h_m = h - (text_h * mi_visible);
  int y = h_m + text_h*item;
  return(y);
}

void MyGuiOnRedraw_5(DISP_OBJ_HE *db, int, int, int)
{
  if(is_menu)
  {
    SetFont(MENU_FONT);
    
    int h_menu = 0;
    int w_menu = 0;
    
    int x = INDENT_W, y = h-(GetImageHeight(' ')+INDENT_H);
    
    for(int i=MENU_MAX; i>=0; i--)
    {
      if(HexEditorMenu_Item_GetVisible(i))
      {
        TEXTID id = Lang_GetTextID((LNG_RES_ID_T)i);
        int w_ = Disp_GetTextIDWidth(id, wstrlen(RecLang((LNG_RES_ID_T)i)))+INDENT_W*2;
        if(w_ > w_menu) w_menu = w_;
        TextID_Destroy(id);
        h_menu += GetImageHeight(' ')+INDENT_H;
      }
    }
    
    h_menu += INDENT_H;
    
    SetFillColor(MENU_BG_COLOR);
    FillRect(0, h-h_menu, w_menu, h_menu);
    
    for(int i=MENU_MAX; i>=0; i--)
    {
      if(HexEditorMenu_Item_GetVisible(i))
      {
        if(HexEditorMenu_Item_GetEnable(i))
          SetForegroundColor(MENU_ENABLE_TX_COLOR);
        else
          SetForegroundColor(MENU_N_ENABLE_TX_COLOR);
        
        DrawText(RecLang((LNG_RES_ID_T)i), x, y, ALIGN_LEFT);
        y -= GetImageHeight(' ')+INDENT_H;
      }
    }
    
    SetFillColor(MENU_CUR_COLOR);
    y = Y_ITEMMENU(menu_item);
    FillRect(0, y, w_menu, GetImageHeight(' '));
  }
}

void DL_FsReadFile(void *buffer, int, int size, int file_handle, int *read)
{
  memset(buffer, 0, CELLS_COUNT); 
  *read = w_fread(file_handle, buffer, size);
}

int LoadBuffer(int off_set)
{
    int r;
    switch(open_mode)
    {
    case OPEN_FILE:
        if(f == FILE_HANDLE_INVALID) return(RESULT_FAIL);
        
        w_lseek( f, (off_set/CELLS_COUNT)*CELLS_COUNT, WSEEK_SET );
        if(file_size < CELLS_COUNT)
          DL_FsReadFile( buffer,  1,  file_size, f, &r );
        else
          DL_FsReadFile( buffer,  1,  CELLS_COUNT, f, &r );
        break;

    case OPEN_RAM:
        int err = 0;
        for(int j=0;; j++)
        {
          OSBOOLEAN i = get_mem(current_process(), off_set, buffer, CELLS_COUNT);
          //debug_printf("\n!!!(LoadBufferRam)\n");
          //debug_printf("\n0x%X = get_mem(0x%X, 0x%X, 0x%X, 0x%X)",
          //             i,
          //             current_process(),
          //             off_set,
          //             buffer,
          //             CELLS_COUNT);
          if(!i)
          {
            if(j) err = -1;
            break;
          }
          off_set++;
        }
        
        //debug_printf("\n!!!(LoadBufferRam) off_set = 0x%X\n", off_set);
        
        //debug_printf("\n!!!(LoadBufferRam) err = 0x%X\n", err);
        
        if(err)
          MessageBox(EMPTY_TEXTID, STR("Ќедоступно! Ѕлижайший адрес установлен."), NOIMAGE, 1, 0, 0);
        
        r = CELLS_COUNT;
        break;
    }

    ////debug_printf("\nRead %d b from %d b\n", r, CELLS_COUNT);
    return(r);
}

int SaveOffset(int off_set, char value)
{
    //debug_printf("\nSave offset 0x%x, new value = 0x%x\n", off_set, value);

    buffer[RelativeOffset(off_set)] = value;

    switch(open_mode)
    {
    case OPEN_FILE:
        w_lseek(f, off_set, WSEEK_SET);
        w_fwrite(f, &value, 1);
        //debug_printf("\n!! w_fwrite new value\n");
        break;
    } 

    return(RESULT_OK);
}

int GoTo(int off_set) 
{
    ////debug_printf("\nGoTo 0x%x\n", off_set);
    offset = off_set;
    LoadBuffer(off_set);
    return(RESULT_OK);
}

void Redraw_Update(int r0, int r1, int r2, int r3, int r4, int r5)
{
  int list[NUM_OF_GUI];
  list[0] = r0; list[1] = r1;
  list[2] = r2; list[3] = r3; list[4] = r4; list[5] = r5;
  for(int i=0; i<NUM_OF_GUI; i++)
  {
    if(list[i])
    {
      DISP_OBJ *disp = GUIObject_GetDispObject(gui_list[i]);
      DispObject_InvalidateRect(disp, 0);
    }
  }
}

int Redraw_UpdateWithPage(int r0, int r1, int r2, int r3, int r4, int r5)
{
  if(page != ((offset/CELLS_COUNT)+1))
  {
    page = (offset/CELLS_COUNT)+1;
    Redraw_Update(r0, r1, r2, r3, r4, r5);
    return(1);
  }
  return(0);
}

void HexEditorMenu_SelectItem(int item)
{
  switch(item)
  {
  case MENU_FILE_OPEN:
    HexEditor_FileOpen();
    break;
    
  case MENU_RAM_OPEN:
    HexEditor_OpenRAM();
    break;
    
  case MENU_FIND:
    HexEditor_Find();
    break;
    
  case MENU_GOTO_OFFSET:
    HexEditor_GoToOffset();
    break;
    
  case MENU_GOTO_PAGE:
    HexEditor_GoToPage();
    break;
    
  case MENU_SETTINGS:
    elfload(PATH_BCFGEDIT,
                (void *)successed_config_path,
                (void *)successed_config_name,
                NULL);
    break;
    
  case MENU_SAVE_FILE:
    HexEditor_SaveFile();
    break;
    
  case MENU_CLOSE_FILE:
    HexEditor_CloseFile();
    break;
    
  case MENU_CLOSE_VIEW_RAM:
    HexEditor_CloseViewRAM();
    break;
    
  case MENU_EXIT:
    CloseHexEditorBook(Find_HexEditorBook(), NULL);
    break;
  }
  TextColor = NULL;
}

int MenuItem_GetItemIfNoVisible()
{
  int i = 0;
  for(int k=0; (k <= menu_item) && (i <= MENU_MAX); k++)
  {
    for(;;) if(HexEditorMenu_Item_GetVisible(i++)) break;
  }
  i--;
  return(i);
}

void HexEditor_ActionEdit(BOOK *book, GUI *gui)
{
  if(!is_menu && (open_mode != NOT_OPEN)) HexEditor_Edit();
  else if(is_menu)
  {
    int i = MenuItem_GetItemIfNoVisible();
    
    if(HexEditorMenu_Item_GetEnable(i))
    {
      HexEditorMenu_SelectItem(i);
      is_menu = false;
    }
    Redraw_Update(false, false, false, false, false, true);
  }
}

void HexEditor_ShowMenu(BOOK *book, GUI *gui)
{
  menu_item = MENU_FILE_OPEN;
  if(!is_menu)
  {
    if(wstrlen(PATH_BCFGEDIT) > 0)
      HexEditorMenu_Item_SetEnable(MENU_SETTINGS, true);
    else
      HexEditorMenu_Item_SetEnable(MENU_SETTINGS, false);
    is_menu = true;
  }
  else is_menu = false;
  Redraw_Update(false, false, false, false, false, true);
}

void HexEditor_HideMenu(BOOK *book, GUI *gui)
{
  if(is_menu) is_menu = false;
  Redraw_Update(false, false, false, false, false, true);
}

void Base_OnKey(int key, int repeat, int type)
{
  if(type == KBD_SHORT_PRESS)
  {
    if((GetChipID()&CHIPID_MASK) != CHIPID_DB2020)
    {
      if(key == KEY_ENTER)
        HexEditor_ActionEdit(NULL, NULL);
      
      else if(key == KEY_LEFT_SOFT)
        HexEditor_ShowMenu(NULL, NULL);
      
      else if(key == KEY_RIGHT_SOFT)
        HexEditor_HideMenu(NULL, NULL);
    }
  }
}

void Main_OnKey(int key, int repeat, int type)
{
  int off_set;
  
  if(type == KBD_SHORT_PRESS)
  {
    off_set = offset;
    
    if(key == KEY_DIGITAL_0)
    {
      int view = view_mode;
      if(view < MAX_MODE) view++;
      else view = HEX_MODE;
      view_mode = (VIEW_MODES_T)view;
      
      FREE(find_text);
      Redraw_Update(false, true, false, false, true, false);
    }
    else if(key == KEY_DIGITAL_0+1)
    {
      HexEditor_GoToOffset();
    }
    else if(key == KEY_DIGITAL_0+3)
    {
      HexEditor_Find();
    }
    else if(key == KEY_DIGITAL_0+5) // || key == KEY_ENTER) // || key == KEY_RIGHT_SOFT
    {
      HexEditor_ActionEdit(NULL, NULL);
    }
    else if(key == KEY_DIGITAL_0+7)
    {
      HexEditor_OpenRAM();
    }
    else if(key == KEY_DIGITAL_0+9)
    {
      if(wstrlen(PATH_BCFGEDIT) > 0)
      {
        elfload(PATH_BCFGEDIT,
                (void *)successed_config_path,
                (void *)successed_config_name,
                NULL);
      }
    }
    else if(key == KEY_DIEZ)
    {
      if(open_mode == OPEN_RAM) offset += CELLS_COUNT;
      else GoTo((offset+CELLS_COUNT<file_size)?(offset+CELLS_COUNT):0);
      Redraw_UpdateWithPage(false, true, true, true, true, false);
    }
    else if(key == KEY_STAR)
    {
      if(open_mode == OPEN_RAM) offset -= CELLS_COUNT;
      else GoTo((offset-CELLS_COUNT>=0)?(offset-CELLS_COUNT):(file_size-1));
      Redraw_UpdateWithPage(false, true, true, true, true, false);
    }
    else if(key == KEY_DEL)
    {
      for(int j=0; j<corrections_list->FirstFree; j++)
      {
        HE_CORRECTION *c = (HE_CORRECTION *)List_Get(corrections_list, j);
        if(c->c_offset == offset)
        {
          mfree(c);
          List_RemoveAt(corrections_list, j);
          Redraw_Update(false, false, false, false, true, false);
          break;
        }
      }
      if(!corrections_list->FirstFree)
        HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
    }
  }
    
  if(type == KBD_SHORT_PRESS || type == KBD_REPEAT)
  {
    if(key == KEY_DIGITAL_0+2 || key == KEY_UP)
    {
      if(offset-COLUMNS_COUNT >= 0) offset -= COLUMNS_COUNT;
      if(Redraw_UpdateWithPage(false, true, true, true, true, false));
      else Redraw_Update(false, false, true, false, false, false);
    }
    else if(key == KEY_DIGITAL_0+4 || key == KEY_LEFT)
    {
      if(offset > 0) offset--;
      if(Redraw_UpdateWithPage(false, true, true, true, true, false));
      else Redraw_Update(false, false, true, false, false, false);
    }
    else if(key == KEY_DIGITAL_0+6 || key == KEY_RIGHT)
    {
      if(open_mode == OPEN_RAM) offset++;
      else if(offset+1 < file_size) offset++;
      if(Redraw_UpdateWithPage(false, true, true, true, true, false));
      else Redraw_Update(false, false, true, false, false, false);
    }
    else if(key == KEY_DIGITAL_0+8 || key == KEY_DOWN)
    {
      if(open_mode == OPEN_RAM) offset += COLUMNS_COUNT;
      else if(offset+COLUMNS_COUNT < file_size) offset += COLUMNS_COUNT;
      if(Redraw_UpdateWithPage(false, true, true, true, true, false));
      else Redraw_Update(false, false, true, false, false, false);
    }
    
    if((off_set/CELLS_COUNT) != (offset/CELLS_COUNT)) LoadBuffer(offset);
  }
}

int HexEditorMenu_GetNumItemsViseble(void)
{
  int mi_visible = 1;
  for(int i=0; i<MENU_MAX; i++) mi_visible += HexEditorMenu_Item_GetVisible(i);
  return(mi_visible);
}

void Menu_OnKey(int key, int repeat, int type)
{
  if((type == KBD_SHORT_PRESS) && (1<=(key-KEY_DIGITAL_0)) && ((key-KEY_DIGITAL_0)<=9))
  {
    int mi_visible = HexEditorMenu_GetNumItemsViseble();
    
    if(key == KEY_DIGITAL_0+1 && mi_visible >= 1)
    {
      if(mi_visible >= 1) menu_item = MENU_FILE_OPEN;
      else return;
    }
    if(key == KEY_DIGITAL_0+2 && mi_visible >= 2)
    {
      if(mi_visible >= 2) menu_item = MENU_RAM_OPEN;
      else return;
    }
    if(key == KEY_DIGITAL_0+3 && mi_visible >= 3)
    {
      if(mi_visible >= 3) menu_item = MENU_FIND;
      else return;
    }
    if(key == KEY_DIGITAL_0+4 && mi_visible >= 4)
    {
      if(mi_visible >= 4) menu_item = MENU_GOTO_OFFSET;
      else return;
    }
    if(key == KEY_DIGITAL_0+5)
    {
      if(mi_visible >= 5) menu_item = MENU_GOTO_PAGE;
      else return;
    }
    if(key == KEY_DIGITAL_0+6)
    {
      if(mi_visible >= 6) menu_item = MENU_SETTINGS;
      else return;
    }
    if(key == KEY_DIGITAL_0+7)
    {
      if(mi_visible >= 7) menu_item = MENU_SAVE_FILE;
      else return;
    }
    if(key == KEY_DIGITAL_0+8)
    {
      if(mi_visible >= 8) menu_item = MENU_CLOSE_FILE;
      else return;
    }
    if(key == KEY_DIGITAL_0+9)
    {
      if(mi_visible >= 9) menu_item = MENU_EXIT;
      else return;
    }
    HexEditor_ActionEdit(NULL, NULL);
  }
  
  if(type == KBD_SHORT_PRESS || type == KBD_REPEAT)
  {
    if(key == KEY_UP || key == KEY_DOWN)
    {
      int mi_visible = HexEditorMenu_GetNumItemsViseble()-1;
      
      if(key == KEY_UP)
      {
        if(menu_item) menu_item--;
        else menu_item = mi_visible;
        Redraw_Update(false, false, false, false, false, true);
      }
      else if(key == KEY_DOWN)
      {
        if(menu_item != mi_visible) menu_item++;
        else menu_item = 0;
        Redraw_Update(false, false, false, false, false, true);
      }
    }
  }
}

void MyGuiOnKey_0(DISP_OBJ_HE *db, int key, int, int repeat, int type)
{
    Base_OnKey(key, repeat, type);
  
  if(!is_menu && (open_mode != NOT_OPEN))
    Main_OnKey(key, repeat, type);
  
  if(is_menu)
    Menu_OnKey(key, repeat, type);
  
    /*
    else if(key == KEY_LEFT_SOFT)
    {
      if(!is_menu)
      {
        if(wstrlen(PATH_BCFGEDIT) > 0) HexEditorMenu_Item_SetEnable(MENU_SETTINGS, true);
        else HexEditorMenu_Item_SetEnable(MENU_SETTINGS, false);
        is_menu = true;
      }
      else is_menu = false;
      Redraw_Update(false, false, false, false, false, true);
    }
    else if(key == KEY_RIGHT_SOFT)
    {
      if(is_menu) is_menu = false;
      Redraw_Update(false, false, false, false, false, true);
    }
    */
}

void MyGuiOnKey_1(DISP_OBJ_HE *db, int key, int, int repeat, int type)
{
}

void MyGuiOnKey_5(DISP_OBJ_HE *db, int key, int, int repeat, int type)
{
}

void gui_constr_x(DISP_DESC *desc,
                  char const *name,
                  int size,
                  DISP_OBJ_ONCREATE_METHOD OnCreate,
                  DISP_OBJ_ONCLOSE_METHOD OnClose,
                  DISP_OBJ_ONREDRAW_METHOD OnRedraw,
                  DISP_OBJ_ONKEY_METHOD OnKey)
{
  DISP_DESC_SetName(desc, name);
  DISP_DESC_SetSize(desc, size);
  DISP_DESC_SetOnCreate(desc, OnCreate);
  DISP_DESC_SetOnClose(desc, OnClose);
  DISP_DESC_SetOnRedraw(desc, OnRedraw);
  DISP_DESC_SetOnKey(desc, OnKey);
}

void MyGui_constr_0(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_0, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_0,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_0,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_0);
}

void MyGui_constr_1(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_1, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_1,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_1,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_1);
}

void MyGui_constr_2(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_2, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_1,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_2,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_1);
}

void MyGui_constr_3(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_3, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_1,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_3,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_1);
}

void MyGui_constr_4(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_4, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_1,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_4,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_1);
}

void MyGui_constr_5(DISP_DESC *desc)
{
  gui_constr_x(desc, GUINAME_5, sizeof(DISP_OBJ_HE),
               (DISP_OBJ_ONCREATE_METHOD)MyGuiOnCreate_5,
               (DISP_OBJ_ONCLOSE_METHOD)MyGuiOnClose,
               (DISP_OBJ_ONREDRAW_METHOD)MyGuiOnRedraw_5,
               (DISP_OBJ_ONKEY_METHOD)MyGuiOnKey_5);
}

void MyGui_destr(GUI *)
{
}

void CloseHexEditorBook(BOOK *book, GUI *gui)
{
  for(int i=0; i<NUM_OF_GUI; i++)
  {
    gui = gui_list[i];
    //char *s = DispObject_GetName(GUIObject_GetDispObject(gui));
    //debug_printf("\n<<<<< gui_name = %s\n", s);
    GUIObject_Destroy(gui);
    //debug_printf("\n<<<<< gui(%s) destroy\n", s);
  }
  FreeBook(book);
}

typedef void (*GD)(GUI *);
typedef void (*DDC)(DISP_DESC *);

DDC gui_constr[NUM_OF_GUI] =
{
  MyGui_constr_0,
  MyGui_constr_1,
  MyGui_constr_2,
  MyGui_constr_3,
  MyGui_constr_4,
  MyGui_constr_5
};

GUI *CreateBaseGUI(BOOK *book)
{
  GUI *gui = new GUI;
  if(!GUIObject_Create(gui, MyGui_destr, gui_constr[0], book, 0, 0, 0)) mfree(gui);
  if(book) BookObj_AddGUIObject(book, gui);
  GUIObject_SetStyle(gui, UI_OverlayStyle_TrueFullScreen);
  GUIObject_SetTitleType(gui, UI_TitleMode_None);
  GUIObject_SoftKeys_Hide(gui);
  GUIObject_SoftKeys_SetAction(gui, ACTION_LONG_BACK, CloseHexEditorBook);
  if((GetChipID()&CHIPID_MASK) == CHIPID_DB2020)
  {
    GUIObject_SoftKeys_SetAction(gui, HE_ACTION_EDIT, HexEditor_ActionEdit);
    GUIObject_SoftKeys_SetAction(gui, HE_ACTION_MENU, HexEditor_ShowMenu);
    GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, HexEditor_HideMenu);
  }
  GUIObject_Show(gui);
  return(gui);
}

GUI *CreateSecGUI(BOOK *book, int num)
{
  GUI *gui = new GUI;
  if(!GUIObject_Create(gui, MyGui_destr, gui_constr[num], book, 0, 0, 0)) mfree(gui);
  if(book) BookObj_AddGUIObject(book, gui);
  GUIObject_SetStyle(gui, UI_OverlayStyle_TrueFullScreen);
  GUIObject_SetTitleType(gui, UI_TitleMode_None);
  GUIObject_SoftKeys_Hide(gui);
  
  DISP_OBJ *disp = GUIObject_GetDispObject(gui);
  DispObject_SetLayerColor(disp, 0x0);
  DispObject_Show(disp, true);
  return(gui);
}

int CreateHexEditorGUI(void *mess, BOOK *book)
{
  GUI *gui;
  for(int i=0; i<NUM_OF_GUI; i++)
  {
    //gui = BookObj_GetGUIObject(book, i);
    //if(gui) GUIObject_Destroy(gui);
    
    if(!i) gui = CreateBaseGUI(book);
    else gui = CreateSecGUI(book, i);
    gui_list[i] = gui;
    
    //char *s = DispObject_GetName(GUIObject_GetDispObject(gui));
    //debug_printf("\n<<<<< create gui(%s)\n", s);
  }
  return(1);
}

int HexEditor_OnExit(void *mess, BOOK *book)
{
  CloseHexEditorBook(book, 0);
  return(1);
}

void HexEditor_OnClose(BOOK *book)
{
  if(f != FILE_HANDLE_INVALID) w_fclose(f);
  CloseLang();
  if(corrections_list)
  {
    if(corrections_list->FirstFree)
    {
      for(int i=(corrections_list->FirstFree-1);i>=0;i--) 
      {
        mfree(List_Get(corrections_list,i));
        List_RemoveAt(corrections_list,i);
      }
    }
    List_Destroy(corrections_list);
  }
  if(find_text) mfree(find_text);
  if(file_o) mfree(file_o);
  //if(CFGfile) mfree(CFGfile);
  if(buffer) mfree(buffer);
  mfree(book_name);
  mfree(elf_name);
  List_Destroy(menu_items);
  SUBPROC(elf_exit);
}

int HexEditorBook_GetNumWindows(void)
{
  int i = 1;
  book_name = new char[13+6+1];
  for( ; ; i++)
  {
    sprintf(book_name, "%s_(w%d)", BOOKNAME, i);
    if(!Find_HexEditorBook()) break;
  }
  mfree(book_name);
  return(i);
}

BOOK *CreateHexEditorBook()
{
  BOOK *book = new BOOK;
  memset(book, 0, sizeof(BOOK));
  
  int num = HexEditorBook_GetNumWindows();
  
  book_name = new char[13+6+1];
  
  sprintf(book_name, "%s_(w%d)", BOOKNAME, num);
  if(!CreateBook(book, HexEditor_OnClose, &HexEditor_BaseDesc, book_name, -1, 0))
  {
    mfree(book_name);
    mfree(book);
    return(0);
  }
  return(book);
}

int OpenFile(wchar_t *file_open)
{
    ////debug_printf("\nOpen file\n");
    
    open_mode = OPEN_FILE;
    
    if(f != FILE_HANDLE_INVALID) w_fclose(f);
    f = w_fopen(file_open, WA_Read | WA_Write, 0x1FF, 0);
    if(f == FILE_HANDLE_INVALID)
    {
      open_mode = NOT_OPEN;
      return(RESULT_FAIL);
    }
    //wstrcpy(file_o, file_open);
    
    offset = 0;

    W_FSTAT w_fst;
    w_fstat(file_open, &w_fst);
    file_size = w_fst.st_size;
    
    if(file_size == 0)
    {
      open_mode = NOT_OPEN;
      w_fclose(f);
      f = FILE_HANDLE_INVALID;
      return(RESULT_FAIL);
    }
    
    ////debug_printf("\nfile_size = %d\n", file_size);

    if(!buffer) buffer = (char *)malloc(CELLS_COUNT);
    memset(buffer, 0, CELLS_COUNT); 
    
    HexEditorMenu_Item_SetVisible(MENU_FIND, true);
    HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, true);
    HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, true);
    HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, true);
    HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, true);
    HexEditorMenu_Item_SetVisible(MENU_CLOSE_VIEW_RAM, false);

    //SaveSetting(CFGfile);

    return(RESULT_OK);
}

int Register(wchar_t* file_open)
{
    //int status = RESULT_OK;
    
    // читаем ленг
    //wstrcpy(file, file_open);
    //wstrcpy(CFGfile, file_open);
    //CFGfile[wstrlen(file_open)-3] = 0;
    //wstrcat(CFGfile, L"lng");	
    

    // читаем конфиг
    //CFGfile[wstrlen(file_open)-3] = 0;
    //wstrcat(CFGfile, L"cfg");

    
    //if(file_open)
    {
        OpenFile(file_open);
        LoadBuffer(0);
    }
    //else
    //{
    //   // ReadSetting(CFGfile);
    //    switch(open_mode)
    //    {
    //    case OPEN_FILE:
    //        OpenFile(file_open);
    //        LoadBuffer(0);
    //        break;
    //
    //    case OPEN_RAM:
    //        //OpenRAM(offset);
    //        break;
    //    }
    //}
    return(1);
}

wchar_t *MakeFullName(wchar_t *path, wchar_t *name)
{
  int len = wstrlen(path)+wstrlen(name)+1;
  wchar_t *fullname = new wchar_t[len+1];
  memset(fullname, 0, (len+1)*2);
  wstrcpy(fullname, path);
  wstrcat(fullname, SPLASH);
  wstrcat(fullname, name);
  return(fullname);
}

int HexEditor_DB_OnCancel(void *mess ,BOOK *book)
{
  //FreeBook(book);
  UnLoadDLL(dll_data);
  return(1);
}

int HexEditor_DB_OnAccept(void *mess, BOOK *book)
{
  FILEITEM *fi = (FILEITEM *)mess;
  
  if(open_mode == NEW_WINDOW)
  {
    open_mode = OPEN_FILE;
    elfload(elf_name, FILEITEM_GetPath(fi), FILEITEM_GetFname(fi), NULL);
    UnLoadDLL(dll_data);
    return(1);
  }
  
  if(open_mode != NOT_OPEN) CloseFile();
  
  if(file_o) mfree(file_o);
  
  file_o = MakeFullName(FILEITEM_GetPath(fi), FILEITEM_GetFname(fi));
  Register(file_o);
  UnLoadDLL(dll_data);
  
  if(open_mode != NOT_OPEN)
  {
    HexEditorMenu_Item_SetVisible(MENU_FIND, true);
    HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, true);
    //HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, false);
    HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, true);
    HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
    HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, true);
  }
  
  Redraw_Update(true, true, true, true, true, false);
  //BookObj_GotoPage(book, &HexEditor_MainDesc);
  return(1);
}

void Find_ActionBack(BOOK *book, GUI *gui)
{
  GUIObject_Destroy(gui);
}

void Find_ActionOk(BOOK *book, wchar_t *txt, int len)
{
  if(len)
  {
    if(!IllegalSymbol(txt, len) || view_mode == TEXT_MODE)
    {
      char buf[256];
      
      int shift = len/2;
      
      if((find_text && wstrcmp(txt, find_text)) || !find_text)
      {
        if(find_text) mfree(find_text);
        else shift = 0;
        find_text = new wchar_t[len+1];
        wstrcpy(find_text, txt);
        find_text[len] = 0;
      }
      
      if(view_mode == TEXT_MODE && shift) shift = len;
      
      Data2Pattern(find_text, buf);
      int err = FindPattern(buf, offset+shift);
      GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
      if(Redraw_UpdateWithPage(false, true, true, true, true, false));
      else Redraw_Update(false, false, true, false, false, false);
      if(err < 0)
        MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_NOT_FOUNT), NOIMAGE, 1, 0, 0);
    }
    else
      MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ILLEGAL_SYMB), NOIMAGE, 1, 0, 0);
  }
  else
    MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ENTER_TEXT), NOIMAGE, 1, 0, 0);
}

int HexEditor_Find(void)
{
  BOOK *book = Find_HexEditorBook();
  int input_mode = IT_STRING;
  char *mode = GetMode();
  
  if(view_mode == DEC_MODE) input_mode = IT_REAL;
  else if(view_mode == HEX_MODE) input_mode = IT_ABC_AND_DIGIT;
  
  GUI *gui = CreateStringInputVA(0,
                             VAR_BOOK(book),
                             VAR_HEADER_TEXT(TextID_Create(mode, ENC_LAT1, strlen(mode))),
                             VAR_STRINP_TEXT(TextID_Create(find_text, ENC_UCS2, TEXTID_ANY_LEN)),
                             VAR_STRINP_MODE(input_mode),
                             VAR_OK_PROC(Find_ActionOk),
                             0);
  GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
  return(1);
}

void wtoh( wchar_t *wstr, int len , int *r)
{
  wchar_t s;
  int res=0;
  while(len--)
  {
    s=*wstr++;
    if ((s>='0') && (s<='9')) s -= '0';
    else if((s>='A') && (s<='F')) s -= '7';
    else if((s>='a') && (s<='f')) s -= ('7' + ' ');
    res<<=4;
    res|=s;
  }
  *r = res;
}

void Edit_ActionOk(BOOK *book, wchar_t *txt, int len)
{
  if(len)
  {
    if(!IllegalSymbol(txt, len) || view_mode == TEXT_MODE)
    {
      int value = 0;
      switch(view_mode)
      {
      case HEX_MODE:
        wtoh(txt, len, &value);
        break;

      case DEC_MODE:
        wtoi(txt, len, &value);
        break;
      
      case TEXT_MODE:
        UCS2toCP1251(txt, (char *)&value);
        break;
      }
      
      HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, true);
      
      int rep = false;
      for(int j=0; j<corrections_list->FirstFree; j++)
      {
        HE_CORRECTION *c = (HE_CORRECTION *)List_Get(corrections_list, j);
        if(c->c_offset == offset)
        {
          if(c->real != value)
          {
            c->correction = (char)value;
            rep = true;
          }
          else
          {
            mfree(c);
            List_RemoveAt(corrections_list, j);
            HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
          }
          break;
        }
      }
      
      if(!rep)
      {
        if(buffer[RelativeOffset(offset)] != value)
        {
          HE_CORRECTION *c = new HE_CORRECTION;
          c->c_offset = offset;
          c->correction = (char)value;
          c->real = buffer[RelativeOffset(offset)];
          List_InsertFirst(corrections_list, c);
        }
        else HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
      }
      
      //SaveOffset(offset, value);
      GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
      Redraw_Update(false, false, false, false, true, false);
    }
    else
      MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ILLEGAL_SYMB), NOIMAGE, 1, 0, 0);
  }
  else
    MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ENTER_TEXT), NOIMAGE, 1, 0, 0);
}

int HexEditor_Edit(void)
{
  BOOK *book = Find_HexEditorBook();
  int num_chars, input_mode;
  wchar_t buf_offset[64];
  wchar_t buf[8];
  
  char value = buffer[RelativeOffset(offset)];
  
  for(int j=0; j<corrections_list->FirstFree; j++)
  {
    HE_CORRECTION *c = (HE_CORRECTION *)List_Get(corrections_list, j);
    if(c->c_offset == offset)
    {
      value = c->correction;
      break;
    }
  }
  
  switch(view_mode)
  {
    case HEX_MODE:
        num_chars = 2;
        input_mode = IT_ABC_AND_DIGIT;
        wstrcpy(buf_offset, L"HEX, 0x");
        snwprintf(buf, 2, L"%02X", value);
        break;

    case DEC_MODE:
        num_chars = 3;
        input_mode = IT_REAL; 
        wstrcpy(buf_offset, L"DEC, 0x");
        snwprintf(buf, 0xFF, L"%d", value);
        break;

    case TEXT_MODE:
        num_chars = 1;
        input_mode = IT_STRING;
        wstrcpy(buf_offset, L"TEXT, 0x");
        buf[0] = value;
        buf[1] = 0;
        break;
    }
  
  snwprintf(buf_offset + wstrlen(buf_offset), 8, L"%08X", offset);
  
  GUI *gui = CreateStringInputVA(0,
                             VAR_BOOK(book),
                             VAR_HEADER_TEXT(TextID_Create(buf_offset, ENC_UCS2, TEXTID_ANY_LEN)),
                             VAR_STRINP_TEXT(TextID_Create(buf, ENC_UCS2, TEXTID_ANY_LEN)),
                             VAR_STRINP_MODE(input_mode),
                             VAR_STRINP_MAX_LEN(num_chars),
                             VAR_OK_PROC(Edit_ActionOk),
                             0);
  GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
  return(1);
}

void GoToOffset_ActionOk(BOOK *book, wchar_t *txt, int len)
{
  if(len > 2)
  {
    txt += 2;
    len -= 2;
    if(!IllegalSymbol(txt, len))
    {
      int value;
      wtoh(txt, len, &value);
      if(file_size > value)
      {
        GoTo(value);
        GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
        Redraw_Update(false, true, true, true, true, false);
      }
      else
        MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_OFFSET_NOT_AV), NOIMAGE, 1, 0, 0);
    }
    else
      MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ILLEGAL_SYMB), NOIMAGE, 1, 0, 0);
  }
  else
    MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ENTER_TEXT), NOIMAGE, 1, 0, 0);
}

void GoToPage_ActionOk(BOOK *book, wchar_t *txt, int len)
{
  if(len > 0)
  {
    int cm = (file_size/CELLS_COUNT)+1;
    int value;
    wtoi(txt, len, &value);
    
    if(!(file_size%CELLS_COUNT)) cm--;
    
    if(cm >= value)
    {
      GoTo((value-1)*CELLS_COUNT);
      GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
      Redraw_Update(false, true, true, true, true, false);
    }
    else
      MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_OFFSET_NOT_AV), NOIMAGE, 1, 0, 0);
  }
}

int HexEditor_GoToPage(void)
{
  if(open_mode != OPEN_RAM)
  {
    GUI *gui = CreateStringInputVA(0,
                             VAR_BOOK(Find_HexEditorBook()),
                             VAR_HEADER_TEXT(Lang_GetTextID(LNG_GOTO_PAGE)),
                             //VAR_STRINP_TEXT(STR("0x")),
                             VAR_STRINP_MODE(IT_REAL),
                             VAR_STRINP_MAX_LEN(10),
                             VAR_OK_PROC(GoToPage_ActionOk),
                             0);
    GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
  }
  return(1);
}

int HexEditor_GoToOffset(void)
{
  if(open_mode != OPEN_RAM)
  {
    GUI *gui = CreateStringInputVA(0,
                             VAR_BOOK(Find_HexEditorBook()),
                             VAR_HEADER_TEXT(Lang_GetTextID(LNG_GOTO_OFFSET)),
                             VAR_STRINP_TEXT(STR("0x")),
                             VAR_STRINP_MODE(IT_ABC_AND_DIGIT),
                             VAR_STRINP_MAX_LEN(10),
                             VAR_OK_PROC(GoToOffset_ActionOk),
                             0);
    GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
  }
  return(1);
}

void OpenRAM_ActionOk(BOOK *book, wchar_t *txt, int len)
{
  if(len > 2)
  {
    txt += 2;
    len -= 2;
    if(!IllegalSymbol(txt, len))
    {
      if(open_mode != NOT_OPEN) CloseFile();
      
      if(!buffer) buffer = (char *)malloc(CELLS_COUNT);
      memset(buffer, 0, CELLS_COUNT);
      
      FREE(file_o);
      FREE(find_text);
      
      if(f != FILE_HANDLE_INVALID) w_fclose(f); 
      
      file_size = CELLS_COUNT;
      
      wtoh(txt, len, &offset);
      
      open_mode = OPEN_RAM;
      
      //debug_printf("\n!!! offset = 0x%X\n", offset);
      
      LoadBuffer(offset);
      
      HexEditorMenu_Item_SetVisible(MENU_FIND, true);
      HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, false);
      HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, false);
      HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, false);
      HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, false);
      HexEditorMenu_Item_SetVisible(MENU_CLOSE_VIEW_RAM, true);
      
      //HexEditorMenu_Item_SetEnable(MENU_GOTO_OFFSET, false);
      //HexEditorMenu_Item_SetEnable(MENU_GOTO_PAGE, false);
      //HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
      //HexEditorMenu_Item_SetEnable(MENU_CLOSE_VIEW_RAM, true);
      GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
      Redraw_Update(true, true, true, true, true, false);
    }
    else
      MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ILLEGAL_SYMB), NOIMAGE, 1, 0, 0);
  }
  else
    MessageBox(EMPTY_TEXTID, Lang_GetTextID(LNG_E_ENTER_TEXT), NOIMAGE, 1, 0, 0);
}

TEXTID Lang_GetTextID(LNG_RES_ID_T num)
{
  TEXTID id = TextID_Create(RecLang(num), ENC_UCS2, TEXTID_ANY_LEN);
  return(id);
}

void InputRamAddress(void)
{
  GUI *gui = CreateStringInputVA(0,
                             VAR_BOOK(Find_HexEditorBook()),
                             VAR_HEADER_TEXT(Lang_GetTextID(LNG_OPEN_RAM)),
                             VAR_STRINP_TEXT(STR("0x")),
                             VAR_STRINP_MODE(IT_ABC_AND_DIGIT),
                             VAR_STRINP_MAX_LEN(10),
                             VAR_OK_PROC(OpenRAM_ActionOk),
                             0);
  GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
}

int HexEditor_OpenRAM(void)
{
  switch(open_mode)
  {
  case NOT_OPEN:
    InputRamAddress();
    break;
    
  case OPEN_FILE:
    HexEditor_CloseFile(); // menu_item = MENU_RAM_OPEN
    break;
    
  case OPEN_RAM:
    HexEditor_CloseViewRAM();  // menu_item = MENU_RAM_OPEN
    break;
  }
  return(1);
}

void CreateFileDialog(wchar_t *dir)
{
  dll_data = (DLL_DATA *)LoadDLL(L"FileDialogDll.dll");
  FILE_DIALOG_Create(dll_data,
                     Find_HexEditorBook(),
                     OFD_SHOW_FILES|OFD_SHOW_FOLDERS|OFD_ACT_INFO_ENABLE,
                     RecLang(LNG_SELECT_FILE),
                     L"*.*",
                     dir,
                     -1);
}

void FolderList_SelectItemAction(BOOK *book, GUI *gui)
{
  int index = ListMenu_GetSelectedItem(gui);
  CreateFileDialog((wchar_t *)List_Get(menu_items, index));
  GUIObject_Destroy(gui);
}

int FolderList_onMessage(GUI_MESSAGE *msg)
{
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    int index = GUIonMessage_GetCreatedItemIndex(msg);
    TEXTID id = TextID_Create((wchar_t *)List_Get(menu_items, index), ENC_UCS2, TEXTID_ANY_LEN);
    GUIonMessage_SetMenuItemText(msg, id);
  }
  return(1);
}

void CreateItems(void)
{
  menu_items = List_Create();
  List_InsertLast(menu_items, GetDir(DIR_USER|MEM_INTERNAL));
  List_InsertLast(menu_items, GetDir(DIR_USER|MEM_EXTERNAL));
  List_InsertLast(menu_items, (void *)L"/tpa");
  List_InsertLast(menu_items, (void *)L"/system");
  List_InsertLast(menu_items, (void *)L"/IFS");
  List_InsertLast(menu_items, (void *)L"/BOOT");
  List_InsertLast(menu_items, (void *)L"/sys");
  
  W_FSTAT w_fst;
  for(int i=0; ; i++)
  {
    if(w_fstat((wchar_t *)List_Get(menu_items, i), &w_fst)) List_RemoveAt(menu_items, i--);
    if(i == (menu_items->FirstFree-1)) break;
  }
}

void CreateFolderList(void)
{
  GUI *menu = CreateListMenu(Find_HexEditorBook(), 0);
  ListMenu_SetItemCount(menu, menu_items->FirstFree);
  GUIObject_SetTitleText(menu, Lang_GetTextID(LNG_SELECT_FILE));
  GUIObject_SetStyle(menu, UI_OverlayStyle_PopupFrame);
  ListMenu_SetCursorToItem(menu, 0);
  ListMenu_SetOnMessage(menu, FolderList_onMessage);
  GUIObject_SoftKeys_SetAction(menu, ACTION_BACK, Find_ActionBack);
  GUIObject_SoftKeys_SetAction(menu, ACTION_SELECT1, FolderList_SelectItemAction);
  GUIObject_Show(menu);
}

void OpenNewWindow_NoAction(BOOK *book, GUI *gui)
{
  GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
  HexEditor_CloseFile();
}

void OpenNewWindow_YesAction(BOOK *book, GUI *gui)
{
  GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
  open_mode = NEW_WINDOW;
  //CreateFileDialog();
  CreateFolderList();
}

void HexEditor_OpenNewWindow(void)
{
  GUI *gui = CreateYesNoQuestionVA(0,
                              VAR_BOOK(Find_HexEditorBook()),                                 
                              VAR_YESNO_QUESTION(Lang_GetTextID(LNG_Q_NEW_WINDOW)),
                              VAR_YESNO_YES_ACTION(OpenNewWindow_YesAction),
                              VAR_YESNO_NO_ACTION(OpenNewWindow_NoAction),
                              0);
  GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
}

int HexEditor_FileOpen(void)
{
  switch(open_mode)
  {
  case NOT_OPEN:
    //CreateFileDialog();
    CreateFolderList();
    break;
    
  case OPEN_FILE:
    //HexEditor_CloseFile(); // menu_item = MENU_FILE_OPEN
    HexEditor_OpenNewWindow();
    break;
    
  case OPEN_RAM:
    HexEditor_CloseViewRAM();  // menu_item = MENU_FILE_OPEN
    break;
  }
  return(1);
}

int HexEditor_SaveFile(void)
{
  for(int j=(corrections_list->FirstFree-1); j>=0; j--)
  {
    HE_CORRECTION *c = (HE_CORRECTION *)List_Get(corrections_list, j);
    SaveOffset(c->c_offset, c->correction);
    mfree(c);
    List_RemoveAt(corrections_list, j);
  }
  HexEditorMenu_Item_SetEnable(MENU_SAVE_FILE, false);
  Redraw_Update(true, true, true, true, true, false);
  return(1);
}

void CloseFile(void)
{
  //FREE(buffer); // оттак не далать!!
  // надо так V
  memset(buffer, 0, CELLS_COUNT);
  if(f != FILE_HANDLE_INVALID) w_fclose(f);
  f = FILE_HANDLE_INVALID;
  FREE(file_o);
  FREE(find_text);
  
  page = 1;
  offset = 0;
  view_mode = HEX_MODE;
  open_mode = NOT_OPEN;
  
  if(corrections_list && corrections_list->FirstFree)
  {
    for(int i=(corrections_list->FirstFree-1);i>=0;i--) 
    {
      mfree(List_Get(corrections_list,i));
      List_RemoveAt(corrections_list,i);
    }
  }
  
  HexEditorMenu_Item_SetVisible(MENU_FIND, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, false);
  HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_VIEW_RAM, false);
  
  Redraw_Update(true, true, true, true, true, false);
}

void CloseFileAndCreateDialogs(void)
{
  //if(menu_item == MENU_FILE_OPEN) CreateFileDialog();
  if(menu_item == MENU_FILE_OPEN) CreateFolderList();
  else if(menu_item == MENU_RAM_OPEN) InputRamAddress();
  else if(menu_item == MENU_CLOSE_FILE) CloseFile();
}

void CloseFile_NoAction(BOOK *book, GUI *gui)
{
  GUIObject_Destroy(BookObj_GetGUIObject(book, NUM_OF_GUI));
  CloseFileAndCreateDialogs();
}

void CloseFile_YesAction(BOOK *book, GUI *gui)
{
  HexEditor_SaveFile();
  CloseFile_NoAction(book, gui);
}

int HexEditor_CloseFile(void)
{
  if(corrections_list && corrections_list->FirstFree)
  {
    GUI *gui = CreateYesNoQuestionVA(0,
                              VAR_BOOK(Find_HexEditorBook()),                                 
                              VAR_YESNO_QUESTION(Lang_GetTextID(LNG_Q_SAVE_CHANGES)),
                              VAR_YESNO_YES_ACTION(CloseFile_YesAction),
                              VAR_YESNO_NO_ACTION(CloseFile_NoAction),
                              0);
    GUIObject_SoftKeys_SetAction(gui, ACTION_BACK, Find_ActionBack);
  }
  else
    CloseFileAndCreateDialogs();
  
  return(1);
}

int HexEditor_CloseViewRAM(void)
{
  open_mode = NOT_OPEN;
  
  if(buffer) memset(buffer, 0, CELLS_COUNT);
  
  HexEditorMenu_Item_SetVisible(MENU_FIND, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_OFFSET, false);
  HexEditorMenu_Item_SetVisible(MENU_GOTO_PAGE, false);
  HexEditorMenu_Item_SetVisible(MENU_SAVE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_FILE, false);
  HexEditorMenu_Item_SetVisible(MENU_CLOSE_VIEW_RAM, false);
  
  Redraw_Update(true, true, true, true, true, false);
  return(1);
}

int HexEditor_Reconfig(void *mess, BOOK *book)
{
  RECONFIG_EVENT_DATA *reconf=(RECONFIG_EVENT_DATA *)mess;
  if(!wstrcmpi(reconf->path,successed_config_path) && !wstrcmpi(reconf->name,successed_config_name))
  {
    InitConfig();
    CELLS_COUNT = LINES_COUNT*COLUMNS_COUNT;
    
    if(buffer) mfree(buffer);
    buffer = (char *)malloc(CELLS_COUNT);
    memset(buffer, 0, CELLS_COUNT);
    
    if(open_mode != NOT_OPEN)
    {
      LoadBuffer(0); //!!!!!!!!
    }
    Redraw_Update(true, true, true, true, true, false);
    return(1);
  }
  return(0);
}

int HexEditor_OnBcfgConfig(void *mess, BOOK *book)
{
  MSG_BCFG *msg = (MSG_BCFG *)mess;
  wchar_t bcfg[FILENAME_MAX_LEN+1];
  memset(bcfg, 0, (FILENAME_MAX_LEN+1)*sizeof(wchar_t));
  wstrcpy(bcfg, msg->bcfg_p);
  wstrcat(bcfg, L"/");
  wstrcat(bcfg, msg->bcfg_n);
  elfload(bcfg, (void *)successed_config_path, (void *)successed_config_name, NULL);
  return(1);
}

void OpenLang(void)
{
  wchar_t lng[FILENAME_MAX_LEN+1];
  memset(lng, 0, sizeof(wchar_t)*(FILENAME_MAX_LEN+1));
  wstrcpy(lng, GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));
  wstrcat(lng, SPLASH);
  wstrcat(lng, HEXEDITOR);
  mkdir(lng);
  wstrcat(lng, SPLASH);
  wstrcat(lng, HEXEDITOR);
  wstrcat(lng, L".lng");
  ReadLang(lng);
}

int main(wchar_t *elf, wchar_t *path, wchar_t *name)
{
  trace_init();
  //if(!Find_HexEditorBook()) 
  {
    BOOK *book;
    if(book = CreateHexEditorBook())
    {
      int len = wstrlen(elf);
      elf_name = new wchar_t[len+1];
      wstrcpy(elf_name, elf);
      elf_name[len] = 0;
      
      CreateItems();
      
      InitConfig();
      CELLS_COUNT = LINES_COUNT*COLUMNS_COUNT;
      page = 1;
      corrections_list = List_Create(); // список с исправлени€ми
      
      OpenLang();
      
      f = FILE_HANDLE_INVALID;
      
      open_mode = NOT_OPEN;
      
      if(path && name)
      {
        if(file_o) mfree(file_o);
        file_o = MakeFullName(path, name);
        Register(file_o);
      }
      BookObj_GotoPage(book, &HexEditor_MainDesc);
    }
    else SUBPROC(elf_exit);
  }
  //else
  //{
  //  MessageBox(EMPTY_TEXTID, STR("HexEditor уже запущен!"), NOIMAGE, 1, 0, 0);
  //  SUBPROC(elf_exit);
  //}
  return(0);
}
