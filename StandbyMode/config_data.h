#ifndef _CONFIGDATA_H_
  #define _CONFIGDATA_H_
#include "..\\include\cfg_items.h"

#ifdef ENG
  #define IMAGES_MSG   "Images..."
  #define IMAGE_MSG    "Image"
  #define SLIDESHOW_MSG "Slideshow"
  #define SHOWING_MSG  "Showing"
  #define NOT_SHOW_MSG "Not show"
  #define SLIDESHOW_FOLDER_MSG "Folder for slideshow"
  #define IMAGES_CHANGING_TIME "Images changing in time (min)"
  #define FILE_MSG     "File"
  #define SHOW_MSG     "Show"
  #define SHOW2_MSG    "Show (sec)"
  #define SHOW_IMAGE_MSG     "Show image"
  #define SHOW_ICON_MSG     "Show icon"
  #define USE_ANIMATION_MSG  "Use animation"
  #define NO_MSG       "no"
  #define YES_MSG      "yes"
  #define POSITION_MSG "XY Position"
  #define Y_OFFSET_MSG "Y Offset"
  #define POSITION2_MSG "Size & Position"
  #define CLOCK_MSG    "Clock"
  #define DATE_MSG     "Date"
  #define DAY_MSG      "Day"
  #define FORMAT_MSG   "Format"
  #define AUTO_MSG     "Auto"
  #define AM_PM_MSG    "am/pm"
  #define HOURS_24_MSG "24 hours"
  #define SECONDS_MSG  "Seconds"
  #define DEL_CLOCK    "Remove orig. clock"
  #define DEL_DATE     "Remove orig. date"
  #define ORIG_CLOCK   "Orig. clock"
  #define ORIG_DATE    "Orig. date"
  #define DEL_YEAR_MSG "Remove year from date"
  #define FONT_MSG     "Font"
  #define COLOR_MSG    "Color"
  #define COLOR2_MSG   "Color"
  #define BORDER_MSG   "Border"
  #define ALIGN_MSG    "Alignment"
  #define LEFT_MSG     "left"
  #define RIGHT_MSG    "right"
  #define CENTER_MSG   "center"
  #define MODE_MSG     "Mode"
  #define MESSAGE_MSG  "Message"
  #define MASSAGE2_MSG "Message"
  #define REDUCED_MSG  "reduced"
  #define FULL_MSG     "full"
  #define FIRST_SYMBOL_MSG "First symbol"
  #define AS_IS_MSG    "as is"
  #define TO_UPPER_MSG "to upper"
  #define TO_LOWER_MSG "to lower"
  #define HEAP_MSG "Heap"
  #define KB_MSG "KB"
  #define MB_MSG "MB"
  #define GB_MSG "GB"
  #define BATT_MSG "Battery"
  #define TEMP_MSG "Temperature"
  #define SYSTEMP_MSG "System temperature"
  #define BATTTEMP_MSG "Battery temperature"
  #define CHARGE_MSG "Charge"
  #define DISCHARGE_MSG "Discharge"
  #define CURRENT_MSG "Charger Current"
  #define CHARGING_MSG "Charge indication"
  #define CHARGING_REDLED_MSG "Red LED Indication"
  #define CHARGING_LED_MSG "LEDs Indication"
  #define ONTIME_MSG "On time (ms)"
  #define OFFTIME_MSG "Off time (ms)"
  #define BLINK_SPEED_MSG "Blink speed (msec)"
  #define PHONE_TYPE_MSG "Phone"
  #define PHONE_TYPE0_MSG "Without leds"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "Other"
  #define PHONE_W580_MSG "For W580 only..."
  #define LEFT_LED_MSG "Left led"
  #define RIGHT_LED_MSG "Right led"
  #define PHONE_OTHER_MSG "For other phone..."
  #define LED_MODE_MSG "Blink mode"
  #define ONE_LED_MSG "One led"
  #define ALL_LEDS_MSG "All leds"
  #define CAPACITY_MSG "Battery Capacity"
  #define CHARGINGONLY_MSG "On charging only"
  #define MEM_INFO_MSG "Memory Info"
  #define PHONE_MSG "Phone"
  #define CARD_MSG "Card"
  #define FREE1_MSG "free (%)"
  #define FREE2_MSG "free (MB)"
  #define FREE3_MSG "free (GB)"
  #define USED1_MSG "used (%)"
  #define USED2_MSG "used (MB)"
  #define USED3_MSG "used (GB)"
  #define FRACTIONAL_MSG "Fractional part format"
  #define FRACTIONAL0_MSG "Without fractional part"
  #define FRACTIONAL1_MSG "One character"
  #define FRACTIONAL2_MSG "Two characters"
  #define UPDATE_MSG "Update (sec)"
  #define UPDATE2_MSG "Update (msec)"
  #define FM_MSG "FM radio"
  #define FREQ_MSG "Frequency of station"
  #define NAME_MSG "Name"
  #define NAME1ST_MSG "Name first"
  #define FREQ_ONLY_MSG "Frequency only"
  #define NAME_ONLY_MSG "Name only"
  #define FREQ_AND_NAME_ONLY_MSG "Frequency and Name"
  #define FM_MODE_MSG "Stereo mode"
  #define RDS_DATA_MSG "RDS data"
  #define CUSTOM_MSG "Custom Messages..."
  #define OK_MSG "Inscription 'Ok'"
  #define SOFT_KEY_MSG "Soft keys"
  #define ON_MSG "on"
  #define OFF_MSG "off"
  #define OPTIONS_MSG "Options..."
  #define HIGHLIGHT_MSG "Highlight"
  #define HIGHLIGHT_NO_MSG "No"
  #define HIGHLIGHT_FULLV1_MSG "Full v1"
  #define HIGHLIGHT_FULLV2_MSG "Full v2"
  #define HIGHLIGHT_SHADOW_MSG "Shadow"
  #define ENC_FILE_MSG "Char. enc. file"
  #define OPERATOR_MSG "Operator logo..."
  #define DEL_1ST_LINE_MSG "Delete 1st line"
  #define DEL_2ND_LINE_MSG "Delete 2nd line"
  #define DEL_3RD_LINE_MSG "Delete 3rd line"
  #define REFRESH_MSG "Refresh time (msec)"
  #define PLAYER_MSG "Music Player"
  #define TRACK_ADDR_MSG "Track address"
  #define TITLE_MSG "Title"
  #define ALBUM_MSG "Album"
  #define ARTIST_MSG "Artist"
  #define YEAR_MSG "Year"
  #define GENRE_MSG "Genre"
  #define FREQUENCY_MSG "Frequency"
  #define BITRATE_MSG "Bitrate"
  #define CHANNELS_MSG "Channels"
  #define FULLTIME_MSG "Duration"
  #define COVER_MSG "Cover Art"
  #define CUR_MSG "Current"
  #define END_MSG "End"
  #define TIME_MSG "time"
  #define BAR_MSG "Bar"
  #define TXT_MSG "Text"
  #define USED_AREA_MSG "Used area"
  #define FREE_AREA_MSG "Free area"
  #define STANDBAY_ONLY_MSG "In standby only"
  #define OFF_IF_PLAY_MSG "Switch off at music"
  #define USE_PHONE_FUNC_MSG "Use phone func. for get ID3"
  #define COVER_FILE_MSG "Custom cover"
  #define SCROLL_MSG "Scrolling"
  #define SCROLL2_MSG "Name scrolling"
  #define SWITCH_OFF_MSG "Switch off"
  #define ALWAYS_MSG "Always switch on"
  #define IF_LONG_MSG "Switch on if long name"
  #define SCROLL_SPEED_MSG "Speed"
  #define SCROLL_DELAY_MSG "Start delay"
  #define EMPTY_AREA_MSG "Empty area"
  #define PERCENT_0_MSG "0%"
  #define PERCENT_5_MSG "5%"
  #define PERCENT_10_MSG "10%"
  #define PERCENT_15_MSG "15%"
  #define PERCENT_20_MSG "20%"
  #define PERCENT_25_MSG "25%"
  #define PERCENT_30_MSG "30%"
  #define PERCENT_35_MSG "35%"
  #define PERCENT_40_MSG "40%"
  #define PERCENT_45_MSG "45%"
  #define PERCENT_50_MSG "50%"
  #define PERCENT_55_MSG "55%"
  #define PERCENT_60_MSG "60%"
  #define PERCENT_65_MSG "65%"
  #define PERCENT_70_MSG "70%"
  #define PERCENT_75_MSG "75%"
  #define PERCENT_80_MSG "80%"
  #define PERCENT_85_MSG "85%"
  #define PERCENT_90_MSG "90%"
  #define PERCENT_95_MSG "95%"
  #define PERCENT_100_MSG "100%"
  #define CUSTOM_NAMES_MSG "Custom namedays"
  #define NAMES_MSG "Namedays"
  #define SUN_MSG "Sunday"
  #define MON_MSG "Monday"
  #define TUE_MSG "Tuesday"
  #define WED_MSG "Wednesday"
  #define THU_MSG "Thursday"
  #define FRI_MSG "Friday"
  #define SAT_MSG "Saturday"
  #define LSUN_MSG L"Sunday"
  #define LMON_MSG L"Monday"
  #define LTUE_MSG L"Tuesday"
  #define LWED_MSG L"Wednesday"
  #define LTHU_MSG L"Thursday"
  #define LFRI_MSG L"Friday"
  #define LSAT_MSG L"Saturday"
  #define COVER_UNDER_IMAGE_MSG "Cover under image"
  #define SYMBOL_MSG "Symbol"
  #define USE_IMAGES_MSG "Use images"
  #define NETWORK_MSG "Network"
  #define CUSTOM_IMAGES_MSG "Custom images"
  #define CUSTOM_IMAGE_MSG "Custom image"
  #define GPRS_MSG "GPRS"
  #define EDGE_MSG "EDGE"
  #define NOT_ACTIVE_MSG "Not active"
  #define ACTIVE_MSG "Active"
  #define NONE_MSG "Without EDGE&GPRS"
  #define FLIGHT_MSG "Flight mode"
  #define BAR_0_MSG "None signal"
  #define BAR_1_MSG "1 line"
  #define BAR_2_MSG "2 line"
  #define BAR_3_MSG "3 line"
  #define BAR_4_MSG "4 line"
  #define BAR_5_MSG "5 line"
  #define DBM_MSG "dBm"
  #define APPLICATION_MSG "Applications"
  #define FILL_BY_ROW_MSG "Fill by row"
  #define ROW_MAX_MSG "Rows max"
  #define COL_MAX_MSG "Columns max"
  #define PERCENT_MSG "percent"
  #define ICONS_MSG "Icons"
  #define BOOKS_CNT_MSG "Books count"
  #define ELFS_CNT_MSG "Elfs count"
  #define JAVA_CNT_MSG "Java count"
  #define NONSTANDBY_MSG "Not in Standby"
  #define IF_PLAYER_RUNNING_MSG "If player running"
  #define STATUSBAR_MSG   "Status icons"
  #define SHOW_ORIG_STATUS_MSG "Sho orig. statusbar"
  #define CUSTOM_ICON_MSG "Custom icon"
  #define ICON_FILE_MSG   "Icon's file"
  #define SILENT_MSG "Silent mode"
  #define PLAYERS_MSG "Player/radio/videoplayer"
  #define JAVA_MSG "Java"
  #define KEY_LOCK_MSG "Key locked"
  #define MISSED_MSG "Missed events"
  #define PROFILE_MSG "Profile"
  #define BLUETOOTH_MSG "Bluetooth"
  #define USB_CON_MSG "USB"
  #define MEDIAPLAYER_MSG "Player"
  #define FMRADIO_MSG "Radio"
  #define VIDEO_MSG "Videoplayer"
  #define M_CALSS_MSG "Calls"
  #define M_SMS_MSG "SMS"
  #define M_MMS_MSG "MMS"
  #define M_EMAIL_MSG "e-mail"
  #define MEETING_MSG "Meeting"
  #define INCAR_MSG "Incar"
  #define OUTDOORS_MSG "Outdoors"
  #define PHF_MSG "Handsfree"
  #define HOME_MSG "Home"
  #define OFFICE_MSG "Office"
  #define BT_VIS_MSG "Visible"
  #define BT_UNVIS_MSG "Unvisible"
  #define BT_ACTIVE_MSG "Active"
  #define SEL_ICONS_MSG "Selection of icons"
  #define HEADSET_MSG "Headset"
  #define HANDSFREE_MSG "Hands free"
  #define INTERNET_MSG "Internet"
  #define ALARM_MSG "Alarm"
  #define SES_ACTIVE_MSG "Inernet active"
  #define SES_ACTIVITY_MSG "Inernet activity"
  #define SES_SUSPENDED_MSG "Inernet suspended"
  #define IRDA_MSG "IR port"
  #define STOPWATCH_MSG "Stopwatch"
  #define ALLOWEDCALLERS_MSG "Accept calls"
  #define CALLERSFROMLIST_MSG "Only from list"
  #define NONECALLERS_MSG "None"
  #define RESSIGNMENT_MSG "Control reassignments"
  #define RESSIGNMENTADDR_MSG "Address"
  #define INCORRECTRESSIGNMENTADDR_MSG L"Incorrect address for ressignment!"
  #define CALLTIME_MSG "Call time"
  #define MESSAGES_MSG "Message sending"
  #define SMS_MSG "SMS"
  #define MMS_MSG "MMS"
  #define EMAIL_MSG "E-mail"
  #define REVERSE_MSG "Reverse"
  #define DOWNLOAD_MSG "Downloading"
  #define BINCLOCK_MSG "Binary Clock"
  #define FILERES_MSG "Resource file"
  #define NUMSTATUS_MSG "Number Status"
  #define HIDE_NUM_MSG "Hide number"
  #define SHOW_NUM_MSG "Show number"
  #define BY_NET_NUM_MSG "Specified by net"
  #define VOICE_IND_MSG "Voice indication"
  #define CHARGE_LEVEL_MSG "Voice on charge"
  #define DISCHARGE_LEVEL_MSG "Voice on discharge"
  #define LEVEL_LESS_MSG "if level less, %"
  #define VOICE_SETINGS_MSG "Sound settings"
  #define PATH_TO_VOICE_MSG "Path to sound"
  #define SOUND_VOLUME_MSG "Sound volume"
  #define ONSILENT_MSG "On silent mode"
  #define VOICE_TIMING_MSG "Timing of work"
  #define START_FROM_MSG "Start from"
  #define END_TO_MSG "End to"

