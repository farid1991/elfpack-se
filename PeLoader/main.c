//#define _wintel

#ifndef _wintel
#include "..\include\Lib_Clara.h"
#include "..\include\Dir.h"

void elf_exit(void)
{
	kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}
#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void elf_exit(void)
{
}
#endif

typedef struct FileHeader
{
	unsigned char	P;
	unsigned char	E;
	unsigned char	null1;
	unsigned char	null2;
	unsigned short	Machine;					//The number that identifies the type of target machine. For more information, see section 3.3.1, “Machine Types.”
	unsigned short	NumberOfSections;			//The number of sections. This indicates the size of the section table, which immediately follows the headers.
	unsigned long	TimeDateStamp;				//The low 32 bits of the number of seconds since 00:00 January 1, 1970 (a C run-time time_t value), that indicates when the file was created.
	unsigned long	PointerToSymbolTable;		//The file offset of the COFF symbol table, or zero if no COFF symbol table is present. This value should be zero for an image because COFF debugging information is deprecated.
	unsigned long	NumberOfSymbols;			//The number of entries in the symbol table. This data can be used to locate the string table, which immediately follows the symbol table. This value should be zero for an image because COFF debugging information is deprecated.
	unsigned short	SizeOfOptionalHeader;		//The size of the optional header, which is required for executable files but not for object files. This value should be zero for an object file. For a description of the header format, see section 3.4, “Optional Header (Image Only).”
	unsigned short	Characteristics;			//The flags that indicate the attributes of the file. For specific flag values, see section 3.3.2, “Characteristics.”

	unsigned short	Magic;						//The unsigned integer that identifies the state of the image file. The most common number is 0x10B, which identifies it as a normal executable file. 0x107 identifies it as a ROM image, and 0x20B identifies it as a PE32+ executable.
	unsigned char	MajorLinkerVersion;			//The linker major version number.
	unsigned char	MinorLinkerVersion;			//The linker minor version number.
	unsigned long	SizeOfCode;					//The size of the code (text) section, or the sum of all code sections if there are multiple sections.
	unsigned long	SizeOfInitializedData;		//The size of the initialized data section, or the sum of all such sections if there are multiple data sections.
	unsigned long	SizeOfUninitializedData;	//The size of the uninitialized data section (BSS), or the sum of all such sections if there are multiple BSS sections.
	unsigned long	AddressOfEntryPoint;		//The address of the entry point relative to the image base when the executable file is loaded into memory. For program images, this is the starting address. For device drivers, this is the address of the initialization function. An entry point is optional for DLLs. When no entry point is present, this field must be zero.
	unsigned long	BaseOfCode;					//The address that is relative to the image base of the beginning-of-code section when it is loaded into memory.
	unsigned long	BaseOfData;					//The address that is relative to the image base of the beginning-of-data section when it is loaded into memory.

	unsigned long	ImageBase;					//The preferred address of the first byte of image when loaded into memory; must be a multiple of 64 K. The default for DLLs is 0x10000000. The default for Windows CE EXEs is 0x00010000. The default for Windows NT, Windows 2000, Windows XP, Windows 95, Windows 98, and Windows Me is 0x00400000.
	unsigned long	SectionAlignment;			//The alignment (in bytes) of sections when they are loaded into memory. It must be greater than or equal to FileAlignment. The default is the page size for the architecture.
	unsigned long	FileAlignment;				//The alignment factor (in bytes) that is used to align the raw data of sections in the image file. The value should be a power of 2 between 512 and 64 K, inclusive. The default is 512. If the SectionAlignment is less than the architecture’s page size, then FileAlignment must match SectionAlignment.
	unsigned short	MajorOperatingSystemVersion;//The major version number of the required operating system.
	unsigned short	MinorOperatingSystemVersion;//The minor version number of the required operating system.
	unsigned short	MajorImageVersion;			//The major version number of the image.
	unsigned short	MinorImageVersion;			//The minor version number of the image.
	unsigned short	MajorSubsystemVersion;		//The major version number of the subsystem.
	unsigned short	MinorSubsystemVersion;		//The minor version number of the subsystem.
	unsigned long	Win32VersionValue;			//Reserved, must be zero.
	unsigned long	SizeOfImage;				//The size (in bytes) of the image, including all headers, as the image is loaded in memory. It must be a multiple of SectionAlignment.
	unsigned long	SizeOfHeaders;				//The combined size of an MS DOS stub, PE header, and section headers rounded up to a multiple of FileAlignment.
	unsigned long	CheckSum;					//The image file checksum. The algorithm for computing the checksum is incorporated into IMAGHELP.DLL. The following are checked for validation at load time: all drivers, any DLL loaded at boot time, and any DLL that is loaded into a critical Windows process.
	unsigned short	Subsystem;					//The subsystem that is required to run this image. For more information, see “Windows Subsystem” later in this specification.
	unsigned short	DllCharacteristics;			//For more information, see “DLL Characteristics” later in this specification.
	unsigned long	SizeOfStackReserve;			//The size of the stack to reserve. Only SizeOfStackCommit is committed; the rest is made available one page at a time until the reserve size is reached.
	unsigned long	SizeOfStackCommit;			//The size of the stack to commit.
	unsigned long	SizeOfHeapReserve;			//The size of the local heap space to reserve. Only SizeOfHeapCommit is committed; the rest is made available one page at a time until the reserve size is reached.
	unsigned long	SizeOfHeapCommit;			//The size of the local heap space to commit.
	unsigned long	LoaderFlags;				//Reserved, must be zero.
	unsigned long	NumberOfRvaAndSizes;		//The number of data-directory entries in the remainder of the optional header. Each describes a location and size.
};

enum MagicNumber
{
	PE32	=0x10b,
	PE32p	=0x20b,
};

