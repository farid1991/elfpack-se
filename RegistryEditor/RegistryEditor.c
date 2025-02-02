#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\include\var_arg.h"
#include "..\\include\dll.h"
#include "..\\Dlls\FileDialog\export\FileDialogDll.h"
#include "RegistryEditor.h"
#include "Pages.h"

int sort_order_global=Ascending_order;
char * wstr2str_mask="%ls";

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


int TerminateElf(void * ,BOOK * book)
{
  FreeBook(book);
  return(1);
}


int ShowAuthorInfo(void *mess ,BOOK * book)
{
  MSG * msg = (MSG*)mess;
  MessageBox(EMPTY_TEXTID,STR("Registry Editor, v1.5.2\n\n(c) IronMaster\n\n(r) the_laser\n(r) Lollylost100"), NOIMAGE, 1, 5000,msg->book);
  return(1);
}


int RegList_Insert_Filter(void * element1,void * element2)
{
  wchar_t string1[39];
  wchar_t string2[39];
  int len1;
  int len2;
  TextID_GetWString((TEXTID)element1,string1,38);
  TextID_GetWString((TEXTID)element2,string2,38);
  len1=wstrlen(string1);
  len2=wstrlen(string2);
  wchar_t chr1=string1[len1+1];
  wchar_t chr2=string2[len2+1];
  wstrnlwr(string1,len1);
  wstrnlwr(string2,len2);
  if (chr1!=chr2)
  {
    if (chr1!=KEY) return 1;
    else return -1;
  }
  if (sort_order_global==Ascending_order) return wstrcmp(string1,string2);
  else return wstrcmp(string2,string1);
}


void RegList_DestroyElements_Destroy(void * element)
{
  TextID_Destroy((TEXTID)element);
}


int RegList_DestroyElements_Compare(void * element)
{
  if (element) return(1);
  return(0);
}

void RegList_DestroyElements(LIST * pRegList)
{
  List_DestroyElements(pRegList,RegList_DestroyElements_Compare,RegList_DestroyElements_Destroy);
  pRegList->FirstFree=0;
}


void RegEdBook_Fill_RegList(char * pKeyPath,RegistryEditorBook * RegEdBook)
{
  IRegistryKey * pIRegistryKey=0;
  char pName[39];
  int elem_count=0;
  int res=0;
  int len;
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  if (!pKeyPath) RegEdBook->pIRegistry->OpenKey("/",&pIRegistryKey);
  else RegEdBook->pIRegistry->OpenKey(pKeyPath,&pIRegistryKey);
  
  if (pIRegistryKey)
  {
    pIRegistryKey->GetKeyPath(sizeof(RegEdBook->CurrentKeyPath),RegEdBook->CurrentKeyPath);
    
    RegList_DestroyElements(RegEdBook->pRegList);
    
    //Get all subkeys for KeyPath start
    res = pIRegistryKey->EnumSubkeys(0);
    if (res>=0)
    {
      for (res = pIRegistryKey->EnumSubkeys(pName); (res>=0)&&!(res&REGISTRY_UED_ENUM_DONE); )
      {
        len=strlen(pName);
        pName[len+1]=KEY;
        TEXTID element=TextID_Create(pName,ENC_LAT1,len+2);
        elem_count=elem_count+1;
        if (sort_order_global==Nosort) List_InsertLast(RegEdBook->pRegList,(void*)element);
        else List_InsertSorted(RegEdBook->pRegList,(void*)element,RegList_Insert_Filter);
        res = pIRegistryKey->EnumSubkeys(pName);
      }
    }
    //Get all subkeys for KeyPath end
    
    //Get all values for KeyPath start
    res = pIRegistryKey->EnumValues(0);
    if (res>=0)
    {
      for (res = pIRegistryKey->EnumValues(pName); (res>=0)&&!(res&REGISTRY_UED_ENUM_DONE); )
      {
        len=strlen(pName);
        pName[len+1]=VALUE;
        TEXTID element=TextID_Create(pName,ENC_LAT1,len+2);
        elem_count=elem_count+1;
        if (sort_order_global==Nosort) List_InsertLast(RegEdBook->pRegList,(void*)element);
        else List_InsertSorted(RegEdBook->pRegList,(void*)element,RegList_Insert_Filter);
        res = pIRegistryKey->EnumValues(pName);
      }
    }
    //Get all values for KeyPath end
    
    pIRegistryKey->Release();
    
    RegEdBook->pIRegistry->EndTransaction(false);
    
    if (!elem_count)
    {
      pName[0]=0;
      strcpy(pName,"<empty>");
      len=strlen(pName);
      pName[len+1]=EMPTY;
      TEXTID element=TextID_Create(pName,ENC_LAT1,len+2);
      List_InsertLast(RegEdBook->pRegList,(void*)element);
    }
  }
  else
  {
    MessageBox(EMPTY_TEXTID, STR("Can't create IRegistryKey\nShutdown elf"), NOIMAGE, 1, 2000, 0);
    FreeBook(RegEdBook);
  }
}


void registry_list_gui_refresh_softkeys(RegistryEditorBook * RegEdBook)
{
  GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,ACTION_SELECT1,true);
  GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,RegEdBook->is_A2);
}


char* get_last_key(char* str)
{
  char c='/';
  char * new_str=str+strlen(str)-2;
  
  for(;new_str!=str; new_str--) if(*new_str == c) return new_str;
  return NULL;
}


void RegEditor_RecreateList(RegistryEditorBook * RegEdBook,int sel_item)
{
  RegEdBook_Fill_RegList(RegEdBook->CurrentKeyPath,RegEdBook);
  TEXTID title;
  char * temp_str=0;
  
  title=TextID_Create(RegEdBook->CurrentKeyPath,ENC_LAT1,TEXTID_ANY_LEN);
  
  SetFont(FONT_E_22R);
  if (Display_GetWidth(0)<=Disp_GetTextIDWidth(title,TEXTID_ANY_LEN))
  {
    char * last_key = get_last_key(RegEdBook->CurrentKeyPath);
    temp_str=new char[strlen(last_key)+4];
    temp_str[0]='.';
    temp_str[1]='.';
    temp_str[2]='.';
    strcpy(temp_str+3,last_key);
    TextID_Destroy(title);
    title=TextID_Create(temp_str,ENC_LAT1,TEXTID_ANY_LEN);
  }
  GUIObject_SetTitleText(RegEdBook->registry_list_gui,title);
  ListMenu_DestroyItems(RegEdBook->registry_list_gui);
  ListMenu_SetItemCount(RegEdBook->registry_list_gui,RegEdBook->pRegList->FirstFree);
  ListMenu_SetCursorToItem(RegEdBook->registry_list_gui,sel_item);
  registry_list_gui_refresh_softkeys(RegEdBook);
  
  if (temp_str) delete temp_str;
}


void RegEditor_SaveBinary(char * binary_buf,int buf_size,char * buf_for_str)
{
  int count_char=0;
  
  while (count_char<buf_size)
  {
    sprintf(buf_for_str+strlen(buf_for_str),"%.2X,",binary_buf[count_char]);
    count_char=count_char+1;
  }
}


void RegEditor_PrintBinary(char * display_string,int value_size,char * buf_for_display_str)
{
  int count_char=0;
  int count_int=0;
  while (count_char<value_size)
  {
    if (!(count_char&3))
    {
      sprintf(buf_for_display_str+strlen(buf_for_display_str),"\n%.4X: ",count_int<<2);
      count_int=count_int+1;
    }
    sprintf(buf_for_display_str+strlen(buf_for_display_str),"%.2X ",display_string[count_char]);
    count_char=count_char+1;
  }
}


void RegEditor_ExtractBinary(wchar_t * string,char * buf_for_value,int * bin_size)
{
  int count;
  int count_to_new_line;
  int chr_count;
  
  int temp_struct[4];
  int data_len;
  int string_len;
  wchar_t temp_chr;
  
  wchar_t * temp_wstr=new wchar_t[wstrlen(string)+1];
  
  string_len=wstrlen(string);
  
  count=0;
  while (count<string_len)
  {
    if (string[count]!=L'\n') break;
    count=count+1;
  }
  
  wstrcpy(temp_wstr,string+count);
  *bin_size=0;
  data_len=wstrlen(temp_wstr);
  
  count=0;
  chr_count=0;
  count_to_new_line=0;
  while (chr_count<data_len)
  {
    temp_chr=temp_wstr[count];
    chr_count=count+1;
    if (temp_chr==0xA||count==data_len-1)
    {
      if (count!=data_len-1) temp_wstr[count]=0x0;
      int scanned=swscanf(temp_wstr+count_to_new_line,L"%*4x: %2x %2x %2x %2x",&temp_struct[0],&temp_struct[1],&temp_struct[2],&temp_struct[3]);
      if (scanned==-1) break;
      
      int read_count=0;
      while (read_count<scanned)
      {
        buf_for_value[*bin_size+read_count]=temp_struct[read_count];
        read_count=read_count+1;
      }
      *bin_size=*bin_size+scanned;
      if (scanned<4) break;
      count_to_new_line=chr_count;
    }
    count=chr_count;
  }
  
  delete temp_wstr;
  
  if (*bin_size>=0x400) *bin_size=0x400;

}