#else
  #define IMAGES_MSG   "��������..."
  #define IMAGE_MSG    "��������"
  #define SLIDESHOW_MSG "��������"
  #define SHOWING_MSG  "����������"
  #define NOT_SHOW_MSG "�� ����������"
  #define SLIDESHOW_FOLDER_MSG "����� ��� ��������"
  #define IMAGES_CHANGING_TIME "������ ����� �������� (���)"
  #define FILE_MSG     "���� ��������"
  #define SHOW_MSG     "����������"
  #define SHOW2_MSG    "���������� (���)"
  #define SHOW_IMAGE_MSG     "���������� ��������"
  #define SHOW_ICON_MSG     "���������� ������"
  #define USE_ANIMATION_MSG  "������������ ��������"
  #define NO_MSG       "���"
  #define YES_MSG      "��"
  #define POSITION_MSG "XY ����������"
  #define Y_OFFSET_MSG "�������� �� Y"
  #define POSITION2_MSG "������ & ����������"
  #define CLOCK_MSG    "�����"
  #define DATE_MSG     "����"
  #define DAY_MSG      "����"
  #define FORMAT_MSG   "������"
  #define AUTO_MSG     "����"
  #define AM_PM_MSG    "am/pm"
  #define HOURS_24_MSG "24 ����"
  #define SECONDS_MSG  "�������"
  #define DEL_CLOCK    "������� ����. ����"
  #define DEL_DATE     "������� ����. ����"
  #define ORIG_CLOCK   "����. ����"
  #define ORIG_DATE    "����. ����"
  #define DEL_YEAR_MSG "������� ��� �� ����"
  #define FONT_MSG     "�����"
  #define COLOR_MSG    "���� ������"
  #define COLOR2_MSG   "����"
  #define BORDER_MSG   "���� �������"
  #define ALIGN_MSG    "������������"
  #define LEFT_MSG     "�����"
  #define RIGHT_MSG    "������"
  #define CENTER_MSG   "�� ������"
  #define MODE_MSG     "������"
  #define MESSAGE_MSG  "���������"
  #define MASSAGE2_MSG "�����"
  #define REDUCED_MSG  "����������"
  #define FULL_MSG     "���������"
  #define FIRST_SYMBOL_MSG "������ �����"
  #define AS_IS_MSG    "��� ����"
  #define TO_UPPER_MSG "�������"
  #define TO_LOWER_MSG "���������"
  #define HEAP_MSG "Heap"
  #define KB_MSG "KB"
  #define MB_MSG "MB"
  #define GB_MSG "GB"
  #define BATT_MSG "�������"
  #define TEMP_MSG "�����������"
  #define SYSTEMP_MSG "����������� ��������"
  #define BATTTEMP_MSG "����������� �������"
  #define CHARGE_MSG "�����"
  #define DISCHARGE_MSG "������"
  #define CURRENT_MSG "��� ������"
  #define CHARGING_MSG "��������� ������"
  #define CHARGING_REDLED_MSG "��������� ��. �����������"
  #define CHARGING_LED_MSG "��������� ������������"
  #define ONTIME_MSG "����� �� ���. ����.(����)"
  #define OFFTIME_MSG "����� � ����. ����.(����)"
  #define BLINK_SPEED_MSG "�������� ���. (����)"
  #define PHONE_TYPE_MSG "�������"
  #define PHONE_TYPE0_MSG "��� �����������"
  #define PHONE_TYPE1_MSG "W580"
  #define PHONE_TYPE2_MSG "������"
  #define PHONE_W580_MSG "������ ��� W580..."
  #define LEFT_LED_MSG "����� �����������"
  #define RIGHT_LED_MSG "������ �����������"
  #define PHONE_OTHER_MSG "��� ������ ���������..."
  #define LED_MODE_MSG "����� �������"
  #define ONE_LED_MSG "����� �����������"
  #define ALL_LEDS_MSG "����� ������������"
  #define CAPACITY_MSG "������� �������"
  #define CHARGINGONLY_MSG "������ �� ����� ������"
  #define MEM_INFO_MSG "���� � ������"
  #define PHONE_MSG "�������"
  #define CARD_MSG "����� ������"
  #define FREE1_MSG "��������� (%)"
  #define FREE2_MSG "��������� (MB)"
  #define FREE3_MSG "��������� (GB)"
  #define USED1_MSG "������������ (%)"
  #define USED2_MSG "������������ (MB)"
  #define USED3_MSG "������������ (GB)"
  #define FRACTIONAL_MSG "������ ������� �����"
  #define FRACTIONAL0_MSG "��� ������� �����"
  #define FRACTIONAL1_MSG "���� ������"
  #define FRACTIONAL2_MSG "��� �������"
  #define UPDATE_MSG "���������� (���)"
  #define UPDATE2_MSG "���������� (����)"
  #define FM_MSG "FM �����"
  #define FREQ_MSG "������� �������"
  #define NAME_MSG "���"
  #define NAME1ST_MSG "��� �������"
  #define FREQ_ONLY_MSG "������ �������"
  #define NAME_ONLY_MSG "������ ���"
  #define FREQ_AND_NAME_ONLY_MSG "������� � ���"
  #define FM_MODE_MSG "����� ������"
  #define RDS_DATA_MSG "RDS ������"
  #define CUSTOM_MSG "���� ���������..."
  #define OK_MSG "������� 'Ok'"
  #define SOFT_KEY_MSG "����� �� ������"
  #define ON_MSG "����������"
  #define OFF_MSG "�� ����������"
  #define OPTIONS_MSG "�������������..."
  #define HIGHLIGHT_MSG "�������"
  #define HIGHLIGHT_NO_MSG "���"
  #define HIGHLIGHT_FULLV1_MSG "������ v1"
  #define HIGHLIGHT_FULLV2_MSG "������ v2"
  #define HIGHLIGHT_SHADOW_MSG "����"
  #define ENC_FILE_MSG "���� ���������"
  #define OPERATOR_MSG "���� ���������..."
  #define DEL_1ST_LINE_MSG "������� ������ ������"
  #define DEL_2ND_LINE_MSG "������� ������ ������"
  #define DEL_3RD_LINE_MSG "������� ������ ������"
  #define REFRESH_MSG "���������� ������ (����)"
  #define PLAYER_MSG "�����"
  #define TRACK_ADDR_MSG "����� �����"
  #define TITLE_MSG "�������� �����"
  #define ALBUM_MSG "������"
  #define ARTIST_MSG "�����������"
  #define YEAR_MSG "���"
  #define GENRE_MSG "����"
  #define FREQUENCY_MSG "������� �������������"
  #define BITRATE_MSG "�������"
  #define CHANNELS_MSG "������"
  #define COVER_MSG "�������"
  #define FULLTIME_MSG "������������"
  #define CUR_MSG "�������"
  #define END_MSG "����������"
  #define TIME_MSG "�����"
  #define BAR_MSG "�������"
  #define TXT_MSG "�����"
  #define USED_AREA_MSG "������������ �����"
  #define FREE_AREA_MSG "��������� �����"
  #define STANDBAY_ONLY_MSG "������ � ������ ��������"
  #define OFF_IF_PLAY_MSG "��������� ��� ��������� ������"
  #define USE_PHONE_FUNC_MSG "������. �-��� ���. ��� ID3"
  #define COVER_FILE_MSG "���� �������"
  #define SCROLL_MSG "���������"
  #define SCROLL2_MSG "��������� ��������"
  #define SWITCH_OFF_MSG "���������"
  #define ALWAYS_MSG "������ ��������"
  #define IF_LONG_MSG "�������� ���� ������� ��������"
  #define SCROLL_SPEED_MSG "��������"
  #define SCROLL_DELAY_MSG "�������� ������"
  #define EMPTY_AREA_MSG "������ �����"
  #define PERCENT_0_MSG "0%"
  #define PERCENT_5_MSG "5%"
  #define PERCENT_10_MSG "10%"
  #define PERCENT_15_MSG "15%"
  #define PERCENT_20_MSG "20%"
  #define PERCENT_25_MSG "25%"
  #define PERCENT_30_MSG "30%"
  #define PERCENT_35_MSG "35%"
  #define PERCENT_40_MSG "40%"
  #define PERCENT_45_MSG "45%"
  #define PERCENT_50_MSG "50%"
  #define PERCENT_55_MSG "55%"
  #define PERCENT_60_MSG "60%"
  #define PERCENT_65_MSG "65%"
  #define PERCENT_70_MSG "70%"
  #define PERCENT_75_MSG "75%"
  #define PERCENT_80_MSG "80%"
  #define PERCENT_85_MSG "85%"
  #define PERCENT_90_MSG "90%"
  #define PERCENT_95_MSG "95%"
  #define PERCENT_100_MSG "100%"
  #define CUSTOM_NAMES_MSG "���� ����� ����"
  #define NAMES_MSG "�����"
  #define SUN_MSG "�����������"
  #define MON_MSG "�����������"
  #define TUE_MSG "�������"
  #define WED_MSG "�����"
  #define THU_MSG "�������"
  #define FRI_MSG "�������"
  #define SAT_MSG "�������"
  #define LSUN_MSG L"�����������"
  #define LMON_MSG L"�����������"
  #define LTUE_MSG L"�������"
  #define LWED_MSG L"�����"
  #define LTHU_MSG L"�������"
  #define LFRI_MSG L"�������"
  #define LSAT_MSG L"�������"
  #define COVER_UNDER_IMAGE_MSG "������� ��� ���������"
  #define SYMBOL_MSG "������"
  #define USE_IMAGES_MSG "������������ ��������"
  #define NETWORK_MSG "����"
  #define CUSTOM_IMAGES_MSG "���� ��������"
  #define CUSTOM_IMAGE_MSG "���� ��������"
  #define GPRS_MSG "GPRS"
  #define EDGE_MSG "EDGE"
  #define NOT_ACTIVE_MSG "�� ��������"
  #define ACTIVE_MSG "��������"
  #define NONE_MSG "��� EDGE � GPRS"
  #define FLIGHT_MSG "����� 'Flight'"
  #define BAR_0_MSG "��� �������"
  #define BAR_1_MSG "1 �������"
  #define BAR_2_MSG "2 �������"
  #define BAR_3_MSG "3 �������"
  #define BAR_4_MSG "4 �������"
  #define BAR_5_MSG "5 �������"
  #define DBM_MSG "���"
  #define APPLICATION_MSG "����������"
  #define FILL_BY_ROW_MSG "��������� �� �������"
  #define ROW_MAX_MSG "����. �����"
  #define COL_MAX_MSG "����. ��������"
  #define PERCENT_MSG "��������"
  #define ICONS_MSG "������"
  #define BOOKS_CNT_MSG "���������� ����"
  #define ELFS_CNT_MSG "���������� ������"
  #define JAVA_CNT_MSG "���������� Java ����������"
  #define NONSTANDBY_MSG "�����, ����� Standby"
  #define IF_PLAYER_RUNNING_MSG "���� ������� �����"
  #define STATUSBAR_MSG   "������ �������"
  #define SHOW_ORIG_STATUS_MSG "���������� ����. ������"
  #define CUSTOM_ICON_MSG "���� ������"
  #define ICON_FILE_MSG   "���� ������"
  #define SILENT_MSG "����� ��� �����"
  #define PLAYERS_MSG "�����/�����/����������"
  #define JAVA_MSG "Java"
  #define KEY_LOCK_MSG "���������� ����."
  #define MISSED_MSG "����������� �����."
  #define PROFILE_MSG "�������"
  #define BLUETOOTH_MSG "Bluetooth"
  #define USB_CON_MSG "�������. USB"
  #define MEDIAPLAYER_MSG "�����"
  #define FMRADIO_MSG "�����"
  #define VIDEO_MSG "����������"
  #define M_CALSS_MSG "������"
  #define M_SMS_MSG "SMS"
  #define M_MMS_MSG "MMS"
  #define M_EMAIL_MSG "e-mail"
  #define MEETING_MSG "���������"
  #define INCAR_MSG "� ����������"
  #define OUTDOORS_MSG "�� �����"
  #define PHF_MSG "����� ����."
  #define HOME_MSG "����"
  #define OFFICE_MSG "�� ������"
  #define BT_VIS_MSG "�������"
  #define BT_UNVIS_MSG "���������"
  #define BT_ACTIVE_MSG "����������"
  #define SEL_ICONS_MSG "����� ������"
  #define HEADSET_MSG "���������"
  #define HANDSFREE_MSG "������������ ���������"
  #define INTERNET_MSG "��������"
  #define ALARM_MSG "���������"
  #define SES_ACTIVE_MSG "���������� ��������"
  #define SES_ACTIVITY_MSG "�������� ��������"
  #define SES_SUSPENDED_MSG "���������������� ��������"
  #define IRDA_MSG "�� ����"
  #define STOPWATCH_MSG "C���������"
  #define ALLOWEDCALLERS_MSG "����������� ��. ������"
  #define CALLERSFROMLIST_MSG "������ �� ������"
  #define NONECALLERS_MSG "����"
  #define RESSIGNMENT_MSG "�������������� ����������"
  #define RESSIGNMENTADDR_MSG "�����"
  #define INCORRECTRESSIGNMENTADDR_MSG L"������������ ����� ��� �������������� ����������!"
  #define CALLTIME_MSG "������������ ������"
  #define MESSAGES_MSG "�������� ���������"
  #define SMS_MSG "SMS"
  #define MMS_MSG "MMS"
  #define EMAIL_MSG "��. �����"
  #define REVERSE_MSG "����������"
  #define DOWNLOAD_MSG "��������"
  #define BINCLOCK_MSG "�������� ����"
  #define FILERES_MSG "���� ��������"
  #define NUMSTATUS_MSG "������ ������"
  #define HIDE_NUM_MSG "������ �����"
  #define SHOW_NUM_MSG "�������� �����"
  #define BY_NET_NUM_MSG "������ �����"
  #define VOICE_IND_MSG "��������� ���������"
  #define CHARGE_LEVEL_MSG "���. ������ �������"
  #define DISCHARGE_LEVEL_MSG "���. ������ ��������"
  #define LEVEL_LESS_MSG "���� ������� �����, %"
  #define VOICE_SETINGS_MSG "��������� �����"
  #define PATH_TO_VOICE_MSG "���� � ������"
  #define SOUND_VOLUME_MSG "���������"
  #define ONSILENT_MSG "� ������ ��� �����"
  #define VOICE_TIMING_MSG "��������� ������� ������"
  #define START_FROM_MSG "������� �"
  #define END_TO_MSG "����������"