enum WindowsSubsystem
{
	IMAGE_SUBSYSTEM_UNKNOWN					=0,		//An unknown subsystem
	IMAGE_SUBSYSTEM_NATIVE					=1,		//Device drivers and native Windows processes
	IMAGE_SUBSYSTEM_WINDOWS_GUI				=2,		//The Windows graphical user interface (GUI) subsystem
	IMAGE_SUBSYSTEM_WINDOWS_CUI				=3,		//The Windows character subsystem
	IMAGE_SUBSYSTEM_POSIX_CUI				=7,		//The Posix character subsystem
	IMAGE_SUBSYSTEM_WINDOWS_CE_GUI			=9,		//Windows CE
	IMAGE_SUBSYSTEM_EFI_APPLICATION			=10,	//An Extensible Firmware Interface (EFI) application
	IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER	=11,	//An EFI driver with boot services
	IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER		=12,	//An EFI driver with run-time services
	IMAGE_SUBSYSTEM_EFI_ROM					=13,	//An EFI ROM image
	IMAGE_SUBSYSTEM_XBOX					=14,	//XBOX
};
enum DLLCharacteristics
{
	IMAGE_DLL_CHARACTERISTICS_0x0001				=0x0001,	//Reserved, must be zero.
	IMAGE_DLL_CHARACTERISTICS_0x0002				=0x0002,	//Reserved, must be zero.
	IMAGE_DLL_CHARACTERISTICS_0x0004				=0x0004,	//Reserved, must be zero.
	IMAGE_DLL_CHARACTERISTICS_0x0008				=0x0008,	//Reserved, must be zero.
	IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE			=0x0040,	//DLL can be relocated at load time.
	IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY		=0x0080,	//Code Integrity checks are enforced.
	IMAGE_DLL_CHARACTERISTICS_NX_COMPAT				=0x0100,	//Image is NX compatible.
	IMAGE_DLLCHARACTERISTICS_NO_ISOLATION			=0x0200,	//Isolation aware, but do not isolate the image.
	IMAGE_DLLCHARACTERISTICS_NO_SEH					=0x0400,	//Does not use structured exception (SE) handling. No SE handler may be called in this image.
	IMAGE_DLLCHARACTERISTICS_NO_BIND				=0x0800,	//Do not bind the image.
	IMAGE_DLLCHARACTERISTICS_0x1000					=0x1000,	//Reserved, must be zero.
	IMAGE_DLLCHARACTERISTICS_WDM_DRIVER				=0x2000,	//A WDM driver.
	IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	=0x8000,	//Terminal Server aware.
};

enum MachineTypes
{
	IMAGE_FILE_MACHINE_UNKNOWN		=0x0,		//The contents of this field are assumed to be applicable to any machine type
	IMAGE_FILE_MACHINE_AM33			=0x1d3,		//Matsushita AM33
	IMAGE_FILE_MACHINE_AMD64		=0x8664,	//x64
	IMAGE_FILE_MACHINE_ARM			=0x1c0,		//ARM little endian
	IMAGE_FILE_MACHINE_EBC			=0xebc,		//EFI byte code
	IMAGE_FILE_MACHINE_I386			=0x14c,		//Intel 386 or later processors and compatible processors
	IMAGE_FILE_MACHINE_IA64			=0x200,		//Intel Itanium processor family
	IMAGE_FILE_MACHINE_M32R			=0x9041,	//Mitsubishi M32R little endian
	IMAGE_FILE_MACHINE_MIPS16		=0x266,		//MIPS16
	IMAGE_FILE_MACHINE_MIPSFPU		=0x366,		//MIPS with FPU
	IMAGE_FILE_MACHINE_MIPSFPU16	=0x466,		//MIPS16 with FPU
	IMAGE_FILE_MACHINE_POWERPC		=0x1f0,		//Power PC little endian
	IMAGE_FILE_MACHINE_POWERPCFP	=0x1f1,		//Power PC with floating point support
	IMAGE_FILE_MACHINE_R4000		=0x166,		//MIPS little endian
	IMAGE_FILE_MACHINE_SH3			=0x1a2,		//Hitachi SH3
	IMAGE_FILE_MACHINE_SH3DSP		=0x1a3,		//Hitachi SH3 DSP
	IMAGE_FILE_MACHINE_SH4			=0x1a6,		//Hitachi SH4
	IMAGE_FILE_MACHINE_SH5			=0x1a8,		//Hitachi SH5
	IMAGE_FILE_MACHINE_THUMB		=0x1c2,		//Thumb
	IMAGE_FILE_MACHINE_WCEMIPSV2	=0x169,		//MIPS little-endian WCE v2
};

//The Characteristics field contains flags that indicate attributes of the object or image file. The following flags are currently defined.
enum Characteristics
{
	IMAGE_FILE_RELOCS_STRIPPED			=0x0001,	//Image only, Windows CE, and Windows NT® and later. This indicates that the file does not contain base relocations and must therefore be loaded at its preferred base address. If the base address is not available, the loader reports an error. The default behavior of the linker is to strip base relocations from executable (EXE) files.
	IMAGE_FILE_EXECUTABLE_IMAGE			=0x0002,	//Image only. This indicates that the image file is valid and can be run. If this flag is not set, it indicates a linker error.
	IMAGE_FILE_LINE_NUMS_STRIPPED		=0x0004,	//COFF line numbers have been removed. This flag is deprecated and should be zero.
	IMAGE_FILE_LOCAL_SYMS_STRIPPED		=0x0008,	//COFF symbol table entries for local symbols have been removed. This flag is deprecated and should be zero.
	IMAGE_FILE_AGGRESSIVE_WS_TRIM		=0x0010,	//Obsolete. Aggressively trim working set. This flag is deprecated for Windows 2000 and later and must be zero.
	IMAGE_FILE_LARGE_ADDRESS_AWARE		=0x0020,	//Application can handle > 2 GB addresses.
	IMAGE_FILE_0x0040					=0x0040,	//This flag is reserved for future use.
	IMAGE_FILE_BYTES_REVERSED_LO		=0x0080,	//Little endian: the least significant bit (LSB) precedes the most significant bit (MSB) in memory. This flag is deprecated and should be zero.
	IMAGE_FILE_32BIT_MACHINE			=0x0100,	//Machine is based on a 32-bit-word architecture.
	IMAGE_FILE_DEBUG_STRIPPED			=0x0200,	//Debugging information is removed from the image file.
	IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	=0x0400,	//If the image is on removable media, fully load it and copy it to the swap file.
	IMAGE_FILE_NET_RUN_FROM_SWAP		=0x0800,	//If the image is on network media, fully load it and copy it to the swap file.
	IMAGE_FILE_SYSTEM					=0x1000,	//The image file is a system file, not a user program.
	IMAGE_FILE_DLL						=0x2000,	//The image file is a dynamic-link library (DLL). Such files are considered executable files for almost all purposes, although they cannot be directly run.
	IMAGE_FILE_UP_SYSTEM_ONLY			=0x4000,	//The file should be run only on a uniprocessor machine.
	IMAGE_FILE_BYTES_REVERSED_HI		=0x8000,	//Big endian: the MSB precedes the LSB in memory. This flag is deprecated and should be zero.
};

