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
  char start; // ������ ������ ��� �����������
  char picture_size; // ������
  char video_length; // ������������
  char focus; // �����������
  char flash; // �������
  char light; // ���������
  char night_mode; // ������ �����
  char effects; // �������
  char white_balance; // ������ ������
  char picture_quality; // ��������
  char save_to; // ��������� �
  char shutter_sound; // ���� �������
  char metering_mode; // ����� ���������
  char unk_E;
  char scenes; // ������� ������
  char microphone; // ��������
  char review; // ��������
  char af_light; // ��������� ������
  char unk_13;
  char unk_14;
  char auto_rotate; // �����������
  char add_position; // �������� �������
  char unk_17;
}CAMERA_SETTINGS;

#endif
