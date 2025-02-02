#include "metadata.h"

wchar_t * GetType_str(int cover_type)
{
	wchar_t *type[]={L"jpg",L"gif",L"png",L"bmp"};
	return(type[cover_type]);
}

wchar_t * CMetaData::Artist()
{
	return(MetaData_Desc_GetTags(mdata,0));
}

wchar_t * CMetaData::Title()
{
	return(MetaData_Desc_GetTags(mdata,1));
}

wchar_t * CMetaData::Album()
{
	return(MetaData_Desc_GetTags(mdata,2));
}

wchar_t * CMetaData::Year()
{
	return(MetaData_Desc_GetTags(mdata,3));
}

wchar_t * CMetaData::Genre()
{
	return(MetaData_Desc_GetTags(mdata,4));
}

int CMetaData::TrackNum()
{
	return(MetaData_Desc_GetTrackNum(mdata,0));
}

wchar_t CMetaData::Cover()
{
	if (MetaData_Desc_GetCoverInfo(mdata,&type,&size,&offset))
	{
		wchar_t pathname[512];
		wstrcpy(pathname, path);
		wstrcat(pathname, L"/");
		wstrcat(pathname, name);
		int f;
		if ((f=w_fopen(pathname,WA_Read, 0x1FF, 0))>=0)
		{
			if (w_lseek(f,offset,WSEEK_SET)>=0)
			{
				char * CoverBuff=new char[size+1];
				memset(CoverBuff,0,size+1);
				w_fread(f,CoverBuff,size);
				ImageID_GetIndirect(CoverBuff,size,0,GetType_str(type),&cover);
			}
			w_fclose(f);
		}
	}
	else
	{
		const wchar_t * ExtCover[]={L"Cover.png",L"Cover.jpg",L"Cover.bmp",L"Album.png",L"Album.jpg",L"Album.bmp"};
		FSTAT fst;
		for(int k=0;k<6;k++)
		{
			if((fstat(path,ExtCover[k],&fst))>=0) ImageID_Get(path,ExtCover[k],&cover);
		}
	}
	return cover;
}

void CMetaData::CoverFree()
{
	if(cover!=NOIMAGE)
	{
		ImageID_Free(cover);
		cover=NOIMAGE;
	}
}

CMetaData::CMetaData(wchar_t *p,wchar_t *n)
{
	path=p; name=n;
	cover=NOIMAGE;
	mdata=MetaData_Desc_Create(p,n);
}

CMetaData::~CMetaData()
{
	CoverFree();
	MetaData_Desc_Destroy(mdata);
}
