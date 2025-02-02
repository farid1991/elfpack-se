#ifndef _METADATA_H_
	#define _METADATA_H_

#include "..\include\Lib_Clara.h"
#include "..\include\dir.h"
#include "..\\classlib\cl.h"

class CMetaData
{
protected:
	void * mdata;
	IMAGEID cover;
	wchar_t *path;
	wchar_t *name;
	char type;
	int size;
	int offset;
public:
	CMetaData(wchar_t *p,wchar_t *n);
	~CMetaData();
	wchar_t * Artist();
	wchar_t * Title();
	wchar_t * Album();
	wchar_t * Year();
	wchar_t * Genre();
	int TrackNum();
	wchar_t Cover();
	void CoverFree();
};


#endif
