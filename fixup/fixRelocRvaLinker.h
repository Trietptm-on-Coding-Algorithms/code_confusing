//---------------------------------------------------------------------------------------------
#ifndef FIXRELOCRVALINKER_H
#define FIXRELOCRVALINKER_H
#include "main.h"
#include "peFuzzLinker.h"
//---------------------------------------------------------------------------------------------
class fixRelocRvaLinker:public peFuzzLinker
{
public:
	bool fixAssemblyLinker(unsigned char* pimagebase);
private:
	//---------------------------------------------------------------------------------------------
	bool fixEntryPoint()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		size_t nfirstrvaoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t noepoffset = fixRelocRvaLinker::getSectionOffsetByRav(pntheader->OptionalHeader.AddressOfEntryPoint);
		pntheader->OptionalHeader.AddressOfEntryPoint -= noepoffset;
		pntheader->OptionalHeader.AddressOfEntryPoint += nfirstrvaoffset;
		return true;
	}
	//---------------------------------------------------------------------------------------------
	bool fixImportTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long nimportrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		if(!nimportrva)
		{
			return false;
		}
		size_t nfirstrvaoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t nimportrvaoffset = fixRelocRvaLinker::getSectionOffsetByRav(nimportrva);
		if(nimportrvaoffset>0)
		{
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress -= nimportrvaoffset;
			nimportrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress += nfirstrvaoffset;
			PIMAGE_IMPORT_DESCRIPTOR pimportdesc = (PIMAGE_IMPORT_DESCRIPTOR)(nimportrva+peFuzzLinker::getBaseAddress());
			for(unsigned long nindex=0;pimportdesc[nindex].Name!=0;nindex++)
			{
				pimportdesc[nindex].FirstThunk -= nimportrvaoffset;
				unsigned long* pthunk = (unsigned long*)(pimportdesc[nindex].FirstThunk+peFuzzLinker::getBaseAddress());
				while(*pthunk!=0)
				{
					if(*pthunk>0x7fffffff)
					{
						pthunk++;
					}
					else
					{
						*pthunk -= nimportrvaoffset;
						*pthunk += nfirstrvaoffset;
						pthunk++;
					}
				}
				pimportdesc[nindex].FirstThunk += nfirstrvaoffset;
				pimportdesc[nindex].TimeDateStamp  = 0;
				pimportdesc[nindex].ForwarderChain = 0;
				pimportdesc[nindex].OriginalFirstThunk -= nimportrvaoffset;
				pthunk = (unsigned long*)(pimportdesc[nindex].OriginalFirstThunk+peFuzzLinker::getBaseAddress());
				pimportdesc[nindex].OriginalFirstThunk += nfirstrvaoffset;
				while(*pthunk!=0)
				{
					if(*pthunk>0x7fffffff)
					{
						pthunk++;
					}
					else
					{
						*pthunk -= nimportrvaoffset;
						*pthunk += nfirstrvaoffset;
						pthunk++;
					}
				}
				pimportdesc[nindex].Name -= nimportrvaoffset;
				pimportdesc[nindex].Name += nfirstrvaoffset;
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------
	bool fixResourceTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long nresrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
		if(!nresrva)
		{
			return false;
		}
		size_t nfirstrvaoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t nresrvaoffset = fixRelocRvaLinker::getSectionOffsetByRav(nresrva);
		if(nresrvaoffset>0)
		{
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress -= nresrvaoffset;
			PIMAGE_RESOURCE_DIRECTORY presdir = (PIMAGE_RESOURCE_DIRECTORY)(
				pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress+peFuzzLinker::getBaseAddress());
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress += nfirstrvaoffset;
			PIMAGE_RESOURCE_DIRECTORY_ENTRY presentry=(PIMAGE_RESOURCE_DIRECTORY_ENTRY)((unsigned long)presdir+sizeof(IMAGE_RESOURCE_DIRECTORY));
			PIMAGE_RESOURCE_DIRECTORY dirTemp;
			PIMAGE_RESOURCE_DIRECTORY_ENTRY entryTemp;
			for(int i=0;i<(presdir->NumberOfIdEntries+presdir->NumberOfNamedEntries);i++,presentry++)
			{
				dirTemp=(PIMAGE_RESOURCE_DIRECTORY)((char *)presdir+presentry->OffsetToDirectory);
				entryTemp=(PIMAGE_RESOURCE_DIRECTORY_ENTRY)((char *)dirTemp+sizeof(IMAGE_RESOURCE_DIRECTORY));
				for(int k=0;k<(dirTemp->NumberOfIdEntries+dirTemp->NumberOfNamedEntries);k++,entryTemp++)
				{
					if(entryTemp->DataIsDirectory>0)
					{
						PIMAGE_RESOURCE_DIRECTORY dirTempICON=(PIMAGE_RESOURCE_DIRECTORY)((char *)presdir + entryTemp->OffsetToDirectory);
						PIMAGE_RESOURCE_DIRECTORY_ENTRY pentry1=(PIMAGE_RESOURCE_DIRECTORY_ENTRY)((char*)dirTempICON+sizeof(IMAGE_RESOURCE_DIRECTORY));
						PIMAGE_RESOURCE_DATA_ENTRY presdata=(PIMAGE_RESOURCE_DATA_ENTRY)((char *)presdir+pentry1->OffsetToData);
						presdata->OffsetToData -= fixRelocRvaLinker::getSectionOffsetByRav(nresrva);
						presdata->OffsetToData += nfirstrvaoffset;
					}
				}
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------
	bool fixRelocTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long nrelocrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
		if(!nrelocrva)
		{
			return false;
		}
		size_t nfirstoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t nrelocrvaoffset = fixRelocRvaLinker::getSectionOffsetByRav(nrelocrva);
		if(nrelocrvaoffset>0)
		{
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress -= nrelocrvaoffset;
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress += nfirstoffset;
			PIMAGE_BASE_RELOCATION prelocrva = (PIMAGE_BASE_RELOCATION)(peFuzzLinker::getBaseAddress()+fixRelocRvaLinker::fixRvaToOffset(nrelocrva));
			while(prelocrva->VirtualAddress!=0)
			{
				prelocrva->VirtualAddress -= fixRelocRvaLinker::getSectionOffsetByRav(prelocrva->VirtualAddress);
				prelocrva->VirtualAddress += nfirstoffset;
				unsigned short* prelocitem = (unsigned short*)(&prelocrva[1]);
				unsigned long nitemnum = ((prelocrva->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/sizeof(unsigned short));
				for(unsigned long nindex=0;nindex<nitemnum;nindex++,prelocitem++)
				{
// 					if(((*prelocitem&0xF000)>>12)!=IMAGE_REL_BASED_HIGHLOW)
// 					{
// 						continue;
// 					}
// 					unsigned long* prelocdata = (unsigned long*)((prelocrva->VirtualAddress+(*prelocitem&0x0FFF))-nfirstoffset+peFuzzLinker::getBaseAddress());
// 					*prelocdata -= pntheader->OptionalHeader.ImageBase;
// 					*prelocdata -= fixRelocRvaLinker::getSectionOffsetByRav(*prelocdata);
// 					*prelocdata += nfirstoffset;
// 					*prelocdata += pntheader->OptionalHeader.ImageBase;
				}
				prelocrva = (PIMAGE_BASE_RELOCATION)prelocitem;
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------
	bool fixDebugTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long ndebugrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress;
		if(!ndebugrva)
		{
			return false;
		}
		size_t nfirstrvaoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t ndebugrvaoffset = fixRelocRvaLinker::getSectionOffsetByRav(ndebugrva);
		if(ndebugrvaoffset>0)
		{
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress -= ndebugrvaoffset;
			ndebugrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress;
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress += nfirstrvaoffset;
			PIMAGE_DEBUG_DIRECTORY pdebugdir = (PIMAGE_DEBUG_DIRECTORY)(ndebugrva+peFuzzLinker::getBaseAddress());
			unsigned long ndebugsize = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size;
			for(unsigned nindex = 0;ndebugsize>0;nindex++)
			{
					pdebugdir[nindex].AddressOfRawData -= ndebugrvaoffset;
					pdebugdir[nindex].AddressOfRawData += nfirstrvaoffset;
					ndebugsize -= sizeof(IMAGE_DEBUG_DIRECTORY);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	//---------------------------------------------------------------------------------------------
	bool fixIatTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long niatrva = pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
		if(!niatrva)
		{
			return false;
		}
		size_t nfirstrvaoffset = fixRelocRvaLinker::getFirstSectionOffset();
		size_t niatrvaoffset = fixRelocRvaLinker::getSectionOffsetByRav(niatrva);
		if(niatrvaoffset>0)
		{
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress -= niatrvaoffset;
			pntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress += nfirstrvaoffset;
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------
	bool fixSectionTable()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		PIMAGE_SECTION_HEADER psectionptr = static_cast<PIMAGE_SECTION_HEADER>(peFuzzLinker::getSectionHeader());
		if(pntheader->FileHeader.NumberOfSections<2)
		{
			return false;
		}
		else
		{
			unsigned long nlastrvasize = psectionptr[peFuzzLinker::getNumberOfSections()-1].Misc.VirtualSize;
			unsigned long nlastrvaoffset = psectionptr[peFuzzLinker::getNumberOfSections()-1].VirtualAddress;
			unsigned long nlastrawsize = psectionptr[peFuzzLinker::getNumberOfSections()-1].SizeOfRawData;
			unsigned long nlastrawoffset = psectionptr[peFuzzLinker::getNumberOfSections()-1].PointerToRawData;
			unsigned long nsectionalign = peFuzzLinker::getSectionAlignment();
			unsigned long nfilealign = peFuzzLinker::getFileAlignment();
			unsigned long nheadersize = pntheader->OptionalHeader.SizeOfHeaders;
			memset(psectionptr[0].Name,0,8);
			strncpy((char*)psectionptr[0].Name,".upx0",5);
			psectionptr[0].Characteristics = 0xE0000040;
			psectionptr[0].VirtualAddress = peFuzzLinker::alignmentCalc(nheadersize,nsectionalign);
			psectionptr[0].PointerToRawData = nheadersize;
			psectionptr[0].Misc.VirtualSize = peFuzzLinker::alignmentCalc(nlastrvasize+nlastrvaoffset,nsectionalign);
			psectionptr[0].SizeOfRawData = (peFuzzLinker::alignmentCalc(nlastrawsize+nlastrawoffset,nfilealign)-nheadersize);
			for(unsigned long i=1;i<peFuzzLinker::getNumberOfSections();i++)
			{
				memset(&psectionptr[i],0,sizeof(IMAGE_SECTION_HEADER));
			}
			strncpy((char*)psectionptr[1].Name,".upx1",5);
			psectionptr[1].Characteristics = 0xE0000040;
			psectionptr[1].VirtualAddress = peFuzzLinker::alignmentCalc(psectionptr[0].VirtualAddress+psectionptr[0].Misc.VirtualSize,nsectionalign);
			psectionptr[1].Misc.VirtualSize = 0x50000;
			psectionptr[1].PointerToRawData = peFuzzLinker::alignmentCalc(psectionptr[0].PointerToRawData+psectionptr[0].SizeOfRawData,nfilealign);
			psectionptr[1].SizeOfRawData = 0x50000;
			pntheader->FileHeader.NumberOfSections = 2;
			pntheader->OptionalHeader.SizeOfImage = peFuzzLinker::alignmentCalc(psectionptr[1].VirtualAddress+psectionptr[1].Misc.VirtualSize,nsectionalign);
			return true;
		}
	}
	//---------------------------------------------------------------------------------------------
	size_t getFirstSectionOffset()
	{
		PIMAGE_NT_HEADERS pntheader = static_cast<PIMAGE_NT_HEADERS>(peFuzzLinker::getNtHeader());
		unsigned long nsectionalign = peFuzzLinker::getSectionAlignment();
		unsigned long nfilealign = peFuzzLinker::getFileAlignment();
		unsigned long nheadersize = pntheader->OptionalHeader.SizeOfHeaders;
		return (peFuzzLinker::alignmentCalc(nheadersize,nsectionalign)-nheadersize);
	}
	//---------------------------------------------------------------------------------------------
	size_t getSectionOffsetByRav(unsigned long nrav)
	{
		PIMAGE_SECTION_HEADER psectionptr = static_cast<PIMAGE_SECTION_HEADER>(peFuzzLinker::getSectionHeader());
		for(unsigned long i=0;i<peFuzzLinker::getNumberOfSections();i++)
		{
			if(psectionptr[i].VirtualAddress<=nrav&&nrav<=(psectionptr[i].VirtualAddress+psectionptr[i].Misc.VirtualSize))
			{
				return (psectionptr[i].VirtualAddress-psectionptr[i].PointerToRawData);
			}
		}
		return 0;
	}
	//---------------------------------------------------------------------------------------------
	size_t fixRvaToOffset(size_t nrva)
	{
		PIMAGE_SECTION_HEADER psection = static_cast<PIMAGE_SECTION_HEADER>(peFuzzLinker::getSectionHeader());
		if(!psection)
		{
			return -1;
		}
		else
		{
			for(unsigned long nindex=0;nindex<peFuzzLinker::getNumberOfSections();nindex++)
			{
				if((nrva>=psection[nindex].VirtualAddress)&&nrva<(psection[nindex].VirtualAddress+psection[nindex].SizeOfRawData))
				{
					return (nrva-psection[nindex].VirtualAddress+psection[nindex].PointerToRawData);
				}
			}
			return -1;
		}
	}
};
//---------------------------------------------------------------------------------------------
#endif
