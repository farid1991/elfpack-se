
#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"

wchar_t*GetNamesOfIMN(int x, int)
{
  switch(x)
  {
#ifdef ENG_LANG
  case 0:
    return L"���";
  case 1:
    return L"������� ���";
  case 2:
    return L"�����1";
  case 3:
    return L"�����2";
  case 4:
    return L"�����3";
  case 5:
    return L"�����4";
  case 6:
    return L"����� ����.������";
#else
  case 0:
    return L"Background";
  case 1:
    return L"Game Background";
  case 2:
    return L"Cube1";
  case 3:
    return L"Cube2";
  case 4:
    return L"Cube3";
  case 5:
    return L"Cube4";
  case 6:
    return L"Cube next figure";
#endif
  }
  return L" ";
}
void ShowInfo()
{
  int met;
#ifdef ENG_LANG
  met=STR("��������: Tetris v0.5.5\n(c) DoCent aka MoneyMasteR\n\n����������:\n\n�����, ����, 1, 3 - ��������� ������\n\n�����, ������, 4, 6 - ����������� ������\n\n�����, 5 - ���������\n\n����� ����, 8 - ������� ������\n\n�������� ����� - �����\n\n������ ����� - �����\n\n# - ���./����. ����\n\n* - ��������\n");
#else
  met=STR("Name: Tetris v0.5.5\n(c) DoCent aka MoneyMasteR\n\nManagement:\n\nUp, Down, 1, 3 - upheaval figure\n\nLeft, Right, 4, 6 - displacement figure\n\nEnter, 5 - speedup figure\n\n����� ����, 8 - fall figure\n\nShort Back - pause\n\nLong Back - exit\n\n# - on/off sound\n\n* - hide\n");
#endif
  MessageBox(STR("����"), met, 0, 2, 0, 0);
}

int ShowAuthorInfo(void *mess ,BOOK* book)
{
  ShowInfo();
  return 1;
}
