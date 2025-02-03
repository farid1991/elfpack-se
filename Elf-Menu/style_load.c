#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "CFile.h"
#include "extern.h"

//обнуляем параметры стиля
void Reset_Style(MENU * mn)
{
#define style mn->st
  style.menu_style=0;
  style.y_offset=0;
  style.x_spacing=0;
  style.y_spacing=0;
  style.x_tab=0;
  style.x_size=0;
  style.y_size=0;
  style.x_size_s=0;
  style.y_size_s=0;
  style.is_selected=0;
  style.cursor_speed=0;
  style.icon_y_speed=0;
  style.icon_max_y_offset=0;
  style.icon_min_y_offset=0;
  style.icon_x_speed=0;
  style.icon_max_x_offset=0;
  style.icon_min_x_offset=0;
  style.lable_style=0;
  style.lable_font=0;
  style.lable_color=0;
  style.lable_stroke_color=0;
  style.lable_x=0;
  style.lable_y=0;
  style.min_scale=0;
  style.max_scale=0;
  style.scale_speed=0;
  style.selected_type=0;
  style.appear_speed=0;
  style.lable_offset=0;
  style.cursor_offset=0;
  style.cursor_color=0;
  style.back_color=0;
  style.eventab_offset=0;
  style.scroll_speed=0;
  style.is_cursor_img=0;
  style.cursor_w=0;
  style.cursor_h=0;
}

void Style_Read(wchar_t*path, MENU * mn)
{
#define style mn->st
  Reset_Style(mn);
  CFile * file=new CFile(path);
  if (file->Stat_Result()==0)
  {
    if(file->Open(WA_Read)>=0)
    {
      file->Read();
      char * pcfg=file->Content();
      if(char* t=manifest_GetParam(pcfg,"[back_color]",0))
      {
        style.back_color=h2i(t);
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[menu_style]",0))
      {
        style.menu_style=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[y_offset]",0))
      {
        style.y_offset=intget(t,'\0');
        DELETE(t);
      }

      if(char* t=manifest_GetParam(pcfg,"[x_spacing]",0))
      {
        style.x_spacing=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[y_spacing]",0))
      {
        style.y_spacing=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[x_tab]",0))
      {
        style.x_tab=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[x_size]",0))
      {
        style.x_size=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[y_size]",0))
      {
        style.y_size=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[is_selected]",0))
      {
        style.is_selected=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[x_size_s]",0))
      {
        style.x_size_s=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[y_size_s]",0))
      {
        style.y_size_s=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[cursor_speed]",0))
      {
        style.cursor_speed=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_y_speed]",0))
      {
        style.icon_y_speed=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_max_y_offset]",0))
      {
        style.icon_max_y_offset=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_min_y_offset]",0))
      {
        style.icon_min_y_offset=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_x_speed]",0))
      {
        style.icon_x_speed=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_max_x_offset]",0))
      {
        style.icon_max_x_offset=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[icon_min_x_offset]",0))
      {
        style.icon_min_x_offset=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[lable_style]",0))
      {
        style.lable_style=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[lable_font]",0))
      {
        style.lable_font=h2i(t);
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[lable_color]",0))
      {
        style.lable_color=h2i(t);
        DELETE(t);
      }
      /*
      if(char* t=manifest_GetParam(pcfg,"[lable_stroke_color]",0))
      {
      style.lable_stroke_color=h2i(t);
      DELETE(t);
    } */
      if(char* t=manifest_GetParam(pcfg,"[lable_x]",0))
      {
        style.lable_x=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[lable_y]",0))
      {
        style.lable_y=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[min_scale]",0))
      {
        style.min_scale=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[max_scale]",0))
      {
        style.max_scale=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[scale_speed]",0))
      {
        style.scale_speed=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[selected_type]",0))
      {
        style.selected_type=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[appear_speed]",0))
      {
        style.appear_speed=intget(t,'\0');
        DELETE(t);
      }
      /*
      if(char* t=manifest_GetParam(pcfg,"[menu_radius]",0))
      {
      style.menu_radius=intget(t,'\0');
      DELETE(t);
    }
      if(char* t=manifest_GetParam(pcfg,"[menu_circle_x]",0))
      {
      style.menu_circle_x=intget(t,'\0');
      DELETE(t);
    }
      if(char* t=manifest_GetParam(pcfg,"[menu_circle_y]",0))
      {
      style.menu_circle_y=intget(t,'\0');
      DELETE(t);
    }
      if(char* t=manifest_GetParam(pcfg,"[menu_circle_speed]",0))
      {
      style.menu_circle_speed=intget(t,'\0');
      DELETE(t);
    }


      if(char* t=manifest_GetParam(pcfg,"[lable_offset]",0))
      {
      style.lable_offset=intget(t,'\0');
      DELETE(t);
    }
      if(char* t=manifest_GetParam(pcfg,"[cursor_offset]",0))
      {
      style.cursor_offset=intget(t,'\0');
      DELETE(t);
    } */
      if(char* t=manifest_GetParam(pcfg,"[cursor_color]",0))
      {
        style.cursor_color=h2i(t);
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[eventab_offset]",0))
      {
        style.eventab_offset=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[scroll_speed]",0))
      {
        style.scroll_speed=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[is_cursor_img]",0))
      {
        style.is_cursor_img=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[cursor_w]",0))
      {
        style.cursor_w=intget(t,'\0');
        DELETE(t);
      }
      if(char* t=manifest_GetParam(pcfg,"[cursor_h]",0))
      {
        style.cursor_h=intget(t,'\0');
        DELETE(t);
      }
      DELETE(pcfg);
      file->Close();
    }
  }
  DELETE(file);
}
