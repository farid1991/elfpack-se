#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "extern.h"

GUI_LIST *CreateFileFolderSelect(MyBOOK * myBook, wchar_t *str);

enum SELF {
  SFOLDER=0,
  SFILE=1
};
#define LGP_NULL 0x6FFFFFFF
int type=SFOLDER;
//выбор папки


typedef struct
{
  void *next;
  int is_folder;
  wchar_t *fullname;
  wchar_t *name;
}FLIST;


volatile FLIST *fltop=NULL;

enum TYPES {IS_BACK, IS_FOLDER, IS_FILE};
const wchar_t back[]=L"..";

void Free_FLIST(void)
{
  FLIST *fl=(FLIST *)fltop;
  fltop=0;
  while(fl)
  {
    FLIST *fl_prev=fl;
    delete fl_prev->fullname;
    delete fl_prev->name;
    fl=(FLIST *)fl->next;
    delete fl_prev;
  }
}

#define FREE_FLGUI(a) Free_FLIST();\
  FREE_GUI(a);

FLIST *AddToFList(const wchar_t* full_name, const wchar_t *name, int is_folder)
{
  //int l_fname;
  FLIST *fl;
  FLIST *fn=new FLIST;
  fn->fullname=new wchar_t[wstrlen(full_name)+1];
  fn->name=new wchar_t[wstrlen(name)+1];
  wstrcpy(fn->fullname,full_name);
  wstrcpy(fn->name,name);
  fn->is_folder=is_folder;
  fn->next=0;
  fl=(FLIST *)fltop;
  if (fl)
  {
    FLIST *pr;
    pr=(FLIST *)&fltop;
    while(wstrcmpi(fl->name,fn->name)<0)
    {
      pr=fl;
      fl=(FLIST *)fl->next;
      if (!fl) break;
    }
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fltop=fn;
  }
  return (fn);
}

int FindFiles(wchar_t *str, int type)  // type == 0 SelectFolder, type == 1 SelectFile
{
  int i, c, n=0;
  Free_FLIST();
  wchar_t *path=new wchar_t[256];
  wchar_t *name=new wchar_t[64];
  wchar_t *rev=NULL, *d=name, *s=str;
  while((c=*s++))
  {
    *d=c;
    if (c=='/' && *s!='\0') rev=d;
    d++;
  }
  if(rev)
  {
    if (rev==name)  // Если идем на корень
      *(rev+1)=0;
    else
      *rev=0;
    AddToFList(name,back,IS_BACK);
    n++;
  }
  void *dir=w_diropen(str);
  if (dir)
  {
    wchar_t *next;
    w_chdir(str);
    while((next=w_dirread(dir)))
    {
      W_FSTAT fs;
      w_fstat(next,&fs);
      i=wstrlen(str);
      wstrcpy(path,str);
      if (rev)
      {
        path[i++]='/';
      }
      wstrcpy(path+i,next);
      if (fs.attr&0x4000)
      {
        snwprintf(name,63,L"[%ls]",next);
        AddToFList(path,name,IS_FOLDER);
        n++;
      }
      else
      {
        if (type==SFILE)
        {
          AddToFList(path,next,IS_FILE);
          n++;
        }
      }
    }
    w_dirclose(dir);
  }
  delete name;
  delete path;
  return n;
}

FLIST *FindFLISTtByNS(int *i, int si)
{
  FLIST *fl;
  fl=(FLIST *)fltop;
  while(fl)
  {
    if (fl->is_folder==si)
    {
      if (!(*i)) return (fl);
      (*i)--;
    }
    fl=(FLIST *)fl->next;
  }
  return fl;
}

FLIST *FindFLISTtByN(int n)
{
  FLIST *fl;
  fl=FindFLISTtByNS(&n,IS_BACK); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FOLDER); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FILE); if ((!n)&&(fl)) return(fl);
  return fl;
}


int OnMessage(GUI_MESSAGE * msg)
{
  int d;
  TEXTID str=LGP_NULL;
  FLIST *f;
  switch(GUIonMessage_GetMsg(msg))
  {
  case 1:
    d=GUIonMessage_GetCreatedItemIndex(msg);
    f=FindFLISTtByN(d);
    str=TextID_Create(f->name,ENC_UCS2,TEXTID_ANY_LEN);
    GUIonMessage_SetMenuItemText(msg,str);
    break;
  }
  return(1);
};

void Self_OnBack(BOOK * bk, GUI *)
{
  MyBOOK * myBook=(MyBOOK *)bk;
  FREE_FLGUI(myBook->selectf);
}

void Self_onEnterPressed(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(mbk->selectf);
  FLIST *fl=FindFLISTtByN(item);
  if (fl)
  {
    wchar_t *path=new wchar_t[256];
    if (fl->is_folder==IS_FOLDER || fl->is_folder==IS_BACK)
    {
      wstrncpy(path, fl->fullname,255);
      FREE_FLGUI(mbk->selectf);
      mbk->selectf=CreateFileFolderSelect(mbk, path);
    }
    else
    {
      FREE_FLGUI(mbk->selectf);
    }
    delete path;
  }
}

void Self_onSelectPressed(BOOK * bk, GUI *)
{
  int item=ListMenu_GetSelectedItem(mbk->selectf);

  FLIST *fl=FindFLISTtByN(item);
  if (fl)
  {
    if ((fl->is_folder==IS_FOLDER && type==SFOLDER) || fl->is_folder==IS_FILE)
    {
      ITEM *it=Menu_GetSelected();
      DELETE(it->link);
      it->link=new wchar_t[wstrlen(fl->fullname)+1];
      wstrcpy(it->link,fl->fullname);
      FREE_FLGUI(mbk->selectf);
    }
  }
}

int isdir(wchar_t *name){
  W_FSTAT fs;
  if (w_fstat(name,&fs)!=-1)
    return(fs.attr&0x4000);
  else return 0;
}

GUI_LIST *CreateFileFolderSelect(MyBOOK * myBook, wchar_t *str)
{
  wchar_t *ustr=new wchar_t[256];
  wchar_t *s;
  GUI_LIST *lo;
  int n;
  TEXTID sid=TextID_Create(type==SFOLDER?L"Select folder":L"Select file",ENC_UCS2,TEXTID_ANY_LEN);
  lo=CreateListMenu(&myBook->book,0);
  GUIObject_SetTitleText(lo,sid);
  wstrcpy(ustr,str);
  str=ustr;






  int f=0;
  do
  {
    if (isdir(ustr))
    {
      f=1;
      break;
    }
    s=wstrrchr(ustr,L'/');
    if (s==ustr) break;
    if (s) *s=0;
  }
  while(s);
  if (!f)
    str=Slash;

  n=FindFiles(str,type);
  ListMenu_SetItemCount(lo,n);
  ListMenu_SetCursorToItem(lo,0);
  ListMenu_SetOnMessage(lo,OnMessage);
  GUIObject_SoftKeys_SetAction(lo,ACTION_SELECT1,Self_onEnterPressed);
  GUIObject_SoftKeys_SetText(lo,ACTION_SELECT1,TextID_Create(L"Open",ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(lo,ACTION_BACK,Self_OnBack);
  GUIObject_SoftKeys_SetAction(lo,1,Self_onSelectPressed);
  GUIObject_SoftKeys_SetText(lo,1,TextID_Create(L"Select",ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_Show(lo);
  delete ustr;
  return (lo);
}