void RegEditor_DisplayValue(char * valuename,RegistryEditorBook * RegEdBook)
{
#define TOTAL_BUF_SIZE 0x1400
#define VALUE_MAX_SIZE 0x400
  IRegistryKey * pIRegistryKey=0;
  RegKey_ValueInfo_t value_info;
  int value_size;
  TEXT_ENCODING string_type=ENC_LAT1;
  char temp_buf;
  char * buf_for_display_str = new char[TOTAL_BUF_SIZE];

  memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
  memset(buf_for_display_str,0,TOTAL_BUF_SIZE);
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  pIRegistryKey->GetValue(valuename,Reg_RegValueType_Unspecified,1,&temp_buf,&value_info);
  value_size=value_info.dataSize;
  
  char * buf_for_value = new char[value_size+2];
  memset(buf_for_value,0,value_size+2);
  pIRegistryKey->GetValue(valuename,Reg_RegValueType_Unspecified,value_size,buf_for_value,&value_info);
  
  switch(value_info.valueType)
  {
  case Reg_RegValueType_Unspecified:
    sprintf(buf_for_display_str,"Name: %s\nValue doesn't exist.\nChoose \"Edit\" to create it.",valuename);
    break;
  case Reg_RegValueType_UInt:
    sprintf(buf_for_display_str,"Name: %s\nValue type: Unsigned int\nData size: %d bytes\n\n%u",valuename,value_size,((int*)buf_for_value)[0]);
    break;
  case Reg_RegValueType_String:
    sprintf(buf_for_display_str,"Name: %s\nValue type: String\nData size: %d bytes\n\n\"%s\"",valuename,value_size,buf_for_value);
    break;
  case Reg_RegValueType_WString:
    string_type=ENC_UCS2;
    snwprintf((wchar_t*)buf_for_display_str,TOTAL_BUF_SIZE,L"Name: %s\nValue type: Wide string\nData size: %d bytes\n\n\"%ls\"",valuename,value_size,buf_for_value);
    break;
  case Reg_RegValueType_Binary:
    sprintf(buf_for_display_str,"Name: %s\nValue type: Binary\nData size: %d bytes",valuename,value_size);
    if (value_size>VALUE_MAX_SIZE) value_size=VALUE_MAX_SIZE;
    RegEditor_PrintBinary(buf_for_value,value_size,buf_for_display_str);
    break;
  }
  
  MessageBox(TextID_Create("Value information",ENC_LAT1,TEXTID_ANY_LEN),TextID_Create(buf_for_display_str,string_type,TEXTID_ANY_LEN),NOIMAGE,2,0,RegEdBook);
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(false);
  
  delete buf_for_display_str;
  delete buf_for_value;
}


void RegEditor_Exit_no_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  GUIObject_Destroy(RegEdBook->yesno_gui);
  RegEdBook->yesno_gui=0;
  FreeBook(book);
}


void RegEditor_Exit_yes_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  GUIObject_Destroy(RegEdBook->yesno_gui);
  RegEdBook->yesno_gui=0;
  REQUEST_SYSTEM_RESTART();
  FreeBook(book);
}


void RegEditor_Exit(BOOK * book)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  if (RegEdBook->reboot_flag==false)
  {
    FreeBook(RegEdBook);
    return;
  }
  
  RegEdBook->yesno_gui = CreateYesNoQuestionVA( 0,
                                             VAR_BOOK( RegEdBook ),
                                             VAR_YESNO_PRE_QUESTION( TextID_Create("Registry was changed",ENC_LAT1,TEXTID_ANY_LEN) ),
                                             VAR_YESNO_QUESTION( TextID_Create("Reboot?",ENC_LAT1,TEXTID_ANY_LEN) ),
                                             0 );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_YES, RegEditor_Exit_yes_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_NO, RegEditor_Exit_no_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_BACK, registry_list_gui_delete_no_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_LONG_BACK, registry_list_gui_delete_no_action );
}


void RegEditor_Sort(BOOK * book,char * text)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  int count=0;
  int max=RegEdBook->Stack_List->FirstFree;
  while(count<max)
  {
    List_InsertFirst(RegEdBook->Stack_List,0);
    count=count+1;
  }
  
  RegEditor_RecreateList(RegEdBook,0);
  MessageBox(EMPTY_TEXTID,TextID_Create(text,ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
}


void registry_list_gui_stringinput_cancel_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  GUIObject_Show(RegEdBook->registry_list_gui);
  GUIObject_Destroy(RegEdBook->stringinput_gui);
  RegEdBook->stringinput_gui=0;
}


int RegistryEditorBook_SelectBackup_Page_Previous(void *mess ,BOOK *book)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  if (RegEdBook->dll) UnLoadDLL(RegEdBook->dll);
  BookObj_ReturnPage(book,PREVIOUS_EVENT);
  return 1;
}


int RegistryEditorBook_SelectBackup_Page_Accept(void *mess ,BOOK *book)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  FILEITEM * fi = (FILEITEM *)mess;
  
  IRegistryKey * pIRegistryKey=0;

  char name_len;
  char key_name[39];
  wchar_t data_len;
  char minibuf[100];
  int GD_count=0;
  char message_buf[50];
  FSTAT fstat_struct;
  
  memset(minibuf,0,sizeof(minibuf));
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey("/gdfs",&pIRegistryKey);

  
  if (fstat(FILEITEM_GetPath(fi),FILEITEM_GetFname(fi),&fstat_struct)>=0)
  {
    int f=_fopen(FILEITEM_GetPath(fi),FILEITEM_GetFname(fi),0x1,0x180,0);
    
    char * data_buf;
    char * bigbuf=0;
    
    if (RegEdBook->restore_var_flag==false)
    {
      fread(f,&name_len,1);
      
      while (name_len!=0xFE)
      {
        fread(f,key_name,name_len+1);         //read key name
        fread(f,&data_len,2);                 //read data len
        
        if (data_len<=sizeof(minibuf)) data_buf=minibuf;
        else
        {
          bigbuf = new char[data_len];
          data_buf=bigbuf;
        }
        
        fread(f,data_buf,data_len);            //read data
        
        pIRegistryKey->SetValue(key_name,Reg_RegValueType_Binary,data_len,data_buf);
        
        if (bigbuf)
        {
          delete bigbuf;
          bigbuf=0;
        }
        
        GD_count=GD_count+1;
        fread(f,&name_len,1);                 //read key name len
      }
    }
    else
    {
      wchar_t temp_wstr[39];
      
      TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,ListMenu_GetSelectedItem(RegEdBook->registry_list_gui)),temp_wstr,38);
      sprintf(key_name,wstr2str_mask,temp_wstr);
      
      if (fstat_struct.fsize<=sizeof(minibuf)) data_buf=minibuf;
      else
      {
        bigbuf = new char[fstat_struct.fsize];
        data_buf=bigbuf;
      }
      
      fread(f,data_buf,fstat_struct.fsize);            //read data
      
      pIRegistryKey->SetValue(key_name,Reg_RegValueType_Binary,fstat_struct.fsize,data_buf);
      
      if (bigbuf)
      {
        delete bigbuf;
        bigbuf=0;
      }
      
      GD_count=GD_count+1;
    }
    
    fclose(f);
    
    sprintf(message_buf,"Restore complete.\n%d vars restored.",GD_count);
    MessageBox(EMPTY_TEXTID,TextID_Create(message_buf,ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
    RegEdBook->reboot_flag=true;
  }
  else
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Backup file not found",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
  }
  
  RegEdBook->pIRegistry->EndTransaction(true);
  pIRegistryKey->Release();
  
  return 1;
}


int RegistryEditorBook_SelectBackup_Page_Enter(void * r0,BOOK * book)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  wchar_t fpath[30];
  wchar_t * filter;
  
  fpath[0]=0;
  wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
  wstrcat(fpath,L"/RegEditor");
  
  RegEdBook->dll = (FILE_DIALOG *)LoadDLL(L"FileDialogDll.dll");
  
  if ((INVALID(RegEdBook->dll)))
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Error Load FileDialogDll.dll",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE, 1,0,0);
    return NULL;
  }
  
  if (RegEdBook->restore_var_flag==true) filter=BACKUP_VAR_EXT_FILTER;
  else filter=BACKUP_EXT_FILTER;
  
  FILE_DIALOG_Create(RegEdBook->dll,
                     RegEdBook,
                     OFD_OPEN_FILE_DIALOG,
                     L"Select backup",
                     filter,
                     fpath,
                     -1
                     );
  
  return 1;
}


