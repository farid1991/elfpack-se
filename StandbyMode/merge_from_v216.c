#include "..\\include\Lib_Clara.h"
#include "..\\include\cfg_items.h"
#include "main.h"
#include "cfg_v2_16.h"
#include "cfg_v2_17.h"

void _merge_from_v2_16(v2_17_config_t *newcfg, v2_16_config_t *oldcfg)
{
//Картинки ...
//Картинка 1
  newcfg->cfg_image1_show = oldcfg->cfg_image1_show;
  newcfg->cfg_image1_x = oldcfg->cfg_image1_x;
  newcfg->cfg_image1_y = oldcfg->cfg_image1_y;
  wstrcpy(newcfg->cfg_file1, oldcfg->cfg_file1);
//Картинка 2
  newcfg->cfg_image2_show = oldcfg->cfg_image2_show;
  newcfg->cfg_image2_x = oldcfg->cfg_image2_x;
  newcfg->cfg_image2_y = oldcfg->cfg_image2_y;
  wstrcpy(newcfg->cfg_file2, oldcfg->cfg_file2);
//Картинка 3
  newcfg->cfg_image3_show = oldcfg->cfg_image3_show;
  newcfg->cfg_image3_x = oldcfg->cfg_image3_x;
  newcfg->cfg_image3_y = oldcfg->cfg_image3_y;
  wstrcpy(newcfg->cfg_file3, oldcfg->cfg_file3);
//Картинка 4
  newcfg->cfg_image4_show = oldcfg->cfg_image4_show;
  newcfg->cfg_image4_x = oldcfg->cfg_image4_x;
  newcfg->cfg_image4_y = oldcfg->cfg_image4_y;
  wstrcpy(newcfg->cfg_file4, oldcfg->cfg_file4);
//Картинка 5
  newcfg->cfg_image5_show = oldcfg->cfg_image5_show;
  newcfg->cfg_image5_x = oldcfg->cfg_image5_x;
  newcfg->cfg_image5_y = oldcfg->cfg_image5_y;
  wstrcpy(newcfg->cfg_file5, oldcfg->cfg_file5);
//Картинка 6
  newcfg->cfg_image6_show = oldcfg->cfg_image6_show;
  newcfg->cfg_image6_x = oldcfg->cfg_image6_x;
  newcfg->cfg_image6_y = oldcfg->cfg_image6_y;
  wstrcpy(newcfg->cfg_file6, oldcfg->cfg_file6);
  newcfg->cfg_image6_slideshow = oldcfg->cfg_image6_slideshow;
  wstrcpy(newcfg->cfg_folder6, oldcfg->cfg_folder6);
  newcfg->cfg_slideshow_time = oldcfg->cfg_slideshow_time;

//Время/Дата/День/Будильник
//Время 1...
  newcfg->cfg_clock_show = oldcfg->cfg_clock_show;
  newcfg->cfg_clock_format = oldcfg->cfg_clock_format;
  newcfg->cfg_seconds = oldcfg->cfg_seconds;
  newcfg->cfg_seconds_font = oldcfg->cfg_seconds_font;
  newcfg->cfg_seconds_highlight = oldcfg->cfg_seconds_highlight;
  newcfg->cfg_seconds_offset_y = oldcfg->cfg_seconds_offset_y;
  newcfg->cfg_remove_clock = oldcfg->cfg_remove_clock;
  newcfg->cfg_clock_font = oldcfg->cfg_clock_font;
  newcfg->cfg_clock_highlight = oldcfg->cfg_clock_highlight;
  newcfg->cfg_clock_color = oldcfg->cfg_clock_color;
  newcfg->cfg_clock_border = oldcfg->cfg_clock_border;
  newcfg->cfg_clock_align = oldcfg->cfg_clock_align;
  newcfg->cfg_clock_x = oldcfg->cfg_clock_x;
  newcfg->cfg_clock_y = oldcfg->cfg_clock_y;
//Время 2...
  newcfg->cfg_clock2_show = oldcfg->cfg_clock2_show;
  newcfg->cfg_clock2_nonstandby_show = oldcfg->cfg_clock2_nonstandby_show;
  newcfg->cfg_clock2_mediapleyer_show = oldcfg->cfg_clock2_mediapleyer_show;
  newcfg->cfg_clock2_format = oldcfg->cfg_clock2_format;
  newcfg->cfg_seconds2 = oldcfg->cfg_seconds2;
  newcfg->cfg_seconds2_font = oldcfg->cfg_seconds2_font;
  newcfg->cfg_seconds2_highlight = oldcfg->cfg_seconds2_highlight;
  newcfg->cfg_seconds2_offset_y = oldcfg->cfg_seconds2_offset_y;
  newcfg->cfg_clock2_font = oldcfg->cfg_clock2_font;
  newcfg->cfg_clock2_highlight = oldcfg->cfg_clock2_highlight;
  newcfg->cfg_clock2_color = oldcfg->cfg_clock2_color;
  newcfg->cfg_clock2_border = oldcfg->cfg_clock2_border;
  newcfg->cfg_clock2_align = oldcfg->cfg_clock2_align;
  newcfg->cfg_clock2_x = oldcfg->cfg_clock2_x;
  newcfg->cfg_clock2_y = oldcfg->cfg_clock2_y;
//Дата...
  newcfg->cfg_date_show = oldcfg->cfg_date_show;
  newcfg->cfg_date_format = oldcfg->cfg_date_format;
  newcfg->cfg_remove_year = oldcfg->cfg_remove_year;
  newcfg->cfg_remove_date = oldcfg->cfg_remove_date;
  newcfg->cfg_date_font = oldcfg->cfg_date_font;
  newcfg->cfg_date_highlight = oldcfg->cfg_date_highlight;
  newcfg->cfg_date_color = oldcfg->cfg_date_color;
  newcfg->cfg_date_border = oldcfg->cfg_date_border;
  newcfg->cfg_date_align = oldcfg->cfg_date_align;
  newcfg->cfg_date_x = oldcfg->cfg_date_x;
  newcfg->cfg_date_y = oldcfg->cfg_date_y;
//День...
  newcfg->cfg_day_show = oldcfg->cfg_day_show;
  newcfg->cfg_day_mode = oldcfg->cfg_day_mode;
  newcfg->cfg_day_format = oldcfg->cfg_day_format;
  newcfg->cfg_day_font = oldcfg->cfg_day_font;
  newcfg->cfg_day_highlight = oldcfg->cfg_day_highlight;
  newcfg->cfg_day_color = oldcfg->cfg_day_color;
  newcfg->cfg_day_border = oldcfg->cfg_day_border;
  newcfg->cfg_day_align = oldcfg->cfg_day_align;
  newcfg->cfg_day_x = oldcfg->cfg_day_x;
  newcfg->cfg_day_y = oldcfg->cfg_day_y;
  newcfg->cfg_custom_day = oldcfg->cfg_custom_day;
  wstrcpy(newcfg->cfg_sun, oldcfg->cfg_sun);
  wstrcpy(newcfg->cfg_mon, oldcfg->cfg_mon);
  wstrcpy(newcfg->cfg_tue, oldcfg->cfg_tue);
  wstrcpy(newcfg->cfg_wed, oldcfg->cfg_wed);
  wstrcpy(newcfg->cfg_thu, oldcfg->cfg_thu);
  wstrcpy(newcfg->cfg_fri, oldcfg->cfg_fri);
  wstrcpy(newcfg->cfg_sat, oldcfg->cfg_sat);
  //Будильник...
  newcfg->cfg_alarm_show = oldcfg->cfg_alarm_show;
  newcfg->cfg_alarm_font = oldcfg->cfg_alarm_font;
  newcfg->cfg_alarm_highlight = oldcfg->cfg_alarm_highlight;
  newcfg->cfg_alarm_color = oldcfg->cfg_alarm_color;
  newcfg->cfg_alarm_border = oldcfg->cfg_alarm_border;
  newcfg->cfg_alarm_align = oldcfg->cfg_alarm_align;
  newcfg->cfg_alarm_x = oldcfg->cfg_alarm_x;
  newcfg->cfg_alarm_y = oldcfg->cfg_alarm_y;
  newcfg->cfg_alarm_icon_show = 1;
  wstrcpy(newcfg->cfg_alarm_file, oldcfg->cfg_alarm_file);
  newcfg->cfg_alarm_image_show = oldcfg->cfg_alarm_image_show;
  newcfg->cfg_alarm_image_x = oldcfg->cfg_alarm_image_x;
  newcfg->cfg_alarm_image_y = oldcfg->cfg_alarm_image_y;
  wstrcpy(newcfg->cfg_alarm_image_file, oldcfg->cfg_alarm_image_file);
  newcfg->cfg_refresh_alarm = oldcfg->cfg_refresh_alarm;

//Heap/Батарея/Температура
//Heap...
  newcfg->cfg_heap_show = oldcfg->cfg_heap_show;
  newcfg->cfg_heap_format = oldcfg->cfg_heap_format;
  newcfg->cfg_heap_font = oldcfg->cfg_heap_font;
  newcfg->cfg_heap_highlight = oldcfg->cfg_heap_highlight;
  newcfg->cfg_heap_color = oldcfg->cfg_heap_color;
  newcfg->cfg_heap_border = oldcfg->cfg_heap_border;
  newcfg->cfg_heap_align = oldcfg->cfg_heap_align;
  newcfg->cfg_heap_x = oldcfg->cfg_heap_x;
  newcfg->cfg_heap_y = oldcfg->cfg_heap_y;
//Емкость батареи...
  newcfg->cfg_batt_show = oldcfg->cfg_batt_show;
  newcfg->cfg_percent = oldcfg->cfg_percent;
  newcfg->cfg_percent_font = oldcfg->cfg_percent_font;
  newcfg->cfg_batt_percent_highlight = oldcfg->cfg_batt_percent_highlight;
  newcfg->cfg_percent_offset_y = oldcfg->cfg_percent_offset_y;
  newcfg->cfg_batt_font = oldcfg->cfg_batt_font;
  newcfg->cfg_batt_highlight = oldcfg->cfg_batt_highlight;
  newcfg->cfg_batt_color = oldcfg->cfg_batt_color;
  newcfg->cfg_batt_border = oldcfg->cfg_batt_border;
  newcfg->cfg_batt_align = oldcfg->cfg_batt_align;
  newcfg->cfg_batt_x = oldcfg->cfg_batt_x;
  newcfg->cfg_batt_y = oldcfg->cfg_batt_y;
  newcfg->cfg_batt_bar_show = oldcfg->cfg_batt_bar_show;
  newcfg->cfg_batt_bar_images = oldcfg->cfg_batt_bar_images;
  newcfg->cfg_batt_bar_highlight = oldcfg->cfg_batt_bar_highlight;
  newcfg->cfg_batt_bar = oldcfg->cfg_batt_bar;
  newcfg->cfg_batt_bar_border = oldcfg->cfg_batt_bar_border;
  newcfg->cfg_batt_empty_color = oldcfg->cfg_batt_empty_color;
  newcfg->cfg_batt_0_color = oldcfg->cfg_batt_0_color;
  newcfg->cfg_batt_10_color = oldcfg->cfg_batt_10_color;
  newcfg->cfg_batt_20_color = oldcfg->cfg_batt_20_color;
  newcfg->cfg_batt_30_color = oldcfg->cfg_batt_30_color;
  newcfg->cfg_batt_40_color = oldcfg->cfg_batt_40_color;
  newcfg->cfg_batt_50_color = oldcfg->cfg_batt_50_color;
  newcfg->cfg_batt_60_color = oldcfg->cfg_batt_60_color;
  newcfg->cfg_batt_70_color = oldcfg->cfg_batt_70_color;
  newcfg->cfg_batt_80_color = oldcfg->cfg_batt_80_color;
  newcfg->cfg_batt_90_color = oldcfg->cfg_batt_90_color;
  newcfg->cfg_batt_100_color = oldcfg->cfg_batt_100_color;
  newcfg->cfg_batt_bar_reverse = 0;

  newcfg->cfg_batt_images_custom = oldcfg->cfg_batt_images_custom;
  newcfg->cfg_batt_bar_x = oldcfg->cfg_batt_bar_x;
  newcfg->cfg_batt_bar_y = oldcfg->cfg_batt_bar_y;
  wstrcpy(newcfg->cfg_batt_0_file, oldcfg->cfg_batt_0_file);
  wstrcpy(newcfg->cfg_batt_5_file, oldcfg->cfg_batt_5_file);
  wstrcpy(newcfg->cfg_batt_15_file, oldcfg->cfg_batt_15_file);
  wstrcpy(newcfg->cfg_batt_25_file, oldcfg->cfg_batt_25_file);
  wstrcpy(newcfg->cfg_batt_35_file, oldcfg->cfg_batt_35_file);
  wstrcpy(newcfg->cfg_batt_45_file, oldcfg->cfg_batt_45_file);
  wstrcpy(newcfg->cfg_batt_55_file, oldcfg->cfg_batt_55_file);
  wstrcpy(newcfg->cfg_batt_65_file, oldcfg->cfg_batt_65_file);
  wstrcpy(newcfg->cfg_batt_75_file, oldcfg->cfg_batt_75_file);
  wstrcpy(newcfg->cfg_batt_85_file, oldcfg->cfg_batt_85_file);
  wstrcpy(newcfg->cfg_batt_100_file, oldcfg->cfg_batt_100_file);

//Ток заряда...
  newcfg->cfg_current_show = oldcfg->cfg_current_show;
  newcfg->cfg_current_font = oldcfg->cfg_current_font;
  newcfg->cfg_current_highlight = oldcfg->cfg_current_highlight;
  newcfg->cfg_current_color = oldcfg->cfg_current_color;
  newcfg->cfg_current_border = oldcfg->cfg_current_border;
  newcfg->cfg_current_align = oldcfg->cfg_current_align;
  newcfg->cfg_current_x = oldcfg->cfg_current_x;
  newcfg->cfg_current_y = oldcfg->cfg_current_y;
//Индикация заряда...
  newcfg->cfg_batt_charge_anim = oldcfg->cfg_batt_charge_anim;
  newcfg->cfg_batt_charge_show = oldcfg->cfg_batt_charge_show;
  newcfg->cfg_batt_charge_custom = oldcfg->cfg_batt_charge_custom;
  newcfg->cfg_batt_charge_x = oldcfg->cfg_batt_charge_x;
  newcfg->cfg_batt_charge_y = oldcfg->cfg_batt_charge_y;
  wstrcpy(newcfg->cfg_batt_charge_file, oldcfg->cfg_batt_charge_file);
//Температура батареи...
  newcfg->cfg_batt_temp_show = oldcfg->cfg_batt_temp_show;
  newcfg->cfg_charging_only = oldcfg->cfg_charging_only;
  newcfg->cfg_batt_temp_font = oldcfg->cfg_batt_temp_font;
  newcfg->cfg_batt_temp_highlight = oldcfg->cfg_batt_temp_highlight;
  newcfg->cfg_batt_temp_color = oldcfg->cfg_batt_temp_color;
  newcfg->cfg_batt_temp_border = oldcfg->cfg_batt_temp_border;
  newcfg->cfg_batt_temp_align = oldcfg->cfg_batt_temp_align;
  newcfg->cfg_batt_temp_x = oldcfg->cfg_batt_temp_x;
  newcfg->cfg_batt_temp_y = oldcfg->cfg_batt_temp_y;
//Температура телефона...
  newcfg->cfg_temp_show = oldcfg->cfg_temp_show;
  newcfg->cfg_temp_font = oldcfg->cfg_temp_font;
  newcfg->cfg_temp_highlight = oldcfg->cfg_temp_highlight;
  newcfg->cfg_temp_color = oldcfg->cfg_temp_color;
  newcfg->cfg_temp_border = oldcfg->cfg_temp_border;
  newcfg->cfg_temp_align = oldcfg->cfg_temp_align;
  newcfg->cfg_temp_x = oldcfg->cfg_temp_x;
  newcfg->cfg_temp_y = oldcfg->cfg_temp_y;
//Дополнительно...
  newcfg->cfg_refresh_heap = oldcfg->cfg_refresh_heap;
  newcfg->cfg_refresh_batt_temp = oldcfg->cfg_refresh_batt_temp;

//Инфо о памяти...
//Телефон...
  newcfg->cfg_phone_show = oldcfg->cfg_phone_show;
  newcfg->cfg_phone_format = oldcfg->cfg_phone_format;
  newcfg->cfg_phone_fractional = oldcfg->cfg_phone_fractional;
  wstrcpy(newcfg->cfg_phone_msg, oldcfg->cfg_phone_msg);
  newcfg->cfg_phone_font = oldcfg->cfg_phone_font;
  newcfg->cfg_phone_highlight = oldcfg->cfg_phone_highlight;
  newcfg->cfg_phone_color = oldcfg->cfg_phone_color;
  newcfg->cfg_phone_border = oldcfg->cfg_phone_border;
  newcfg->cfg_phone_align = oldcfg->cfg_phone_align;
  newcfg->cfg_phone_x = oldcfg->cfg_phone_x;
  newcfg->cfg_phone_y = oldcfg->cfg_phone_y;
  newcfg->cfg_phone_bar_show = oldcfg->cfg_phone_bar_show;
  newcfg->cfg_phone_bar_highlight = oldcfg->cfg_phone_bar_highlight;
  newcfg->cfg_phone_bar = oldcfg->cfg_phone_bar;
  newcfg->cfg_phone_used_color = oldcfg->cfg_phone_used_color;
  newcfg->cfg_phone_free_color = oldcfg->cfg_phone_free_color;
  newcfg->cfg_phone_bar_border = oldcfg->cfg_phone_bar_border;
//Карта памяти...
  newcfg->cfg_card_show = oldcfg->cfg_card_show;
  newcfg->cfg_card_format = oldcfg->cfg_card_format;
  newcfg->cfg_card_fractional = oldcfg->cfg_card_fractional;
  wstrcpy(newcfg->cfg_card_msg, oldcfg->cfg_card_msg);
  newcfg->cfg_card_font = oldcfg->cfg_card_font;
  newcfg->cfg_card_highlight = oldcfg->cfg_card_highlight;
  newcfg->cfg_card_color = oldcfg->cfg_card_color;
  newcfg->cfg_card_border = oldcfg->cfg_card_border;
  newcfg->cfg_card_align = oldcfg->cfg_card_align;
  newcfg->cfg_card_x = oldcfg->cfg_card_x;
  newcfg->cfg_card_y = oldcfg->cfg_card_y;
  newcfg->cfg_card_bar_show = oldcfg->cfg_card_bar_show;
  newcfg->cfg_card_bar_highlight = oldcfg->cfg_card_bar_highlight;
  newcfg->cfg_card_bar = oldcfg->cfg_card_bar;
  newcfg->cfg_card_used_color = oldcfg->cfg_card_used_color;
  newcfg->cfg_card_free_color = oldcfg->cfg_card_free_color;
  newcfg->cfg_card_bar_border = oldcfg->cfg_card_bar_border;

  newcfg->cfg_mem_update = oldcfg->cfg_mem_update;

//FM радио/Плеер = oldcfg->//FM радио/Плеер
//FM радио...
//Частота станции...
  newcfg->cfg_fm_show = oldcfg->cfg_fm_show;
  newcfg->cfg_fm_name_first = oldcfg->cfg_fm_name_first;
  newcfg->cfg_fm_font = oldcfg->cfg_fm_font;
  newcfg->cfg_fm_highlight = oldcfg->cfg_fm_highlight;
  newcfg->cfg_fm_color = oldcfg->cfg_fm_color;
  newcfg->cfg_fm_border = oldcfg->cfg_fm_border;
  newcfg->cfg_fm_align = oldcfg->cfg_fm_align;
  newcfg->cfg_fm_x = oldcfg->cfg_fm_x;
  newcfg->cfg_fm_y = oldcfg->cfg_fm_y;
  wstrcpy(newcfg->cfg_fm_msg, oldcfg->cfg_fm_msg);
//Режим стерео...
  newcfg->cfg_fm_mode_show = oldcfg->cfg_fm_mode_show;
  newcfg->cfg_fm_mode_font = oldcfg->cfg_fm_mode_font;
  newcfg->cfg_fm_mode_highlight = oldcfg->cfg_fm_mode_highlight;
  newcfg->cfg_fm_mode_color = oldcfg->cfg_fm_mode_color;
  newcfg->cfg_fm_mode_border = oldcfg->cfg_fm_mode_border;
  newcfg->cfg_fm_mode_align = oldcfg->cfg_fm_mode_align;
  newcfg->cfg_fm_mode_x = oldcfg->cfg_fm_mode_x;
  newcfg->cfg_fm_mode_y = oldcfg->cfg_fm_mode_y;
//RDS данные...
  newcfg->cfg_fm_rdsdata_show = oldcfg->cfg_fm_rdsdata_show;
  newcfg->cfg_fm_rdsdata_font = oldcfg->cfg_fm_rdsdata_font;
  newcfg->cfg_fm_rdsdata_highlight = oldcfg->cfg_fm_rdsdata_highlight;
  newcfg->cfg_fm_rdsdata_color = oldcfg->cfg_fm_rdsdata_color;
  newcfg->cfg_fm_rdsdata_border = oldcfg->cfg_fm_rdsdata_border;
  newcfg->cfg_fm_rdsdata_align = oldcfg->cfg_fm_rdsdata_align;
  newcfg->cfg_fm_rdsdata_x = oldcfg->cfg_fm_rdsdata_x;
  newcfg->cfg_fm_rdsdata_y = oldcfg->cfg_fm_rdsdata_y;
//Картинка...
  newcfg->cfg_fm_image_show = oldcfg->cfg_fm_image_show;
  newcfg->cfg_fm_image_x = oldcfg->cfg_fm_image_x;
  newcfg->cfg_fm_image_y = oldcfg->cfg_fm_image_y;
  wstrcpy(newcfg->cfg_fm_file, oldcfg->cfg_fm_file);
//Плеер...
//Название песни
  newcfg->cfg_player_title_show = oldcfg->cfg_player_title_show;
  newcfg->cfg_player_title_font = oldcfg->cfg_player_title_font;
  newcfg->cfg_player_title_highlight = oldcfg->cfg_player_title_highlight;
  newcfg->cfg_player_title_color = oldcfg->cfg_player_title_color;
  newcfg->cfg_player_title_border = oldcfg->cfg_player_title_border;
  newcfg->cfg_player_title_align = oldcfg->cfg_player_title_align;
  newcfg->cfg_player_title_rc = oldcfg->cfg_player_title_rc;
  newcfg->cfg_player_title_scroll = oldcfg->cfg_player_title_scroll;
//Альбом
  newcfg->cfg_player_album_show = oldcfg->cfg_player_album_show;
  newcfg->cfg_player_album_font = oldcfg->cfg_player_album_font;
  newcfg->cfg_player_album_highlight = oldcfg->cfg_player_album_highlight;
  newcfg->cfg_player_album_color = oldcfg->cfg_player_album_color;
  newcfg->cfg_player_album_border = oldcfg->cfg_player_album_border;
  newcfg->cfg_player_album_align = oldcfg->cfg_player_album_align;
  newcfg->cfg_player_album_rc = oldcfg->cfg_player_album_rc;
  newcfg->cfg_player_album_scroll = oldcfg->cfg_player_album_scroll;
//Исполнитель
  newcfg->cfg_player_artist_show = oldcfg->cfg_player_artist_show;
  newcfg->cfg_player_artist_font = oldcfg->cfg_player_artist_font;
  newcfg->cfg_player_artist_highlight = oldcfg->cfg_player_artist_highlight;
  newcfg->cfg_player_artist_color = oldcfg->cfg_player_artist_color;
  newcfg->cfg_player_artist_border = oldcfg->cfg_player_artist_border;
  newcfg->cfg_player_artist_align = oldcfg->cfg_player_artist_align;
  newcfg->cfg_player_artist_rc = oldcfg->cfg_player_artist_rc;
  newcfg->cfg_player_artist_scroll = oldcfg->cfg_player_artist_scroll;
//Год
  newcfg->cfg_player_year_show = oldcfg->cfg_player_year_show;
  newcfg->cfg_player_year_font = oldcfg->cfg_player_year_font;
  newcfg->cfg_player_year_highlight = oldcfg->cfg_player_year_highlight;
  newcfg->cfg_player_year_color = oldcfg->cfg_player_year_color;
  newcfg->cfg_player_year_border = oldcfg->cfg_player_year_border;
  newcfg->cfg_player_year_align = oldcfg->cfg_player_year_align;
  newcfg->cfg_player_year_rc = oldcfg->cfg_player_year_rc;
  newcfg->cfg_player_year_scroll = oldcfg->cfg_player_year_scroll;
//Жанр
  newcfg->cfg_player_genre_show = oldcfg->cfg_player_genre_show;
  newcfg->cfg_player_genre_font = oldcfg->cfg_player_genre_font;
  newcfg->cfg_player_genre_highlight = oldcfg->cfg_player_genre_highlight;
  newcfg->cfg_player_genre_color = oldcfg->cfg_player_genre_color;
  newcfg->cfg_player_genre_border = oldcfg->cfg_player_genre_border;
  newcfg->cfg_player_genre_align = oldcfg->cfg_player_genre_align;
  newcfg->cfg_player_genre_rc = oldcfg->cfg_player_genre_rc;
  newcfg->cfg_player_genre_scroll = oldcfg->cfg_player_genre_scroll;
//Частота дискретизации
  newcfg->cfg_player_frequency_show = oldcfg->cfg_player_frequency_show;
  newcfg->cfg_player_frequency_font = oldcfg->cfg_player_frequency_font;
  newcfg->cfg_player_frequency_highlight = oldcfg->cfg_player_frequency_highlight;
  newcfg->cfg_player_frequency_color = oldcfg->cfg_player_frequency_color;
  newcfg->cfg_player_frequency_border = oldcfg->cfg_player_frequency_border;
  newcfg->cfg_player_frequency_align = oldcfg->cfg_player_frequency_align;
  newcfg->cfg_player_frequency_rc = oldcfg->cfg_player_frequency_rc;
  newcfg->cfg_player_frequency_scroll = oldcfg->cfg_player_frequency_scroll;
//Битрейт
  newcfg->cfg_player_bitrate_show = oldcfg->cfg_player_bitrate_show;
  newcfg->cfg_player_bitrate_font = oldcfg->cfg_player_bitrate_font;
  newcfg->cfg_player_bitrate_highlight = oldcfg->cfg_player_bitrate_highlight;
  newcfg->cfg_player_bitrate_color = oldcfg->cfg_player_bitrate_color;
  newcfg->cfg_player_bitrate_border = oldcfg->cfg_player_bitrate_border;
  newcfg->cfg_player_bitrate_align = oldcfg->cfg_player_bitrate_align;
  newcfg->cfg_player_bitrate_rc = oldcfg->cfg_player_bitrate_rc;
  newcfg->cfg_player_bitrate_scroll = oldcfg->cfg_player_bitrate_scroll;
//Каналы
  newcfg->cfg_player_channels_show = oldcfg->cfg_player_channels_show;
  newcfg->cfg_player_channels_font = oldcfg->cfg_player_channels_font;
  newcfg->cfg_player_channels_highlight = oldcfg->cfg_player_channels_highlight;
  newcfg->cfg_player_channels_color = oldcfg->cfg_player_channels_color;
  newcfg->cfg_player_channels_border = oldcfg->cfg_player_channels_border;
  newcfg->cfg_player_channels_align = oldcfg->cfg_player_channels_align;
  newcfg->cfg_player_channels_rc = oldcfg->cfg_player_channels_rc;
  newcfg->cfg_player_channels_scroll = oldcfg->cfg_player_channels_scroll;
//Длительность...
  newcfg->cfg_player_fulltime_show = oldcfg->cfg_player_fulltime_show;
  newcfg->cfg_player_fulltime_font = oldcfg->cfg_player_fulltime_font;
  newcfg->cfg_player_fulltime_highlight = oldcfg->cfg_player_fulltime_highlight;
  newcfg->cfg_player_fulltime_color = oldcfg->cfg_player_fulltime_color;
  newcfg->cfg_player_fulltime_border = oldcfg->cfg_player_fulltime_border;
  newcfg->cfg_player_fulltime_align = oldcfg->cfg_player_fulltime_align;
  newcfg->cfg_player_fulltime_rc = oldcfg->cfg_player_fulltime_rc;
  newcfg->cfg_player_fulltime_scroll = oldcfg->cfg_player_fulltime_scroll;
//Текущее/оставшееся время...
  newcfg->cfg_player_curtime_show = oldcfg->cfg_player_curtime_show;
  newcfg->cfg_player_endtime_show = oldcfg->cfg_player_endtime_show;
  newcfg->cfg_player_curtime_font = oldcfg->cfg_player_curtime_font;
  newcfg->cfg_player_curtime_highlight = oldcfg->cfg_player_curtime_highlight;
  newcfg->cfg_player_curtime_color = oldcfg->cfg_player_curtime_color;
  newcfg->cfg_player_curtime_border = oldcfg->cfg_player_curtime_border;
  newcfg->cfg_player_curtime_align = oldcfg->cfg_player_curtime_align;
  newcfg->cfg_player_curtime_rc = oldcfg->cfg_player_curtime_rc;
  newcfg->cfg_player_curtime_scroll = oldcfg->cfg_player_curtime_scroll;
//Полоска...
  newcfg->cfg_player_timebar_show = oldcfg->cfg_player_timebar_show;
  newcfg->cfg_player_timebar = oldcfg->cfg_player_timebar;
  newcfg->cfg_player_timebar_highlight = oldcfg->cfg_player_timebar_highlight;
  newcfg->cfg_player_timebar_cur_color = oldcfg->cfg_player_timebar_cur_color;
  newcfg->cfg_player_timebar_rem_color = oldcfg->cfg_player_timebar_rem_color;
  newcfg->cfg_player_timebar_border = oldcfg->cfg_player_timebar_border;
//Обложка
  newcfg->cfg_player_cover_show = oldcfg->cfg_player_cover_show;
  newcfg->cfg_player_cover = oldcfg->cfg_player_cover;
  strcpy(newcfg->cfg_player_cover_file, oldcfg->cfg_player_cover_file);
  newcfg->cfg_player_cover_under_image = oldcfg->cfg_player_cover_under_image;
//Картинка
  newcfg->cfg_player_image_show = oldcfg->cfg_player_image_show;
  newcfg->cfg_player_image_x = oldcfg->cfg_player_image_x;
  newcfg->cfg_player_image_y = oldcfg->cfg_player_image_y;
  wstrcpy(newcfg->cfg_player_file, oldcfg->cfg_player_file);
//Отключать при включении музыки
//Картинки ...
  newcfg->cfg_image1_not_show_if_player = oldcfg->cfg_image1_not_show_if_player;
  newcfg->cfg_image2_not_show_if_player = oldcfg->cfg_image2_not_show_if_player;
  newcfg->cfg_image3_not_show_if_player = oldcfg->cfg_image3_not_show_if_player;
  newcfg->cfg_image4_not_show_if_player = oldcfg->cfg_image4_not_show_if_player;
  newcfg->cfg_image5_not_show_if_player = oldcfg->cfg_image5_not_show_if_player;
  newcfg->cfg_image6_not_show_if_player = oldcfg->cfg_image6_not_show_if_player;
//Время/Дата/День/Будильник
  newcfg->cfg_clock_not_show_if_player = oldcfg->cfg_clock_not_show_if_player;
  newcfg->cfg_clock2_not_show_if_player = oldcfg->cfg_clock2_not_show_if_player;
  newcfg->cfg_orig_clock_not_show_if_player = oldcfg->cfg_orig_clock_not_show_if_player;
  newcfg->cfg_date_not_show_if_player = oldcfg->cfg_date_not_show_if_player;
  newcfg->cfg_orig_date_not_show_if_player = oldcfg->cfg_orig_date_not_show_if_player;
  newcfg->cfg_day_not_show_if_player = oldcfg->cfg_day_not_show_if_player;
  newcfg->cfg_alarm_not_show_if_player = oldcfg->cfg_alarm_not_show_if_player;
//Heap/Батарея/Температура
  newcfg->cfg_heap_not_show_if_player = oldcfg->cfg_heap_not_show_if_player;
  newcfg->cfg_batt_not_show_if_player = oldcfg->cfg_batt_not_show_if_player;
  newcfg->cfg_batt_bar_not_show_if_player = oldcfg->cfg_batt_bar_not_show_if_player;
  newcfg->cfg_current_not_show_if_player = oldcfg->cfg_current_not_show_if_player;
  newcfg->cfg_batt_charge_not_show_if_player = oldcfg->cfg_batt_charge_not_show_if_player;
  newcfg->cfg_batt_temp_not_show_if_player = oldcfg->cfg_batt_temp_not_show_if_player;
  newcfg->cfg_temp_not_show_if_player = oldcfg->cfg_temp_not_show_if_player;
//Инфо о памяти...
  newcfg->cfg_phone_not_show_if_player = oldcfg->cfg_phone_not_show_if_player;
  newcfg->cfg_card_not_show_if_player = oldcfg->cfg_card_not_show_if_player;
//Иконки статуса...
  newcfg->cfg_status_not_show_if_player = oldcfg->cfg_status_not_show_if_player;
//Сеть...
  newcfg->cfg_network_rssi_not_show_if_player = oldcfg->cfg_network_rssi_not_show_if_player;
  newcfg->cfg_network_rssi_percent_not_show_if_player = oldcfg->cfg_network_rssi_percent_not_show_if_player;
  newcfg->cfg_network_not_show_if_player = oldcfg->cfg_network_not_show_if_player;
//Длительность звонка
  newcfg->cfg_call_not_show_if_player = 0;
//Приложения...
  newcfg->cfg_app_not_show_if_player = oldcfg->cfg_app_not_show_if_player;
  newcfg->cfg_app_book_not_show_if_player = oldcfg->cfg_app_book_not_show_if_player;
  newcfg->cfg_app_elf_not_show_if_player = oldcfg->cfg_app_elf_not_show_if_player;
  newcfg->cfg_app_java_not_show_if_player = oldcfg->cfg_app_java_not_show_if_player;
//Свои сообщения...
  newcfg->cfg_msg1_not_show_if_player = oldcfg->cfg_msg1_not_show_if_player;
  newcfg->cfg_msg2_not_show_if_player = oldcfg->cfg_msg2_not_show_if_player;
  newcfg->cfg_msg3_not_show_if_player = oldcfg->cfg_msg3_not_show_if_player;
  newcfg->cfg_msg4_not_show_if_player = oldcfg->cfg_msg4_not_show_if_player;
  newcfg->cfg_msg5_not_show_if_player = oldcfg->cfg_msg5_not_show_if_player;
  newcfg->cfg_msg6_not_show_if_player = oldcfg->cfg_msg6_not_show_if_player;
  newcfg->cfg_soft_key_not_show_if_player = oldcfg->cfg_soft_key_not_show_if_player;
//Лого оператора...
  newcfg->cfg_remove_operator_1st_line_if_player = oldcfg->cfg_remove_operator_1st_line_if_player;
  newcfg->cfg_remove_operator_2nd_line_if_player = oldcfg->cfg_remove_operator_2nd_line_if_player;
  newcfg->cfg_remove_operator_3rd_line_if_player = oldcfg->cfg_remove_operator_3rd_line_if_player;
//Прокрутка
  newcfg->cfg_player_scroll_speed = oldcfg->cfg_player_scroll_speed;
  newcfg->cfg_player_scroll_delay = oldcfg->cfg_player_scroll_delay;
//Какие функции использоват для получения ID3
  newcfg->cfg_id3_use_int_func = oldcfg->cfg_id3_use_int_func;
  newcfg->cfg_refresh_media = oldcfg->cfg_refresh_media;
//Иконки статуса...
  newcfg->cfg_status_show = oldcfg->cfg_status_show;
  newcfg->cfg_orig_status_show = oldcfg->cfg_orig_status_show;
  newcfg->cfg_status_row_max = oldcfg->cfg_status_row_max;
  newcfg->cfg_status_column_max = oldcfg->cfg_status_column_max;
  newcfg->cfg_status_fill_by_row = oldcfg->cfg_status_fill_by_row;
  newcfg->cfg_status_align = oldcfg->cfg_status_align;
  newcfg->cfg_status_x = oldcfg->cfg_status_x;
  newcfg->cfg_status_y = oldcfg->cfg_status_y;
  newcfg->cfg_status_refresh = oldcfg->cfg_status_refresh;
  newcfg->cfg_status_silent_show = oldcfg->cfg_status_silent_show;
  newcfg->cfg_status_lock_show = oldcfg->cfg_status_lock_show;
  newcfg->cfg_status_missed_show = oldcfg->cfg_status_missed_show;
  newcfg->cfg_status_media_show = oldcfg->cfg_status_media_show;
  newcfg->cfg_status_java_show = oldcfg->cfg_status_java_show;
  newcfg->cfg_status_profile_show = oldcfg->cfg_status_profile_show;
  newcfg->cfg_status_bluetooth_show = oldcfg->cfg_status_bluetooth_show;
  newcfg->cfg_status_usb_show = oldcfg->cfg_status_usb_show;
  newcfg->cfg_status_phf_show = oldcfg->cfg_status_phf_show;
  newcfg->cfg_status_chf_show = oldcfg->cfg_status_chf_show;
  newcfg->cfg_status_internet_show = oldcfg->cfg_status_internet_show;
  newcfg->cfg_status_alarm_show = oldcfg->cfg_status_alarm_show ;
  newcfg->cfg_status_irda_show = oldcfg->cfg_status_irda_show;
  newcfg->cfg_status_stopwatch_show = oldcfg->cfg_status_stopwatch_show;
  newcfg->cfg_status_allowedcallers_show = oldcfg->cfg_status_allowedcallers_show;
  newcfg->cfg_status_ressignment_show = oldcfg->cfg_status_ressignment_show;
  newcfg->cfg_status_msg_show = 1;
  newcfg->cfg_status_download_show = 1;

#define STATUS_ICN_CFG_D(num) \
  newcfg->cfg_status_custom##num = oldcfg->cfg_status_custom##num; \
  wstrcpy(newcfg->cfg_status_file##num, oldcfg->cfg_status_file##num)

#define STATUS_ICN_CFG_D_NEW(num, file) \
  newcfg->cfg_status_custom##num = 0; \
  wstrcpy(newcfg->cfg_status_file##num, L"%INI_INT%/Standby Mode/status/"); \
  wstrcat(newcfg->cfg_status_file##num, file)


  STATUS_ICN_CFG_D(0);
  STATUS_ICN_CFG_D(1);
  STATUS_ICN_CFG_D(2);
  STATUS_ICN_CFG_D(3);
  STATUS_ICN_CFG_D(4);
  STATUS_ICN_CFG_D(5);
  STATUS_ICN_CFG_D(6);
  STATUS_ICN_CFG_D(7);
  STATUS_ICN_CFG_D(8);
  STATUS_ICN_CFG_D(9);
  STATUS_ICN_CFG_D(10);
  STATUS_ICN_CFG_D(11);
  STATUS_ICN_CFG_D(12);
  STATUS_ICN_CFG_D(13);
  STATUS_ICN_CFG_D(14);
  STATUS_ICN_CFG_D(15);
  STATUS_ICN_CFG_D(16);
  STATUS_ICN_CFG_D(17);
  STATUS_ICN_CFG_D(18);
  STATUS_ICN_CFG_D(19);
  STATUS_ICN_CFG_D(20);
  STATUS_ICN_CFG_D(21);
  STATUS_ICN_CFG_D(22);
  STATUS_ICN_CFG_D(23);
  STATUS_ICN_CFG_D(24);
  STATUS_ICN_CFG_D(25);
  STATUS_ICN_CFG_D(26);
  STATUS_ICN_CFG_D(27);
  STATUS_ICN_CFG_D(28);
  STATUS_ICN_CFG_D(29);
  STATUS_ICN_CFG_D(30);

  STATUS_ICN_CFG_D_NEW(31, L"message.png");
  STATUS_ICN_CFG_D_NEW(32, L"message.png");
  STATUS_ICN_CFG_D_NEW(33, L"downloading.png");

  wstrcpy(newcfg->cfg_ressignment_addr, oldcfg->cfg_ressignment_addr);

//Лого оператора...
  newcfg->cfg_remove_operator_1st_line = oldcfg->cfg_remove_operator_1st_line;
  newcfg->cfg_remove_operator_2nd_line = oldcfg->cfg_remove_operator_2nd_line;
  newcfg->cfg_remove_operator_3rd_line = oldcfg->cfg_remove_operator_3rd_line;

//Сеть ...
  newcfg->cfg_network_rssi_show = oldcfg->cfg_network_rssi_show;
  newcfg->cfg_network_rssi_dbm_show = oldcfg->cfg_network_rssi_dbm_show;
  newcfg->cfg_network_rssi_font = oldcfg->cfg_network_rssi_font;
  newcfg->cfg_network_rssi_highlight = oldcfg->cfg_network_rssi_highlight;
  newcfg->cfg_network_rssi_color = oldcfg->cfg_network_rssi_color;
  newcfg->cfg_network_rssi_border = oldcfg->cfg_network_rssi_border;
  newcfg->cfg_network_rssi_align = oldcfg->cfg_network_rssi_align;
  newcfg->cfg_network_rssi_x = oldcfg->cfg_network_rssi_x;
  newcfg->cfg_network_rssi_y = oldcfg->cfg_network_rssi_y;

  newcfg->cfg_network_rssi_percent_show = oldcfg->cfg_network_rssi_percent_show;
  newcfg->cfg_network_rssi_percent_font = oldcfg->cfg_network_rssi_percent_font;
  newcfg->cfg_network_rssi_percent_highlight = oldcfg->cfg_network_rssi_percent_highlight;
  newcfg->cfg_network_rssi_percent_color = oldcfg->cfg_network_rssi_percent_color;
  newcfg->cfg_network_rssi_percent_border = oldcfg->cfg_network_rssi_percent_border;
  newcfg->cfg_network_rssi_percent_align = oldcfg->cfg_network_rssi_percent_align;
  newcfg->cfg_network_rssi_percent_x = oldcfg->cfg_network_rssi_percent_x;
  newcfg->cfg_network_rssi_percent_y = oldcfg->cfg_network_rssi_percent_y;

  newcfg->cfg_network_show = oldcfg->cfg_network_show;
  newcfg->cfg_network_custom_images = oldcfg->cfg_network_custom_images;
  newcfg->cfg_network_x = oldcfg->cfg_network_x;
  newcfg->cfg_network_y = oldcfg->cfg_network_y;
  wstrcpy(newcfg->cfg_net_act_edge_0_file, oldcfg->cfg_net_act_edge_0_file);
  wstrcpy(newcfg->cfg_net_act_edge_1_file, oldcfg->cfg_net_act_edge_1_file);
  wstrcpy(newcfg->cfg_net_act_edge_2_file, oldcfg->cfg_net_act_edge_2_file);
  wstrcpy(newcfg->cfg_net_act_edge_3_file, oldcfg->cfg_net_act_edge_3_file);
  wstrcpy(newcfg->cfg_net_act_edge_4_file, oldcfg->cfg_net_act_edge_4_file);
  wstrcpy(newcfg->cfg_net_act_edge_5_file, oldcfg->cfg_net_act_edge_5_file);
  wstrcpy(newcfg->cfg_net_edge_0_file, oldcfg->cfg_net_edge_0_file);
  wstrcpy(newcfg->cfg_net_edge_1_file, oldcfg->cfg_net_edge_1_file);
  wstrcpy(newcfg->cfg_net_edge_2_file, oldcfg->cfg_net_edge_2_file);
  wstrcpy(newcfg->cfg_net_edge_3_file, oldcfg->cfg_net_edge_3_file);
  wstrcpy(newcfg->cfg_net_edge_4_file, oldcfg->cfg_net_edge_4_file);
  wstrcpy(newcfg->cfg_net_edge_5_file, oldcfg->cfg_net_edge_5_file);
  wstrcpy(newcfg->cfg_net_act_gprs_0_file, oldcfg->cfg_net_act_gprs_0_file);
  wstrcpy(newcfg->cfg_net_act_gprs_1_file, oldcfg->cfg_net_act_gprs_1_file);
  wstrcpy(newcfg->cfg_net_act_gprs_2_file, oldcfg->cfg_net_act_gprs_2_file);
  wstrcpy(newcfg->cfg_net_act_gprs_3_file, oldcfg->cfg_net_act_gprs_3_file);
  wstrcpy(newcfg->cfg_net_act_gprs_4_file, oldcfg->cfg_net_act_gprs_4_file);
  wstrcpy(newcfg->cfg_net_act_gprs_5_file, oldcfg->cfg_net_act_gprs_5_file);
  wstrcpy(newcfg->cfg_net_gprs_0_file, oldcfg->cfg_net_gprs_0_file);
  wstrcpy(newcfg->cfg_net_gprs_1_file, oldcfg->cfg_net_gprs_1_file);
  wstrcpy(newcfg->cfg_net_gprs_2_file, oldcfg->cfg_net_gprs_2_file);
  wstrcpy(newcfg->cfg_net_gprs_3_file, oldcfg->cfg_net_gprs_3_file);
  wstrcpy(newcfg->cfg_net_gprs_4_file, oldcfg->cfg_net_gprs_4_file);
  wstrcpy(newcfg->cfg_net_gprs_5_file, oldcfg->cfg_net_gprs_5_file);
  wstrcpy(newcfg->cfg_net_none_0_file, oldcfg->cfg_net_none_0_file);
  wstrcpy(newcfg->cfg_net_none_1_file, oldcfg->cfg_net_none_1_file);
  wstrcpy(newcfg->cfg_net_none_2_file, oldcfg->cfg_net_none_2_file);
  wstrcpy(newcfg->cfg_net_none_3_file, oldcfg->cfg_net_none_3_file);
  wstrcpy(newcfg->cfg_net_none_4_file, oldcfg->cfg_net_none_4_file);
  wstrcpy(newcfg->cfg_net_none_5_file, oldcfg->cfg_net_none_5_file);
  wstrcpy(newcfg->cfg_net_flight_file, oldcfg->cfg_net_flight_file);
  newcfg->cfg_refresh_network = oldcfg->cfg_refresh_network;

//Длительность звонка
  newcfg->cfg_call_show = 0;
  newcfg->cfg_call_font=0;
  newcfg->cfg_call_highlight = 0;
  newcfg->cfg_call_color=0xFFFFFFFF;
  newcfg->cfg_call_border=0xFF000000;
  newcfg->cfg_call_align = 0;
  newcfg->cfg_call_x = 0;
  newcfg->cfg_call_y = 0;
  newcfg->cfg_call_icon_show = 1;
  wstrcpy(newcfg->cfg_call_file, L"%INI_INT%/Standby Mode/call.png");

//Приложения...
  newcfg->cfg_app_show = oldcfg->cfg_app_show;
  newcfg->cfg_app_row_max = oldcfg->cfg_app_row_max;
  newcfg->cfg_app_column_max = oldcfg->cfg_app_column_max;
  newcfg->cfg_app_fill_by_row = oldcfg->cfg_app_fill_by_row;
  newcfg->cfg_app_align = oldcfg->cfg_app_align;
  newcfg->cfg_app_x = oldcfg->cfg_app_x;
  newcfg->cfg_app_y = oldcfg->cfg_app_y;

  newcfg->cfg_app_book_show = oldcfg->cfg_app_book_show;
  wstrcpy(newcfg->cfg_app_book_msg,oldcfg->cfg_app_book_msg);
  newcfg->cfg_app_book_font = oldcfg->cfg_app_book_font;
  newcfg->cfg_app_book_highlight = oldcfg->cfg_app_book_highlight;
  newcfg->cfg_app_book_color = oldcfg->cfg_app_book_color;
  newcfg->cfg_app_book_border = oldcfg->cfg_app_book_border;
  newcfg->cfg_app_book_align = oldcfg->cfg_app_book_align;
  newcfg->cfg_app_book_x = oldcfg->cfg_app_book_x;
  newcfg->cfg_app_book_y = oldcfg->cfg_app_book_y;

  newcfg->cfg_app_elf_show = oldcfg->cfg_app_elf_show;
  wstrcpy(newcfg->cfg_app_elf_msg,oldcfg->cfg_app_elf_msg);
  newcfg->cfg_app_elf_font = oldcfg->cfg_app_elf_font;
  newcfg->cfg_app_elf_highlight = oldcfg->cfg_app_elf_highlight;
  newcfg->cfg_app_elf_color = oldcfg->cfg_app_elf_color;
  newcfg->cfg_app_elf_border = oldcfg->cfg_app_elf_border;
  newcfg->cfg_app_elf_align = oldcfg->cfg_app_elf_align;
  newcfg->cfg_app_elf_x = oldcfg->cfg_app_elf_x;
  newcfg->cfg_app_elf_y = oldcfg->cfg_app_elf_y;

  newcfg->cfg_app_java_show = oldcfg->cfg_app_java_show;
  wstrcpy(newcfg->cfg_app_java_msg,oldcfg->cfg_app_java_msg);
  newcfg->cfg_app_java_font = oldcfg->cfg_app_java_font;
  newcfg->cfg_app_java_highlight = oldcfg->cfg_app_java_highlight;
  newcfg->cfg_app_java_color = oldcfg->cfg_app_java_color;
  newcfg->cfg_app_java_border = oldcfg->cfg_app_java_border;
  newcfg->cfg_app_java_align = oldcfg->cfg_app_java_align;
  newcfg->cfg_app_java_x = oldcfg->cfg_app_java_x;
  newcfg->cfg_app_java_y = oldcfg->cfg_app_java_y;

//Свои сообщения...
//Сообщение 1
  wstrcpy(newcfg->cfg_msg1, oldcfg->cfg_msg1);
  newcfg->cfg_msg1_show = oldcfg->cfg_msg1_show;
  newcfg->cfg_msg1_font = oldcfg->cfg_msg1_font;
  newcfg->cfg_msg1_highlight = oldcfg->cfg_msg1_highlight;
  newcfg->cfg_msg1_color = oldcfg->cfg_msg1_color;
  newcfg->cfg_msg1_border = oldcfg->cfg_msg1_border;
  newcfg->cfg_msg1_align = oldcfg->cfg_msg1_align;
  newcfg->cfg_msg1_x = oldcfg->cfg_msg1_x;
  newcfg->cfg_msg1_y = oldcfg->cfg_msg1_y;
//Сообщение 2
  wstrcpy(newcfg->cfg_msg2, oldcfg->cfg_msg2);
  newcfg->cfg_msg2_show = oldcfg->cfg_msg2_show;
  newcfg->cfg_msg2_font = oldcfg->cfg_msg2_font;
  newcfg->cfg_msg2_highlight = oldcfg->cfg_msg2_highlight;
  newcfg->cfg_msg2_color = oldcfg->cfg_msg2_color;
  newcfg->cfg_msg2_border = oldcfg->cfg_msg2_border;
  newcfg->cfg_msg2_align = oldcfg->cfg_msg2_align;
  newcfg->cfg_msg2_x = oldcfg->cfg_msg2_x;
  newcfg->cfg_msg2_y = oldcfg->cfg_msg2_y;
//Сообщение 3
  wstrcpy(newcfg->cfg_msg3, oldcfg->cfg_msg3);
  newcfg->cfg_msg3_show = oldcfg->cfg_msg3_show;
  newcfg->cfg_msg3_font = oldcfg->cfg_msg3_font;
  newcfg->cfg_msg3_highlight = oldcfg->cfg_msg3_highlight;
  newcfg->cfg_msg3_color = oldcfg->cfg_msg3_color;
  newcfg->cfg_msg3_border = oldcfg->cfg_msg3_border;
  newcfg->cfg_msg3_align = oldcfg->cfg_msg3_align;
  newcfg->cfg_msg3_x = oldcfg->cfg_msg3_x;
  newcfg->cfg_msg3_y = oldcfg->cfg_msg3_y;
//Сообщение 4
  wstrcpy(newcfg->cfg_msg4, oldcfg->cfg_msg4);
  newcfg->cfg_msg4_show = oldcfg->cfg_msg4_show;
  newcfg->cfg_msg4_font = oldcfg->cfg_msg4_font;
  newcfg->cfg_msg4_highlight = oldcfg->cfg_msg4_highlight;
  newcfg->cfg_msg4_color = oldcfg->cfg_msg4_color;
  newcfg->cfg_msg4_border = oldcfg->cfg_msg4_border;
  newcfg->cfg_msg4_align = oldcfg->cfg_msg4_align;
  newcfg->cfg_msg4_x = oldcfg->cfg_msg4_x;
  newcfg->cfg_msg4_y = oldcfg->cfg_msg4_y;
//Сообщение 5
  wstrcpy(newcfg->cfg_msg5, oldcfg->cfg_msg5);
  newcfg->cfg_msg5_show = oldcfg->cfg_msg5_show;
  newcfg->cfg_msg5_font = oldcfg->cfg_msg5_font;
  newcfg->cfg_msg5_highlight = oldcfg->cfg_msg5_highlight;
  newcfg->cfg_msg5_color = oldcfg->cfg_msg5_color;
  newcfg->cfg_msg5_border = oldcfg->cfg_msg5_border;
  newcfg->cfg_msg5_align = oldcfg->cfg_msg5_align;
  newcfg->cfg_msg5_x = oldcfg->cfg_msg5_x;
  newcfg->cfg_msg5_y = oldcfg->cfg_msg5_y;
//Сообщение 6
  wstrcpy(newcfg->cfg_msg6, oldcfg->cfg_msg6);
  newcfg->cfg_msg6_show = oldcfg->cfg_msg6_show;
  newcfg->cfg_msg6_font = oldcfg->cfg_msg6_font;
  newcfg->cfg_msg6_highlight = oldcfg->cfg_msg6_highlight;
  newcfg->cfg_msg6_color = oldcfg->cfg_msg6_color;
  newcfg->cfg_msg6_border = oldcfg->cfg_msg6_border;
  newcfg->cfg_msg6_align = oldcfg->cfg_msg6_align;
  newcfg->cfg_msg6_x = oldcfg->cfg_msg6_x;
  newcfg->cfg_msg6_y = oldcfg->cfg_msg6_y;

//Дополнительно...
  newcfg->cfg_soft_key_show = oldcfg->cfg_soft_key_show;
  newcfg->cfg_refresh_time = oldcfg->cfg_refresh_time;
  newcfg->cfg_standby_only = oldcfg->cfg_standby_only;
}

int merge_from_v2_16(v2_17_config_t *newcfg, void *oldcfg, int oldcfgsize)
{
  int result = -1;
  if(oldcfgsize == sizeof(v2_16_config_t))
  {
    _merge_from_v2_16(newcfg, (v2_16_config_t *)oldcfg);
    result = 0;
  }
  else
  {
    char *buf;
    if ((buf=new char[sizeof(v2_16_config_t)]))
    {
      if(merge_from_v2_15((v2_16_config_t *)buf, oldcfg, oldcfgsize) == 0)
      {
        _merge_from_v2_16(newcfg, (v2_16_config_t *)buf);
        result = 0;
      }
    }
    delete buf;
  }
  return(result);
}

