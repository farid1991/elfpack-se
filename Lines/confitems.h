#pragma diag_suppress=Pe177

#ifndef CD_CLEAN

#define uint unsigned int

#if defined CD_HEADER_MODE && defined CD_LANG_MODE
#error Both CD_HEADER_MODE and CD_LANG_MODE are defined.
#endif

#ifndef NAME_PREFIX
#define FUNC2(x,y) x##y
#else
#define FUNC4(x,y) x##y
#define FUNC3(x,y) FUNC4(x,y)
#define FUNC2(x,y) FUNC3(NAME_PREFIX,x##y)
#endif
#define FUNC1(x,y) FUNC2(x,y)
#define _NAME(x) FUNC1(x,__LINE__)

#if !defined CD_HEADER_MODE && !defined CD_LANG_MODE

#include "cfg_items.h"

#define CD_HEADER_MODE
#include _SOURCE_NAME_
#undef CD_HEADER_MODE

#pragma diag_suppress=Pe047

#define LANG(rus, eng) rus
#define _HEADER(type, x, min, max) __root const CFG_HDR _NAME(cfg) = { type, x, min, max }
#define _VAR(type, name, val) __root const type name = val
#define _CBOXITEMS(items, ...) __root const CFG_CBOX_ITEM _NAME(cbox)[items] = {__VA_ARGS__}

#elif defined CD_HEADER_MODE

#define LANG(rus, eng)
#define _HEADER(type, x, min, max) extern const CFG_HDR _NAME(cfg)
#define _VAR(type, name, val) extern const type name
#define _CBOXITEMS(items, ...) extern const CFG_CBOX_ITEM _NAME(cbox)[items]

#elif defined CD_LANG_MODE

#if defined CD_LANG_RUS && defined CD_LANG_ENG
#error Both CD_LANG_RUS and CD_LANG_ENG are defined.
#endif

#pragma diag_suppress=Pe047

#ifdef CD_LANG_RUS
#define LANG(rus, eng) rus
#else
#define LANG(rus, eng) eng
#endif

#define _HEADER(type, x, min, max) strcpy( (char *)_NAME(cfg).name, x )
#define _VAR(type, name, val)
#define _CBOXITEMS(items, ...) SetCBox( _NAME(cbox), items, __VA_ARGS__ )

#endif


#define LEVEL(x) _HEADER(CFG_LEVEL,x,1,0);
#if !defined CD_HEADER_MODE && !defined CD_LANG_MODE
#define LEVELUP __root const CFG_HDR _NAME(cfg) = { CFG_LEVEL, "", 0, 0 };//_HEADER(CFG_LEVEL,"",0,0);
#else
#define LEVELUP
#endif

#define UINT(x,name,min,max,stdval) _HEADER(CFG_UINT,x,min,max); \
  _VAR(uint, name, stdval);

#define CHECKBOX(x,name,stdval) _HEADER(CFG_CHECKBOX,x,0,0); \
  _VAR(int, name, stdval);

#define UTF16_STRING(x,name,stdval,min,max) _HEADER(CFG_UTF16_STRING,x,min,max-1); \
  _VAR(wchar_t, name[max], L##stdval);

#define KEYCODE(x,name_key,name_mode,std_key,std_mode) _HEADER(CFG_KEYCODE,x,0,0); \
  _VAR(int, name_key, std_key); \
  _VAR(int, name_mode, std_mode);

#define COORDINATES(x,fname,ffname,fval,ffval) _HEADER(CFG_COORDINATES,x,0,0); \
  _VAR(int, fname, fval); \
  _VAR(int, ffname, ffval);

#define COLOR_INT(x,vname,stdval) _HEADER(CFG_COLOR_INT,x,0,0); \
  _VAR(uint, vname, stdval);

#define FONT(x,vname,stdval) _HEADER(CFG_FONT,x,0,0); \
  _VAR(int, vname, stdval);

#define CBOX(x,vname,items,stdval, ...) _HEADER(CFG_CBOX,x,0,items); \
  _VAR(int, vname, stdval); \
  _CBOXITEMS(items, __VA_ARGS__);

#define RCT(x1,x2,y1,y2) {x1,x2,y1,y2}
#define RECT(x,fname,x1,x2,y1,y2) _HEADER(CFG_RECT,x,0,0); \
  _VAR(RECT, fname, RCT(x1,x2,y1,y2) );



#else

#undef FUNC4
#undef FUNC3
#undef FUNC2
#undef FUNC1
#undef _NAME
#undef LANG
#undef _HEADER
#undef _VAR
#undef _CBOXITEMS

#undef LEVEL
#undef LEVELUP
#undef UINT
#undef CHECKBOX
#undef UTF16_STRING
#undef KEYCODE
#undef COORDINATES
#undef COLOR_INT
#undef FONT
#undef CBOX
#undef RCT
#undef RECT

#endif
