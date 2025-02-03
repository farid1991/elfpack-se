#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"

int isBrightnessControllerBook(BOOK * book)
{
	if(!strcmp(book->xbook->name,"BrightnessController"))
		return 1;
	return 0;
}

int isImageViewerBook (BOOK * book)
{
	if(!strcmp(book->xbook->name,"ImageViewerBook"))
		return 1;
	return 0;
}

int isCamBook (BOOK * book)
{
	if(!strcmp(book->xbook->name,"CameraBook"))
		return 1;
	return 0;
}
