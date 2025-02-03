#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "extern.h"
#include "draw_lib.h"
#include <math.h>


double pi = 3.1415926535;
#define style Menu_Get()->st
double get_rad(double grad)
{
  return(grad* (pi/180));
}
double get_rad(int grad)
{
  return(grad* (pi/180));
}
int selected_item=0;

void GMenu_Refresh()
{
  FREE_GUI(mbk->gmenu);
  BookObj_CallPage(&mbk->book,&bk_GMenu);
}

//настройки стиля
//потом обязательно занести в структуру!
/*
int menu_style=0;       //стиль меню
int y_offset=24;      //отступ по y
int x_offset_a=0;       //отступ по x
int x_spacing=0;     //гор. отступ между ячейками
int y_spacing=5;     //вертикальный отступ между ячейками
int x_tab=4;            //столбцы
int y_tab=4;            //строки
int x_size=55;          //ширина картинки
int y_size=55;          //высота
int x_tab_size=60;      //ширина ячейки
int y_tab_size=70;      //высота
int cursor_speed=25;    //скорость курсора
int icon_y_speed=1;        //скорость аним. иконки по y
int icon_max_y_offset=10;  //макс. координата по y
int icon_min_y_offset=0;   //мин координата по y
int icon_x_speed=1;        //скорость аним. иконки по x
int icon_max_x_offset=10;  //макс. координата по x
int icon_min_x_offset=-5;   //мин координата по x
int lable_style=1;         //стиль надписей
int lable_font=0;          //шрифт
int lable_color=clWhite;   //цвет
int lable_stroke_color=clRed;  //цвет обводки
int lable_x=0;              //x координата текста заголовка
int lable_y=1;              //y координата текста заголовка
int min_scale=-15;          //мин. предел увеличения
int max_scale=0;            //макс. предел увеличения
int scale_speed=2;          //скорость увеличения
int selected_type=4;        //тип анимации выделенной иконки
int appear_speed=30;        //скорость появления

int menu_radius=150;        //радиус при стиле Карусельное
int menu_circle_x=50;      //x координата центра окружности
int menu_circle_y=320;      //y координата центра окружности
double menu_circle_speed=0.09;  //скорость прокрутки
*/

int disp_w;
int disp_h;

int scale_state=0;
int icon_y_state=0;
int icon_x_state=0;
int x_pos=-240;
int cursor_x=0;
int cursor_y=0;
int cursor_x_dest=0;
int cursor_y_dest=0;
int cursor=0;           //выделенный пункт
int is_appeared=0;
int nav_x=0, nav_y=0;  //позиция курсора
int scroll_y=0;
int scroll_y_dest=0;

int scale[100];
int y_offset[100];
int x_offset[100];
int icon_x[100];   //координаты для некоторых стилей
int icon_y[100];

int get_nav(int x, int y)
{
  int res=0;
  if(y>=1)
  {
    res=style.x_tab*y;
    res+=x;
  }
  else
  {
    res=x;
  }
  return(res);
}



//функция возвращает номер последней строки
int get_max_y(int num)
{
  int res=0;
  int tmp=num;
  int tmp2=tmp;
  //брут
  for(;;)
  {
    tmp-=style.x_tab;
    if(tmp>=0)res++;
    else
    {
      int ost=tmp2%style.x_tab; //остаток = кол-во элементов на последней неполной строке
      if(nav_x<=(ost-1))res++;      //если остаток больше или равен x-позиции курсора, то увеличиваем результат
      break;
    }
  }
  return(res-1);
}

void set_nav()
{
  nav_y=get_max_y(cursor+1);
  if(nav_y>0)nav_x=(cursor)%style.x_tab;
  else nav_x=cursor;
}
//кончились настройки
u16 timer;
void DrawIcn(GC*GC_,RECT rct,int ypos,int xpos,short icn)
{
  GC_PutChar(GC_,rct.x1+xpos,ypos+rct.y1,rct.x2-rct.x1,rct.y2-rct.y1,icn);
}
/*
int GetSize()
{
int a=Display_GetHeight(0);
switch (a)
{
  case 320:
return(30);
  case 220:
return(26);
  case 160:
return(18);
default:
return(26);
  }
};
*/