void registry_list_gui_restore_gdfs_var_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->restore_var_flag=true;
  BookObj_CallPage(book,&RegistryEditorBook_SelectBackup_Page);
}


void registry_list_gui_restore_gdfs_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->restore_var_flag=false;
  BookObj_CallPage(book,&RegistryEditorBook_SelectBackup_Page);
}


char * strchr(char * str,char c)
{
  for(;*str;str++)if(*str==c)return str;
  return NULL;
}


void win12512unicode(wchar_t * ws,char * s,int len)
{
  int c;
  
  while( ( c = *s++ ) && len-- > 0 )
  {
    if ( c == 0xA8 )
      c = 0x401;
    if ( c == 0xAA )
      c = 0x404;
    if ( c == 0xAF )
      c = 0x407;
    if ( c == 0xB8 )
      c = 0x451;
    if ( c == 0xBA )
      c = 0x454;
    if ( c == 0xBF )
      c = 0x457;
    if ( c >= 0xC0 && c < 0x100 )
      c += 0x350;
    *ws++ = c;
  }
  *ws = 0;
}


int registry_restore(wchar_t * fpath,wchar_t * fname)
{
  ISEMCRegistryManager * pISEMCRegistryManager=0;
  IRegistry * pIRegistry=0;
  IRegistryKey * pIRegistryKey=0;
  
  FSTAT fstat_struct;
  char * file_buf;
  char * cur_pos;
  
  int value_count=0;
  
  CoCreateInstance(&CID_CSEMCRegistryManager,&IID_ISEMCRegistryManager,PPINTERFACE(&pISEMCRegistryManager));
  if (pISEMCRegistryManager)
  {
    pISEMCRegistryManager->CreateRegistry(&pIRegistry);
    pISEMCRegistryManager->Release();
  }
  else
  {
    MessageBox(EMPTY_TEXTID, STR("Can't create ISEMCRegistryManager\nShutdown elf"), NOIMAGE, 1, 2000, 0);
    return value_count;
  }
  
  fstat(fpath,fname,&fstat_struct);
  
  if (fstat_struct.fsize)
  {
    file_buf = new char[fstat_struct.fsize+1];
    
    int f=_fopen(fpath,fname,0x1,0x180,0);
    fread(f,file_buf,fstat_struct.fsize);
    fclose(f);
    file_buf[fstat_struct.fsize]=0;
    
    cur_pos=file_buf;
    
    pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
    
    while (cur_pos-file_buf<fstat_struct.fsize)
    {
      char main_key_name[0x122];
      cur_pos=strchr(cur_pos,'[');
      if (!cur_pos) break;
      
      sscanf(cur_pos,"[%[^]]",main_key_name);
      
      pIRegistry->CreateKey(main_key_name,&pIRegistryKey);
      
      cur_pos=cur_pos+strlen(main_key_name)+sizeof("[]")-sizeof("");
      
      char * next_key = strchr(cur_pos,'[');
      char * next_value=cur_pos;
      
      if (!next_key) next_key=file_buf+fstat_struct.fsize;
      
      char type[10];
      char * data = new char[1024*3+1];
      while ((next_value=strchr(cur_pos,'"'))<next_key&&next_value)
      {
        cur_pos=next_value;
        sscanf(cur_pos,"\"%[^\"]",main_key_name);
        cur_pos=cur_pos+strlen(main_key_name)+sizeof("\"\"")-sizeof("");
        sscanf(cur_pos,"=%[^:]",type);
        cur_pos=cur_pos+strlen(type)+sizeof("=")-sizeof("");
        if (cur_pos[1]=='\r'||cur_pos[1]=='\n') data[0]=0;
        else
        {
          sscanf(cur_pos,":%[^\n]",data);
          int x=strlen(data);
          if (data[x-1]=='\r'||data[x-1]=='\n') data[x-1]=0;
          cur_pos=cur_pos+strlen(data)+sizeof(":")-sizeof("");
        }
        
        if (!strcmp(type,REG_UINT))
        {
          int val=0;
          sscanf(data,"%d",&val);
          pIRegistryKey->SetValue(main_key_name,Reg_RegValueType_UInt,0,&val);
          value_count=value_count+1;
        }
        else if (!strcmp(type,REG_STRING))
        {
          pIRegistryKey->SetValue(main_key_name,Reg_RegValueType_String,0,data);
          value_count=value_count+1;
        }
        else if (!strcmp(type,REG_WSTRING))
        {
          int len = strlen(data);
          wchar_t * wstr = new wchar_t[len+1];
          wstr[0]=0;
          win12512unicode(wstr,data,len);
          pIRegistryKey->SetValue(main_key_name,Reg_RegValueType_WString,0,wstr);
          value_count=value_count+1;
          delete wstr;
        }
        else if (!strcmp(type,REG_BINARY))
        {
          int len = strlen(data);
          int size = (len+1)/3;
          if (size>0x400) size=0x400;
          char * bin_data = new char[size];
          int count=0;
          int chr;
          while (count<size)
          {
            sscanf(data+count*3,"%2x,",&chr);
            bin_data[count]=chr;
            count=count+1;
          }
          pIRegistryKey->SetValue(main_key_name,Reg_RegValueType_Binary,size,bin_data);
          value_count=value_count+1;
          delete bin_data;
        }
      }
      delete data;
      pIRegistryKey->Release();

    }
    delete file_buf;
    
    pIRegistry->EndTransaction(true);
  }
  pIRegistry->Release();
  
  return value_count;
}


void registry_save_value(int f,IRegistryKey * pIRegistryKey,char * pName)
{
  RegKey_ValueInfo_t value_info;
  char minibuf[100];
  char temp_str[45];
  char temp_buf;
  
  memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
  
  sprintf(temp_str,"\"%s\"=",pName);
  fwrite(f,temp_str,strlen(temp_str));
  
  pIRegistryKey->GetValue(pName,Reg_RegValueType_Unspecified,1,&temp_buf,&value_info);
  
  char * type_str;
  
  switch(value_info.valueType)
  {
  case Reg_RegValueType_UInt:
    type_str=REG_UINT;
    break;
  case Reg_RegValueType_String:
    type_str=REG_STRING;
    break;
  case Reg_RegValueType_WString:
    type_str=REG_WSTRING;
    break;
  case Reg_RegValueType_Binary:
    type_str=REG_BINARY;
    break;
  }
  
  char type_buf[10];
  sprintf(type_buf,"%s:",type_str);
  fwrite(f,type_buf,strlen(type_buf));
  
  char * buf_ptr;
  char * bigbuf=0;
  
  if (value_info.dataSize<=sizeof(minibuf)) buf_ptr=minibuf;
  else
  {
    bigbuf = new char[value_info.dataSize];
    buf_ptr=bigbuf;
  }
  pIRegistryKey->GetValue(pName,Reg_RegValueType_Unspecified,value_info.dataSize,buf_ptr,&value_info);
  
  char * data_buf;
  
  switch(value_info.valueType)
  {
  case Reg_RegValueType_UInt:
    data_buf = new char[20];
    sprintf(data_buf,"%u\r\n",((int*)buf_ptr)[0]);
    break;
  case Reg_RegValueType_String:
    data_buf = new char[value_info.dataSize+10];
    sprintf(data_buf,"%s\r\n",buf_ptr);
    break;
  case Reg_RegValueType_WString:
    data_buf = new char[value_info.dataSize+10];
    sprintf(data_buf,"%ls\r\n",(wchar_t*)buf_ptr);
    break;
  case Reg_RegValueType_Binary:
    data_buf = new char[value_info.dataSize*3+10];
    memset(data_buf,0,value_info.dataSize*3+10);
    char * buf_for_bin=data_buf+9;
    RegEditor_SaveBinary(buf_ptr,value_info.dataSize,buf_for_bin);
    buf_for_bin[strlen(buf_for_bin)-1]=0;
    sprintf(data_buf,"%s\r\n",buf_for_bin);
    break;
  }
  
  fwrite(f,data_buf,strlen(data_buf));
  
  delete data_buf;
  
  if (bigbuf)
  {
    delete bigbuf;
    bigbuf=0;
  }
}


