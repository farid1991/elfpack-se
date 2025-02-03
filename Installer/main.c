#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"
#include "..\include\zlib.h"
#include "..\deleaker\mem2.h"

/*
Elf Installer v1.0
эльф дл€ упрощени€ процесса установки новых эльфов.
необходимо ассоциировать расширение pkg с файлом Installer.elf
после этого в диспетчере файлов телефона наступаем на *.pkg и все
необходимые запчасти от эльфа раскладываютс€ по своим папкам.

далее - дл€ эльфописателей.

создание инстал€ционного пакета.
создаЄм на диске структуру папок, необходимую дл€ работы ¬ашего эльфа.
например BookManager.
создаЄм:
%DIR_ELFS_DAEMONS_EXT%/BookManager.elf
%DIR_ELFS_DAEMONS_EXT%/RunOnce.elf
%DIR_INI%/bookman/1.png
%DIR_INI%/bookman/2.png
%DIR_INI%/bookman/3.png
%DIR_INI%/bookman/4.png
%DIR_INI%/bookman/bookman.ini
%DIR_INI%/bookman/shortcuts.ini

далее пакуем это дело в ZIP и переименовываем в BookManager.pkg

если в инстал€ционном пакете присутствует эльф, одноименный с названием пакета(с учетом регистра),
то по завершению инстал€ции он будет запущен.

если в инстал€ционном пакете присутствует RunOnce.elf - то он (RunOnce.elf) будет запущен и удалЄн.

*****************************************************************************************************

Elf Installer v1.2

+ визуализаци€
+ лог файл.
  ведетс€ в папке с Installer.elf
+ unzip
  ассоциируйте расширение zip с Installer.elf.

устанавливаетс€ в папку "%DIR_ELFS_EXT%\Utils\Installer"
(кого не устраивает - правим путь в pkg-файле.)
не забываем исправить ext.ini!!

необходимы swi 0x30D...0x311

*/
BOOK *MyBook;
int total_files=0;
int total_folders=0;
int total_root_folders=0;
int total_entry=0;
int canceled=0;
int log_file=-1;
int log_size=0;
int cur_log_size=0;
int pakage=0;
wchar_t *run_once=0;
wchar_t *pkg_name=0;
wchar_t *main_name=0;
wchar_t *post_run=0;
wchar_t icons[20];
GUI *pg;
u16 timer=0;
u16 animation_timer=0;

wchar_t *log_fpath=0;


#define ACTION_LOG 1

int TerminateElf(void *,BOOK*book)
{
	FreeBook(book);
	return(1);
}

typedef struct
{
	BOOK *book;
}MSG;

int ShowAuthorInfo(void *mess ,BOOK*book)
{
	MSG *msg = (MSG *)mess;
	MessageBox(EMPTY_TEXTID,STR("Elf Installer v1.24\n (c) Hussein"),NOIMAGE,1,0,msg->book);
	return(1);
}

const PAGE_MSG HW_PageEvents[]@ "DYN_PAGE" =
{
	ELF_TERMINATE_EVENT,  TerminateElf,
	ELF_SHOW_INFO_EVENT,  ShowAuthorInfo,
	NIL_EVENT_TAG,        NULL
};

PAGE_DESC base_page ={"Installer_BasePage",0,HW_PageEvents};

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

void onCloseBook(BOOK *book)
{
	GUIObject_Destroy(pg);
	//  wchar_t ws[10];
	for(int i=1;i<19;i++)
	{
		//    snwprintf(ws,9,L"%d.png",i);
		ImageID_Free(icons[i]);
	}
	if(timer) Timer_Kill(&timer);
	if(animation_timer) Timer_Kill(&animation_timer);
	if(run_once) mfree(run_once);
	if(pkg_name) mfree(pkg_name);
	if(main_name) mfree(main_name);
	if(post_run) mfree(post_run);
	if(log_file!=-1) w_fclose(log_file);
	if(log_fpath) mfree(log_fpath);
	trace_done();
	SUBPROC(elf_exit);
}

void win12512unicode(wchar_t *ws, char *s, int len)
{
	int c;
	while((c=*s++)&&((len--)>0))
	{
		if(c==0xA8) c=0x401;
		if(c==0xAA) c=0x404;
		if(c==0xAF) c=0x407;
		if(c==0xB8) c=0x451;
		if(c==0xBA) c=0x454;
		if(c==0xBF) c=0x457;
		if((c>=0xC0)&&(c<0x100)) c+=0x350;
		*ws++=c;
	}
	*ws=0;
}

