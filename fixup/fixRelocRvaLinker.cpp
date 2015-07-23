//---------------------------------------------------------------------------------------------
#include "fixRelocRvaLinker.h"
//---------------------------------------------------------------------------------------------
bool fixRelocRvaLinker::fixAssemblyLinker(unsigned char* pimagebase)
{
	if(!peFuzzLinker::setPeMemoryImage(pimagebase))
	{
		return false;
	}
	else
	{
		fixRelocRvaLinker::fixImportTable();
		fixRelocRvaLinker::fixResourceTable();
 		fixRelocRvaLinker::fixRelocTable();
		fixRelocRvaLinker::fixDebugTable();
		fixRelocRvaLinker::fixIatTable();
		fixRelocRvaLinker::fixSectionTable();
		return (fixRelocRvaLinker::fixEntryPoint());
	}
}