typedef struct SectionTable
{
	unsigned char	Name[8];				//An 8-byte, null-padded UTF-8 encoded string. If the string is exactly 8 characters long, there is no terminating null. For longer names, this field contains a slash (/) that is followed by an ASCII representation of a decimal number that is an offset into the string table. Executable images do not use a string table and do not support section names longer than 8 characters. Long names in object files are truncated if they are emitted to an executable file.
	unsigned long	VirtualSize;			//The total size of the section when loaded into memory. If this value is greater than SizeOfRawData, the section is zero-padded. This field is valid only for executable images and should be set to zero for object files.
	unsigned long	VirtualAddress;			//For executable images, the address of the first byte of the section relative to the image base when the section is loaded into memory. For object files, this field is the address of the first byte before relocation is applied; for simplicity, compilers should set this to zero. Otherwise, it is an arbitrary value that is subtracted from offsets during relocation.
	unsigned long	SizeOfRawData;			//The size of the section (for object files) or the size of the initialized data on disk (for image files). For executable images, this must be a multiple of FileAlignment from the optional header. If this is less than VirtualSize, the remainder of the section is zero-filled. Because the SizeOfRawData field is rounded but the VirtualSize field is not, it is possible for SizeOfRawData to be greater than VirtualSize as well. When a section contains only uninitialized data, this field should be zero.
	unsigned long	PointerToRawData;		//The file pointer to the first page of the section within the COFF file. For executable images, this must be a multiple of FileAlignment from the optional header. For object files, the value should be aligned on a 4 byte boundary for best performance. When a section contains only uninitialized data, this field should be zero.
	unsigned long	PointerToRelocations;	//The file pointer to the beginning of relocation entries for the section. This is set to zero for executable images or if there are no relocations.
	unsigned long	PointerToLinenumbers;	//The file pointer to the beginning of line-number entries for the section. This is set to zero if there are no COFF line numbers. This value should be zero for an image because COFF debugging information is deprecated.
	unsigned short	NumberOfRelocations;	//The number of relocation entries for the section. This is set to zero for executable images.
	unsigned short	NumberOfLinenumbers;	//The number of line-number entries for the section. This value should be zero for an image because COFF debugging information is deprecated.
	unsigned long	Characteristics;		//The flags that describe the characteristics of the section. For more information, see section 4.1, “Section Flags.”
};


enum SectionFlags
{
	IMAGE_SCN_0x00000000				=0x00000000,	//Reserved for future use.
	IMAGE_SCN_0x00000001				=0x00000001,	//Reserved for future use.
	IMAGE_SCN_0x00000002				=0x00000002,	//Reserved for future use.
	IMAGE_SCN_0x00000004				=0x00000004,	//Reserved for future use.
	IMAGE_SCN_TYPE_NO_PAD				=0x00000008,	//The section should not be padded to the next boundary. This flag is obsolete and is replaced by IMAGE_SCN_ALIGN_1BYTES. This is valid only for object files.
	IMAGE_SCN_0x00000010				=0x00000010,	//Reserved for future use.
	IMAGE_SCN_CNT_CODE					=0x00000020,	//The section contains executable code.
	IMAGE_SCN_CNT_INITIALIZED_DATA		=0x00000040,	//The section contains initialized data.
	IMAGE_SCN_CNT_UNINITIALIZED_DATA	=0x00000080,	//The section contains uninitialized data.
	IMAGE_SCN_LNK_OTHER					=0x00000100,	//Reserved for future use.
	IMAGE_SCN_LNK_INFO					=0x00000200,	//The section contains comments or other information. The .drectve section has this type. This is valid for object files only.
	IMAGE_SCN_0x00000400				=0x00000400,	//Reserved for future use.
	IMAGE_SCN_LNK_REMOVE				=0x00000800,	//The section will not become part of the image. This is valid only for object files.
	IMAGE_SCN_LNK_COMDAT				=0x00001000,	//The section contains COMDAT data. For more information, see section 5.5.6, “COMDAT Sections (Object Only).” This is valid only for object files.
	IMAGE_SCN_GPREL						=0x00008000,	//The section contains data referenced through the global pointer (GP).
	IMAGE_SCN_MEM_PURGEABLE				=0x00020000,	//Reserved for future use.
	IMAGE_SCN_MEM_16BIT					=0x00020000,	//Reserved for future use.
	IMAGE_SCN_MEM_LOCKED				=0x00040000,	//Reserved for future use.
	IMAGE_SCN_MEM_PRELOAD				=0x00080000,	//Reserved for future use.
	IMAGE_SCN_ALIGN_1BYTES				=0x00100000,	//Align data on a 1-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_2BYTES				=0x00200000,	//Align data on a 2-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_4BYTES				=0x00300000,	//Align data on a 4-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_8BYTES				=0x00400000,	//Align data on an 8-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_16BYTES				=0x00500000,	//Align data on a 16-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_32BYTES				=0x00600000,	//Align data on a 32-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_64BYTES				=0x00700000,	//Align data on a 64-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_128BYTES			=0x00800000,	//Align data on a 128-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_256BYTES			=0x00900000,	//Align data on a 256-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_512BYTES			=0x00A00000,	//Align data on a 512-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_1024BYTES			=0x00B00000,	//Align data on a 1024-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_2048BYTES			=0x00C00000,	//Align data on a 2048-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_4096BYTES			=0x00D00000,	//Align data on a 4096-byte boundary. Valid only for object files.
	IMAGE_SCN_ALIGN_8192BYTES			=0x00E00000,	//Align data on an 8192-byte boundary. Valid only for object files.
	IMAGE_SCN_LNK_NRELOC_OVFL			=0x01000000,	//The section contains extended relocations.
	IMAGE_SCN_MEM_DISCARDABLE			=0x02000000,	//The section can be discarded as needed.
	IMAGE_SCN_MEM_NOT_CACHED			=0x04000000,	//The section cannot be cached.
	IMAGE_SCN_MEM_NOT_PAGED				=0x08000000,	//The section is not pageable.
	IMAGE_SCN_MEM_SHARED				=0x10000000,	//The section can be shared in memory.
	IMAGE_SCN_MEM_EXECUTE				=0x20000000,	//The section can be executed as code.
	IMAGE_SCN_MEM_READ					=0x40000000,	//The section can be read.
	IMAGE_SCN_MEM_WRITE					=0x80000000,	//The section can be written to.
};