#endif

extern const int cfg_image1_show;
extern const unsigned int cfg_image1_x;
extern const unsigned int cfg_image1_y;
extern const wchar_t cfg_file1[256];

extern const int cfg_image2_show;
extern const unsigned int cfg_image2_x;
extern const unsigned int cfg_image2_y;
extern const wchar_t cfg_file2[256];

extern const int cfg_image3_show;
extern const unsigned int cfg_image3_x;
extern const unsigned int cfg_image3_y;
extern const wchar_t cfg_file3[256];

extern const int cfg_image4_show;
extern const unsigned int cfg_image4_x;
extern const unsigned int cfg_image4_y;
extern const wchar_t cfg_file4[256];

extern const int cfg_image5_show;
extern const unsigned int cfg_image5_x;
extern const unsigned int cfg_image5_y;
extern const wchar_t cfg_file5[256];

extern const int cfg_image6_show;
extern const unsigned int cfg_image6_x;
extern const unsigned int cfg_image6_y;
extern const RECT cfg_image6_slideshow;
extern const wchar_t cfg_file6[256];
extern const wchar_t cfg_folder6[256];
extern const unsigned int cfg_slideshow_time;

extern const int cfg_fm_image_show;
extern const unsigned int cfg_fm_image_x;
extern const unsigned int cfg_fm_image_y;
extern const wchar_t cfg_fm_file[256];