void recursive_registry_save(RegistryEditorBook * RegEdBook,char * key_name,int f)
{
  IRegistryKey * pIRegistryKey=0;

  char pName[39];
  char parrent_key[0x122];

  int res=0;
  int key_len;

  LIST * subkeys_list=0;
  int count=0;
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(key_name,&pIRegistryKey);
  
  key_len=strlen(key_name);
  
  //Get all values for KeyPath start
  res = pIRegistryKey->EnumValues(0);
  if (res>=0)
  {
    for (res = pIRegistryKey->EnumValues(pName); (res>=0)&&!(res&REGISTRY_UED_ENUM_DONE); )
    {
      if (!count)
      {
        sprintf(parrent_key,"\r\n[%s]\r\n",key_name);
        fwrite(f,parrent_key,strlen(parrent_key));
      }
      
      registry_save_value(f,pIRegistryKey,pName);
      count=count+1;
      res = pIRegistryKey->EnumValues(pName);
    }
  }
  //Get all values for KeyPath end
  
  //Get all subkeys for KeyPath start
  res = pIRegistryKey->EnumSubkeys(0);
  if (res>=0)
  {
    subkeys_list=List_Create();
    for (res = pIRegistryKey->EnumSubkeys(pName); (res>=0)&&!(res&REGISTRY_UED_ENUM_DONE); )
    {
      char * subkey_buf=new char[key_len+strlen(pName)+1+1];
      strcpy(subkey_buf,key_name);
      subkey_buf[key_len]='/';
      subkey_buf[key_len+1]=0;
      strcat(subkey_buf,pName);
      List_InsertFirst(subkeys_list,subkey_buf);
      res = pIRegistryKey->EnumSubkeys(pName);
    }
  }
  //Get all subkeys for KeyPath end
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(false);
  
  count=0;
  int last_count=subkeys_list->FirstFree;
  while (count<last_count)
  {
    char * elem = (char*)List_RemoveAt(subkeys_list,0);
    recursive_registry_save(RegEdBook,elem,f);
    delete elem;
    count=count+1;
  }
  List_Destroy(subkeys_list);
}


void registry_list_gui_save_registry_key_accept_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  if (FSX_GetInvalidChar(string))
  {
    int str_id;
    textidname2id(L"DB_ILLEGAL_CHAR_TXT",TEXTID_ANY_LEN,&str_id);
    MessageBox(EMPTY_TEXTID,str_id, NOIMAGE, 0, 2000,0);
    
    FILEITEM * item_desc=FILEITEM_Create();
    wchar_t fname[40];
    wstrcpy(fname,string);
    FILEITEM_SetFname(item_desc, fname);
    FSX_RemoveIllegalSymbolsName(item_desc);
    StringInput_DispObject_SetText(GUIObject_GetDispObject(RegEdBook->stringinput_gui),TextID_Create(FILEITEM_GetFname(item_desc),ENC_UCS2,TEXTID_ANY_LEN));
    FILEITEM_Destroy(item_desc);
  }
  else
  {  
    IRegistryKey * pIRegistryKey=0;
    
    wchar_t fpath[30];
    char KeyPath[0x122];
    wchar_t fname[45];
    wchar_t temp_wstr[39];
    char temp_str[39];
    
    wstrcpy(fname,string);
    wstrcat(fname,BACKUP_REGISTRY_EXT);
    
    fpath[0]=0;
    wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
    wstrcat(fpath,L"/RegEditor");
    
    int f=_fopen(fpath,fname,0x2,0x180,0);
    
    TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,ListMenu_GetSelectedItem(RegEdBook->registry_list_gui)),temp_wstr,38);
    sprintf(temp_str,wstr2str_mask,temp_wstr);
    
    if (temp_wstr[wstrlen(temp_wstr)+1]==KEY)
    {
      strcpy(KeyPath,RegEdBook->CurrentKeyPath);
      strcat(KeyPath,temp_str);
      recursive_registry_save(RegEdBook,KeyPath,f);
    }
    else
    {
      RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
      RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
      
      sprintf(KeyPath,"\r\n[%s]\r\n",RegEdBook->CurrentKeyPath);
      fwrite(f,KeyPath,strlen(KeyPath));
      
      registry_save_value(f,pIRegistryKey,temp_str);
      
      pIRegistryKey->Release();
      RegEdBook->pIRegistry->EndTransaction(false);
    }
    fclose(f);
    
    GUIObject_Destroy(RegEdBook->stringinput_gui);
    RegEdBook->stringinput_gui=0;
  }
}


void registry_list_gui_save_registry_key_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_STRINP_FIXED_TEXT ( TextID_Create("Enter file name:\n",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_TEXT ( TextID_Copy((TEXTID)List_Get(RegEdBook->pRegList,ListMenu_GetSelectedItem(gui))) ),
                                                 VAR_STRINP_MAX_LEN( 39 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_save_registry_key_accept_action ),
                                                 0 );
}


void registry_list_gui_registry_tools_action(BOOK * book, GUI * gui)
{
}


void registry_list_gui_save_gdfs_var_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  RegKey_ValueInfo_t value_info;
  
  wchar_t fpath[30];
  wchar_t fname[45];
  wchar_t temp_wstr[39];
  char temp_str[39];
  char temp_buf;
  char minibuf[100];
  
  memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
  
  TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,ListMenu_GetSelectedItem(gui)),temp_wstr,38);
  wstrcpy(fname,temp_wstr);
  wstrcat(fname,BACKUP_VAR_EXT);
  
  fpath[0]=0;
  wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
  wstrcat(fpath,L"/RegEditor");
  
  if (fstat(fpath,fname,0)>=0) FileDelete(fpath,fname,0);
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey("/gdfs",&pIRegistryKey);
  
  sprintf(temp_str,wstr2str_mask,temp_wstr);
  pIRegistryKey->GetValue(temp_str,Reg_RegValueType_Unspecified,1,&temp_buf,&value_info);
  
  if (value_info.dataSize!=0)
  {
    char * buf_ptr;
    char * bigbuf=0;
      
    if (value_info.dataSize<=sizeof(minibuf)) buf_ptr=minibuf;
    else
    {
      bigbuf = new char[value_info.dataSize];
      buf_ptr=bigbuf;
    }
    pIRegistryKey->GetValue(temp_str,Reg_RegValueType_Unspecified,value_info.dataSize,buf_ptr,&value_info);
    
    int f=_fopen(fpath,fname,0x2,0x180,0);
    fwrite(f,buf_ptr,value_info.dataSize);  //write data
    fclose(f);
    
    if (bigbuf)
    {
      delete bigbuf;
      bigbuf=0;
    }
    
    MessageBox(EMPTY_TEXTID,TextID_Create("GDFS Var saved",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
  }
  else
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("GDFS Var is empty",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
  }
  
  RegEdBook->pIRegistry->EndTransaction(false);
  pIRegistryKey->Release();
}


void registry_list_gui_save_gdfs_accept_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  if (FSX_GetInvalidChar(string))
  {
    int str_id;
    textidname2id(L"DB_ILLEGAL_CHAR_TXT",TEXTID_ANY_LEN,&str_id);
    MessageBox(EMPTY_TEXTID,str_id, NOIMAGE, 0, 2000,0);
    
    FILEITEM * item_desc=FILEITEM_Create();
    wchar_t fname[40];
    wstrcpy(fname,string);
    FILEITEM_SetFname(item_desc, fname);
    FSX_RemoveIllegalSymbolsName(item_desc);
    StringInput_DispObject_SetText(GUIObject_GetDispObject(RegEdBook->stringinput_gui),TextID_Create(FILEITEM_GetFname(item_desc),ENC_UCS2,TEXTID_ANY_LEN));
    FILEITEM_Destroy(item_desc);
  }
  else
  {
    IRegistryKey * pIRegistryKey=0;
    RegKey_ValueInfo_t value_info;
    
    wchar_t temp_wstr[39];
    char temp_str[39];
    char minibuf[100];
    char temp_buf;
    wchar_t fpath[30];
    wchar_t fname[25];
    int count=0;
    int GD_count=0;
    char message_buf[50];
    
    memset(minibuf,0,sizeof(minibuf));
    memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
    
    fpath[0]=0;
    wstrcpy(fpath,GetDir(DIR_OTHER | MEM_EXTERNAL));
    wstrcat(fpath,L"/RegEditor");
    
    fname[0]=0;
    wstrcpy(fname,string);
    wstrcat(fname,BACKUP_EXT);
    
    if (fstat(fpath,fname,0)<0)
    {
      RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
      RegEdBook->pIRegistry->OpenKey("/gdfs",&pIRegistryKey);
      
      int f=_fopen(fpath,fname,0x2,0x180,0);
      
      while (count<RegEdBook->pRegList->FirstFree)
      {
        char * buf_ptr;
        char * bigbuf=0;
        
        TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,count),temp_wstr,38);
        sprintf(temp_str,wstr2str_mask,temp_wstr);
        pIRegistryKey->GetValue(temp_str,Reg_RegValueType_Unspecified,1,&temp_buf,&value_info);
        
        if (value_info.dataSize!=0)
        {
          if (value_info.dataSize<=sizeof(minibuf)) buf_ptr=minibuf;
          else
          {
            bigbuf = new char[value_info.dataSize];
            buf_ptr=bigbuf;
          }
          pIRegistryKey->GetValue(temp_str,Reg_RegValueType_Unspecified,value_info.dataSize,buf_ptr,&value_info);
          int key_len=strlen(temp_str);
          fwrite(f,&key_len,1);                   //write key name len
          fwrite(f,temp_str,key_len+1);           //write key name
          fwrite(f,&value_info.dataSize,2);       //write data len
          fwrite(f,buf_ptr,value_info.dataSize);  //write data
          
          if (bigbuf)
          {
            delete bigbuf;
            bigbuf=0;
          }
          GD_count=GD_count+1;
          value_info.dataSize=0;
        }
        count=count+1;
      }
      char marker=0xFE;
      fwrite(f,&marker,1);                       //write end marker
      fclose(f);
      
      sprintf(message_buf,"Backup complete.\n%d vars saved.",GD_count);
      MessageBox(EMPTY_TEXTID,TextID_Create(message_buf,ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
      
      RegEdBook->pIRegistry->EndTransaction(false);
      pIRegistryKey->Release();
      
      GUIObject_Destroy(RegEdBook->stringinput_gui);
      RegEdBook->stringinput_gui=0;
    }
    else
    {
      MessageBox(EMPTY_TEXTID,TextID_Create("Filename exists",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,2000,RegEdBook);
    }
  }
}


void registry_list_gui_save_gdfs_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_STRINP_FIXED_TEXT ( TextID_Create("Enter file name:\n",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_TEXT ( TextID_Create("backup",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_MAX_LEN( 20 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_save_gdfs_accept_action ),
                                                 0 );
}


void registry_list_gui_gdfs_tools_action(BOOK * book, GUI * gui)
{
}


void registry_list_gui_rename_accept_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  
  wchar_t temp_wstr[39];
  char temp_str[39];
  char new_name[39];
  int res;
  int failed=false;
  
  GUIObject_Show(RegEdBook->registry_list_gui);
  
  TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,RegEdBook->sel_item),temp_wstr,38);
  sprintf(temp_str,wstr2str_mask,temp_wstr);
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  sprintf(new_name,wstr2str_mask,string);
  
  switch(temp_wstr[wstrlen(temp_wstr)+1])
  {
  case 1:
    res=pIRegistryKey->RenameSubkey(temp_str,new_name);
    break;
  case 2:
    res=pIRegistryKey->RenameValue(temp_str,new_name);
    break;
  }
  
  if (res<0)
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Rename failed",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,0,RegEdBook);
    failed=true;
  }
  
  GUIObject_Destroy(RegEdBook->stringinput_gui);
  RegEdBook->stringinput_gui=0;
  
  RegEdBook->pIRegistry->EndTransaction(true);
  
  if (failed==false)
  {
    RegEditor_RecreateList(RegEdBook,RegEdBook->sel_item);
    MessageBox(EMPTY_TEXTID,TextID_Create("Rename successful",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  }
  
  pIRegistryKey->Release();
}


void registry_list_gui_rename_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
   
  wchar_t temp_wstr[39];
  TEXTID fixed_text;
  
  RegEdBook->sel_item=ListMenu_GetSelectedItem(gui);
  
  TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,RegEdBook->sel_item),temp_wstr,38);
  
  switch(temp_wstr[wstrlen(temp_wstr)+1])
  {
  case 1:
    fixed_text=TextID_Create("New key name:\n ",ENC_LAT1,TEXTID_ANY_LEN);
    break;
  case 2:
    fixed_text=TextID_Create("New value name:\n ",ENC_LAT1,TEXTID_ANY_LEN);
    break;
  default:
    return;
  }
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_STRINP_FIXED_TEXT (fixed_text),
                                                 VAR_STRINP_TEXT (TextID_Create(temp_wstr,ENC_UCS2,TEXTID_ANY_LEN)),
                                                 VAR_STRINP_MAX_LEN( 35 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_rename_accept_action ),
                                                 0 );
}