typedef struct Relocations
{
	unsigned long  VirtualAddress;	//The address of the item to which relocation is applied. This is the offset from the beginning of the section, plus the value of the section’s RVA/Offset field. See section 4, “Section Table (Section Headers).” For example, if the first byte of the section has an address of 0x10, the third byte has an address of 0x12.
	unsigned long  SymbolTableIndex;	//A zero-based index into the symbol table. This symbol gives the address that is to be used for the relocation. If the specified symbol has section storage class, then the symbol’s address is the address with the first section of the same name.
	unsigned short Type;				//A value that indicates the kind of relocation that should be performed. Valid relocation types depend on machine type. See section 5.2.1, “Type Indicators.”
};

enum RelocationTypeARM
{
	IMAGE_REL_ARM_ABSOLUTE	=0x0000,	//The relocation is ignored.
	IMAGE_REL_ARM_ADDR32	=0x0001,	//The 32-bit VA of the target.
	IMAGE_REL_ARM_ADDR32NB	=0x0002,	//The 32-bit RVA of the target.
	IMAGE_REL_ARM_BRANCH24	=0x0003,	//The 24-bit relative displacement to the target.
	IMAGE_REL_ARM_BRANCH11	=0x0004,	//The reference to a subroutine call. The reference consists of two 16-bit instructions with 11-bit offsets.
	IMAGE_REL_ARM_SECTION	=0x000E,	//The 16-bit section index of the section that contains the target. This is used to support debugging information.
	IMAGE_REL_ARM_SECREL	=0x000F,	//The 32-bit offset of the target from the beginning of its section. This is used to support debugging information and static thread local storage.
};

struct mz_hdr {
	unsigned char  mz_sign[2];
	unsigned short mz_bytes_int_last_page;
	unsigned short mz_total_number_of_pages;
	unsigned short mz_num_of_reloc_entries;
	unsigned short mz_header_size_in_paragraphs;
	unsigned short mz_unk1; //Minimum paragraphs of memory allocated in addition to the code size
	unsigned short mz_unk2; //Maximum number of paragraphs allocated in addition to the code size
	unsigned short mz_ss; //Initial SS relative to start of executable
	unsigned short mz_sp; //Initial SP
	unsigned short mz_checksum; //Checksum (or 0) of executable
	unsigned long  mz_entry; //CS:IP relative to start of executable (entry point)
	unsigned short mz_reloc_table_offset; //Offset of relocation table; 40h for new-(NE,LE,LX,W3,PE etc.) executable
	unsigned short mz_overlay_num; //Overlay number (0h = main program)
	unsigned long  mz_unk3; //???
	unsigned short mz_unk4; //Behaviour bits
	unsigned char  mz_reserved[26];
	unsigned long  mz_pe_header; //Offset of new executable header from start of file (or 0 if plain MZ executable)
};

enum
{
	text = 0,
	idata,
	reloc,
	dconst,
	edata,
	scount
};

typedef struct SEGMENT_LIST
{
	int	index;
	char * name;
};
SEGMENT_LIST slist[] =
{
	{text, ".text"},
	{idata, ".idata"},
	{reloc, ".reloc"},
	{dconst, ".dconst"},
	{edata, ".edata"}
};

typedef struct BaseRelocationBlock
{
	unsigned long PageRVA;		//The image base plus the page RVA is added to each offset to create the VA where the base relocation must be applied.
	unsigned long BlockSize;	//The total number of bytes in the base relocation block, including the Page RVA and Block Size fields and the Type/Offset fields that follow.
};
/*
The Block Size field is then followed by any number of Type or Offset field entries. Each entry is a WORD (2 bytes) and has the following structure.
Offset	Size	Field	Description
0	4 bits	Type	Stored in the high 4 bits of the WORD, a value that indicates the type of base relocation to be applied. For more information, see section 6.6.2, “Base Relocation Types.”
0	12 bits	Offset	Stored in the remaining 12 bits of the WORD, an offset from the starting address that was specified in the Page RVA field for the block. This offset specifies where the base relocation is to be applied.
*/
enum
{
	IMAGE_REL_BASED_ABSOLUTE		=0,	//The base relocation is skipped. This type can be used to pad a block.
	IMAGE_REL_BASED_HIGH			=1,	//The base relocation adds the high 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the high value of a 32-bit word.
	IMAGE_REL_BASED_LOW				=2,	//The base relocation adds the low 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the low half of a 32-bit word.
	IMAGE_REL_BASED_HIGHLOW			=3,	//The base relocation applies all 32 bits of the difference to the 32-bit field at offset.
	IMAGE_REL_BASED_HIGHADJ			=4,	//The base relocation adds the high 16 bits of the difference to the 16-bit field at offset. The 16-bit field represents the high value of a 32-bit word. The low 16 bits of the 32-bit value are stored in the 16-bit word that follows this base relocation. This means that this base relocation occupies two slots.
	IMAGE_REL_BASED_MIPS_JMPADDR	=5,	//The base relocation applies to a MIPS jump instruction.
	IMAGE_REL_NULL1					=6,	//Reserved, must be zero.
	IMAGE_REL_NULL2					=7,	//Reserved, must be zero.
	IMAGE_REL_BASED_MIPS_JMPADDR16	=9,	//The base relocation applies to a MIPS16 jump instruction.
	IMAGE_REL_BASED_DIR64			=10	//The base relocation applies the difference to the 64-bit field at offset.
};

