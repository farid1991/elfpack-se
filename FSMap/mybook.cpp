#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "mybook.h"

extern void elf_exit(void);

//структура, которую шлет букман
typedef struct
{
  BOOK * bookmanbook;
}BOOKMANMSG;

//сообщение от букмана о выгрузке
int CMyBook::TerminateElf(CBook** pbookmanbook, CMyBook* book)
{
  book->Free();
  return 1;
}

//показ автора
int CMyBook::ShowAuthorInfo(CBook** pbookmanbook, CMyBook* book)
{
  MessageBox(EMPTY_TEXTID,STR("FSMap\nver.1\n(c)arban"),NOIMAGE, 1 ,5000, *pbookmanbook);
  return 1;
}

DECLARE_PAGE_DESC_BEGIN(CMyBook::base_page, "My_BasePage")
DECLARE_PAGE_DESC_MSG( ELF_TERMINATE_EVENT, CMyBook::TerminateElf )
DECLARE_PAGE_DESC_MSG( ELF_SHOW_INFO_EVENT, CMyBook::ShowAuthorInfo )
DECLARE_PAGE_DESC_END


struct IT
{
  wchar_t * fp;
  LIST * lst;
};

#define FA_DIR 0x4000

const wchar_t root_c[2]=L"/";

unsigned int FS_SIZE=0;
unsigned int FS_FILES=0;
unsigned int FS_FOLDERS=0;

void rr(IT*item,wchar_t const*fpath,CFile*cf)
{
  void * dir = w_diropen(fpath);
  wchar_t * path;
  item->lst=List_Create();
  while(path=w_dirread(dir))
  {
    IT * it=new IT;
    memset(it,0,sizeof(IT));
    it->fp=new wchar_t[wstrlen(path)+wstrlen(item->fp)+2];
    wstrcpy(it->fp,item->fp);
    wstrcat(it->fp,root_c);
    wstrcat(it->fp,path);
    W_FSTAT fstat;
    w_fstat(it->fp,&fstat);
    if(fstat.attr&FA_DIR)
    {
      rr(it,it->fp,cf);
      FS_FOLDERS++;
    }
    else
    {
      FS_SIZE+=fstat.st_size;
      FS_FILES++;
      it->lst=0;
    }

    cf->WriteUTF8(it->fp);
    cf->WriteUTF8(L"\n");

    List_InsertLast(item->lst, it);
  }
  w_dirclose(dir);

}

void map_destroy(IT * ri)
{
  if(ri->fp)delete(ri->fp);
  if(ri->lst)
  {
    while(ri->lst->FirstFree)
    {
      IT * dit=(IT*)List_RemoveAt(ri->lst,ri->lst->FirstFree-1);
      map_destroy(dit);
      delete(dit);
    }
    List_Destroy(ri->lst);
  }
}

//бука создаётся
CMyBook::CMyBook() :CBook((char*)"FSMap",&base_page)
{
  IT * rootit=new IT;
  memset(rootit,0,sizeof(IT));
  rootit->lst=0;
  rootit->fp=new wchar_t[wstrlen(L"")+1];
  wstrcpy(rootit->fp,L"");
  wchar_t buf[90];
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  snwprintf(buf,49,L"/card/fs_map_%02d_%02d-%02d_%02d.txt",dt.date.mon,dt.date.day,dt.time.hour,dt.time.min);
  CFile * file=new CFile(buf);
  file->Open(WA_Read+WA_Write+WA_Create+WA_Truncate);
  file->WriteUTF8_BOM();
  rr(rootit,root_c,file);
  snwprintf(buf,89,L"\n------Info------\nFiles: %d\nFolders: %d\nFull size: %d\n",FS_FILES,FS_FOLDERS,FS_SIZE);
  file->WriteUTF8(buf);
  delete(file);
  delete(rootit);
  map_destroy(rootit);
  this->Free();
}

//бука убивается
CMyBook::~CMyBook()
{
  SUBPROC(elf_exit);
}