void dos2unicode(wchar_t *ws, char *s, int len)
{
	int c;
	while((c=*s++)&&((len--)>0))
	{
		if(c==0xF0) c=0x401;
		if(c==0xF1) c=0x451;
		if(c==0xF2) c=0x404;
		if(c==0xF3) c=0x451;
		if(c==0xF4) c=0x407;
		if(c==0xF5) c=0x457;
		if((c>=0x80)&&(c<0xB0)) c+=0x390;
		if((c>=0xE0)&&(c<0xF0)) c+=0x360;
		*ws++=c;
	}
	*ws=0;
}

void CreateMyBook()
{
	MyBook = new BOOK;
	CreateBook(MyBook,onCloseBook,&base_page,"Installer",-1,0);
}

wchar_t *dirs[]=
{
	L"%DIR_AUDIO_EXT%",
	L"%DIR_AUDIO_INT%",
	L"%DIR_IMAGE_EXT%",
	L"%DIR_IMAGE_INT%",
	L"%DIR_VIDEO_EXT%",
	L"%DIR_VIDEO_INT%",
	L"%DIR_THEME_EXT%",
	L"%DIR_THEME_INT%",
	L"%DIR_OTHER_EXT%",
	L"%DIR_OTHER_INT%",
	L"%DIR_RESERVED1_EXT%",
	L"%DIR_RESERVED1_INT%",
	L"%DIR_RESERVED2_EXT%",
	L"%DIR_RESERVED2_INT%",
	L"%DIR_RESERVED3_EXT%",
	L"%DIR_RESERVED3_INT%",
	L"%DIR_ELFS_EXT%",
	L"%DIR_ELFS_INT%",
	L"%DIR_ELFS_DAEMONS_EXT%",
	L"%DIR_ELFS_DAEMONS_INT%",
	L"%DIR_ELFS_CONFIG_EXT%",
	L"%DIR_ELFS_CONFIG_INT%",
	L"%DIR_INI%",
	L"%%",
	L"%DIR_DLL%",
	L"%%",
	//  L"%DIR_DLL_EXT%",
	//  L"%DIR_DLL_INT%",
	0
};

void WriteLog(wchar_t *ws)
{
	if(log_file!=-1)
	{
		w_fwrite(log_file,ws,wstrlen(ws)*2);
		cur_log_size += (wstrlen(ws)*2);
	}
}

int DirStr2Num(wchar_t *wstr)
{
	int n=0;
	wchar_t **wp = dirs;

	while (*wp)
	{
		if(!wstrncmp(*wp,wstr,wstrlen(*wp))) return(n);
		n++;
		wp++;
	}
	return(DIR_OTHER | MEM_INTERNAL);
}

int GetPath(wchar_t *wstr)
{
	int res = 0;
	wchar_t log[512];
	wchar_t *path;
	wchar_t *path1;
	wchar_t *path2;

	if(pakage)
	{
		path1 = GetDir(DirStr2Num(wstr));
		path2 = wstrwstr(wstr,L"%/");
	}
	else
	{
		path1 = main_name;
		path2 = wstr;
		path2--;
	}
	if(path2)
	{
		path2++;
		if(wstrlen(path2)>1)
		{
			path = new wchar_t[wstrlen(path1)+wstrlen(path2)+1];
			wstrcpy(path,path1);
			wstrcat(path,path2);
			if(wstrlen(path)>256)
			{
				snwprintf(log,511,L"\r\n  !!!%ls - too long path! skiped!",wstr);
				WriteLog(log);
				mfree(path);
				return(0);
			}

			wstrcpy(wstr,path);

			if((wstrrchr(wstr,L'/')-wstr) == (wstrlen(wstr)-1))
			{
				// папка
				*wstrrchr(wstr,L'/') = 0;
				w_mkdir(path,0x1FF);
				total_folders++;
				snwprintf(log,511,L"\r\nCreating folder : %ls",path);

				WriteLog(log);
			}
			else
			{
				//файло..
				res = 1;
				total_files++;
				path1 = wstrrchr(wstr,L'/');
				snwprintf(log,511,L"\r\n  -->%ls",wstr);
				WriteLog(log);
				if(pakage)
				{
					path1++;
					if(!wstrcmpi(path1,L"RunOnce.elf"))
					{
						run_once = new wchar_t[wstrlen(wstr)+1];
						wstrcpy(run_once,wstr);
					}
					if(!wstrcmpi(path1,L"PostRun.elf"))
					{
						post_run = new wchar_t[wstrlen(wstr)+1];
						wstrcpy(post_run,wstr);
					}
					if(!wstrncmp(path1,pkg_name, wstrlen(pkg_name)-((wstrrchr(pkg_name,L'.')-pkg_name)/2)))
					{
						if(main_name) mfree(main_name);
						main_name = new wchar_t[wstrlen(wstr)+1];
						wstrcpy(main_name,wstr);
					}
				}
			}
			mfree(path);
		}
		else
		{
			total_root_folders++;
		}
	}
	return(res);
}

