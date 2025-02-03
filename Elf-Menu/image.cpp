#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"

int GetIconID(wchar_t *txt)
{
  int id=0xFFFF;
  iconidname2id(txt,0xFFFFFFFF,&id);
  return id;
}

void Reg_Image(IMAGEID*img,wchar_t*path,wchar_t*name)
{
  ImageID_Get(path,name,img);
}
void Icon_Reg(ITEM*it,wchar_t*name,int is_sel)
{
  FSTAT stat;
  if(!is_sel)
  {
    if(fstat(skin_path_int,name,&stat)==0)
      Reg_Image(&it->image,skin_path_int,name);
    else if(fstat(skin_path_ext,name,&stat)==0)
      Reg_Image(&it->image,skin_path_ext,name);
    else
      it->image=NOIMAGE;
  }
  else
  {
    it->simage=NOIMAGE;
    if(Menu_CheckStyle())
    {
      if(fstat(skin_path_int,name,&stat)==0)
        Reg_Image(&it->simage,skin_path_int,name);
      else if(fstat(skin_path_ext,name,&stat)==0)
        Reg_Image(&it->simage,skin_path_ext,name);
    }
  }
}

void Icon_Unreg(ITEM*it, int is_sel)
{
  if(!is_sel)
  {
    IDFREE(it->image);
  }
  else
  {
    IDFREE(it->simage);
  }
}

//режим дата-браузера для выбора картинки
//0 - невыделенная
//1 - выделенная
//2 - курсор

int IMAGEDB_MODE=0;

int onAcceptImageDB(void * data, BOOK * book)
{
  FILEITEM* fi = (FILEITEM*)data;
  wchar_t*fname=FILEITEM_GetFname(fi);
  ITEM*it=Menu_GetSelected();

  switch(IMAGEDB_MODE)
  {
  case 0:
    {
      DELETE(it->img);
      it->img=new wchar_t[wstrlen(fname)+1];
      wstrcpy(it->img,fname);

      Icon_Unreg(it,0);
      Icon_Reg(it,it->img,0);

      if(Menu_CheckStyle())
      {
        if(!GetImageWidth(it->simage)||!GetImageHeight(it->simage))
        {
          DELETE(it->simg);
          it->simg=new wchar_t[wstrlen(fname)+1];
          wstrcpy(it->simg,fname);
          Icon_Unreg(it,1);
          Icon_Reg(it,it->simg,1);
        }
      }
      else
      {
        Icon_Unreg(it,1);
        DELETE(it->simg);
      }
      break;
    }
  case 1:
    {
      DELETE(it->simg);
      it->simg=new wchar_t[wstrlen(fname)+1];
      wstrcpy(it->simg,fname);
      Icon_Unreg(it,1);
      Icon_Reg(it,it->simg,1);
      break;
    }
  case 2:
    {
      DELETE(Menu_Get()->cursor_img);
      Menu_Get()->cursor_img=new wchar_t[wstrlen(fname)+1];
      wstrcpy(Menu_Get()->cursor_img,fname);
      FSTAT stat;
      IDFREE(Menu_Get()->cursorID);
      if(fstat(skin_path_int,Menu_Get()->cursor_img,&stat)==0)
        Reg_Image(&Menu_Get()->cursorID,skin_path_int,Menu_Get()->cursor_img);
      else if(fstat(skin_path_ext,Menu_Get()->cursor_img,&stat)==0)
        Reg_Image(&Menu_Get()->cursorID,skin_path_ext,Menu_Get()->cursor_img);
      else
        Menu_Get()->cursorID=NOIMAGE;
      break;
    }
  }
  Book_Refresh();
  SetRefresh();
  return(0);

}

int CreateImageDB(void *data, BOOK * book)
{
  void * DB_Desc=DataBrowserDesc_Create();
  const wchar_t * folder_list[3];
  folder_list[0]=skin_path_int;
  folder_list[1]=skin_path_ext;
  folder_list[2]=0;
  TEXTID strid;
  switch(IMAGEDB_MODE)
  {
  case 0:
    strid=TextID_Create(names[38],ENC_UCS2,TEXTID_ANY_LEN);
    break;
  case 1:
    strid=TextID_Create(names[39],ENC_UCS2,TEXTID_ANY_LEN);
    break;
  case 2:
    strid=TextID_Create(names[44],ENC_UCS2,TEXTID_ANY_LEN);
    break;
  }
  DataBrowserDesc_SetHeaderText(DB_Desc,strid);
  DataBrowserDesc_SetBookID(DB_Desc,BookObj_GetBookID(book));
  DataBrowserDesc_SetFolders(DB_Desc,folder_list);
  DataBrowserDesc_SetFileExtList(DB_Desc,L"*.png");
  DataBrowserDesc_SetItemFilter(DB_Desc,DB_Filter);
  DataBrowserDesc_SetFoldersNumber(DB_Desc,2);
  DataBrowserDesc_SetSelectAction(DB_Desc,1);
  DataBrowserDesc_SetItemStyle(DB_Desc , 1);
  DataBrowser_Create(DB_Desc);
  DataBrowserDesc_Destroy(DB_Desc);
  return(0);
}

const PAGE_MSG SelectImage_PageEvents[]@ "DYN_PAGE" ={
  PAGE_ENTER_EVENT_TAG,CreateImageDB,
  ACCEPT_EVENT_TAG,onAcceptImageDB,
  NIL_EVENT_TAG,0
};

PAGE_DESC SelectImage_page ={"SelectImage_Page",0,SelectImage_PageEvents};

void Create_SelectImageDB(int mode)
{
  IMAGEDB_MODE=mode;
  BookObj_CallPage(&mbk->book,&SelectImage_page);
}