extern const int cfg_player_image_show;
extern const unsigned int cfg_player_image_x;
extern const unsigned int cfg_player_image_y;
extern const wchar_t cfg_player_file[256];

extern const int cfg_clock_show;
extern const int cfg_clock_format;
extern const int cfg_seconds;
extern const unsigned int cfg_seconds_font;
extern const int cfg_seconds_highlight;
extern const int cfg_seconds_offset_y;
extern const int cfg_remove_clock;
extern const unsigned int cfg_clock_font;
extern const int cfg_clock_highlight;
extern const unsigned int cfg_clock_color;
extern const unsigned int cfg_clock_border;
extern const int cfg_clock_align;
extern const unsigned int cfg_clock_x;
extern const unsigned int cfg_clock_y;

extern const int cfg_clock2_show;
extern const int cfg_clock2_nonstandby_show;
extern const int cfg_clock2_mediapleyer_show;
extern const int cfg_clock2_format;
extern const int cfg_seconds2;
extern const unsigned int cfg_seconds2_font;
extern const int cfg_seconds2_highlight;
extern const int cfg_seconds2_offset_y;
extern const unsigned int cfg_clock2_font;
extern const int cfg_clock2_highlight;
extern const unsigned int cfg_clock2_color;
extern const unsigned int cfg_clock2_border;
extern const int cfg_clock2_align;
extern const unsigned int cfg_clock2_x;
extern const unsigned int cfg_clock2_y;

