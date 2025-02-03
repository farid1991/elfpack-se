/*
Загрузчик конфигурации
(c) Rst7, Kibab
Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _CONFLOADER_H_
#define _CONFLOADER_H_

// Вызвать обязательно перед обращением к переменным конфигурации
void InitConfig(void);
int SaveConfigData(const wchar_t *path,const wchar_t *fname);
unsigned long ConfigCrc32();

extern const wchar_t* successed_config_path;
extern const wchar_t* successed_config_name;
#endif
