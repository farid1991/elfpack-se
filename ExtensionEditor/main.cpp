#include "main.h"


EXTENSION_METHODS * methods;

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int main (void)
{
  methods = (EXTENSION_METHODS *)get_envp(get_bid(current_process()), "extension_methods");
  if (methods)
  {
    CreateEditor();
  }
  else
  {
    MessageBox(EMPTY_TEXTID, TextID_Create(L"Run extman.elf first", ENC_UCS2, TEXTID_ANY_LEN), NOIMAGE, 1, 0, 0);
    SUBPROC(elf_exit);
  }
  return 0;
}