extern const int cfg_clock3_show;
extern const unsigned int cfg_clock3_x;
extern const unsigned int cfg_clock3_y;
extern const wchar_t cfg_clock3_resource[256];

extern const int cfg_date_show;
extern const int cfg_date_format;
extern const int cfg_remove_year;
extern const int cfg_remove_date;
extern const unsigned int cfg_date_font;
extern const int cfg_date_highlight;
extern const unsigned int cfg_date_color;
extern const unsigned int cfg_date_border;
extern const int cfg_date_align;
extern const unsigned int cfg_date_x;
extern const unsigned int cfg_date_y;

extern const int cfg_day_show;
extern const int cfg_day_mode;
extern const int cfg_day_format;
extern const unsigned int cfg_day_font;
extern const int cfg_day_highlight;
extern const unsigned int cfg_day_color;
extern const unsigned int cfg_day_border;
extern const int cfg_day_align;
extern const unsigned int cfg_day_x;
extern const unsigned int cfg_day_y;
extern const int cfg_custom_day;
extern const wchar_t cfg_sun[32];
extern const wchar_t cfg_mon[32];
extern const wchar_t cfg_tue[32];
extern const wchar_t cfg_wed[32];
extern const wchar_t cfg_thu[32];
extern const wchar_t cfg_fri[32];
extern const wchar_t cfg_sat[32];

extern const int cfg_alarm_show;
extern const unsigned int cfg_alarm_font;
extern const int cfg_alarm_highlight;
extern const unsigned int cfg_alarm_color;
extern const unsigned int cfg_alarm_border;
extern const int cfg_alarm_align;
extern const unsigned int cfg_alarm_x;
extern const unsigned int cfg_alarm_y;
extern const int cfg_alarm_icon_show;
extern const wchar_t cfg_alarm_file[256];
extern const int cfg_alarm_image_show;
extern const unsigned int cfg_alarm_image_x;
extern const unsigned int cfg_alarm_image_y;
extern const wchar_t cfg_alarm_image_file[256];
extern const unsigned int cfg_refresh_alarm;

extern const int cfg_heap_show;
extern const int cfg_heap_format;
extern const unsigned int cfg_heap_font;
extern const int cfg_heap_highlight;
extern const unsigned int cfg_heap_color;
extern const unsigned int cfg_heap_border;
extern const int cfg_heap_align;
extern const unsigned int cfg_heap_x;
extern const unsigned int cfg_heap_y;
extern const unsigned int cfg_refresh_heap;

extern const int cfg_batt_show;
extern const int cfg_percent;
extern const unsigned int cfg_percent_font;
extern const int cfg_batt_percent_highlight;
extern const int cfg_percent_offset_y;
extern const unsigned int cfg_batt_font;
extern const int cfg_batt_highlight;
extern const unsigned int cfg_batt_color;
extern const unsigned int cfg_batt_border;
extern const int cfg_batt_align;
extern const unsigned int cfg_batt_x;
extern const unsigned int cfg_batt_y;

extern const int cfg_batt_bar_show;
extern const int cfg_batt_bar_images;
extern const int cfg_batt_images_custom;
extern const int cfg_batt_bar_highlight;
extern const RECT cfg_batt_bar;
extern const unsigned int cfg_batt_bar_border;
extern const unsigned int cfg_batt_empty_color;
extern const unsigned int cfg_batt_0_color;
extern const unsigned int cfg_batt_10_color;
extern const unsigned int cfg_batt_20_color;
extern const unsigned int cfg_batt_30_color;
extern const unsigned int cfg_batt_40_color;
extern const unsigned int cfg_batt_50_color;
extern const unsigned int cfg_batt_60_color;
extern const unsigned int cfg_batt_70_color;
extern const unsigned int cfg_batt_80_color;
extern const unsigned int cfg_batt_90_color;
extern const unsigned int cfg_batt_100_color;
extern const unsigned int cfg_batt_bar_x;
extern const unsigned int cfg_batt_bar_y;
extern const int cfg_batt_bar_reverse;
extern const wchar_t cfg_batt_0_file[256];
extern const wchar_t cfg_batt_5_file[256];
extern const wchar_t cfg_batt_15_file[256];
extern const wchar_t cfg_batt_25_file[256];
extern const wchar_t cfg_batt_35_file[256];
extern const wchar_t cfg_batt_45_file[256];
extern const wchar_t cfg_batt_55_file[256];
extern const wchar_t cfg_batt_65_file[256];
extern const wchar_t cfg_batt_75_file[256];
extern const wchar_t cfg_batt_85_file[256];
extern const wchar_t cfg_batt_100_file[256];

extern const int cfg_current_show;
extern const unsigned int cfg_current_font;
extern const int cfg_current_highlight;
extern const unsigned int cfg_current_color;
extern const unsigned int cfg_current_border;
extern const int cfg_current_align;
extern const unsigned int cfg_current_x;
extern const unsigned int cfg_current_y;

extern const int cfg_batt_charge_anim;
extern const int cfg_batt_charge_show;
extern const int cfg_batt_charge_custom;
extern const unsigned int cfg_batt_charge_x;
extern const unsigned int cfg_batt_charge_y;
extern const wchar_t cfg_batt_charge_file[256];

//��������� ������������
extern const int cfg_charge_Led_show;
extern const unsigned int cfg_charge_Led_OnTime;
extern const unsigned int cfg_charge_Led_OffTime;

extern const int cfg_charge_RedLed_show;
extern const unsigned int cfg_charge_RedLed_OnTime;
extern const unsigned int cfg_charge_RedLed_OffTime;

extern const int cfg_charge_say_levels_up;
extern const int cfg_charge_say_levels_down;
extern const unsigned int cfg_charge_begin_from;
extern const wchar_t cfg_charge_folder_path[256];
extern const unsigned int cfg_charge_volume;
extern const int cfg_charge_silent;
extern const TIME cfg_charge_TimeFrom;
extern const TIME cfg_charge_TimeTo;

extern const int cfg_batt_temp_show;
extern const int cfg_charging_only;
extern const unsigned int cfg_batt_temp_font;
extern const int cfg_batt_temp_highlight;
extern const unsigned int cfg_batt_temp_color;
extern const unsigned int cfg_batt_temp_border;
extern const int cfg_batt_temp_align;
extern const unsigned int cfg_batt_temp_x;
extern const unsigned int cfg_batt_temp_y;

extern const int cfg_temp_show;
extern const unsigned int cfg_temp_font;
extern const int cfg_temp_highlight;
extern const unsigned int cfg_temp_color;
extern const unsigned int cfg_temp_border;
extern const int cfg_temp_align;
extern const unsigned int cfg_temp_x;
extern const unsigned int cfg_temp_y;

extern const unsigned int cfg_refresh_batt_temp;

