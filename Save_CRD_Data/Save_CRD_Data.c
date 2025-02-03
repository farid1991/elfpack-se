#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\include\var_arg.h"
#include "RegistryClasses.h"

#define CRASH_DATA_PATH L"/card/other/CrashData"
#define CRASH_DATA_NAME_MASK L"crash_data_%d.bin"
#define CRASH_DATA_COUNTER_NAME L"crash_data_counter.bin"
#define CRASH_DATA_COUNTER_PATH GetDir(MEM_EXTERNAL|DIR_ELFS_CONFIG)
#define VAR_DATA_NAME "GD_CRD_Crash_Data_Array"
#define VAR_COUNT_NAME "GD_CRD_Crash_Counter"

void elf_exit(void)
{
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}


void save_crd_var()
{
  ISEMCRegistryManager * pISEMCRegistryManager=0;
  IRegistry * pIRegistry=0;
  IRegistryKey * pIRegistryKey=0;
  
  RegKey_ValueInfo_t value_info;
  int crd_count=0;
  wchar_t fname[20];
  char temp_buf;
  wchar_t crash_count_current;
  wchar_t crash_count_last;
  int f;
  int need_to_save=0;
  
  CoCreateInstance(&CID_CSEMCRegistryManager,&IID_ISEMCRegistryManager,PPINTERFACE(&pISEMCRegistryManager));
  pISEMCRegistryManager->CreateRegistry(&pIRegistry);
  
  pIRegistry->BeginTransaction(Reg_RegistryTransactionClass_AutoCommit);
  pIRegistry->OpenKey("/gdfs",&pIRegistryKey);
  
  pIRegistryKey->GetValue(VAR_COUNT_NAME,Reg_RegValueType_Binary,sizeof(crash_count_current),&crash_count_current,0);
  
  if (fstat(CRASH_DATA_COUNTER_PATH,CRASH_DATA_COUNTER_NAME,0)>=0)
  {
    f=_fopen(CRASH_DATA_COUNTER_PATH,CRASH_DATA_COUNTER_NAME,FSX_O_RDWR,FSX_S_IRUSR|FSX_S_IWUSR,0);
    fread(f,&crash_count_last,sizeof(crash_count_last));  //read count
    if (crash_count_last!=crash_count_current)
    {
      lseek(f,0,0);
      fwrite(f,&crash_count_current,sizeof(crash_count_current));
      need_to_save=1;
    }
    fclose(f);
  }
  else
  {
    need_to_save=1;
    f=_fopen(CRASH_DATA_COUNTER_PATH,CRASH_DATA_COUNTER_NAME,FSX_O_WRONLY,FSX_S_IRUSR|FSX_S_IWUSR,0);
    fwrite(f,&crash_count_current,sizeof(crash_count_current));
    fclose(f);
  }
  
  if (need_to_save)
  {
    do
    {
      snwprintf(fname,sizeof(fname),CRASH_DATA_NAME_MASK,crd_count);
      crd_count++;
    }
    while (fstat(CRASH_DATA_PATH,fname,0)>=0);
    
    memset(&value_info,0,sizeof(RegKey_ValueInfo_t));
    
    pIRegistryKey->GetValue(VAR_DATA_NAME,Reg_RegValueType_Unspecified,1,&temp_buf,&value_info);
    
    if (value_info.dataSize!=0)
    {
      char * buf_ptr = new char[value_info.dataSize];
      
      pIRegistryKey->GetValue(VAR_DATA_NAME,Reg_RegValueType_Binary,value_info.dataSize,buf_ptr,0);
      
      f=_fopen(CRASH_DATA_PATH,fname,FSX_O_WRONLY,FSX_S_IRUSR|FSX_S_IWUSR,0);
      fwrite(f,buf_ptr,value_info.dataSize);  //write data
      fclose(f);
      
      delete buf_ptr;
    }
  }

  pIRegistry->EndTransaction(false);
  
  pISEMCRegistryManager->Release();
  pIRegistry->Release();
  pIRegistryKey->Release();
}


int main (wchar_t * elfname,wchar_t * fpath,wchar_t * fname)
{
  save_crd_var();
  SUBPROC(elf_exit);
  return 0;
}
