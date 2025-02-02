#ifndef _MAIN_H_
#define _MAIN_H_

#define BOOK_NAME "CameraLenscover"
#define ABOUT_TEXT "CameraLenscover v. 4.0\n\n(c) E1kolyan\n(r) yr4ik"

typedef struct _CameraLenscoverBook : BOOK
{
  GUI_LIST* gui;
  int led;
  u16 led_timer;
  wchar_t FilePath[512];
  wchar_t FileName[512];
}CameraLenscoverBook;

typedef struct
{
  BOOK* book;
}MSG;

#define ELF_BCFG_CONFIG_EVENT 994

typedef struct
{
  BOOK* book;
  wchar_t* bcfg_p;
  wchar_t* bcfg_n;
}MSG_BCFG;

#define START_PHOTO 1
#define START_VIDEO 2

typedef struct
{
  char unk_0;
  char start; // Запуск камеры или видеокамеры
  char picture_size; // Размер
  char video_length; // Длительность
  char focus; // Фокусировка
  char flash; // Вспышка
  char light; // Подсветка
  char night_mode; // Ночной режим
  char effects; // Эффекты
  char white_balance; // Баланс белого
  char picture_quality; // Качество
  char save_to; // Сохранить в
  char shutter_sound; // Звук затвора
  char metering_mode; // Режим измерения
  char unk_E;
  char scenes; // Условия съемки
  char microphone; // Микрофон
  char review; // Просмотр
  char af_light; // Подсветка фокуса
  char unk_13;
  char unk_14;
  char auto_rotate; // Автоповорот
  char add_position; // Добавить позицию
  char unk_17;
}CAMERA_SETTINGS;

#endif