typedef struct ImportDirectoryTable
{
	unsigned long	ImportLookupTableRVA;	//(Characteristics)	The RVA of the import lookup table. This table contains a name or ordinal for each import. (The name “Characteristics” is used in Winnt.h, but no longer describes this field.)
	unsigned long	TimeDateStamp;			//The stamp that is set to zero until the image is bound. After the image is bound, this field is set to the time/data stamp of the DLL.
	unsigned long	ForwarderChain;			//The index of the first forwarder reference.
	unsigned long	NameRVA;				//The address of an ASCII string that contains the name of the DLL. This address is relative to the image base.
	unsigned long	ImportAddressTableRVA;	//(Thunk Table)	The RVA of the import address table. The contents of this table are identical to the contents of the import lookup table until the image is bound.
};

typedef struct ImportLookup
{
	unsigned short	Ordinal;
	unsigned char	Name[2];
};

typedef struct _ImportDirectoryTable
{
	ImportLookup**	ImportLookupTable;		//(Characteristics)	The RVA of the import lookup table. This table contains a name or ordinal for each import. (The name “Characteristics” is used in Winnt.h, but no longer describes this field.)
	unsigned long	TimeDateStamp;			//The stamp that is set to zero until the image is bound. After the image is bound, this field is set to the time/data stamp of the DLL.
	unsigned long	ForwarderChain;			//The index of the first forwarder reference.
	unsigned char*	DllName;				//The address of an ASCII string that contains the name of the DLL. This address is relative to the image base.
	unsigned long*	ImportAddressTable;	//(Thunk Table)	The RVA of the import address table. The contents of this table are identical to the contents of the import lookup table until the image is bound.
};

typedef struct ExportDirectoryTable
{
	//The export symbol information begins with the export directory table, which describes the remainder of the export symbol information. The export directory table contains address information that is used to resolve imports to the entry points within this image.

	unsigned long	ExportFlags;			//Reserved, must be 0.
	unsigned long	TimeDateStamp;			//The time and date that the export data was created.
	unsigned short	MajorVersion;			//The major version number. The major and minor version numbers can be set by the user.
	unsigned short	MinorVersion;			//The minor version number.
	unsigned long	NameRVA;				//The address of the ASCII string that contains the name of the DLL. This address is relative to the image base.
	unsigned long	OrdinalBase;			//The starting ordinal number for exports in this image. This field specifies the starting ordinal number for the export address table. It is usually set to 1.
	unsigned long	AddressTableEntries;	//The number of entries in the export address table.
	unsigned long	NumberOfNamePointers;	//The number of entries in the name pointer table. This is also the number of entries in the ordinal table.
	unsigned long	ExportAddressTableRVA;	//The address of the export address table, relative to the image base.
	unsigned long	NamePointerRVA;			//The address of the export name pointer table, relative to the image base. The table size is given by the Number of Name Pointers field.
	unsigned long	OrdinalTableRVA;		//The address of the ordinal table, relative to the image base.
};
typedef struct _ExportDirectoryTable
{
	//The export symbol information begins with the export directory table, which describes the remainder of the export symbol information. The export directory table contains address information that is used to resolve imports to the entry points within this image.

	unsigned long	DllBase;				//ExportFlags;			//Reserved, must be 0.
	unsigned long	CallsCount;				//The time and date that the export data was created.
	unsigned short	MajorVersion;			//The major version number. The major and minor version numbers can be set by the user.
	unsigned short	MinorVersion;			//The minor version number.
	unsigned char*	DllName;				//The address of the ASCII string that contains the name of the DLL. This address is relative to the image base.
	unsigned long	OrdinalBase;			//The starting ordinal number for exports in this image. This field specifies the starting ordinal number for the export address table. It is usually set to 1.
	unsigned long	AddressTableEntries;	//The number of entries in the export address table.
	unsigned long	NumberOfNamePointers;	//The number of entries in the name pointer table. This is also the number of entries in the ordinal table.
	unsigned long*	ExportAddressTable;		//The address of the export address table, relative to the image base.
	unsigned char**	NamePointer;			//The address of the export name pointer table, relative to the image base. The table size is given by the Number of Name Pointers field.
	unsigned short*	OrdinalTable;			//The address of the ordinal table, relative to the image base.
};

typedef struct ApplicationHeader
{
	void (*exit)(ApplicationHeader * applicationHeader);
	int DllCount;
	_ExportDirectoryTable ** DllList;
};


enum
{
	PE_TYPE_UNDEFINED = 0,
	PE_TYPE_EXE,
	PE_TYPE_DLL
};


char * _wstr2str(char * str, wchar_t * wstr)
{
	char * _str = str;
	do
	{
		*str = *wstr;
		str++;
	}
	while(*wstr++);
	return _str;
}

wchar_t * _str2wstr(wchar_t * wstr, char * str)
{
	wchar_t * _wstr = wstr;
	do
	{
		*wstr = *str;
		wstr++;
	}
	while(*str++);
	return _wstr;
}

int _dllcmp(wchar_t * wstr, char * str)
{
	while(*wstr && *str && (*wstr == (wchar_t)*str))
	{
		wstr++;
		str++;
	}
	return *wstr - *str;
}

#ifndef _wintel
#define _IMB IMB
#pragma swi_number=0x108
__swi __arm void _SUBPROC( void (*proc)( void* ), ... );
#define _printf debug_printf
#define _fseek_set(__FILE__,__POS__) w_lseek((int)__FILE__, (int)__POS__, WSEEK_SET)
#define _fread(__FILE__,__MEM__,__SIZE__) w_fread((int)__FILE__, (void*)__MEM__, (int)__SIZE__)
#define _fclose(__FILE__) w_fclose((int)__FILE__)
int _fopen_rb(wchar_t * fname)
{
	return w_fopen(fname, WA_Read, 0x1FF, 0);
}
wchar_t * MakePath(wchar_t * fname)
{
	wchar_t * dir = GetDir(DIR_OTHER|MEM_EXTERNAL);
	wchar_t * res = new wchar_t[wstrlen(dir) + wstrlen(fname) + 4];
	wstrcpy(res, dir);
	wstrcat(res, L"/0/");
	wstrcat(res, fname);
	return res;
}
int _load_dc(int ** data, int * size)
{
	int f = 0;
	FSTAT _fstat;
	*size = 0;
	if (fstat( GetDir(DIR_ELFS_CONFIG | MEM_INTERNAL), L"DYN_CONST.bin", &_fstat)==0)
	{
		wchar_t * pp = new wchar_t[wstrlen( GetDir(DIR_ELFS_CONFIG | MEM_INTERNAL)) + wstrlen(L"/DYN_CONST.bin")+1];
		wstrcpy(pp, GetDir(DIR_ELFS_CONFIG | MEM_INTERNAL));
		wstrcat(pp, L"/DYN_CONST.bin");
		if ((f=_fopen_rb( pp))<0) return (-101);	//?? ??????????? DYN_CONST.
		delete pp;
		*size = _fstat.fsize;
		char * DATA = new char[_fstat.fsize];
		if ( _fread(f, DATA, _fstat.fsize) != _fstat.fsize)
		{
			delete(DATA);
			_fclose(f);
			return(0);
		}
		_fclose(f);
		return 0;
	}
	return 0;
}