int getint(unsigned char *ptr)
{
	return ptr[0]|(ptr[1]<<8)|((ptr[2])<<16)|((ptr[3])<<24);
}

typedef struct
{
	wchar_t *path;
	wchar_t *fname;
}PATH_NAME;

void onCompleteTimer(u16 timerID, LPARAM)
{
	FreeBook(MyBook);
}

int UpdateProgress(int value, wchar_t *w_name)
{
	wchar_t ws[200];
	wchar_t *w = wstrrchr(w_name,L'/');
	if(w) w++;
	else w = w_name;
	snwprintf(ws,199,L"%d / %d\n\n%ls",value,total_entry,w);
	ProgressBar_SetText(pg,TextID_Create(ws,ENC_UCS2,TEXTID_ANY_LEN));
	ProgressBar_SetPercentValue(pg,(value*100)/total_entry);
	mfree(w_name);
	return(0);
}

enum ERRORS {
	ZER_GOOD=0,
	USER_ABORTED=-100,
	NO_MEMORY=-101,
	INPUT_FILE_NOT_OPENED=-102,
	READ_ERROR=-103,
	WRITE_ERROR=-104,
	CREATE_FILE_ERROR=-105
};


void cancel_proc(BOOK *, GUI *)
{
	canceled=1;
}
void ok_proc(BOOK *MyBook, GUI *)
{
	FreeBook(MyBook);
}

void log_proc(BOOK *MyBook, GUI *)
{
	if(log_file!=-1)
	{
		if(cur_log_size)
		{
			//w_lseek(log_file,-cur_log_size,WSEEK_CUR);
			//char *buf  = new char[cur_log_size+2];
			//memset(buf,0,cur_log_size+2);
			//w_fread(log_file,buf,cur_log_size);
			//MessageBox(TextID_Create(pkg_name,ENC_UCS2,TEXTID_ANY_LEN),
			//           TextID_Create(buf,ENC_UCS2,TEXTID_ANY_LEN),
			//           NOIMAGE,2,0,0);
			//mfree(buf);

			wchar_t *name = wstrrchr(log_fpath,L'/')+1;
			*wstrrchr(log_fpath,L'/') = 0;
			char *URIScheme = GetURIScheme(file);
			char *URI = CreateURI(log_fpath,name,URIScheme);
			Browser_OpenURI(MyBook,URI,2);
			mfree(URI);
			*(name-1) = L'/';
		}
	}
	FreeBook(MyBook);
}

int Completed(int err, wchar_t *w_name)
{
	wchar_t ws[200];
	wchar_t *w;
	if(pakage)
	{
		w = L"”становка";
	}
	else
	{
		w = L"–аспаковка";
	}
	WriteLog(L"\r\n\r\n");
	if(!err)
	{
		snwprintf(ws,199,L"%ls '%ls' завершена.",w,pkg_name);
	}
	else
	{
		switch(err)
		{
		case USER_ABORTED:
			snwprintf(ws,199,L"%ls '%ls' прервана пользователем",w,pkg_name);
			break;
		default:
			snwprintf(ws,199,L"%ls '%ls' ќшибка %d",w,pkg_name,err);
			break;
		}
	}
	WriteLog(ws);
	if(pakage)
	{
		snwprintf(ws,199,L"\r\n\r\nFiles:%d\r\nFolders:%d\r\nRootFolders:%d\r\n\r\n",total_files,total_folders,total_root_folders);
	}
	else
	{
		snwprintf(ws,199,L"\r\n\r\nFiles:%d\r\nFolders:%d\r\n\r\n",total_files,total_folders);
	}
	WriteLog(ws);
	if((!err) && (pakage))
	{
		if(run_once) elfload(run_once,0,0,0);
		if(main_name) elfload(main_name,0,0,0);
		if(post_run) elfload(post_run,0,0,0);
	}

	if(animation_timer) Timer_Kill(&animation_timer);
	ProgressBar_SetText(pg,TextID_Create(ws,ENC_UCS2,TEXTID_ANY_LEN));
	ProgressBar_SetIcon(pg,NOIMAGE);
	ProgressBar_SetBarDisabled(pg,1);
	GUIObject_SoftKeys_SetVisible(pg,ACTION_DONE,1);
	GUIObject_SoftKeys_SetVisible(pg,ACTION_LOG,1);
	GUIObject_SoftKeys_SetVisible(pg,ACTION_CANCEL,0);
	GUIObject_SoftKeys_SetAction(pg,ACTION_BACK,ok_proc);

	timer = Timer_Set(5000,onCompleteTimer,0);
	mfree(w_name);
	return(0);
}