/*
Задача: РОВНО нарисовать столбцы и строки с иконками, учитывая отступ между ними (как вертикальный, так и горизонтальный),
также нужно сделать горизонтально выравнивание нарисованной менюхи по-середине

Дано:
xn - количество столбцов
yn - количество строк
xs - ширина иконки
ys - высота иконки
xo - горизонтальный отступ между иконками
yo - вертикальный отступ между иконками
DW - ширина диспа
DH - высота диспа

Решение:
Пусть xn=3,yn=5,xs=32,ys=32,yo=15,xo=16,DW=240,DH=320

Представим нашу "конструкцию": 0 - иконка, 1 - отступ: 01010.
Из этого следует, что отступов нужно на 1 меньше (для корректного рисования).

Расчет координат иконки следует выполнить следующим образом:

int i,x,y;
for(i=0;i<xn;i++)
{
if(i==0)
{
x=0;
}
else
{
x=i*(xs+xo);
}

Расчет ширины строки (нужно для выравнивания):
w=xn*(xs+xo)-xo;

Собственно выравнивание (точнее коэффициент):
HA=(DW-w)/2;
*/

void ImageID_size_get(int * xsize, int * ysize, wchar_t*imageid,ITEM*it,int c,int i)
{
  //если выделенная иконка
  if((c==nav_x)&&(i==nav_y))
  {
    if((it->simage!=NOIMAGE) && (it->simage!=0) && style.is_selected)
    {
      *xsize=style.x_size_s;
      *ysize=style.y_size_s;
      *imageid=it->simage;
    }
    else
    {
      *xsize=style.x_size;
      *ysize=style.y_size;
      *imageid=it->image;
    }
  }
  else  //если невыделенная
  {
    *xsize=style.x_size;
    *ysize=style.y_size;
    *imageid=it->image;
  }
  //если размер равен 0, то берем размер иконки
  if(*xsize==0)*xsize=GetImageWidth(*imageid);
  if(*ysize==0)*ysize=GetImageHeight(*imageid);
}

