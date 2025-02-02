#include "..\include\Lib_Clara.h"
#include "Images.h"
#include "config_data.h"
#include "main.h"
#include "misc_util.h"
#include "MediaInfo.h"

void FreeImages(void);

int imageWidth[IMAGE_COUNT] = {0,0,0,0,0,0};
int imageHeight[IMAGE_COUNT] = {0,0,0,0,0,0};

unsigned int image6_x = 0;
unsigned int image6_y = 0;

IMAGEID image[IMAGE_COUNT] = {NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE,NOIMAGE};

const wchar_t *cfg_file[IMAGE_COUNT] = {
  cfg_file1,
  cfg_file2,
  cfg_file3,
  cfg_file4,
  cfg_file5,
  cfg_file6
};

const int *cfg_image_show[IMAGE_COUNT] = {
  &cfg_image1_show,
  &cfg_image2_show,
  &cfg_image3_show,
  &cfg_image4_show,
  &cfg_image5_show,
  &cfg_image6_show
};

const unsigned int *cfg_image_x[IMAGE_COUNT] = {
  &cfg_image1_x,
  &cfg_image2_x,
  &cfg_image3_x,
  &cfg_image4_x,
  &cfg_image5_x,
  (const unsigned int *)&image6_x
};

const unsigned int *cfg_image_y[IMAGE_COUNT] = {
  &cfg_image1_y,
  &cfg_image2_y,
  &cfg_image3_y,
  &cfg_image4_y,
  &cfg_image5_y,
  (const unsigned int *)&image6_y
};

const int *cfg_image_not_show_if_player[IMAGE_COUNT] = {
  &cfg_image1_not_show_if_player,
  &cfg_image2_not_show_if_player,
  &cfg_image3_not_show_if_player,
  &cfg_image4_not_show_if_player,
  &cfg_image5_not_show_if_player,
  &cfg_image6_not_show_if_player
};

int image_show[IMAGE_COUNT] = {0,0,0,0,0,0};

u16 timer_slideshow = 0;
wchar_t slideshow_path[256];
wchar_t slideshow_filter[256];
DIR_HANDLE * SlideShowDirHandle = 0;

void UpdateImages(void)
{
  if(image[IMAGE_COUNT - 1] != NOIMAGE)
  {
    ImageID_Free(image[IMAGE_COUNT - 1]);
    image[IMAGE_COUNT - 1] = NOIMAGE;
  }

  if(!SlideShowDirHandle)
  {
    SlideShowDirHandle = AllocDirHandle(slideshow_path);
    if(SlideShowDirHandle)
    {
//      DirHandle_SetFilterStr(SlideShowDirHandle, L".jpg;.gif;.png");
      DirHandle_SetFilterStr(SlideShowDirHandle, slideshow_filter);
    }
    else
    {
      return;
    }
  }

  FILELISTITEM *item = (FILELISTITEM *)malloc(sizeof(FILELISTITEM));
  FILELISTITEM *new_item = GetFname(SlideShowDirHandle,item);
  if(new_item)
  {
    if(ImageID_Get(new_item->path,new_item->fname,&image[IMAGE_COUNT - 1])<0) image[IMAGE_COUNT - 1] = NOIMAGE;
  }
  else
  {
     DestroyDirHandle(SlideShowDirHandle);
     SlideShowDirHandle = AllocDirHandle(slideshow_path);
     if(SlideShowDirHandle)
     {
       DirHandle_SetFilterStr(SlideShowDirHandle, slideshow_filter);
       new_item = GetFname(SlideShowDirHandle,item);
       if(new_item)
       {
         if(ImageID_Get(new_item->path,new_item->fname,&image[IMAGE_COUNT - 1])<0) image[IMAGE_COUNT - 1] = NOIMAGE;
       }
     }
  }
  mfree(item);

  if(image[IMAGE_COUNT - 1] != NOIMAGE)
  {
    int Width  = GetImageWidth(image[IMAGE_COUNT - 1]);
    int Height = GetImageHeight(image[IMAGE_COUNT - 1]);
    int Width0  = cfg_image6_slideshow.x2 - cfg_image6_slideshow.x1;
    int Height0 = cfg_image6_slideshow.y2 - cfg_image6_slideshow.y1;
    if((imageWidth[IMAGE_COUNT - 1] = (Width * Height0) / Height) <= Width0)
    {
      imageHeight[IMAGE_COUNT - 1] = Height0;
    }
    else
    {
      imageWidth[IMAGE_COUNT - 1]  = Width0;
      imageHeight[IMAGE_COUNT - 1] = (Height * Width0) / Width;
    }
  }
  else
  {
    imageWidth[IMAGE_COUNT - 1] = 0;
    imageHeight[IMAGE_COUNT - 1] = 0;
  }
}