int inf(int source, int dest, int in_len)
{
	int CHUNK =16384;//GetFreeBytesOnHeap()/10;
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char *in  = new unsigned char[CHUNK];
	unsigned char *out  = new unsigned char[CHUNK];
	int n;

	/*allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit2(&strm,-MAX_WBITS);
	if(ret != Z_OK)
	{
		mfree(in);
		mfree(out);
		return ret;
	}
	/*decompress until deflate stream ends or end of file */
	do
	{
		if(canceled)
		{
			(void)inflateEnd(&strm);
			mfree(in);
			mfree(out);
			return(USER_ABORTED);
		}
		if(in_len>=CHUNK)
		{
			n=w_fread(source,in, CHUNK);
			in_len-=CHUNK;
		}
		else
		{
			n=w_fread(source,in, in_len);
			in_len=0;
		}
		strm.avail_in = n;

		if(strm.avail_in == 0)
			break;
		strm.next_in = in;

		/*run inflate() on input until output buffer not full */
		do
		{
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);

			if(ret==Z_STREAM_ERROR )
			{
				mfree(in);
				mfree(out);
				return(ret);//assert(ret != Z_STREAM_ERROR);
			}
			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /*and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				mfree(in);
				mfree(out);

				return ret;
			}
			have = CHUNK - strm.avail_out;
			n=w_fwrite(dest,out, have);
			if( n!= have )
			{
				(void)inflateEnd(&strm);
				mfree(in);
				mfree(out);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

		/*done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/*clean up and return */
	(void)inflateEnd(&strm);
	mfree(in);
	mfree(out);
	return(ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR);
}

static const wchar_t SLASH[] =L"/";

void Install(int p0, PATH_NAME *pn)
{
	wchar_t *path = pn->path;
	wchar_t *fname = pn->fname;
	int in,out;
	int packsize = 1;
	int unpacksize = 2;
	int filenamesize = 3;
	int extrasize = 0;
	int offset = 0;
	int root_len = 0;
	char ch_name[256];
	wchar_t w_name[256];
	unsigned char header[30];
	int err = ZER_GOOD;

	wstrcpy(w_name,path);
	wstrcat(w_name,SLASH);
	wstrcat(w_name,fname);
	root_len = wstrlen(w_name)+1;
	if(fname && ((in = w_fopen(w_name,WA_Read,0x124,0))!=-1))
	{
		W_FSTAT wstat;
		w_fstat(w_name,&wstat);
		w_lseek(in,wstat.st_size-22,WSEEK_SET);
		if(w_fread(in,header,22)!=22) return;
		total_entry = getint(header+10)&0xFFFF;
		w_lseek(in,0,WSEEK_SET);

		while(1)
		{
			w_lseek(in,offset,WSEEK_SET);
			if(canceled)
			{
				err = USER_ABORTED;
				break;
			}
			if(w_fread(in,header,30)!=30) break;
			if(getint(header)!=0x4034b50) break;
			packsize = getint(header+18);
			unpacksize = getint(header+22);
			filenamesize = (getint(header+26))&0xFFFF;
			extrasize = (getint(header+28))&0xFFFF;
			if(w_fread(in,ch_name,filenamesize)!=filenamesize)
			{
				err = READ_ERROR;
				break;
			}
			offset += (30+packsize+filenamesize+extrasize);
			if((root_len+filenamesize)>255)
			{
				WriteLog(L"  !!WARNING!! Filename too long... skipped...");
			}
			else
			{
				ch_name[filenamesize] = 0;
				dos2unicode(w_name,ch_name,filenamesize);
				wchar_t *mmi_wc = new wchar_t[filenamesize+1];
				wstrcpy(mmi_wc,w_name);
				MMIPROC(((void(*)(int, void *))UpdateProgress),(total_files+total_folders+total_root_folders),(void *)mmi_wc);
				if(GetPath(w_name))
				{
					if((out = w_fopen(w_name,WA_Create | WA_Write ,0x1FF,0))!=-1)
					{
						if(unpacksize == packsize)
						{
							char *buf = new char[packsize];
							w_fread(in,buf,packsize);
							w_fwrite(out,buf,packsize);
							mfree(buf);
						}
						else
						{
							err = inf(in,out,packsize);
						}
						w_fclose(out);
						if(err) break;
					}
					else
					{
						err = CREATE_FILE_ERROR;
						break;
					}
				}
			}
		}
		w_fclose(in);
	}
	else
	{
		err = INPUT_FILE_NOT_OPENED;
	}
	wchar_t *ws = new wchar_t[wstrlen(pn->fname)+1];
	wstrcpy(ws,pn->fname);
	MMIPROC((void(*)(int,void*))Completed,err,(void*)ws);
	mfree(path);
	mfree(fname);
	mfree(pn);
}

void InitIcons(wchar_t *path)
{
	wchar_t ws[10];
	wchar_t *p = new wchar_t[wstrlen(path)+wstrlen(L"/images")+2];
	*wstrrchr(path,L'/') = 0;
	wstrcpy(p,path);
	wstrcat(p,L"/images");

	for(int i=1;i<19;i++)
	{
		snwprintf(ws,9,L"%d.png",i);
		ImageID_Get(p,ws,&icons[i]);
	}
	mfree(p);
}

void onAnimationTimer(u16 timerID, LPARAM lparam)
{
	int num = (int)lparam;
	ProgressBar_SetIcon(pg,ImageID_Copy(icons[num++]));
	if(num==19) num=1;
	Timer_ReSet(&animation_timer,300,onAnimationTimer,(LPARAM)num);
}

int main(wchar_t *self_name, wchar_t *path, wchar_t *fname)
{
	if(!path || !fname)
	{
		elf_exit();
		return(0);
	}

	trace_init(L"installer_leaks.txt");

	wchar_t *my_self_name = new wchar_t[wstrlen(self_name)+1];
	wstrcpy(my_self_name,self_name);

	pakage = 0;
	wchar_t log[200];
	W_FSTAT wstat;
	DATETIME dt;
	PATH_NAME *pn = new PATH_NAME;
	pn->path = new wchar_t[wstrlen(path)+1];
	pn->fname = new wchar_t[wstrlen(fname)+1];
	wstrcpy(pn->path,path);
	wstrcpy(pn->fname,fname);

	REQUEST_DATEANDTIME_GET(SYNC,&dt);
	pkg_name = new wchar_t[wstrlen(fname)+1];
	wstrcpy(pkg_name,fname);
	if((wstrwstr(pkg_name,L".pkg")) || (wstrwstr(pkg_name,L".PKG"))) pakage = 1;
	if(!pakage)
	{
		main_name = new wchar_t[wstrlen(path)+wstrlen(fname)+2];
		wstrcpy(main_name,path);
		wstrcat(main_name,SLASH);
		wstrcat(main_name,fname);
		wchar_t *w = wstrrchr(main_name,L'.');
		*w++ = L'/';
		*w = 0;
		w_mkdir(main_name,0x1FF);
	}
	CreateMyBook();
	wstrcpy(wstrrchr(my_self_name,L'.'),L".txt");
	log_fpath = new wchar_t[wstrlen(my_self_name)+1];
	wstrcpy(log_fpath,my_self_name);
	log_file = w_fopen(log_fpath,WA_Create | WA_Append | WA_Write | WA_Read ,0x1FF,0);
	w_fstat(log_fpath,&wstat);
	log_size = wstat.st_size;
	snwprintf(log,199,L"****************** %ls  ******************\r\n%d-%d-%d  %.02d:%.02d:%.02d\r\n",fname,dt.date.day,dt.date.mon,dt.date.year,dt.time.hour,dt.time.min,dt.time.sec);
	WriteLog(log);
	mfree(my_self_name);

	InitIcons(my_self_name);

	pg = CreateProgressBar(MyBook,0);
	GUIObject_SoftKeys_SetAction(pg,ACTION_CANCEL,cancel_proc);
	GUIObject_SoftKeys_SetAction(pg,ACTION_LOG,log_proc);
	GUIObject_SoftKeys_SetAction(pg,ACTION_DONE,ok_proc);
	GUIObject_SoftKeys_SetText(pg,ACTION_LOG,STR("Log"));
	GUIObject_SoftKeys_SetVisible(pg,ACTION_DONE,0);
	GUIObject_SoftKeys_SetVisible(pg,ACTION_LOG,0);
	GUIObject_Show(pg);

	SUBPROC(((void(*)(int, void *))Install),0,(void *)pn);
	animation_timer = Timer_Set(300,onAnimationTimer,1);
	return(0);
}
