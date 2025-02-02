#ifndef _MAIN_H_
#define _MAIN_H_

#define BOOK_NAME "MemReader"
#define ABOUT_TEXT "MemReader\n\n(c) E1kolyan"

typedef struct
{
  BOOK* book;
}MSG;

typedef struct
{
  BOOK book;
  GUI_LIST* gui_sel;
  GUI_LIST* gui_book;
  GUI* gui_size;
  GUI* gui_offset;
  int choice_item;
  int book_item;
  LIST* books_list;
  int books_count;
  int int_offset;
}MemReaderBook;

#define FLASH_MASK 0xF8000000
#define MAX_BOOK_NAME_LEN 50

typedef struct
{
  BOOK* book;
  char* book_name;
}BookList;

enum INPUT_TYPES
{
  IT_REAL = 0,
  IT_STRING = 1,
  IT_INTEGER = 2,
  IT_PHONE_NUMBER = 3,
  IT_DIGITAL_PASS = 4,
  IT_DIGITAL_IP = 5,
  IT_URL = 6,
  IT_ABC_AND_DIGIT = 8,
  IT_UNSIGNED_DIGIT = 9,
  IT_EXTRA_DIGIT = 10,
  IT_ABC_OR_DIGIT = 11,
  IT_EXTRA_DIGIT_2 = 12,
  IT_ABC_OR_DIGIT_2 = 13,
};

#define GUI_DESTROY(a) if(a) {a = GUIObject_Destroy(a); a = NULL;};

#define SELECT_BOOK 0
#define ENTER_RAM 1
#define MODE 2
#define ENTER_OFFSET 3
#define ENTER_SIZE 4
#define OX 5
#define ILLEGAL 6

void Book_Select(BOOK* book, GUI* gui);
void Offset_Select(MemReaderBook* mem_bk);
void SetBook(MemReaderBook* mem_bk);
int CreateChoise(void* data, BOOK* book);

#endif
