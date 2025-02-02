/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

// Вызвать обязательно перед обращением к переменным конфигурации
void InitConfig(void);

extern wchar_t successed_config_path[512];
extern wchar_t successed_config_name[512];
#endif