void registry_list_gui_edit_accept_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  RegKey_ValueInfo_t value_info;
  
  int uint, bin_size=0;
  int res;
  bool failed=false;
  char * buf_for_value = new char[0x400];

  memset(buf_for_value,0,0x400);
  memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  pIRegistryKey->GetValue(RegEdBook->value_create_edit,Reg_RegValueType_Unspecified,0x400,buf_for_value,&value_info);
  
  switch(value_info.valueType)
  {
  case Reg_RegValueType_UInt:
    swscanf(string,L"%u",&uint);
    res=pIRegistryKey->SetValue(RegEdBook->value_create_edit,Reg_RegValueType_UInt,0,&uint);
    break;
  case Reg_RegValueType_String:
    sprintf(buf_for_value,wstr2str_mask,string);
    res=pIRegistryKey->SetValue(RegEdBook->value_create_edit,Reg_RegValueType_String,0,buf_for_value);
    break;
  case Reg_RegValueType_WString:
    res=pIRegistryKey->SetValue(RegEdBook->value_create_edit,Reg_RegValueType_WString,0,string);
    break;
  case Reg_RegValueType_Unspecified:
  case Reg_RegValueType_Binary:
    RegEditor_ExtractBinary(string,buf_for_value,&bin_size);
    res=pIRegistryKey->SetValue(RegEdBook->value_create_edit,Reg_RegValueType_Binary,bin_size,buf_for_value);
    break;
  }
  if (res<0)
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Edit failed",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,0,RegEdBook);
    failed=true;
  }
  
  delete buf_for_value;
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(false);
  
  GUIObject_Show(RegEdBook->registry_list_gui);
  GUIObject_Destroy(RegEdBook->stringinput_gui);
  RegEdBook->stringinput_gui=0;
  
  if (failed==true) return;
  
  MessageBox(EMPTY_TEXTID,TextID_Create("Edit successful",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  RegEdBook->reboot_flag=true;
}


void registry_list_gui_edit_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  RegKey_ValueInfo_t value_info;
  
  wchar_t var_arg;
  int var_arg_data;
  wchar_t temp_wstr[39];
  int max_num_chr;
  int input_type;
  
  char * buf_for_display_str = new char[0x1800];
  char * buf_for_value = new char[0x400];
  memset(buf_for_display_str,0,0x1800);
  memset(buf_for_value,0,0x400);
  memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
  
  if (RegEdBook->edit_flag==false)
  {
    RegEdBook->sel_item=ListMenu_GetSelectedItem(RegEdBook->registry_list_gui);
    TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,RegEdBook->sel_item),temp_wstr,38);
    sprintf(RegEdBook->value_create_edit,wstr2str_mask,temp_wstr);
  }
  else RegEdBook->sel_item=0;
  RegEdBook->edit_flag=false;
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  pIRegistryKey->GetValue(RegEdBook->value_create_edit,Reg_RegValueType_Unspecified,0x400,buf_for_value,&value_info);
  
/*  if (RegEdBook->editor_mode==GDFS_MODE&&value_info.valueType==Reg_RegValueType_Unspecified)
  {
    value_info.valueType=Reg_RegValueType_Binary;
    value_info.dataSize=1;
  }*/
  
  switch(value_info.valueType)
  {
  case Reg_RegValueType_UInt:
    max_num_chr=0xA;
    var_arg=VAR_ARG_STRINP_SET_INT_VAL;
    var_arg_data=((int*)buf_for_value)[0];
    input_type=INTEGER;
    break;
  case Reg_RegValueType_String:
    max_num_chr=0x400;
    input_type=STRING;
    var_arg=VAR_ARG_STRINP_TEXT;
    var_arg_data=TextID_Create(buf_for_value,ENC_LAT1,TEXTID_ANY_LEN);
    break;
  case Reg_RegValueType_WString:
    max_num_chr=0x200;
    input_type=STRING;
    var_arg=VAR_ARG_STRINP_TEXT;
    var_arg_data=TextID_Create(buf_for_value,ENC_UCS2,TEXTID_ANY_LEN);
    break;
  case Reg_RegValueType_Unspecified:
    value_info.valueType=Reg_RegValueType_Binary;
    value_info.dataSize=1;
  case Reg_RegValueType_Binary:
    max_num_chr=0x1800;
    input_type=STRING;
    RegEditor_PrintBinary(buf_for_value,value_info.dataSize,buf_for_display_str);
    var_arg=VAR_ARG_STRINP_TEXT;
    var_arg_data=TextID_Create(buf_for_display_str,ENC_LAT1,TEXTID_ANY_LEN);
    break;
  }
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 &var_arg,var_arg_data,
                                                 VAR_STRINP_MAX_LEN( max_num_chr ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( input_type ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_edit_accept_action ),
                                                 0 );
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(false);
  
  delete buf_for_display_str;
  delete buf_for_value;
}