void GMenuGui_OnRedraw(DISP_OBJ *db,int ,int,int)
{
  GC* gc=(GC*)get_DisplayGC();
  if(style.menu_style==0)
  {
    int yy=0;
    int cnt=0;

    int HSize=style.x_size; //временная переменная для хранения гор.размера иконки
    if(HSize==0)            //проверка, чтобы всё норм было: если размер указан 0, то эльф сам возьмет размер иконки из первого пункта
    {
      ITEM *it=(ITEM *)List_Get(Menu_Get()->list, 0);
      HSize=GetImageWidth(it->image);
      if(HSize==0)HSize=32; //если опять 0, то задаем стандартный размер
    }
    int VSize=style.y_size; //временная переменная для хранения верт.размера иконки
    if(VSize==0)            //проверка, чтобы всё норм было: если размер указан 0, то эльф сам возьмет размер иконки из первого пункта
    {
      ITEM *it=(ITEM *)List_Get(Menu_Get()->list, 0);
      VSize=GetImageHeight(it->image);
      if(VSize==0)VSize=32; //если опять 0, то задаем стандартный размер
    }
    //ширина строки иконок
    int str_width=style.x_tab*(HSize+style.x_spacing)-style.x_spacing;
    //коээфициент горизонтального выравнивания
    int hor_align=(disp_w-str_width)/2;

    //переменные для хранения размеров курсора (для корректной отрисовки)
    int curW=style.cursor_w,curH=style.cursor_h;
    if(curW==0)            //проверка, чтобы всё норм было: если размер указан 0, то эльф сам возьмет размер иконки из картинки курсора
    {
      curW=GetImageWidth(Menu_Get()->cursorID);
    }
    if(curH==0)            //проверка, чтобы всё норм было: если размер указан 0, то эльф сам возьмет размер иконки из картинки курсора
    {
      curH=GetImageHeight(Menu_Get()->cursorID);
    }

    if(curW==0)curW=56;
    if(curH==0)curH=56;
    curW/=2;
    curH/=2;
    //рисуем курсор
    if(style.is_cursor_img==0)
    {
      DrawRect(x_pos+cursor_x-curW,
               cursor_y+scroll_y-curH,
               x_pos+cursor_x+curW,
               cursor_y+scroll_y+curH,
               style.cursor_color,
               style.cursor_color);
    }
    else
    {
      GC_PutChar(gc,
                 x_pos+cursor_x-curW,
                 cursor_y+scroll_y-curH,
                 curW*2,
                 curH*2,
                 Menu_Get()->cursorID);
    }
    //рисуем матрицу с иконками
    for(int i = 0; i < 100; i++)
    {
      for(int c = 0; c <= style.x_tab; c++)
      {
        if(cnt<Menu_Get()->list->FirstFree)
        {
          if(c<style.x_tab)
          {
            //создаем "ячейку" в таблице
            ITEM *it=(ITEM *)List_Get(Menu_Get()->list, cnt);
            int xsize,ysize;
            wchar_t imageid;
            ImageID_size_get(&xsize,&ysize,&imageid,it,c,i);

            //скроллинг
            if(i==nav_y)
            {
              int temp=yy+VSize+style.y_offset;
              if((cursor_y_dest-scroll_y+ysize>disp_h))
              {
                scroll_y_dest=disp_h-temp-ysize;
              }
              else
              {
                scroll_y_dest=0;
              }
            }
            //если столбик четный, то делаем отступ его по Y
            int taboff=0;
            if(c%2)taboff=style.eventab_offset;

            //выравнивание иконок относительно точных координат
            int icon_align=((xsize+scale[cnt])-HSize)/2;
            RECT rc;
            rc.x1=x_pos+hor_align+c*(HSize+style.x_spacing)-icon_align;
            rc.x2=x_pos+hor_align+c*(HSize+style.x_spacing)-icon_align+xsize+scale[cnt];
            rc.y1=yy+style.y_offset+taboff+scroll_y;
            rc.y2=yy+ysize+scale[cnt]+style.y_offset+taboff+scroll_y;



            //если это выделенная иконка
            if(cursor==cnt)
            {
              //обновляем координаты для курсора
              cursor_x_dest=x_pos+hor_align+c*(HSize+style.x_spacing)+(HSize/2);
              cursor_y_dest=yy+style.y_offset+taboff+(VSize/2);//style.y_spacing+yy;*/
            }

            //рисуем иконку
            DrawIcn(gc,rc,y_offset[cnt],x_offset[cnt],imageid);
            //если перемещаем пункт, то рисуем перемещаемую иконку поверх текущей
            if(cursor==cnt)
            {
              if(move)
              {
                ITEM *it2=(ITEM *)List_Get(Menu_Get()->list, move_index);
                ImageID_size_get(&xsize,&ysize,&imageid,it2,c,i);
                DrawIcn(gc,rc,-7,0,imageid);
              }
            }
          }
          else
          {
            yy+=VSize+style.y_spacing;
            break;
          }
          cnt++;
        }
      }
    }
    cnt=0;
    yy=0;

    //работа с текстом
    //если стиль - 0, то рисуем название выделенного пункта в заданных координатах
    if((style.lable_style==0)||(style.lable_style==2))
    {
      ITEM *it=(ITEM *)List_Get(Menu_Get()->list, cursor);
      int str=TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN);
      DrawS(style.lable_font,str,2, x_pos+style.lable_x, style.lable_y, disp_w, 100, style.lable_stroke_color,style.lable_color, 1);
      TEXT_FREE(str);
    }
    //если стиль - 1 или 2, то под каждой иконкой
    else if((style.lable_style==1)||(style.lable_style==2))
    {
      for(int i = 0; i < 100; i++)
      {
        for(int c = 0; c <= style.x_tab; c++)
        {
          if(cnt<Menu_Get()->list->FirstFree)
          {
            if(c<style.x_tab)
            {
              ITEM *it=(ITEM *)List_Get(Menu_Get()->list, cnt);

              int xsize,ysize;
              wchar_t imageid;
              ImageID_size_get(&xsize,&ysize,&imageid,it,c,i);

              //если столбик четный, то делаем отступ его по Y
            int taboff=0;
            if(c%2)taboff=style.eventab_offset;


              //создаем "ячейку" в таблице
              int icon_align=((xsize+scale[cnt])-HSize)/2;
              RECT rc;
              rc.x1=x_pos+hor_align+c*(HSize+style.x_spacing)-icon_align;
              rc.x2=x_pos+hor_align+c*(HSize+style.x_spacing)-icon_align+xsize+scale[cnt];
              rc.y1=yy+style.y_offset+taboff+scroll_y;
              rc.y2=yy+ysize+scale[cnt]+style.y_offset+taboff+scroll_y;

              //рисуем текст под каждой иконкой

              int str=TextID_Create(it->name,ENC_UCS2,TEXTID_ANY_LEN);
              DrawS(0xC,str,2, rc.x1, rc.y2+1, rc.x2,rc.y2+1+GetImageHeight(' '), style.lable_stroke_color,style.lable_color, 0);
              TEXT_FREE(str);
            }
            else
            {
              yy+=VSize+style.y_spacing;
              break;
            }
            cnt++;
          }
        }
      }
    }
  }
}