void UpdateImagesShowParams(void)
{
    for(int i=0; i<IMAGE_COUNT; i++)
      image_show[i] = *cfg_image_show[i] && (!(PlayerIsStarted && *cfg_image_not_show_if_player[i]));
}

void ShowImages(int y, int location)
{
  GC *GC_DISP=get_DisplayGC ();
  for(int i=0; i<IMAGE_COUNT; i++)
  {
   if (image_show[i] && (image[i] != NOIMAGE))
   {
      putchar(GC_DISP, *cfg_image_x[i], *cfg_image_y[i] - y, imageWidth[i], imageHeight[i], image[i]);
   }
  }
}

void onTimer_slideshow (u16 timerID, LPARAM lparam)
{
    UpdateImages();
    Timer_ReSet(&timer_slideshow,cfg_slideshow_time*60*1000,onTimer_slideshow,0);
}

void UpdateImagesParams(void)
{
    FSTAT _fstat;

    FreeImages();

    image6_x = cfg_image6_x;
    image6_y = cfg_image6_y;

    for(int i=0; i< IMAGE_COUNT; i++)
    {
      imageWidth[i] = 0;
      imageHeight[i] = 0;

      if(*cfg_image_show[i] == 1)
      {
        extractdir(tmppath, tmpname, cfg_file[i]);
        if(fstat(tmppath,tmpname,&_fstat)!=0)
        {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Картинка %d не найдена"),i+1);
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        else if(ImageID_Get(tmppath,tmpname,&image[i])<0) image[i] = NOIMAGE;
      }
    }

    getdir(slideshow_path, cfg_folder6);
    if(SlideShowDirHandle)
    {
      DestroyDirHandle(SlideShowDirHandle);
      SlideShowDirHandle = 0;
    }

    if(cfg_image6_show == 2)
    {
      image6_x = cfg_image6_slideshow.x1;
      image6_y = cfg_image6_slideshow.y1;

      if(image[IMAGE_COUNT - 1] != NOIMAGE)
      {
        ImageID_Free(image[IMAGE_COUNT - 1]);
        image[IMAGE_COUNT - 1] = NOIMAGE;
      }

      DIR_HANDLE *dirHandle = AllocDirHandle(slideshow_path);
      if(dirHandle)
      {
        FILELISTITEM *item = (FILELISTITEM *)malloc(sizeof(FILELISTITEM));
        wstrcpy(slideshow_filter,slideshow_path);
        wstrcat(slideshow_filter,L"/*.png");
        DirHandle_SetFilterStr(dirHandle, slideshow_filter);
        if(GetFname(dirHandle,item))
        {
          UpdateImages();
          Timer_ReSet(&timer_slideshow,cfg_slideshow_time*60*1000,onTimer_slideshow,0);
        }
        else
        {
            wchar_t temp[128];
            snwprintf(temp, MAXELEMS(temp), _T("Картинки для слайдшоу не найдены"));
            MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
        }
        DestroyDirHandle(dirHandle);
        mfree(item);
      }
      else
      {
          wchar_t temp[128];
          snwprintf(temp, MAXELEMS(temp), _T("Папка для слайдшоу не найдена"));
          MessageBox(EMPTY_TEXTID,TextID_Create(temp,ENC_UCS2,MAXELEMS(temp)),NOIMAGE,1,5000,0);
      }
    }
    else if(timer_slideshow)
    {
      Timer_Kill(&timer_slideshow);
      timer_slideshow = 0;
    }

}

void FreeImages(void)
{
  for (int i=0;i<IMAGE_COUNT;i++)
  {
      if(image[i] != NOIMAGE)
      {
          ImageID_Free(image[i]);
          image[i] = NOIMAGE;
      }
  }
}

void FreeImagesParams(void)
{
  if(timer_slideshow)
  {
      Timer_Kill(&timer_slideshow);
      timer_slideshow = 0;
  }

  if(SlideShowDirHandle)
  {
    DestroyDirHandle(SlideShowDirHandle);
    SlideShowDirHandle = 0;
  }

  FreeImages();
}