extern const int cfg_phone_show;
extern const int cfg_phone_format;
extern const int cfg_phone_fractional;
extern const wchar_t cfg_phone_msg[64];
extern const unsigned int cfg_phone_font;
extern const int cfg_phone_highlight;
extern const unsigned int cfg_phone_color;
extern const unsigned int cfg_phone_border;
extern const int cfg_phone_align;
extern const unsigned int cfg_phone_x;
extern const unsigned int cfg_phone_y;
extern const int cfg_phone_bar_show;
extern const int cfg_phone_bar_highlight;
extern const RECT cfg_phone_bar;
extern const unsigned int cfg_phone_used_color;
extern const unsigned int cfg_phone_free_color;
extern const unsigned int cfg_phone_bar_border;

extern const int cfg_card_show;
extern const int cfg_card_format;
extern const int cfg_card_fractional;
extern const wchar_t cfg_card_msg[64];
extern const unsigned int cfg_card_font;
extern const int cfg_card_highlight;
extern const unsigned int cfg_card_color;
extern const unsigned int cfg_card_border;
extern const int cfg_card_align;
extern const unsigned int cfg_card_x;
extern const unsigned int cfg_card_y;
extern const int cfg_card_bar_show;
extern const int cfg_card_bar_highlight;
extern const RECT cfg_card_bar;
extern const unsigned int cfg_card_used_color;
extern const unsigned int cfg_card_free_color;
extern const unsigned int cfg_card_bar_border;

extern const unsigned int cfg_mem_update;

extern const int cfg_fm_show;
extern const int cfg_fm_name_first;
extern const unsigned int cfg_fm_font;
extern const int cfg_fm_highlight;
extern const unsigned int cfg_fm_color;
extern const unsigned int cfg_fm_border;
extern const int cfg_fm_align;
extern const unsigned int cfg_fm_x;
extern const unsigned int cfg_fm_y;
extern const wchar_t cfg_fm_msg[64];

extern const int cfg_fm_mode_show;
extern const unsigned int cfg_fm_mode_font;
extern const int cfg_fm_mode_highlight;
extern const unsigned int cfg_fm_mode_color;
extern const unsigned int cfg_fm_mode_border;
extern const int cfg_fm_mode_align;
extern const unsigned int cfg_fm_mode_x;
extern const unsigned int cfg_fm_mode_y;

extern const int cfg_fm_rdsdata_show;
extern const unsigned int cfg_fm_rdsdata_font;
extern const int cfg_fm_rdsdata_highlight;
extern const unsigned int cfg_fm_rdsdata_color;
extern const unsigned int cfg_fm_rdsdata_border;
extern const int cfg_fm_rdsdata_align;
extern const unsigned int cfg_fm_rdsdata_x;
extern const unsigned int cfg_fm_rdsdata_y;

extern const unsigned int cfg_refresh_media;

extern const int cfg_player_title_show;
extern const unsigned int cfg_player_title_font;
extern const int cfg_player_title_highlight;
extern const unsigned int cfg_player_title_color;
extern const unsigned int cfg_player_title_border;
extern const int cfg_player_title_align;
extern const unsigned int cfg_player_title_x;
extern const unsigned int cfg_player_title_y;
extern const RECT cfg_player_title_rc;

extern const int cfg_player_album_show;
extern const unsigned int cfg_player_album_font;
extern const int cfg_player_album_highlight;
extern const unsigned int cfg_player_album_color;
extern const unsigned int cfg_player_album_border;
extern const int cfg_player_album_align;
extern const RECT cfg_player_album_rc;

extern const int cfg_player_artist_show;
extern const unsigned int cfg_player_artist_font;
extern const int cfg_player_artist_highlight;
extern const unsigned int cfg_player_artist_color;
extern const unsigned int cfg_player_artist_border;
extern const int cfg_player_artist_align;
extern const RECT cfg_player_artist_rc;

extern const int cfg_player_year_show;
extern const unsigned int cfg_player_year_font;
extern const int cfg_player_year_highlight;
extern const unsigned int cfg_player_year_color;
extern const unsigned int cfg_player_year_border;
extern const int cfg_player_year_align;
extern const RECT cfg_player_year_rc;

extern const int cfg_player_genre_show;
extern const unsigned int cfg_player_genre_font;
extern const int cfg_player_genre_highlight;
extern const unsigned int cfg_player_genre_color;
extern const unsigned int cfg_player_genre_border;
extern const int cfg_player_genre_align;
extern const RECT cfg_player_genre_rc;

extern const int cfg_player_frequency_show;
extern const unsigned int cfg_player_frequency_font;
extern const int cfg_player_frequency_highlight;
extern const unsigned int cfg_player_frequency_color;
extern const unsigned int cfg_player_frequency_border;
extern const int cfg_player_frequency_align;
extern const RECT cfg_player_frequency_rc;

extern const int cfg_player_bitrate_show;
extern const unsigned int cfg_player_bitrate_font;
extern const int cfg_player_bitrate_highlight;
extern const unsigned int cfg_player_bitrate_color;
extern const unsigned int cfg_player_bitrate_border;
extern const int cfg_player_bitrate_align;
extern const RECT cfg_player_bitrate_rc;

extern const int cfg_player_channels_show;
extern const unsigned int cfg_player_channels_font;
extern const int cfg_player_channels_highlight;
extern const unsigned int cfg_player_channels_color;
extern const unsigned int cfg_player_channels_border;
extern const int cfg_player_channels_align;
extern const RECT cfg_player_channels_rc;

extern const int cfg_player_fulltime_show;
extern const unsigned int cfg_player_fulltime_font;
extern const int cfg_player_fulltime_highlight;
extern const unsigned int cfg_player_fulltime_color;
extern const unsigned int cfg_player_fulltime_border;
extern const int cfg_player_fulltime_align;
extern const RECT cfg_player_fulltime_rc;

extern const int cfg_player_curtime_show;
extern const int cfg_player_endtime_show;
extern const unsigned int cfg_player_curtime_font;
extern const int cfg_player_curtime_highlight;
extern const unsigned int cfg_player_curtime_color;
extern const unsigned int cfg_player_curtime_border;
extern const int cfg_player_curtime_align;
extern const RECT cfg_player_curtime_rc;

extern const int cfg_player_timebar_show;
extern const int cfg_player_timebar_highlight;
extern const RECT cfg_player_timebar;
extern const unsigned int cfg_player_timebar_cur_color;
extern const unsigned int cfg_player_timebar_rem_color;
extern const unsigned int cfg_player_timebar_border;

extern const int cfg_player_cover_show;
extern const RECT cfg_player_cover;
extern const char cfg_player_cover_file[64];
extern const int cfg_player_cover_under_image;

extern const int cfg_player_title_scroll;
extern const int cfg_player_album_scroll;
extern const int cfg_player_artist_scroll;
extern const int cfg_player_year_scroll;
extern const int cfg_player_genre_scroll;
extern const int cfg_player_frequency_scroll;
extern const int cfg_player_bitrate_scroll;
extern const int cfg_player_channels_scroll;
extern const int cfg_player_fulltime_scroll;
extern const int cfg_player_curtime_scroll;
extern const unsigned int cfg_player_scroll_speed;
extern const unsigned int cfg_player_scroll_delay;