int cstate=0;

void onTimer (u16 unk , LPARAM)
{
  if(Display_GetTopBook(0)==FindBook(isMyBook))
  {
    //выдвигаем меню
    AnimSpeedMove(&x_pos,0, style.appear_speed);

    if(style.menu_style==0)
    {
      //прокручиваем экран, если надо
      AnimSpeedMove(&scroll_y,scroll_y_dest, style.scroll_speed);
      //анимируем курсор
      AnimSpeedMove(&cursor_x,cursor_x_dest, style.cursor_speed);
      AnimSpeedMove(&cursor_y,cursor_y_dest, style.cursor_speed);

    }

    //анимируем иконку
    for(int q = 0; q < 100; q++)
    {
      if(q!=cursor)
      {
        //возвращаем иконку на место
        AnimSpeedMove(&y_offset[q],0, style.icon_y_speed);
        AnimSpeedMove(&x_offset[q],0, style.icon_x_speed);
        AnimSpeedMove(&scale[q],style.min_scale,style.scale_speed);
      }
      else
      {
        //сдвиг по Y
        if(style.selected_type==0)
          AnimSpeedMove(&y_offset[q],style.icon_min_y_offset, style.icon_y_speed);
        //прыгание по Y
        else if(style.selected_type==1)
          AnimCicleMove(&y_offset[q],style.icon_min_y_offset,style.icon_max_y_offset,style.icon_y_speed,&icon_y_state);
        //увеличение
        else if(style.selected_type==2)
          AnimSpeedMove(&scale[q],style.max_scale,style.scale_speed);
        //циклическое увеличение
        else if(style.selected_type==3)
          AnimCicleMove(&scale[q],style.min_scale,style.max_scale,style.scale_speed,&scale_state);
        //сдвиг по X
        else if(style.selected_type==4)
          AnimCicleMove(&x_offset[q],style.icon_min_x_offset,style.icon_max_x_offset,style.icon_x_speed,&icon_x_state);
        //прыгание по X
        else if(style.selected_type==5)
          AnimSpeedMove(&x_offset[q],style.icon_min_x_offset, style.icon_x_speed);
      }

    }
    //обновляем экран
    DispObject_InvalidateRect(GUIObject_GetDispObject(mbk->gmenu),0);
  }
  //перезапускаем таймер
  Timer_ReSet(&timer,1,onTimer,0);
}