#else
#define _IMB
#define _SUBPROC
#define wstrcmpi wcsicmp
#define wstrlen wcslen
#define wstrcpy wcscpy
#define wstrcat wcscat
#define _printf printf
#define _fseek_set(__FILE__,__POS__) fseek((FILE*)__FILE__, (int)__POS__, SEEK_SET)
#define _fseek_end(__FILE__,__POS__) fseek((FILE*)__FILE__, (int)__POS__, SEEK_END)
#define _fread(__FILE__,__MEM__,__SIZE__) fread( (void*)__MEM__, (int)__SIZE__, 1, (FILE*)__FILE__)
#define _fclose(__FILE__) fclose((FILE*)__FILE__)

int _fopen_rb(wchar_t * fname)
{
	char * s = new char[wstrlen(fname)+1];
	_wstr2str(s, fname);
	int f = (int)fopen(s, "rb");
	delete s;
	return f;
}
int _load_dc(int ** data, int * size)
{
	int f = _fopen_rb(L"DYN_CONST.bin");
	_fseek_end(f, 0);
	long l = ftell((FILE*)f);
	_fseek_set(f, 0);
	*size = l;
	char * DATA = new char[l];
	_fread(f, DATA, l);
	*data = (int*)DATA;
	_fclose(f);
	return 0;
}
#endif
int GetPeType(wchar_t * pename)
{
	int pos = wstrlen(pename)-4;
	pename += pos;
	if (!wstrcmpi(pename, L".exe"))
	{
		return PE_TYPE_EXE;
	}
	if (!wstrcmpi(pename, L".dll"))
	{
		return PE_TYPE_DLL;
	}
	return PE_TYPE_UNDEFINED;
}

#define MAX_DLL_COUNT 10
_ExportDirectoryTable ** GlobalDllList = 0;

void _ApplicationExit(ApplicationHeader * applicationHeader)
{
	int DllCount = applicationHeader->DllCount;
	_ExportDirectoryTable ** DllList = applicationHeader->DllList;
	if (DllList)
	{
		for (int i = 0; i < DllCount; i++)
		{
			_ExportDirectoryTable * Dll = DllList[i];
			if (!--Dll->CallsCount)
			{
				for (int j = 0; j < MAX_DLL_COUNT; j++)
				{
					if (GlobalDllList[j] == Dll)
					{
						GlobalDllList[j] = 0;
						break;
					}
				}
				_ApplicationExit((ApplicationHeader*)Dll->DllBase);
			}
		}
		delete DllList;
	}
	delete applicationHeader;
}

void KillMem(void * mem)
{
	delete mem;
}

void ApplicationExit(ApplicationHeader * applicationHeader)
{
	ApplicationHeader * _applicationHeader = new ApplicationHeader;
	memcpy(_applicationHeader, applicationHeader, sizeof(ApplicationHeader));
	_ApplicationExit(_applicationHeader);

	_SUBPROC(KillMem, applicationHeader);
}

