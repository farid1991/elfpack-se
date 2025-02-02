#ifndef _CFILE_H
#define _CFILE_H

#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

extern char * wstr2utf8(wchar_t* src);
extern wchar_t *utf82wstr(char *string);

//класс для чтения/записи файлов
class CFile
{
public:
  CFile(wchar_t * path);
  CFile(wchar_t * path,wchar_t * fname);
  ~CFile(void);
  int Open(int attr);
  int Close();
  int Write(void * buf, int len);
  int WriteUTF8(wchar_t * buf);
  int WriteUTF8_BOM();
  int WriteNum(int d);
  int WriteNum(float d);
  int WriteNum(double d);
  int WriteParam(wchar_t * param, wchar_t * buf);
  void Read();
  wchar_t * ReadUTF8();
  wchar_t * ReadParam(wchar_t * param);
  char * ReadN(int n);
  int Size();
  int Stat_Result();
  bool Exists();
  int FileID();
  char * Content();
  W_FSTAT FStat();
  void ChangePath(wchar_t * path);
private:
  char * buffer;
  wchar_t * file_path;
  int size;
  W_FSTAT info;
  int stat_result;
  int file;
};

#endif
