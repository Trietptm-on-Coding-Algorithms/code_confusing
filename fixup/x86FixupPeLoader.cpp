//---------------------------------------------------------------------------------------------
#include "x86FixupPeLoader.h"
//---------------------------------------------------------------------------------------------
bool x86FixupPeLoader::fixEmulationPeLoader(unsigned char* pimagebase)
{
	if(!x86FixupPeDefined::setPeMemoryImage(pimagebase))
	{
		return false;
	}
	else
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
		unsigned long nimagesize = pntheader->OptionalHeader.SizeOfImage;
		unsigned char* pimagememory = new unsigned char[nimagesize];
		if(pimagememory==NULL)
		{
			return false;
		}
		else
		{
			unsigned long noldprotect = 0;
			VirtualProtect(pimagememory,nimagesize,PAGE_EXECUTE_READWRITE,&noldprotect);
			memset(pimagememory,0,nimagesize);
			memcpy(pimagememory,pimagebase,pntheader->OptionalHeader.SizeOfHeaders);
		}
		x86FixupPeLoader::fixFullSectionTable(pimagememory,pimagebase);
		if(!x86FixupPeLoader::fixFullImportTable(pimagememory,pimagebase))
		{
			return false;
		}
		else
		{
			x86FixupPeLoader::fixFullRelocTable(pimagememory,pimagebase);
			if((pntheader->FileHeader.Characteristics&IMAGE_FILE_DLL))
			{
				x86FixupPeLoader::fixLoaderDllMain(pimagememory);
			}
			else if((pntheader->FileHeader.Characteristics&IMAGE_FILE_EXECUTABLE_IMAGE))
			{
				if(pntheader->OptionalHeader.Subsystem&IMAGE_SUBSYSTEM_WINDOWS_GUI)
				{
					x86FixupPeLoader::fixLoaderWinMain(pimagememory);
				}
				else if(pntheader->OptionalHeader.Subsystem&IMAGE_SUBSYSTEM_WINDOWS_CUI)
				{
					x86FixupPeLoader::fixLoaderConsole(pimagememory);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupPeLoader::fixFullSectionTable(unsigned char* pimagememory,unsigned char* pimagebase)
{
	PIMAGE_SECTION_HEADER psection = static_cast<PIMAGE_SECTION_HEADER>(x86FixupPeDefined::getSectionHeader());
	for(unsigned long nindex=0;nindex<x86FixupPeDefined::getNumberOfSections();nindex++)
	{
		memcpy(pimagememory+psection[nindex].VirtualAddress,pimagebase+psection[nindex].PointerToRawData,psection[nindex].SizeOfRawData);
	}
}
//---------------------------------------------------------------------------------------------
bool x86FixupPeLoader::fixFullImportTable(unsigned char* pimagememory,unsigned char* pimagebase)
{
	PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
	if(pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size>0)
	{
		unsigned long nimportva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		PIMAGE_IMPORT_DESCRIPTOR pIID = (PIMAGE_IMPORT_DESCRIPTOR)(pimagememory+nimportva);
		for(;pIID->Name!=NULL;pIID++)
		{
			PIMAGE_THUNK_DATA pthunk = (IMAGE_THUNK_DATA *)(pimagememory+pIID->FirstThunk);
			HMODULE hmod = LoadLibraryA((char*)(pimagememory+pIID->Name));
			if(hmod == NULL)
			{
				delete pimagememory;
				return false;
			}
			for(;pthunk->u1.Ordinal!=0;pthunk++)
			{
				FARPROC fpFun;
				if(pthunk->u1.Ordinal & IMAGE_ORDINAL_FLAG32)
				{
					fpFun = GetProcAddress(hmod,(LPCSTR)(pthunk->u1.Ordinal & 0x0000ffff));
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME pIIBN = (PIMAGE_IMPORT_BY_NAME)(pimagememory+pthunk->u1.Ordinal);
					fpFun = GetProcAddress(hmod,(LPCSTR)pIIBN->Name);
				}
				if(fpFun == NULL)
				{
					delete pimagememory;
					return false;
				}
				pthunk->u1.Ordinal = (unsigned long)fpFun;
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------------------
void x86FixupPeLoader::fixFullRelocTable(unsigned char* pimagememory,unsigned char* pimagebase)
{
	PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
	if(pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size>0)
	{
		unsigned long nrelocva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
		PIMAGE_BASE_RELOCATION preloc = (PIMAGE_BASE_RELOCATION)(pimagememory+nrelocva);
		unsigned long ndifference = (unsigned long)pimagememory-pntheader->OptionalHeader.ImageBase;
		while(preloc->VirtualAddress!=0)
		{
			char *lpMemPage = reinterpret_cast<char*>(pimagememory+preloc->VirtualAddress);
			unsigned short *prelocitem = (unsigned short*)((char *)preloc+sizeof(IMAGE_BASE_RELOCATION));
			unsigned long nitemnum = (preloc->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))>>1;
			for(unsigned long nindex=0;nindex<nitemnum;nindex++)
			{
				if(((prelocitem[nindex]&0xF000)>>12)==IMAGE_REL_BASED_HIGHLOW)
				{
					*(unsigned long*)(lpMemPage+(prelocitem[nindex]&0x0FFF))+=ndifference;
				}
			}
			preloc=(PIMAGE_BASE_RELOCATION)(prelocitem+nitemnum);
		}
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupPeLoader::fixLoaderDllMain(unsigned char* pimagebase)
{
	PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
	unsigned long noeprva = pntheader->OptionalHeader.AddressOfEntryPoint;
	x86FixupPeLoader::LOADERDLLMAIN LoaderDllMain= (x86FixupPeLoader::LOADERDLLMAIN)(noeprva+(unsigned long)pimagebase);
	_asm pushad;
	LoaderDllMain(pimagebase,DLL_PROCESS_ATTACH,NULL);
	_asm popad;
}
//---------------------------------------------------------------------------------------------
void x86FixupPeLoader::fixLoaderWinMain(unsigned char* pimagebase)
{
	PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
	unsigned long noeprva = pntheader->OptionalHeader.AddressOfEntryPoint;
	x86FixupPeLoader::LOADERWINMAIN LoaderWinMain= (x86FixupPeLoader::LOADERWINMAIN)(noeprva+(unsigned long)pimagebase);
	_asm pushad;
	LoaderWinMain(pimagebase,NULL,NULL,SW_SHOW);
	_asm popad;
}
//---------------------------------------------------------------------------------------------
void x86FixupPeLoader::fixLoaderConsole(unsigned char* pimagebase)
{
	PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(x86FixupPeDefined::getNtHeader());
	unsigned long noeprva = pntheader->OptionalHeader.AddressOfEntryPoint;
	x86FixupPeLoader::LOADERCONSOLE LoaderConsole= (x86FixupPeLoader::LOADERCONSOLE)(noeprva+(unsigned long)pimagebase);
	_asm pushad;
	LoaderConsole(0,NULL);
	_asm popad;
}