void RegEditor_SetValue(RegistryEditorBook * RegEdBook,char * value_create_edit,Reg_RegValueType_t value_type,void * data,int datasize)
{
  IRegistryKey * pIRegistryKey=0;
  bool failed=false;
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  
  if (pIRegistryKey->SetValue(value_create_edit,value_type,datasize,data)<0)
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Value creation failed",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,0,RegEdBook);
    failed=true;
  }
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(true);
  
  if (failed==true) return;
  
  RegEditor_RecreateList(RegEdBook,0);
  MessageBox(EMPTY_TEXTID,TextID_Create("Value created",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  RegEdBook->reboot_flag=true;
}


void registry_list_gui_create_value_accept_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  int uint, bin;
  int res;
  char * temp_str;
  
  GUIObject_Show(RegEdBook->registry_list_gui);
  
  switch(RegEdBook->value_type)
  {
  case Reg_RegValueType_UInt:
    res=swscanf(string,L"%u",&uint);
    if (res==0||res==-1) return;
    RegEditor_SetValue(RegEdBook,RegEdBook->value_create_edit,RegEdBook->value_type,&uint,0);
    break;
  case Reg_RegValueType_String:
    temp_str=new char[len+1];
    sprintf(temp_str,wstr2str_mask,string);
    RegEditor_SetValue(RegEdBook,RegEdBook->value_create_edit,RegEdBook->value_type,temp_str,0);
    delete temp_str;
    break;
  case Reg_RegValueType_WString:
    RegEditor_SetValue(RegEdBook,RegEdBook->value_create_edit,RegEdBook->value_type,string,0);
    break;
  case Reg_RegValueType_Binary:
    res=swscanf(string,L"%u",&bin);
    if (res==0||res==-1) return;
    temp_str=new char[bin];
    memset(temp_str,0,bin);
    RegEditor_SetValue(RegEdBook,RegEdBook->value_create_edit,RegEdBook->value_type,temp_str,bin);
    
    GUIObject_Destroy(RegEdBook->stringinput_gui);
    RegEdBook->stringinput_gui=0;
    
    RegEdBook->edit_flag=true;
    registry_list_gui_edit_action(RegEdBook,RegEdBook->registry_list_gui);
    delete temp_str;
    return;
  }
  
  GUIObject_Destroy(RegEdBook->stringinput_gui);
  RegEdBook->stringinput_gui=0;
}


bool is_name_exists(RegistryEditorBook * RegEdBook,wchar_t * key_name,ELEM_ID elem_type)
{
  int count=0;
  wchar_t temp_wstr[39];
  
  while (count<RegEdBook->pRegList->FirstFree)
  {
    TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,count),temp_wstr,38);
    if ((temp_wstr[wstrlen(temp_wstr)+1]==elem_type)&&(!wstrcmp(key_name,temp_wstr))) return true;
    count=count+1;
  }
  
  return false;
}


void registry_list_gui_create_value_entered_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  wchar_t var_arg;
  int var_arg_data;
  int input_type;
  char * fixed_text;
  
  if (is_name_exists(RegEdBook,string,VALUE)==false)
  {
    sprintf(RegEdBook->value_create_edit,"%ls",string);
    
    GUIObject_Show(RegEdBook->registry_list_gui);
    GUIObject_Destroy(RegEdBook->stringinput_gui);
    RegEdBook->stringinput_gui=0;
    
    if (RegEdBook->value_type==Reg_RegValueType_UInt||RegEdBook->value_type==Reg_RegValueType_Binary) input_type=INTEGER;
    else input_type=STRING;
    
    switch(RegEdBook->value_type)
    {
    case Reg_RegValueType_UInt:
      var_arg=VAR_ARG_STRINP_MAX_LEN;
      var_arg_data=0xA;
      fixed_text="Enter number:\n ";
      break;
    case Reg_RegValueType_String:
      var_arg=VAR_ARG_STRINP_MAX_LEN;
      var_arg_data=0x400;
      fixed_text="Enter string:\n ";
      break;
    case Reg_RegValueType_WString:
      var_arg=VAR_ARG_STRINP_MAX_LEN;
      var_arg_data=0x200;
      fixed_text="Enter string:\n ";
      break;
    case Reg_RegValueType_Binary:
      var_arg=VAR_ARG_STRINP_MAX_INT_VAL;
      var_arg_data=0x400;
      fixed_text="Data size in bytes:\n ";
      break;
    }
    
    RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                   VAR_STRINP_FIXED_TEXT( TextID_Create(fixed_text,ENC_LAT1,TEXTID_ANY_LEN) ),
                                                   &var_arg,var_arg_data,
                                                   VAR_STRINP_MODE( input_type ),
                                                   VAR_BOOK( RegEdBook ),
                                                   VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                   VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                   VAR_OK_PROC( registry_list_gui_create_value_accept_action ),
                                                   0 );
  }
  else
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Value name exists",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  }
}


void registry_list_gui_create_value(BOOK * book,Reg_RegValueType_t type)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->value_type=type;
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_STRINP_FIXED_TEXT( TextID_Create("Value name:\n ",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_MAX_LEN( 35 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_create_value_entered_action ),
                                                 0 );
}


void registry_list_gui_more_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  wchar_t temp_wstr[39];
  bool rename;
  bool edit;
  bool save;
  
  if (RegEdBook->editor_mode==REGISTRY_MODE)
  {
    TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,ListMenu_GetSelectedItem(gui)),temp_wstr,38);
    
    switch(temp_wstr[wstrlen(temp_wstr)+1])
    {
    case KEY:
      rename=true;
      edit=false;
      save=true;
      break;
    case VALUE:
      rename=true;
      edit=true;
      save=true;
      break;
    case EMPTY:
      rename=false;
      edit=false;
      save=false;
      break;
    default:
      return;
    }
    
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,RENAME_ACTION,rename);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,EDIT_ACTION,edit);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,REGISTRY_EXPORT_ACTION,save);
  }
}


void registry_list_gui_editor_mode_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->CurrentKeyPath[0]=0;
  if (RegEdBook->editor_mode==REGISTRY_MODE)
  {
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,RENAME_ACTION,false);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,CREATE_KEY_ACTION,false);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,false);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,EDIT_ACTION,true);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,true);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,REGISTRY_EXPORT_ACTION,false);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,TextID_Create("Registry Editor",ENC_LAT1,TEXTID_ANY_LEN));
    
    strcpy(RegEdBook->CurrentKeyPath,"/gdfs");
    RegEdBook->editor_mode=GDFS_MODE;
  }
  else
  {
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,CREATE_KEY_ACTION,true);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,true);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,false);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,REGISTRY_EXPORT_ACTION,true);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,TextID_Create("GDFS Editor",ENC_LAT1,TEXTID_ANY_LEN));
    
    RegEdBook->CurrentKeyPath[0]='/';
    RegEdBook->CurrentKeyPath[1]=0;
    RegEdBook->editor_mode=REGISTRY_MODE;
  }
  RegEditor_RecreateList(RegEdBook,0);
}


void registry_list_gui_sorting_nosorting_action(BOOK * book, GUI * gui)
{
  sort_order_global=Nosort;
  RegEditor_Sort(book,"No sorting");
}


void registry_list_gui_sorting_descending_action(BOOK * book, GUI * gui)
{
  sort_order_global=Descending_order;
  RegEditor_Sort(book,"Descending sorting");
}


void registry_list_gui_sorting_ascending_action(BOOK * book, GUI * gui)
{
  sort_order_global=Ascending_order;
  RegEditor_Sort(book,"Ascending sorting");
}


void registry_list_gui_sorting_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,EDIT_ACTION,false);
}


void registry_list_gui_create_uint_action(BOOK * book, GUI * gui)
{
  registry_list_gui_create_value(book,Reg_RegValueType_UInt);
}


void registry_list_gui_create_string_action(BOOK * book, GUI * gui)
{
  registry_list_gui_create_value(book,Reg_RegValueType_String);
}


void registry_list_gui_create_wstring_action(BOOK * book, GUI * gui)
{
  registry_list_gui_create_value(book,Reg_RegValueType_WString);
}


void registry_list_gui_create_binary_action(BOOK * book, GUI * gui)
{
  registry_list_gui_create_value(book,Reg_RegValueType_Binary);
}


void registry_list_gui_create_value_action(BOOK * book, GUI * gui)
{
}


