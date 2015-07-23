#ifndef X86FIXUPPELOADER_H
#define X86FIXUPPELOADER_H
#include "main.h"
#include "x86FixupPeDefined.h"
//---------------------------------------------------------------------------------------------
class x86FixupPeLoader:public x86FixupPeDefined
{
public:
	bool fixEmulationPeLoader(unsigned char* pimagebase);
private:
	void fixFullSectionTable(unsigned char* pimagememory,unsigned char* pimagebase);
	bool fixFullImportTable(unsigned char* pimagememory,unsigned char* pimagebase);
	void fixFullRelocTable(unsigned char* pimagememory,unsigned char* pimagebase);
	void fixLoaderDllMain(unsigned char* pimagebase);
	void fixLoaderWinMain(unsigned char* pimagebase);
	void fixLoaderConsole(unsigned char* pimagebase);
private:
	//
	typedef BOOL (_stdcall* LOADERDLLMAIN)(void* _HDllHandle,unsigned _Reason,void* _Reserved);
	typedef BOOL (_stdcall* LOADERWINMAIN)(void* hInstance,void* hPrevInstance,char* lpCmdLine,int nShowCmd);
	typedef BOOL (_stdcall* LOADERCONSOLE)(int argc,wchar_t** argv);
};
#endif