extern const int cfg_image1_not_show_if_player;
extern const int cfg_image2_not_show_if_player;
extern const int cfg_image3_not_show_if_player;
extern const int cfg_image4_not_show_if_player;
extern const int cfg_image5_not_show_if_player;
extern const int cfg_image6_not_show_if_player;
extern const int cfg_clock_not_show_if_player;
extern const int cfg_clock2_not_show_if_player;
extern const int cfg_clock3_not_show_if_player;
extern const int cfg_orig_clock_not_show_if_player;
extern const int cfg_date_not_show_if_player;
extern const int cfg_orig_date_not_show_if_player;
extern const int cfg_day_not_show_if_player;
extern const int cfg_alarm_not_show_if_player;
extern const int cfg_heap_not_show_if_player;
extern const int cfg_batt_not_show_if_player;
extern const int cfg_batt_bar_not_show_if_player;
extern const int cfg_current_not_show_if_player;
extern const int cfg_batt_charge_not_show_if_player;
extern const int cfg_batt_temp_not_show_if_player;
extern const int cfg_temp_not_show_if_player;
extern const int cfg_phone_not_show_if_player;
extern const int cfg_card_not_show_if_player;
extern const int cfg_status_not_show_if_player;
extern const int cfg_network_rssi_not_show_if_player;
extern const int cfg_network_rssi_percent_not_show_if_player;
extern const int cfg_network_not_show_if_player;
extern const int cfg_call_not_show_if_player;
extern const int cfg_app_not_show_if_player;
extern const int cfg_app_java_not_show_if_player;
extern const int cfg_app_book_not_show_if_player;
extern const int cfg_app_elf_not_show_if_player;
extern const int cfg_msg1_not_show_if_player;
extern const int cfg_msg2_not_show_if_player;
extern const int cfg_msg3_not_show_if_player;
extern const int cfg_msg4_not_show_if_player;
extern const int cfg_msg5_not_show_if_player;
extern const int cfg_msg6_not_show_if_player;
extern const int cfg_soft_key_not_show_if_player;
extern const int cfg_remove_operator_1st_line_if_player;
extern const int cfg_remove_operator_2nd_line_if_player;
extern const int cfg_remove_operator_3rd_line_if_player;

extern const int cfg_id3_use_int_func;

extern const int cfg_status_show;
extern const int cfg_orig_status_show;
extern const unsigned int cfg_status_row_max;
extern const unsigned int cfg_status_column_max;
extern const int cfg_status_fill_by_row;
extern const int cfg_status_align;
extern const unsigned int cfg_status_x;
extern const unsigned int cfg_status_y;
extern const unsigned int cfg_status_refresh;

extern const int cfg_status_silent_show;
extern const int cfg_status_media_show;
extern const int cfg_status_java_show;
extern const int cfg_status_lock_show;
extern const int cfg_status_missed_show;
extern const int cfg_status_profile_show;
extern const int cfg_status_bluetooth_show;
extern const int cfg_status_usb_show;
extern const int cfg_status_phf_show;
extern const int cfg_status_chf_show;
extern const int cfg_status_internet_show;
extern const int cfg_status_alarm_show;
extern const int cfg_status_irda_show;
extern const int cfg_status_stopwatch_show;
extern const int cfg_status_allowedcallers_show;
extern const int cfg_status_ressignment_show;
extern const int cfg_status_msg_show;
extern const int cfg_status_download_show;
extern const int cfg_status_phonenum_state_show;

extern const wchar_t cfg_status_file0[256];
extern const wchar_t cfg_status_file1[256];
extern const wchar_t cfg_status_file2[256];
extern const wchar_t cfg_status_file3[256];
extern const wchar_t cfg_status_file4[256];
extern const wchar_t cfg_status_file5[256];
extern const wchar_t cfg_status_file6[256];
extern const wchar_t cfg_status_file7[256];
extern const wchar_t cfg_status_file8[256];
extern const wchar_t cfg_status_file9[256];
extern const wchar_t cfg_status_file10[256];
extern const wchar_t cfg_status_file11[256];
extern const wchar_t cfg_status_file12[256];
extern const wchar_t cfg_status_file13[256];
extern const wchar_t cfg_status_file14[256];
extern const wchar_t cfg_status_file15[256];
extern const wchar_t cfg_status_file16[256];
extern const wchar_t cfg_status_file17[256];
extern const wchar_t cfg_status_file18[256];
extern const wchar_t cfg_status_file19[256];
extern const wchar_t cfg_status_file20[256];
extern const wchar_t cfg_status_file21[256];
extern const wchar_t cfg_status_file22[256];
extern const wchar_t cfg_status_file23[256];
extern const wchar_t cfg_status_file24[256];
extern const wchar_t cfg_status_file25[256];
extern const wchar_t cfg_status_file26[256];
extern const wchar_t cfg_status_file27[256];
extern const wchar_t cfg_status_file28[256];
extern const wchar_t cfg_status_file29[256];
extern const wchar_t cfg_status_file30[256];
extern const wchar_t cfg_status_file31[256];
extern const wchar_t cfg_status_file32[256];
extern const wchar_t cfg_status_file33[256];
extern const wchar_t cfg_status_file34[256];
extern const wchar_t cfg_status_file35[256];
extern const wchar_t cfg_status_file36[256];

extern const int cfg_status_custom0;
extern const int cfg_status_custom1;
extern const int cfg_status_custom2;
extern const int cfg_status_custom3;
extern const int cfg_status_custom4;
extern const int cfg_status_custom5;
extern const int cfg_status_custom6;
extern const int cfg_status_custom7;
extern const int cfg_status_custom8;
extern const int cfg_status_custom9;
extern const int cfg_status_custom10;
extern const int cfg_status_custom11;
extern const int cfg_status_custom12;
extern const int cfg_status_custom13;
extern const int cfg_status_custom14;
extern const int cfg_status_custom15;
extern const int cfg_status_custom16;
extern const int cfg_status_custom17;
extern const int cfg_status_custom18;
extern const int cfg_status_custom19;
extern const int cfg_status_custom20;
extern const int cfg_status_custom21;
extern const int cfg_status_custom22;
extern const int cfg_status_custom23;
extern const int cfg_status_custom24;
extern const int cfg_status_custom25;
extern const int cfg_status_custom26;
extern const int cfg_status_custom27;
extern const int cfg_status_custom28;
extern const int cfg_status_custom29;
extern const int cfg_status_custom30;
extern const int cfg_status_custom31;
extern const int cfg_status_custom32;
extern const int cfg_status_custom33;
extern const int cfg_status_custom34;
extern const int cfg_status_custom35;
extern const int cfg_status_custom36;

extern const CFG_HDR cfghdr_status_0;
extern const CFG_HDR cfghdr_status_1;
extern const CFG_HDR cfghdr_status_2;
extern const CFG_HDR cfghdr_status_3;
extern const CFG_HDR cfghdr_status_4;
extern const CFG_HDR cfghdr_status_5;
extern const CFG_HDR cfghdr_status_6;
extern const CFG_HDR cfghdr_status_7;
extern const CFG_HDR cfghdr_status_8;
extern const CFG_HDR cfghdr_status_9;
extern const CFG_HDR cfghdr_status_10;
extern const CFG_HDR cfghdr_status_11;
extern const CFG_HDR cfghdr_status_12;
extern const CFG_HDR cfghdr_status_13;
extern const CFG_HDR cfghdr_status_14;
extern const CFG_HDR cfghdr_status_15;
extern const CFG_HDR cfghdr_status_16;
extern const CFG_HDR cfghdr_status_17;
extern const CFG_HDR cfghdr_status_18;
extern const CFG_HDR cfghdr_status_19;
extern const CFG_HDR cfghdr_status_20;
extern const CFG_HDR cfghdr_status_21;
extern const CFG_HDR cfghdr_status_22;
extern const CFG_HDR cfghdr_status_23;
extern const CFG_HDR cfghdr_status_24;
extern const CFG_HDR cfghdr_status_25;
extern const CFG_HDR cfghdr_status_26;
extern const CFG_HDR cfghdr_status_27;
extern const CFG_HDR cfghdr_status_28;
extern const CFG_HDR cfghdr_status_29;
extern const CFG_HDR cfghdr_status_30;
extern const CFG_HDR cfghdr_status_31;
extern const CFG_HDR cfghdr_status_32;
extern const CFG_HDR cfghdr_status_33;
extern const CFG_HDR cfghdr_status_34;
extern const CFG_HDR cfghdr_status_35;
extern const CFG_HDR cfghdr_status_36;