void GMenuGui_OnKey(DISP_OBJ *db,int key,int,int repeat,int type)
{
  if(style.menu_style==0)
  {
    if((key==KEY_RIGHT)&&((type==KBD_SHORT_PRESS)||(type==KBD_REPEAT)))
    {
      if(nav_x<(style.x_tab-1))nav_x++;
      else
      {
        nav_x=0;
        nav_y++;
      }
      cursor=get_nav(nav_x, nav_y);
      if(cursor>=Menu_Get()->list->FirstFree)
      {
        nav_y=0;
        nav_x=0;
        cursor=get_nav(nav_x, nav_y);
      }
      if(nav_y<0)
      {
        nav_y=get_max_y(Menu_Get()->list->FirstFree);
        nav_x=style.x_tab-1;
        cursor=get_nav(nav_x, nav_y);
      }

    }
    if((key==KEY_LEFT)&&((type==KBD_SHORT_PRESS)||(type==KBD_REPEAT)))
    {
      if(nav_x>0)nav_x--;
      else
      {
        nav_x=style.x_tab-1;
        nav_y--;
      }
      cursor=get_nav(nav_x, nav_y);
      if(cursor>=Menu_Get()->list->FirstFree)
      {
        nav_y=0;
        nav_x=0;
        cursor=get_nav(nav_x, nav_y);
      }
      if(nav_y<0)
      {
        nav_y=get_max_y(Menu_Get()->list->FirstFree);
        nav_x=style.x_tab-1;
        cursor=get_nav(nav_x, nav_y);
      }

    }
    if((key==KEY_UP)&&((type==KBD_SHORT_PRESS)||(type==KBD_REPEAT)))
    {
      nav_y--;
      cursor=get_nav(nav_x, nav_y);
      if(nav_y<0)
      {
        nav_y=get_max_y(Menu_Get()->list->FirstFree);
        cursor=get_nav(nav_x, nav_y);
      }

    }
    if((key==KEY_DOWN)&&((type==KBD_SHORT_PRESS)||(type==KBD_REPEAT)))
    {
      nav_y++;
      cursor=get_nav(nav_x, nav_y);
      if(cursor>=Menu_Get()->list->FirstFree)
      {
        nav_y=0;
        cursor=get_nav(nav_x, nav_y);
      }
    }
  }
  if ((type==KBD_SHORT_RELEASE))
  {
    HotKey(key);
  }

}
void GMenuOnEnterPressed(BOOK *bk, GUI*lt)
{
  ;
}
static const char SplashGuiName[]="Gui_Elf_Splash";

int GMenuGui_OnCreate(DISP_OBJ *db)
{
  //регаем курсор, если включена опция в стиле
  if(style.is_cursor_img==1)
  {
    FSTAT stat;
    IDFREE(Menu_Get()->cursorID);
    if(fstat(skin_path_int,Menu_Get()->cursor_img,&stat)==0)
      Reg_Image(&Menu_Get()->cursorID,skin_path_int,Menu_Get()->cursor_img);
    else if(fstat(skin_path_ext,Menu_Get()->cursor_img,&stat)==0)
      Reg_Image(&Menu_Get()->cursorID,skin_path_ext,Menu_Get()->cursor_img);
    else
      Menu_Get()->cursorID=NOIMAGE;
  }
  //
  scale_state=0;
  icon_y_state=0;
  icon_x_state=0;
  cursor_x_dest=0;
  cursor_y_dest=0;
  is_appeared=0;
  scroll_y=0;
  scroll_y_dest=0;
  disp_w=Display_GetWidth(0);
  disp_h=Display_GetHeight(0);
  for(int i=0;i<100;i++)
  {
    scale[i]=style.min_scale;
    x_offset[i]=0;
  }
  cursor_x=0;
  cursor_y=0;
  cursor=0;
  nav_x=0, nav_y=0;
  if(Menu_Get()->last<Menu_Get()->list->FirstFree)
  {
    cursor=Menu_Get()->last;
  }
  set_nav();
  x_pos=-disp_h;
  MMIPROC((void (*)(void))onTimer);
  return 1;
};

void GMenuGui_OnClose(DISP_OBJ *db)
{
  Timer_Kill(&timer);
};