void registry_list_gui_create_key_entered_action(BOOK * book, wchar_t * string, int len)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  char temp_str[39];
  bool failed=false;
  
  if (is_name_exists(RegEdBook,string,KEY)==false)
  {
    sprintf(temp_str,wstr2str_mask,string);
    
    GUIObject_Show(RegEdBook->registry_list_gui);
    GUIObject_Destroy(RegEdBook->stringinput_gui);
    RegEdBook->stringinput_gui=0;
    
    RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
    RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
    
    if (pIRegistryKey->CreateSubkey(temp_str,false)<0)
    {
      MessageBox(EMPTY_TEXTID,TextID_Create("Key creation failed",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,0,RegEdBook);
      failed=true;
    }
    pIRegistryKey->Release();
    RegEdBook->pIRegistry->EndTransaction(true);
  }
  else
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Key name exists",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
    failed=true;
  }
  
  if (failed==true) return;
  
  RegEditor_RecreateList(RegEdBook,0);
  MessageBox(EMPTY_TEXTID,TextID_Create("Key successfully created",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  RegEdBook->reboot_flag=true;
}


void registry_list_gui_create_key_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  RegEdBook->stringinput_gui=CreateStringInputVA(0,
                                                 VAR_STRINP_FIXED_TEXT( TextID_Create("Key name:\n ",ENC_LAT1,TEXTID_ANY_LEN) ),
                                                 VAR_STRINP_MAX_LEN( 35 ),
                                                 VAR_STRINP_MIN_LEN( 1 ),
                                                 VAR_STRINP_MODE( STRING ),
                                                 VAR_BOOK( RegEdBook ),
                                                 VAR_PREV_ACTION_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_LONG_BACK_PROC( registry_list_gui_stringinput_cancel_action ),
                                                 VAR_OK_PROC( registry_list_gui_create_key_entered_action ),
                                                 0 );
}


void registry_list_gui_delete_no_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  if (RegEdBook->registry_list_gui)
  {
    GUIObject_Show(RegEdBook->registry_list_gui);
    GUIObject_Destroy(RegEdBook->yesno_gui);
    RegEdBook->yesno_gui=0;
  }
  else 
  {
    GUIObject_Destroy(RegEdBook->yesno_gui);
    RegEdBook->yesno_gui=0;
    FreeBook(book);
  }
}


void registry_list_gui_delete_yes_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  wchar_t temp_wstr[39];
  char temp_str[39];
  int res=0;
  int failed=false;
  
  GUIObject_Show(RegEdBook->registry_list_gui);
  GUIObject_Destroy(RegEdBook->yesno_gui);
  RegEdBook->yesno_gui=0;
  
  TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,RegEdBook->sel_item),temp_wstr,38);
  sprintf(temp_str,wstr2str_mask,temp_wstr);
  
  wchar_t chr=temp_wstr[wstrlen(temp_wstr)+1];
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  
  if (chr==KEY) res=pIRegistryKey->DeleteSubkey(temp_str);
  if (chr==VALUE)res=pIRegistryKey->DeleteValue(temp_str);
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(true);
  
  if (res<0) 
  {
    MessageBox(EMPTY_TEXTID,TextID_Create("Delete failed",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,0,RegEdBook);
    failed=true;
  }
  if (failed==true) return;
  
  RegEditor_RecreateList(RegEdBook,RegEdBook->sel_item);
  MessageBox(EMPTY_TEXTID,TextID_Create("Delete successful",ENC_LAT1,TEXTID_ANY_LEN),NOIMAGE,0,750,RegEdBook);
  RegEdBook->reboot_flag=true;
}


void registry_list_gui_delete_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  int sel_item;
  wchar_t temp_wstr[39];
  char * question;
  
  sel_item=ListMenu_GetSelectedItem(gui);
  TEXTID element=(TEXTID)List_Get(RegEdBook->pRegList,sel_item);
  TextID_GetWString(element,temp_wstr,38);
  wchar_t chr=temp_wstr[wstrlen(temp_wstr)+1];
  
  if (chr==KEY) question="Deleting a key will remove all subkeys and values. Proceed?";
  else if (chr==VALUE) question="Delete value?";
  else return;
  
  RegEdBook->sel_item=sel_item;
  RegEdBook->yesno_gui = CreateYesNoQuestionVA( 0,
                                             VAR_BOOK( RegEdBook ),
                                             VAR_YESNO_PRE_QUESTION( TextID_Create( temp_wstr, ENC_UCS2, TEXTID_ANY_LEN ) ),
                                             VAR_YESNO_QUESTION( TextID_Create( question, ENC_LAT1, TEXTID_ANY_LEN ) ),
                                             0 );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_YES, registry_list_gui_delete_yes_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_NO, registry_list_gui_delete_no_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_BACK, registry_list_gui_delete_no_action );
  GUIObject_SoftKeys_SetAction( RegEdBook->yesno_gui, ACTION_LONG_BACK, registry_list_gui_delete_no_action );
}


void registry_list_gui_cancel_action(BOOK * book, GUI * gui)
{
  RegEditor_Exit(book);
}


void registry_list_gui_prev_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  int res;
  
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  res=pIRegistryKey->OpenSubkey("..");
  
  //If root key
  if (res&&REGISTRY_UED_KEY_NOT_FOUND)
  {
    pIRegistryKey->Release();
    RegEdBook->pIRegistry->EndTransaction(false);
    RegEditor_Exit(book);
    return;
  }
  
  //if subkey
  if (res>=0)
  {
    //Set current key to the prev key
    pIRegistryKey->GetKeyPath(sizeof(RegEdBook->CurrentKeyPath),RegEdBook->CurrentKeyPath);
    RegEdBook->pIRegistry->EndTransaction(false);
    
    RegEditor_RecreateList(RegEdBook,(int)List_RemoveAt(RegEdBook->Stack_List,0));
  }
  
  RegEdBook->pIRegistry->EndTransaction(false);
  pIRegistryKey->Release();
}


void registry_list_gui_select_action(BOOK * book, GUI * gui)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  IRegistryKey * pIRegistryKey=0;
  wchar_t temp_wstr[39];
  char temp_str[39];
  
  int sel_item=ListMenu_GetSelectedItem(gui);
  RegEdBook->pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  RegEdBook->pIRegistry->OpenKey(RegEdBook->CurrentKeyPath,&pIRegistryKey);
  
  TextID_GetWString((TEXTID)List_Get(RegEdBook->pRegList,sel_item),temp_wstr,38);
  sprintf(temp_str,wstr2str_mask,temp_wstr);
    
  wchar_t temp_chr=temp_wstr[wstrlen(temp_wstr)+1];
  
  //If selected item <empty>
  if (temp_chr==EMPTY)
  {
    pIRegistryKey->Release();
    RegEdBook->pIRegistry->EndTransaction(false);
    return;
  }
  
  //If selected item value
  if (temp_chr==VALUE)
  {
    pIRegistryKey->Release();
    RegEdBook->sel_item=sel_item;
    RegEdBook->pIRegistry->EndTransaction(false);
    RegEditor_DisplayValue(temp_str,RegEdBook);
    return;
  }
  
  //If selected item key
  //Prepare new key
  pIRegistryKey->OpenSubkey(temp_str);
  pIRegistryKey->GetKeyPath(sizeof(RegEdBook->CurrentKeyPath),RegEdBook->CurrentKeyPath);
  RegEdBook->pIRegistry->EndTransaction(false);
  
  //Save history of selected items
  List_InsertFirst(RegEdBook->Stack_List,(void*)sel_item);
  
  RegEditor_RecreateList(RegEdBook,0);
  
  pIRegistryKey->Release();
  RegEdBook->pIRegistry->EndTransaction(false);
}


int registry_list_gui_callback(GUI_MESSAGE * msg)
{
  wchar_t temp_wstr[39];
  int imgID=NOIMAGE;
  if (GUIonMessage_GetMsg(msg)==LISTMSG_GetItem)
  {
    RegistryEditorBook * RegEdBook = (RegistryEditorBook*)GUIonMessage_GetBook(msg);
    TEXTID element = (TEXTID)List_Get(RegEdBook->pRegList,GUIonMessage_GetCreatedItemIndex(msg));
    TextID_GetWString(element,temp_wstr,38);
    wchar_t temp_chr = temp_wstr[wstrlen(temp_wstr)+1];
    GUIonMessage_SetMenuItemText(msg,TextID_Copy(element));
    if (temp_chr==KEY) iconidname2id(KEY_ICON,TEXTID_ANY_LEN,&imgID);
    if (temp_chr==VALUE) iconidname2id(VALUE_ICON,TEXTID_ANY_LEN,&imgID);
    GUIonMessage_SetMenuItemIcon(msg,0,imgID);
    return 1;
  }
  return 0;
}