extern const wchar_t cfg_ressignment_addr[12];

extern const int cfg_remove_operator_1st_line;
extern const int cfg_remove_operator_2nd_line;
extern const int cfg_remove_operator_3rd_line;

extern const int cfg_network_rssi_show;
extern const int cfg_network_rssi_dbm_show;
extern const unsigned int cfg_network_rssi_font;
extern const int cfg_network_rssi_highlight;
extern const unsigned int cfg_network_rssi_color;
extern const unsigned int cfg_network_rssi_border;
extern const int cfg_network_rssi_align;
extern const unsigned int cfg_network_rssi_x;
extern const unsigned int cfg_network_rssi_y;

extern const int cfg_network_rssi_percent_show;
extern const unsigned int cfg_network_rssi_percent_font;
extern const int cfg_network_rssi_percent_highlight;
extern const unsigned int cfg_network_rssi_percent_color;
extern const unsigned int cfg_network_rssi_percent_border;
extern const int cfg_network_rssi_percent_align;
extern const unsigned int cfg_network_rssi_percent_x;
extern const unsigned int cfg_network_rssi_percent_y;

extern const int cfg_network_show;
extern const int cfg_network_custom_images;
extern const unsigned int cfg_network_x;
extern const unsigned int cfg_network_y;
extern const unsigned int cfg_refresh_network;
extern const wchar_t cfg_net_none_0_file[256];
extern const wchar_t cfg_net_none_1_file[256];
extern const wchar_t cfg_net_none_2_file[256];
extern const wchar_t cfg_net_none_3_file[256];
extern const wchar_t cfg_net_none_4_file[256];
extern const wchar_t cfg_net_none_5_file[256];
extern const wchar_t cfg_net_edge_0_file[256];
extern const wchar_t cfg_net_edge_1_file[256];
extern const wchar_t cfg_net_edge_2_file[256];
extern const wchar_t cfg_net_edge_3_file[256];
extern const wchar_t cfg_net_edge_4_file[256];
extern const wchar_t cfg_net_edge_5_file[256];
extern const wchar_t cfg_net_act_edge_0_file[256];
extern const wchar_t cfg_net_act_edge_1_file[256];
extern const wchar_t cfg_net_act_edge_2_file[256];
extern const wchar_t cfg_net_act_edge_3_file[256];
extern const wchar_t cfg_net_act_edge_4_file[256];
extern const wchar_t cfg_net_act_edge_5_file[256];
extern const wchar_t cfg_net_gprs_0_file[256];
extern const wchar_t cfg_net_gprs_1_file[256];
extern const wchar_t cfg_net_gprs_2_file[256];
extern const wchar_t cfg_net_gprs_3_file[256];
extern const wchar_t cfg_net_gprs_4_file[256];
extern const wchar_t cfg_net_gprs_5_file[256];
extern const wchar_t cfg_net_act_gprs_0_file[256];
extern const wchar_t cfg_net_act_gprs_1_file[256];
extern const wchar_t cfg_net_act_gprs_2_file[256];
extern const wchar_t cfg_net_act_gprs_3_file[256];
extern const wchar_t cfg_net_act_gprs_4_file[256];
extern const wchar_t cfg_net_act_gprs_5_file[256];
extern const wchar_t cfg_net_flight_file[256];

extern const int cfg_call_show;
extern const unsigned int cfg_call_font;
extern const int cfg_call_highlight;
extern const unsigned int cfg_call_color;
extern const unsigned int cfg_call_border;
extern const int cfg_call_align;
extern const unsigned int cfg_call_x;
extern const unsigned int cfg_call_y;
extern const int cfg_call_icon_show;
extern const wchar_t cfg_call_file[256];

extern const int cfg_app_show;
extern const int cfg_app_align;
extern const unsigned int cfg_app_x;
extern const unsigned int cfg_app_y;
extern const int cfg_app_fill_by_row;
extern const unsigned int cfg_app_row_max;
extern const unsigned int cfg_app_column_max;

extern const int cfg_app_book_show;
extern const wchar_t cfg_app_book_msg[32];
extern const unsigned int cfg_app_book_font;
extern const int cfg_app_book_highlight;
extern const unsigned int cfg_app_book_color;
extern const unsigned int cfg_app_book_border;
extern const int cfg_app_book_align;
extern const unsigned int cfg_app_book_x;
extern const unsigned int cfg_app_book_y;

extern const int cfg_app_elf_show;
extern const wchar_t cfg_app_elf_msg[32];
extern const unsigned int cfg_app_elf_font;
extern const int cfg_app_elf_highlight;
extern const unsigned int cfg_app_elf_color;
extern const unsigned int cfg_app_elf_border;
extern const int cfg_app_elf_align;
extern const unsigned int cfg_app_elf_x;
extern const unsigned int cfg_app_elf_y;

extern const int cfg_app_java_show;
extern const wchar_t cfg_app_java_msg[32];
extern const unsigned int cfg_app_java_font;
extern const int cfg_app_java_highlight;
extern const unsigned int cfg_app_java_color;
extern const unsigned int cfg_app_java_border;
extern const int cfg_app_java_align;
extern const unsigned int cfg_app_java_x;
extern const unsigned int cfg_app_java_y;

extern const wchar_t cfg_msg1[64];
extern const int cfg_msg1_show;
extern const unsigned int cfg_msg1_font;
extern const int cfg_msg1_highlight;
extern const unsigned int cfg_msg1_color;
extern const unsigned int cfg_msg1_border;
extern const int cfg_msg1_align;
extern const unsigned int cfg_msg1_x;
extern const unsigned int cfg_msg1_y;

extern const wchar_t cfg_msg2[64];
extern const int cfg_msg2_show;
extern const unsigned int cfg_msg2_font;
extern const int cfg_msg2_highlight;
extern const unsigned int cfg_msg2_color;
extern const unsigned int cfg_msg2_border;
extern const int cfg_msg2_align;
extern const unsigned int cfg_msg2_x;
extern const unsigned int cfg_msg2_y;

extern const wchar_t cfg_msg3[64];
extern const int cfg_msg3_show;
extern const unsigned int cfg_msg3_font;
extern const int cfg_msg3_highlight;
extern const unsigned int cfg_msg3_color;
extern const unsigned int cfg_msg3_border;
extern const int cfg_msg3_align;
extern const unsigned int cfg_msg3_x;
extern const unsigned int cfg_msg3_y;

extern const wchar_t cfg_msg4[64];
extern const int cfg_msg4_show;
extern const unsigned int cfg_msg4_font;
extern const int cfg_msg4_highlight;
extern const unsigned int cfg_msg4_color;
extern const unsigned int cfg_msg4_border;
extern const int cfg_msg4_align;
extern const unsigned int cfg_msg4_x;
extern const unsigned int cfg_msg4_y;

extern const wchar_t cfg_msg5[64];
extern const int cfg_msg5_show;
extern const unsigned int cfg_msg5_font;
extern const int cfg_msg5_highlight;
extern const unsigned int cfg_msg5_color;
extern const unsigned int cfg_msg5_border;
extern const int cfg_msg5_align;
extern const unsigned int cfg_msg5_x;
extern const unsigned int cfg_msg5_y;

extern const wchar_t cfg_msg6[64];
extern const int cfg_msg6_show;
extern const unsigned int cfg_msg6_font;
extern const int cfg_msg6_highlight;
extern const unsigned int cfg_msg6_color;
extern const unsigned int cfg_msg6_border;
extern const int cfg_msg6_align;
extern const unsigned int cfg_msg6_x;
extern const unsigned int cfg_msg6_y;

extern const int cfg_soft_key_show;
extern const unsigned int cfg_refresh_time;
extern const int cfg_standby_only;

#endif
