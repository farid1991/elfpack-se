/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

unsigned long Crc32(unsigned char *buf, unsigned long len);
// Вызвать обязательно перед обращением к переменным конфигурации
void InitConfig(void);

extern const wchar_t *successed_config_path;
extern const wchar_t *successed_config_name;

#define REF_CRC32   0x844968E5
extern unsigned long cfg_crc;


#endif
