/*
��������� ������������
(c) Rst7, Kibab
 ������������ �������� ������� � ������������� ������� �������������
*/
#ifndef _CONFLOADER_H_
  #define _CONFLOADER_H_

// ������� ����������� ����� ���������� � ���������� ������������

int SaveConfigData(const wchar_t *path,const wchar_t *fname);
void InitConfig(void);

extern const wchar_t *successed_config_path;
extern const wchar_t *successed_config_name;
#endif