void GMenuGui_constr(DISP_DESC *desc)
{
  DISP_DESC_SetName(desc,SplashGuiName);
  DISP_DESC_SetSize(desc,sizeof(DISP_OBJ));
  DISP_DESC_SetOnCreate(desc,(DISP_OBJ_ONCREATE_METHOD)GMenuGui_OnCreate);
  DISP_DESC_SetOnClose(desc,(DISP_OBJ_ONCLOSE_METHOD)GMenuGui_OnClose);
  DISP_DESC_SetOnRedraw(desc,(DISP_OBJ_ONREDRAW_METHOD)GMenuGui_OnRedraw);
  DISP_DESC_SetOnKey(desc,(DISP_OBJ_ONKEY_METHOD)GMenuGui_OnKey);
};

void GMenuGui_destr(GUI*g)
{
  return;
};

GUI_MENU *CreateGMenuGUI()
{
  GUI_MENU *gui_skin=new GUI_MENU;
  if (!GUIObject_Create((GUI*)gui_skin,GMenuGui_destr, GMenuGui_constr,&mbk->book,0,0,0))
  {
    delete gui_skin;
    return 0;
  }
  if (mbk) BookObj_AddGUIObject(&mbk->book,(GUI*)gui_skin);
  mbk->gmenu=(GUI *)gui_skin;
  GUIObject_SetStyle((GUI*)gui_skin,Menu_Get()->gui_style);
  GUIObject_SoftKeys_Hide((GUI*)gui_skin);
  GUIObject_SetTitleType((GUI*)gui_skin, 1);
  return gui_skin;
};

int GMenuOnCreate(void *, BOOK * bk)
{
  GUI * skin_gui=(GUI *)CreateGMenuGUI();

  GUIObject_SoftKeys_SetAction(skin_gui,ACTION_SELECT1,MenuOnEnterPressed);
  GUIObject_SoftKeys_SetAction(skin_gui,0,Edit_);
  GUIObject_SoftKeys_SetText(skin_gui,0,TextID_Create(names[4],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,1,MenuAdd);
  GUIObject_SoftKeys_SetText(skin_gui,1,TextID_Create(names[5],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,2,SubMenuAdd);
  GUIObject_SoftKeys_SetText(skin_gui,2,TextID_Create(names[29],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,3,Move);
  GUIObject_SoftKeys_SetText(skin_gui,3,TextID_Create(names[46],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,4,Style_);
  GUIObject_SoftKeys_SetText(skin_gui,4,TextID_Create(names[6],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,5,Options_);
  GUIObject_SoftKeys_SetText(skin_gui,5,TextID_Create(names[0],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,6,Skin_);
  GUIObject_SoftKeys_SetText(skin_gui,6,TextID_Create(names[31],ENC_UCS2,TEXTID_ANY_LEN));
  GUIObject_SoftKeys_SetAction(skin_gui,ACTION_DELETE, OnDelGui);
  GUIObject_SoftKeys_SetText(skin_gui,ACTION_DELETE,TextID_Create(names[25],ENC_UCS2,TEXTID_ANY_LEN));

  GUIObject_SoftKeys_SetAction(skin_gui,ACTION_BACK, MenuBack);
  GUIObject_SoftKeys_SetAction(skin_gui,ACTION_LONG_BACK, MenuBack);
  DISP_OBJ*db=GUIObject_GetDispObject(skin_gui);
  DispObject_SetLayerColor(db,style.back_color);
  GUIObject_Show(skin_gui);
  return (1);
};

int GMenuOnExit(void *, BOOK * bk)
{
  return (1);
};
const PAGE_MSG bk_msglst_GMenu[] @ "DYN_PAGE"  =
{
  PAGE_ENTER_EVENT_TAG,            GMenuOnCreate,
  PAGE_EXIT_EVENT_TAG,             GMenuOnExit,
  NIL_EVENT_TAG,                   NULL
};

PAGE_DESC bk_GMenu = {"GMenu",0,bk_msglst_GMenu};
