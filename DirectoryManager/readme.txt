
// ��� ��� ������ ��� ����������

/*
������ ���������� ������� �������� "wchar_t * dir" �� ����������� "Dir_GetList".
������ �������� �������� ������������ ��������������, �� �������� ����� ��������
��� ���������� �������� ��� �����������.
��������������� ������ ������������� ���� � ����� (������������ DIR_USER)
��� ������ � �������� ����������� ������������� ���������� �����, � ����� ������
��� ����� �������������� �������� � ������������� ����.
������ ���������� �������� ��������� dir ��� ����� "ZBin":
  1. GetDir(DIR_ELFS)
  2. GetDir(DIR_ELFS|MEM_INTERNAL)
  3. GetDir(DIR_ELFS|MEM_EXTERNAL)
  4. L"/other/zbin"
  5. L"/oThEr/zbiN"
  ����������:
  - ���������� �������� �������� �������������
  - ����� ���� ����������� ������ ���������� � ������� "/"
  - ����������� ������� ������� "/" � ����� ���� (���� ����� ��������� �������)

� ������ ������� ����� ���������� 0, ����� 1

enum
{
  HIDDEN = 0,
  DISABLED,
  ENABLED
};

typedef struct
{
  wchar_t * (*Dir_GetShortPath)(wchar_t * dir);
  int (*Dir_Registered)(wchar_t * dir);
  int (*Dir_GetList)(wchar_t *** dir_list);
  int (*Dir_Add)(wchar_t * dir);
  int (*Dir_Rename)(wchar_t * dir, wchar_t * new_dir);
  int (*Dir_Remove)(wchar_t * dir);
  int (*Dir_SetName)(wchar_t * dir, wchar_t * name);
  int (*Dir_GetName)(wchar_t * dir, wchar_t ** name);
  int (*Dir_SetDeleteState)(wchar_t * dir, char state);
  int (*Dir_GetDeleteState)(wchar_t * dir, char * state);
  int (*Dir_SetRenameState)(wchar_t * dir, char state);
  int (*Dir_GetRenameState)(wchar_t * dir, char * state);
  int (*Dir_SetSmallIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetSmallIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetSmallIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
  int (*Dir_SetBigIconId)(wchar_t * dir, wchar_t iconid);
  int (*Dir_SetBigIconFile)(wchar_t * dir, wchar_t * path, wchar_t * name);
  int (*Dir_GetBigIcon)(wchar_t * dir, wchar_t * iconid, wchar_t ** path, wchar_t ** name);
}DIRECTORY_METHODS;


int RegistrDir()
{
  DIRECTORY_METHODS * methods;
  methods = (DIRECTORY_METHODS *)get_envp(get_bid(current_process()), "directory_methods");
  if (!methods) return 0; //(������ dirman.elf �� �������)
  wchar_t * dir = GetDir(DIR_ELFS);
  methods->Dir_Add(dir); //���� ��� ��������������� �� ������ ������ 1
  methods->Dir_SetName(dir, L"new name");
  methods->Dir_SetDeleteState(dir, HIDDEN);
  methods->Dir_SetRenameState(dir, DISABLED);
  methods->Dir_SetSmallIconId(dir, 0xE540);
  methods->Dir_SetBigIconFile(dir, GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL), L"dir_b.png");
  wchar_t * dir2 = GetDir(DIR_ELFS_CONFIG);
  methods->Dir_Rename(dir, dir2); //dir ���������� �� dir2, ��������� ��������, dir �� ����������������
  wchar_t iconid;
  wchar_t *path, *name;
  int res = methods->Dir_GetSmallIcon(dir2, &iconid, &path, &name);
  switch (res)
  {
  case 0:
    //������ ��� ������
    break;
  case 1:
    //��� ������ �� �������, �������� � iconid
    break;
  case 2:
    //��� ������ �� �����, �������� id � iconid, ���� � ������ � path, ��� ����� � name
    //!!!�������� ������ ������������
    break;
  }
  wchar_t * shortpath = methods->Dir_GetShortPath(dir);
  //� shortpath �������� L"/other/zbin" (� ������ ��������)
  //!!! ��� �� �������� ���������� ������
  delete shortpath;
  return 1;
}
