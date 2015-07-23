//---------------------------------------------------------------------------------------------
#include "main.h"
#include "x86FixupAvoidKill.h"
//---------------------------------------------------------------------------------------------
int x86FixupScanfAvoidKill(void)
{
	x86FixupAvoidKill fuzz;
	wchar_t pimagepath[MAX_PATH] = {0};
	wprintf(L"fill path:");
	wscanf(L"%s",pimagepath);
	if(!fuzz.x86FixupFullTarget(pimagepath))
	{
		return EXIT_FAILURE;
	}
	else
	{
		if(!fuzz.x86FixupFuzzAssembly())
		{
			return EXIT_FAILURE;
		}
		else
		{
			fuzz.x86FixupGenFuzzResult();
			fuzz.logFuzzme("fuzz ok!");
			return EXIT_SUCCESS;
 		}
	}
}
//---------------------------------------------------------------------------------------------
int wmain(int argc,wchar_t* argv[])
{
	return (x86FixupScanfAvoidKill());
}
