
// вот все методы дл€ директорий

/*
ѕервым параметром методом €вл€етс€ "wchar_t * dir" за исключением "Dir_GetList".
ƒанный параметр €вл€етс€ своеобразным идентификаторм, по которуму можно получить
или установить свойства дл€ дирректории.
»дентификатором служит относительный путь к папке (относительно DIR_USER)
ѕри работе с методами допускаетс€ использование абсолютных путей, в таком случае
они будут преобразованны методами в относительные пути.
ѕример допустимых значений параметра dir дл€ папки "ZBin":
  1. GetDir(DIR_ELFS)
  2. GetDir(DIR_ELFS|MEM_INTERNAL)
  3. GetDir(DIR_ELFS|MEM_EXTERNAL)
  4. L"/other/zbin"
  5. L"/oThEr/zbiN"
  примечание:
  - соблюдение регистра символов необ€зательно
  - любой путь об€зательно должен начинатьс€ с символа "/"
  - недопустимо наличие символа "/" в конце пути (путь будет распознан неверно)

¬ случае неудачи меотд возвращает 0, иначе 1

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
  if (!methods) return 0; //(видимо dirman.elf не запущен)
  wchar_t * dir = GetDir(DIR_ELFS);
  methods->Dir_Add(dir); //если уже зарегестрирован то просто вернет 1
  methods->Dir_SetName(dir, L"new name");
  methods->Dir_SetDeleteState(dir, HIDDEN);
  methods->Dir_SetRenameState(dir, DISABLED);
  methods->Dir_SetSmallIconId(dir, 0xE540);
  methods->Dir_SetBigIconFile(dir, GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL), L"dir_b.png");
  wchar_t * dir2 = GetDir(DIR_ELFS_CONFIG);
  methods->Dir_Rename(dir, dir2); //dir замен€етс€ на dir2, параемтры остаютс€, dir не зарегестрирована
  wchar_t iconid;
  wchar_t *path, *name;
  int res = methods->Dir_GetSmallIcon(dir2, &iconid, &path, &name);
  switch (res)
  {
  case 0:
    //ошибка при чтении
    break;
  case 1:
    //это иконка из пршивки, значение в iconid
    break;
  case 2:
    //это иконка из файла, значение id в iconid, путь к иконки в path, им€ файла в name
    //!!!значени€ нельз€ высвобождать
    break;
  }
  wchar_t * shortpath = methods->Dir_GetShortPath(dir);
  //в shortpath получили L"/other/zbin" (в нижнем регистре)
  //!!! тут не забываем освободить пам€ть
  delete shortpath;
  return 1;
}
