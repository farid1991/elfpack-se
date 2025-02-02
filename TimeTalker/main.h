#define BOOK_NAME "TimeTalker"
#define ELFNAME L"TimeTalker"
#define VERSION L"2.1"
#define AUTHOR L"Slawwan"

typedef void (*TALK)(void * );

typedef struct _MYBOOK : BOOK
{
	TALK talk;
	u16 Ttimer;
	int play;
	int key;
}MyBOOK;



typedef struct
{
  BOOK * book;
}MSG;

int NewKey(int key, int rep_count, int mode, MyBOOK*, DISP_OBJ*);
int isTTBook(BOOK * struc);
void TalkTime(MyBOOK * myBook);
void onAPtimer (u16 unk , void * data);
int isTT(BOOK * book);

extern u16 APtimer;
extern int  time[14];