int FindFunc(char ** list, int count, char * iName)
{
	int maxpos = count-1;
	int min = 0;
	int max = maxpos;
	int lastpos=-1;
	int pos;
	while (1)
	{
		//_printf("min %d max %d",min,max);
		pos = (max + min) / 2;
		if (pos == lastpos)
		{
			pos++;
			//_printf(" lastpos == pos ");
		}
		lastpos = pos;
		//_printf(" POS %d\n",pos);
		int res = strcmp((char*)list[pos], iName);
		if (!res)
		{
			//_printf("FOUND NAME: %s, POS %d\n",(char*)_exportDirectoryTable->NamePointer[pos],pos);
			return pos;
		}
		if (res < 0)
		{
			if (min == pos || pos == maxpos)
			{
				return -1;
			}
			min = pos;
			//_printf("min = pos %d\n", pos);
		}
		else
		{
			if (max == pos || pos == 0)
			{
				return -1;
			}
			max = pos;
			//_printf("max = pos %d\n", pos);
		}
	}
}
int LoadPE(wchar_t * pename, _ExportDirectoryTable ** out_etable, int * out_epoint)
{
	_printf("--------------------\n");
	_printf("LOADING:\t%ls\n", pename);
	int PeType = GetPeType(pename);
	_printf("PE TYPE:\t%s\n", (PeType==PE_TYPE_EXE)?"exe":(PeType==PE_TYPE_DLL)?"dll":"unk");
	if (PeType == PE_TYPE_UNDEFINED)
	{
		return -1;
	}
	if (PeType == PE_TYPE_DLL)
	{
		for (int j = 0; j < MAX_DLL_COUNT; j++)
		{
			if (GlobalDllList[j])
			{
				if (!_dllcmp(pename, (char*)GlobalDllList[j]->DllName))
				{
					GlobalDllList[j]->CallsCount++;
					*out_etable = GlobalDllList[j];
					_printf("\n!!!!!!!!!!!!!!   DLL FOUND\n\n");
					return 0;
				}
			}
		}
#ifndef _wintel
		pename = MakePath(pename);
#endif
	}
	int f = _fopen_rb(pename);
	mz_hdr * mz = new mz_hdr;
	_fread(f, mz, sizeof(mz_hdr));
	_fseek_set(f, mz->mz_pe_header);
	FileHeader * pe = new FileHeader;
	_fread(f,pe, sizeof(FileHeader));
	_printf("CPU TYPE:\t0x%X\n",pe->Machine);
	_printf("OBJ NUM:\t%d\n",pe->NumberOfSections);

	_fseek_set(f, (int)0x178);
	SectionTable ** oheaders = new SectionTable*[scount];
	SectionTable * oh = (SectionTable *)malloc(sizeof(SectionTable)*pe->NumberOfSections);
	_fread(f,oh, sizeof(SectionTable)*pe->NumberOfSections);
	for (int i = 0; i < scount; i++)
	{
		oheaders[i] = 0;
		for (int j = 0; j < pe->NumberOfSections; j++)
		{
			SectionTable * _oh = oh+j;
			if (!strncmp((char*)_oh->Name, slist[i].name, 8))
			{
				oheaders[i] = _oh;
				break;
			}
		}
	}

	_printf("OBJECT HEADER LIST:\n");
	for (int i = 0; i < scount; i++)
	{
		if (oheaders[i])
			_printf("%s\t%X\t%X\t%X\t%X\n",oheaders[i]->Name,oheaders[i]->VirtualSize,oheaders[i]->VirtualAddress,oheaders[i]->PointerToRawData);
		else
			_printf("%s\tUNDEFINED\n", slist[i].name);
	}
	int import_count = 0;
	int import_off = 0;
	int import_poff = 0;
	int import_voff = 0;
	int mem_size = oheaders[text]->VirtualSize;
	_ExportDirectoryTable ** iDllList = 0;
	int iDllCount = 0;
	if (oheaders[idata])
	{
		_printf("\nIDATA SEGMENT:\n");
		char * IDATA = new char[oheaders[idata]->VirtualSize];
		_fseek_set(f, oheaders[idata]->PointerToRawData);
		_fread(f,IDATA, oheaders[idata]->VirtualSize);
		int _iDllCount = (*((int*)IDATA)-oheaders[idata]->VirtualAddress) / sizeof(ImportDirectoryTable);
		while((((ImportDirectoryTable*)IDATA)+iDllCount)->NameRVA)
		{
			if (++iDllCount > _iDllCount)
			{
				_printf("ERROR COUNT ImportDirectoryTable\n");
				return -1;
				break;
			}
		}

		if (!iDllCount)
		{
			oheaders[idata] = 0;
			goto QWE;
		}

		iDllList = new _ExportDirectoryTable*[iDllCount+1];
		iDllList[iDllCount] = 0;

		_printf("DLL COUNT:\t%d\n", iDllCount);
		import_off = mem_size;
		import_voff = *((int*)IDATA+4);
		import_poff = *((int*)IDATA+4)-oheaders[idata]->VirtualAddress + (int)IDATA;
		for (int j = 0; j < iDllCount; j++)
		{
			unsigned long idata_off = (int)IDATA - oheaders[idata]->VirtualAddress;
			ImportDirectoryTable * importDirectoryTable = ((ImportDirectoryTable*)IDATA)+j;
			importDirectoryTable->NameRVA += idata_off;
			importDirectoryTable->ImportLookupTableRVA += idata_off;
			importDirectoryTable->ImportAddressTableRVA += idata_off;

			_ImportDirectoryTable * _importDirectoryTable = (_ImportDirectoryTable*)importDirectoryTable;

			_printf("%d\t%s\n",j+1, _importDirectoryTable->DllName);

			_ExportDirectoryTable * _exportDirectoryTable=0;
			char * dllname = (char*)_importDirectoryTable->DllName;
			wchar_t * wdllname = new wchar_t[strlen(dllname)+1];
			_str2wstr(wdllname,dllname);
			LoadPE(wdllname, &_exportDirectoryTable, 0);

			iDllList[j] = _exportDirectoryTable;

			ImportLookup ** importLookupTable = _importDirectoryTable->ImportLookupTable;
			unsigned long * importAddressTable = _importDirectoryTable->ImportAddressTable;
			unsigned long NumberOfNamePointers = _exportDirectoryTable->NumberOfNamePointers;

			char** namePointer = (char**)_exportDirectoryTable->NamePointer;

			for (int i = 0; importAddressTable[i] != 0; i++, import_count++)
			{
				importLookupTable[i] = (ImportLookup*)((unsigned long)importLookupTable[i] + idata_off);
				int pos = FindFunc(namePointer,
					NumberOfNamePointers,
					(char*)&importLookupTable[i]->Name);
				if (pos == -1)
				{
					_printf("ERROR FUNCTION NOT FOUND: NAME: %s ORDINAL: %hd\n", (char*)&importLookupTable[i]->Name, importLookupTable[i]->Ordinal);
				}
				else
				{
					unsigned short iOrdinal = _exportDirectoryTable->OrdinalTable[pos];
					importAddressTable[i] = _exportDirectoryTable->ExportAddressTable[iOrdinal];
				}
				_printf("%hd\t0x%X\t%s\n",
					importLookupTable[i]->Ordinal,
					importAddressTable[i],
					&importLookupTable[i]->Name);
			}
		}
		_printf("IMPORTS COUNT:\t%d\n", import_count);
		import_count+= iDllCount;
		mem_size += import_count*4;
	}
QWE:
	_printf("\nTEXT SEGMENT:\n");
	char * MEMORY = new char[mem_size];
	_printf("MEMORY:\t%X\n", MEMORY);
	_fseek_set(f, oheaders[text]->PointerToRawData);
	_fread(f,MEMORY, oheaders[text]->VirtualSize);
	if (oheaders[idata])
	{
		memcpy((void*)((int)MEMORY+import_off), (void*)import_poff, import_count*4);
		import_voff = (int)MEMORY - import_voff + import_off;
		//delete IDATA;
	}
	ApplicationHeader * applicationHeader = (ApplicationHeader*)MEMORY;
	applicationHeader->exit = ApplicationExit;
	applicationHeader->DllCount = iDllCount;
	applicationHeader->DllList = iDllList;


	if (oheaders[reloc])
	{
		_printf("\nRELOC SEGMENT:\n");
		char * RELOC_TABLE = new char[oheaders[reloc]->VirtualSize];
		_fseek_set(f, oheaders[reloc]->PointerToRawData);
		_fread(f,RELOC_TABLE, oheaders[reloc]->VirtualSize);
		int * DYN_CONST = 0;
		int  DYN_SIZE = 0;
		int move = 0;
		while(move < oheaders[reloc]->VirtualSize)
		{
			BaseRelocationBlock * baseRelocationBlock = (BaseRelocationBlock*)(RELOC_TABLE+move);
			if (baseRelocationBlock->PageRVA != oheaders[text]->VirtualAddress)
			{
				continue;
			}
			unsigned short * fields = (unsigned short *) (RELOC_TABLE + move + sizeof(BaseRelocationBlock));
			int fields_count = (baseRelocationBlock->BlockSize - sizeof(BaseRelocationBlock))/ 2;
			move += baseRelocationBlock->BlockSize;
			for (int i = 0; i < fields_count; i++)
			{
				int * field = (int*)(MEMORY + (fields[i] & 0xFFF));
				int _field = *field & 0xFFF00000;
				if (_field == oheaders[text]->VirtualAddress)
				{
					*field += ((int)MEMORY - oheaders[text]->VirtualAddress);
					_printf("CODE\n");
					//_printf("0x%X\n",oheaders[text]->VirtualAddress);
				}
				else if (oheaders[dconst] && _field == oheaders[dconst]->VirtualAddress)
				{
					if (!DYN_CONST)
					{
						_load_dc(&DYN_CONST, &DYN_SIZE);
					}
					_printf("CONST\n");
					int pos = *field - oheaders[dconst]->VirtualAddress;
					if (pos < DYN_SIZE)
					{
						*field = DYN_CONST[pos>>2];
					}
					else
					{
						*field = 0;
					}

				}
				else if (oheaders[idata] && _field == oheaders[idata]->VirtualAddress)
				{
					*field +=import_voff;
					_printf("IMPORT\n");
				}
			}
		}
		if (DYN_CONST)
		{
			delete DYN_CONST;
		}
	}
	if (PeType == PE_TYPE_DLL && oheaders[edata])
	{
		_printf("\nEDATA SEGMENT:\n");
		char * EDATA = new char[oheaders[edata]->VirtualSize];
		_fseek_set(f, oheaders[edata]->PointerToRawData);
		_fread(f,EDATA, oheaders[edata]->VirtualSize);


		unsigned long edata_off = (int)EDATA - oheaders[edata]->VirtualAddress;

		ExportDirectoryTable * exportDirectoryTable = (ExportDirectoryTable*)EDATA;
		exportDirectoryTable->NameRVA += edata_off;
		exportDirectoryTable->ExportAddressTableRVA += edata_off;
		exportDirectoryTable->NamePointerRVA += edata_off;
		exportDirectoryTable->OrdinalTableRVA += edata_off;

		int count = exportDirectoryTable->AddressTableEntries;

		_ExportDirectoryTable * _exportDirectoryTable = (_ExportDirectoryTable*)exportDirectoryTable;
		_exportDirectoryTable->DllBase = (int)MEMORY;

		_printf("DLL NAME:\t%s\n", _exportDirectoryTable->DllName);
		_printf("DLL BASE:\t%X\n", _exportDirectoryTable->DllBase);

		_printf("EXPORTS COUNT:\t%d\n", count);
		for (int i = 0; i < count; i++)
		{
			if (_exportDirectoryTable->ExportAddressTable[i])
			{
				_exportDirectoryTable->ExportAddressTable[i] += ((int)MEMORY - oheaders[text]->VirtualAddress);
			}
		}
		for (int i = 0; i < _exportDirectoryTable->NumberOfNamePointers; i++)
		{
			if (_exportDirectoryTable->NamePointer[i])
			{
				_exportDirectoryTable->NamePointer[i] += edata_off;
				_printf("%s\n", _exportDirectoryTable->NamePointer[i]);
			}
		}
		if (out_etable)
		{
			_exportDirectoryTable->CallsCount = 1;
			for (int i = 0; i < MAX_DLL_COUNT; i++)
			{
				if (!GlobalDllList[i])
				{
					GlobalDllList[i] = _exportDirectoryTable;
					break;
				}
			}
			*out_etable = _exportDirectoryTable;
		}
	}
	/*
	char fname[20];
	sprintf(fname, "%X", MEMORY);
	FILE * ff = fopen(fname,"wb");
	fwrite(MEMORY, mem_size, 1, ff);
	fclose(ff);
	*/
	int epoint = pe->AddressOfEntryPoint + (int)MEMORY - oheaders[text]->VirtualAddress;
	if (out_epoint)
	{
		*out_epoint = epoint;
	}
	delete oh;
	//delete MEMORY;
	_fclose(f);
	_printf("--------------------\n");
	return 0;
}