int RegistryEditorBook_MainPage_Enter(void * r0,BOOK * book)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  
  //Create interfaces start
  ISEMCRegistryManager * pISEMCRegistryManager=0;
  CoCreateInstance(&CID_CSEMCRegistryManager,&IID_ISEMCRegistryManager,PPINTERFACE(&pISEMCRegistryManager));
  if (pISEMCRegistryManager)
  {
    pISEMCRegistryManager->CreateRegistry(&RegEdBook->pIRegistry);
    pISEMCRegistryManager->Release();
    //Create interfaces end
    
    //Fill RegList start
    RegEdBook_Fill_RegList(RegEdBook->CurrentKeyPath,RegEdBook);
    //Fill RegList end
    
    RegEdBook->registry_list_gui=CreateListMenu(RegEdBook,0);
    ListMenu_SetOnMessage(RegEdBook->registry_list_gui,registry_list_gui_callback);
    ListMenu_SetItemStyle(RegEdBook->registry_list_gui,0);
    ListMenu_SetCursorToItem(RegEdBook->registry_list_gui,0);
    ListMenu_SetItemCount(RegEdBook->registry_list_gui,RegEdBook->pRegList->FirstFree);
    GUIObject_SetTitleText(RegEdBook->registry_list_gui,TextID_Create(RegEdBook->CurrentKeyPath,ENC_LAT1,TEXTID_ANY_LEN));
        
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,ACTION_BACK,registry_list_gui_prev_action);
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,ACTION_LONG_BACK,registry_list_gui_cancel_action);
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,ACTION_SELECT1,registry_list_gui_select_action);
    //Delete action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,ACTION_DELETE,registry_list_gui_delete_action);
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,ACTION_DELETE,false);
    //Create key action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_KEY_ACTION,registry_list_gui_create_key_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_KEY_ACTION,TextID_Create("Create key",ENC_LAT1,TEXTID_ANY_LEN));
    //Create value action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,registry_list_gui_create_value_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,TextID_Create("Create value",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_UINT_ACTION,registry_list_gui_create_uint_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_UINT_ACTION,TextID_Create("Unsigned int",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_STRING_ACTION,registry_list_gui_create_string_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_STRING_ACTION,TextID_Create("String",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_WSTRING_ACTION,registry_list_gui_create_wstring_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_WSTRING_ACTION,TextID_Create("Wide string",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,CREATE_BINARY_ACTION,registry_list_gui_create_binary_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,CREATE_BINARY_ACTION,TextID_Create("Binary",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,CREATE_UINT_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,CREATE_STRING_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,CREATE_WSTRING_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,CREATE_VALUE_ACTION,CREATE_BINARY_ACTION);
    //Create edit action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,EDIT_ACTION,registry_list_gui_edit_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,EDIT_ACTION,TextID_Create("Edit",ENC_LAT1,TEXTID_ANY_LEN));
    //Create rename action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,RENAME_ACTION,registry_list_gui_rename_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,RENAME_ACTION,TextID_Create("Rename",ENC_LAT1,TEXTID_ANY_LEN));
    //Create sorting action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,SORTING_ACTION,registry_list_gui_sorting_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,SORTING_ACTION,TextID_Create("Sorting",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,SORTING_ASCENDING_ACTION,registry_list_gui_sorting_ascending_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,SORTING_ASCENDING_ACTION,TextID_Create("Ascending",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,SORTING_DESCENDING_ACTION,registry_list_gui_sorting_descending_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,SORTING_DESCENDING_ACTION,TextID_Create("Descending",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,SORTING_NOSORT_ACTION,registry_list_gui_sorting_nosorting_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,SORTING_NOSORT_ACTION,TextID_Create("None",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,SORTING_ACTION,SORTING_ASCENDING_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,SORTING_ACTION,SORTING_DESCENDING_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,SORTING_ACTION,SORTING_NOSORT_ACTION);
    //Create GDFS Tools action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,registry_list_gui_gdfs_tools_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,TextID_Create("GDFS Tools",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,false);
    //Create Save GDFS action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,GDFS_SAVE_ACTION,registry_list_gui_save_gdfs_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,GDFS_SAVE_ACTION,TextID_Create("Save GDFS",ENC_LAT1,TEXTID_ANY_LEN));
    //Create Restore GDFS action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,GDFS_RESTORE_ACTION,registry_list_gui_restore_gdfs_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,GDFS_RESTORE_ACTION,TextID_Create("Restore GDFS",ENC_LAT1,TEXTID_ANY_LEN));
    //Create Save GDFS Var action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,GDFS_SAVE_VAR_ACTION,registry_list_gui_save_gdfs_var_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,GDFS_SAVE_VAR_ACTION,TextID_Create("Save GDFS Var",ENC_LAT1,TEXTID_ANY_LEN));
    //Create Restore GDFS Var action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,GDFS_RESTORE_VAR_ACTION,registry_list_gui_restore_gdfs_var_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,GDFS_RESTORE_VAR_ACTION,TextID_Create("Restore GDFS Var",ENC_LAT1,TEXTID_ANY_LEN));
    
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,GDFS_SAVE_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,GDFS_RESTORE_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,GDFS_SAVE_VAR_ACTION);
    GUIObject_SoftKeys_SetItemAsSubItem(RegEdBook->registry_list_gui,GDFS_TOOLS_ACTION,GDFS_RESTORE_VAR_ACTION);
    //Create Registry Save action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,REGISTRY_EXPORT_ACTION,registry_list_gui_save_registry_key_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,REGISTRY_EXPORT_ACTION,TextID_Create("Export",ENC_LAT1,TEXTID_ANY_LEN));
    
    //Create GDFS Editor action
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,registry_list_gui_editor_mode_action);
    GUIObject_SoftKeys_SetText(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,TextID_Create("GDFS Editor",ENC_LAT1,TEXTID_ANY_LEN));
    GUIObject_SoftKeys_SetVisible(RegEdBook->registry_list_gui,EDITOR_MODE_ACTION,RegEdBook->is_A2);
    //Refresh softkey list when press More
    GUIObject_SoftKeys_SetAction(RegEdBook->registry_list_gui,ACTION_MORELIST,registry_list_gui_more_action);
    
    GUIObject_Show(RegEdBook->registry_list_gui);
  }
  else
  {
    MessageBox(EMPTY_TEXTID, STR("Can't create ISEMCRegistryManager\nShutdown elf"), NOIMAGE, 1, 2000, 0);
    FreeBook(book);
  }
  return 0;
}


void RegEdBook_Destroy( BOOK * book )
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)book;
  if (RegEdBook->registry_list_gui) GUIObject_Destroy(RegEdBook->registry_list_gui);
  if (RegEdBook->yesno_gui) GUIObject_Destroy(RegEdBook->yesno_gui);
  if (RegEdBook->stringinput_gui) GUIObject_Destroy(RegEdBook->stringinput_gui);
  
  if (RegEdBook->pRegList)
  {
    RegList_DestroyElements(RegEdBook->pRegList);
    List_Destroy(RegEdBook->pRegList);
  }
  
  if (RegEdBook->Stack_List) List_Destroy(RegEdBook->Stack_List);
  
  if (RegEdBook->pIRegistry) RegEdBook->pIRegistry->Release();
  
  if (RegEdBook->dll) UnLoadDLL(RegEdBook->dll);
  
  SUBPROC(elf_exit);
}

void CreateRegistryEditorBook(wchar_t * fpath,wchar_t * fname)
{
  RegistryEditorBook * RegEdBook = (RegistryEditorBook*)malloc(sizeof(RegistryEditorBook));
  CreateBook(RegEdBook,RegEdBook_Destroy,&RegistryEditorBook_Base_Page,"Registry Editor",-1,0);
  RegEdBook->pRegList=List_Create();
  RegEdBook->pIRegistry=0;
  RegEdBook->Stack_List=List_Create();
  RegEdBook->registry_list_gui=0;
  RegEdBook->yesno_gui=0;
  RegEdBook->stringinput_gui=0;
  RegEdBook->edit_flag=false;
  RegEdBook->reboot_flag=false;
  RegEdBook->dll=0;
  
  int platform=GetChipID()&CHIPID_MASK;
  if (platform==CHIPID_DB3150||platform==CHIPID_DB3200||platform==CHIPID_DB3210||platform==CHIPID_DB3350) RegEdBook->is_A2=true;
  else RegEdBook->is_A2=false;
  
  if (fpath&&fname)
  {
    if (registry_restore(fpath,fname)>0) RegEdBook->reboot_flag=true;
    RegEditor_Exit(RegEdBook);
  }
  else
  {
    RegEdBook->editor_mode=REGISTRY_MODE;
    
    RegEdBook->CurrentKeyPath[0]='/';
    RegEdBook->CurrentKeyPath[1]=0;
    
    BookObj_GotoPage(RegEdBook,&RegistryEditorBook_Main_Page);
    
//    if (platform==CHIPID_DB3150) MessageBox(EMPTY_TEXTID, STR("Your phone is db3150\nReboot may take place"), NOIMAGE, 1, 0, 0);
  }
}

int isRegistryEditorBook(BOOK* book)
{
  if(!strcmp(book->xbook->name,"Registry Editor")) return(1);
  return(0);
}


int main (wchar_t * elfname,wchar_t * fpath,wchar_t * fname)
{
  if (fpath&&fname)
  {
    CreateRegistryEditorBook(fpath,fname);
  }
  else
  {
    if (FindBook(isRegistryEditorBook))
    {
      MessageBox(EMPTY_TEXTID, STR("Already runed"), NOIMAGE, 1, 5000, 0);
      SUBPROC(elf_exit);
    }
    else
    {
      CreateRegistryEditorBook(0,0);
    }
  }
  return 0;;
}
