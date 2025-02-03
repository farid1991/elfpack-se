#include "main.h"


DIRECTORY_METHODS * methods;

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int main (void)
{
  methods = (DIRECTORY_METHODS *)get_envp(get_bid(current_process()), "directory_methods");
  if (methods)
  {
    CreateEditor();
  }
  else
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(L"Run dirman.elf first", ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 1, 0, 0);
    SUBPROC(elf_exit);
  }
  return 0;
}