#ifndef _wintel

int main()
{
	GlobalDllList = new _ExportDirectoryTable*[MAX_DLL_COUNT];
	for (int i = 0; i < MAX_DLL_COUNT; i++)
	{
		GlobalDllList[i] = 0;
	}
	int e;
	wchar_t * pp = MakePath(L"test_exe.exe");
	LoadPE(pp, 0, &e);
	IMB();
	((void(*)())e)();
	/*LoadPE(pp, 0, &e);
	IMB();
	((void(*)())e)();
	LoadPE(pp, 0, &e);
	IMB();
	((void(*)())e)();
	LoadPE(pp, 0, &e);
	IMB();
	((void(*)())e)();*/
	//int f = _fopen(L"/card/MSSEMC/Media files/other/0/test_exe.exe", WA_Read, 0x1FF, 0);
	//delete GlobalDllList;
	//SUBPROC(elf_exit);
	return(0);
}
#else
int main()
{
	GlobalDllList = new _ExportDirectoryTable*[MAX_DLL_COUNT];
	for (int i = 0; i < MAX_DLL_COUNT; i++)
	{
		GlobalDllList[i] = 0;
	}
	int e;
	LoadPE(L"test_exe.exe", 0, &e);
	LoadPE(L"test_exe.exe", 0, &e);
	printf("0x%X\n",e);
	//LoadPE("libb_mingw1.dll", 0, 0, 0);
	delete GlobalDllList;
	system("pause");
	return 0;
}
#endif

