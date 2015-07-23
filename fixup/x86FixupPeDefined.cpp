//---------------------------------------------------------------------------------------------
#include "x86FixupPeDefined.h"
//---------------------------------------------------------------------------------------------
bool x86FixupPeDefined::setPeMemoryImage(unsigned char* pimagebase)
{
	if(*(unsigned short*)pimagebase == IMAGE_DOS_SIGNATURE)
	{
		x86FixupPeDefined::_dos_header = (PIMAGE_DOS_HEADER)pimagebase;
		unsigned char *_uchar_base = pimagebase;
		for(char index = 0; index < 0x200; index++)
		{
			if(*(unsigned long*)_uchar_base == IMAGE_NT_SIGNATURE)
			{
				x86FixupPeDefined::_nt32_header = (PIMAGE_NT_HEADERS32) _uchar_base;
				x86FixupPeDefined::_file_header = (PIMAGE_FILE_HEADER) &_nt32_header->FileHeader;
				x86FixupPeDefined::_optional32_header = (PIMAGE_OPTIONAL_HEADER32) &(x86FixupPeDefined::_nt32_header->OptionalHeader);
				x86FixupPeDefined::_section_header = (PIMAGE_SECTION_HEADER)((unsigned long)x86FixupPeDefined::_nt32_header+sizeof(IMAGE_OPTIONAL_HEADER32)+ 
											sizeof(IMAGE_FILE_HEADER)+sizeof(unsigned long));
				return true;
			}
			_uchar_base++;
		}
		return false;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getBaseAddress()
{
	return (reinterpret_cast<unsigned long>(x86FixupPeDefined::_dos_header));
}
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getRvaImageBaseMax()
{
	return (x86FixupPeDefined::_optional32_header->ImageBase + x86FixupPeDefined::_optional32_header->SizeOfImage);
}
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getRvaImageBaseMin()
{
	return (x86FixupPeDefined::_optional32_header->ImageBase);
}
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getNumberOfSections()
{
	return (x86FixupPeDefined::_file_header->NumberOfSections);
}
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getSectionAlignment()
{
	return (x86FixupPeDefined::_optional32_header->SectionAlignment);
}
//---------------------------------------------------------------------------------------------
unsigned long x86FixupPeDefined::getFileAlignment()
{
	return (x86FixupPeDefined::_optional32_header->FileAlignment);
}
//---------------------------------------------------------------------------------------------
void* x86FixupPeDefined::getDosHeader()
{
	return (x86FixupPeDefined::_dos_header);
}
//---------------------------------------------------------------------------------------------
void* x86FixupPeDefined::getNtHeader()
{
	return (x86FixupPeDefined::_nt32_header);
}
//---------------------------------------------------------------------------------------------
void* x86FixupPeDefined::getFileHeader()
{
	return (x86FixupPeDefined::_file_header);
}
//---------------------------------------------------------------------------------------------
void* x86FixupPeDefined::getOptionHeader()
{
	return (x86FixupPeDefined::_optional32_header);
}
//---------------------------------------------------------------------------------------------
void* x86FixupPeDefined::getSectionHeader()
{
	return (x86FixupPeDefined::_section_header);
}
//---------------------------------------------------------------------------------------------
size_t x86FixupPeDefined::alignmentCalc(size_t nsize, size_t nvalue)
{
	if((nsize % nvalue) != 0)
		return  (nsize / nvalue + 1) * nvalue;
	return nsize;
}