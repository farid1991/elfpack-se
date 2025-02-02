/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

// Вызвать обязательно перед обращением к переменным конфигурации

int SaveConfigData(const wchar_t *path,const wchar_t *fname);
void InitConfig(void);

extern const wchar_t *successed_config_path;
extern const wchar_t *successed_config_name;
#endif
