//---------------------------------------------------------------------------------------------
#include "x86FixupAvoidKill.h"
#include "x86FixupDisassembly.h"
#include "x86FixupPeLoader.h"
//---------------------------------------------------------------------------------------------
bool x86FixupAvoidKill::x86FixupFullTarget(wchar_t* pfilename)
{
	if(!pfilename)
	{
		x86FixupAvoidKill::logFuzzme("fuzz path as empty!");
		return false;
	}
	else if(pfilename[0]=='\"')
	{
		wcsncpy(x86FixupAvoidKill::_pimagepath,pfilename,wcslen(pfilename));
		x86FixupAvoidKill::logFuzzme("fuzz path error!");
		return false;
	}
	else
	{
		wcsncpy(x86FixupAvoidKill::_pimagepath,pfilename,wcslen(pfilename));
	}
	FILE* fp = _wfopen(pfilename,L"rb");
	if(fp!=NULL)
	{
		fseek(fp,0,SEEK_END);
		x86FixupAvoidKill::_nfilesize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		if(!x86FixupAvoidKill::_nfilesize)
		{
			x86FixupAvoidKill::logFuzzme("fuzz object size as zero!");
			return false;
		}
		x86FixupAvoidKill::_poldimagedata = new unsigned char[x86FixupAvoidKill::_nfilesize+1];
		if(x86FixupAvoidKill::_poldimagedata!=NULL)
		{
			memset(x86FixupAvoidKill::_poldimagedata,0,x86FixupAvoidKill::_nfilesize+1);
			fread(x86FixupAvoidKill::_poldimagedata,1,x86FixupAvoidKill::_nfilesize,fp);
		}
		x86FixupAvoidKill::_pnewimagedata = new unsigned char[x86FixupAvoidKill::_nfilesize*2];
		if(x86FixupAvoidKill::_pnewimagedata!=NULL)
		{
			memset(x86FixupAvoidKill::_pnewimagedata,0,x86FixupAvoidKill::_nfilesize*2);
			memcpy(x86FixupAvoidKill::_pnewimagedata,x86FixupAvoidKill::_poldimagedata,x86FixupAvoidKill::_nfilesize);
		}
		return true;
	}
	else
	{
		x86FixupAvoidKill::logFuzzme("open fuzz object file fail!");
		return false;
	}
}
//---------------------------------------------------------------------------------------------
bool x86FixupAvoidKill::x86FixupFuzzAssembly()
{
	PIMAGE_OPTIONAL_HEADER32 poptptr = NULL;
	PIMAGE_SECTION_HEADER psectionptr = NULL;
//test
	if(!x86FixupPeDefined::setPeMemoryImage(x86FixupAvoidKill::_pnewimagedata))
	{
		x86FixupAvoidKill::logFuzzme("fuzz object file format error!");
		return false;
	}
	else
	{
		poptptr = static_cast<PIMAGE_OPTIONAL_HEADER32>(x86FixupPeDefined::getOptionHeader());
		psectionptr = static_cast<PIMAGE_SECTION_HEADER>(x86FixupPeDefined::getSectionHeader());
	}
	unsigned long ncodesavelen = 0;
	unsigned long nimagebaseoffset = (x86FixupPeDefined::getRvaImageBaseMin()-x86FixupPeDefined::getBaseAddress());
	unsigned long nimagebasemin = x86FixupPeDefined::getRvaImageBaseMin();
	unsigned long nimagebasemax = x86FixupPeDefined::getRvaImageBaseMax();
	unsigned char* pcodesaveptr = (unsigned char*)(psectionptr[x86FixupPeDefined::getNumberOfSections()-1].PointerToRawData+x86FixupPeDefined::getBaseAddress());
	unsigned long  psaveoffsetptr = psectionptr[x86FixupPeDefined::getNumberOfSections()-1].VirtualAddress - psectionptr[x86FixupPeDefined::getNumberOfSections()-1].PointerToRawData;
	pcodesaveptr += psectionptr[x86FixupPeDefined::getNumberOfSections()-1].SizeOfRawData;
	unsigned long nsecindex = 0;
	if(!psectionptr)
	{
		x86FixupAvoidKill::logFuzzme("fuzz object file error!");
		return false;
	}
	else
	{
		for(nsecindex=0;nsecindex<x86FixupPeDefined::getNumberOfSections()-1;nsecindex++)
		{
			wnsprintfA((char*)psectionptr[nsecindex].Name,8,".mutate%d\0\0\0",nsecindex);
			psectionptr[nsecindex].Characteristics = 0xE0000080;
			unsigned long ncoderawptr = psectionptr[nsecindex].PointerToRawData;
			unsigned long ncoderawlen = psectionptr[nsecindex].SizeOfRawData;
			unsigned long ncodeoffset = psectionptr[nsecindex].VirtualAddress - ncoderawptr;
			unsigned char* pcodeptr = (unsigned char*)(x86FixupPeDefined::getBaseAddress()+ncoderawptr);
			if(IMAGE_SCN_MEM_EXECUTE&psectionptr[nsecindex].Characteristics)
			{
				for(unsigned long nindex=0;nindex<ncoderawlen;)
				{
					unsigned long ndatacode = *(unsigned long*)(pcodeptr+1);
					unsigned long nlen = decodeOpcodeSize((void*)pcodeptr);
					if(*(unsigned char*)pcodeptr==0xE8&&nlen==5)
					{
						if(*(unsigned short*)(pcodeptr+3)==0 && *(unsigned short*)(pcodeptr+1)!=0)
						{
							x86FixupAssembler::fixAssemblyCallRva(pcodesaveptr,psaveoffsetptr,pcodeptr,ncodeoffset,nimagebaseoffset);
							pcodesaveptr += 18;
							ncodesavelen += 18;
							pcodeptr += 5;
							nindex += 5;
						}
						else
						{
							x86FixupAssembler::fixAssemblyCallRva(pcodesaveptr,psaveoffsetptr,pcodeptr,ncodeoffset,nimagebaseoffset);
							pcodesaveptr += 18;
							ncodesavelen += 18;
							pcodeptr += 5;
							nindex += 5;
						}
						Sleep(5);
					}
					else if(*(unsigned short*)pcodeptr==0x15ff&&nlen==6)
					{
						if((*(unsigned long*)(pcodeptr+2))>nimagebasemin && (*(unsigned long*)(pcodeptr+2))<nimagebasemax)
						{
							x86FixupAssembler::fixAssemblyCallMem(pcodesaveptr,psaveoffsetptr,pcodeptr,ncodeoffset,nimagebaseoffset);
							pcodesaveptr += 19;
							ncodesavelen += 19;
							pcodeptr += 6;
							nindex += 6;
						}
						Sleep(5);
					}
					else if(*(unsigned char*)pcodeptr==0x68&&nlen==5)
					{
						if(ndatacode>nimagebasemin && ndatacode<nimagebasemax)
						{
							x86FixupAssembler::fixAssemblyPushImm(pcodesaveptr,psaveoffsetptr,pcodeptr,ncodeoffset,nimagebaseoffset);
							pcodesaveptr += 0xD;
							ncodesavelen += 0xD;
							pcodeptr += 5;
							nindex += 5;
						}
						else
						{
							pcodeptr += nlen;
							nindex += nlen;
						}
					}
					else
					{
						pcodeptr += nlen;
						nindex += nlen;
					}
				}
			}
		}
	}
	if(ncodesavelen)
	{
		wnsprintfA((char*)psectionptr[nsecindex].Name,8,".mutate%d\0\0\0",nsecindex);
		psectionptr[nsecindex].Characteristics = 0xE0000080;
		psectionptr[nsecindex].SizeOfRawData += x86FixupPeDefined::alignmentCalc(ncodesavelen,x86FixupPeDefined::getFileAlignment());
		psectionptr[nsecindex].Misc.VirtualSize += x86FixupPeDefined::alignmentCalc(ncodesavelen,x86FixupPeDefined::getSectionAlignment());
		poptptr->SizeOfImage += x86FixupPeDefined::alignmentCalc(ncodesavelen,x86FixupPeDefined::getSectionAlignment());
		x86FixupAvoidKill::_nfilesize += x86FixupPeDefined::alignmentCalc(ncodesavelen,x86FixupPeDefined::getFileAlignment());
	}
	return (ncodesavelen>0);
}
//---------------------------------------------------------------------------------------------
void x86FixupAvoidKill::x86FixupCleanTarget()
{
	if(x86FixupAvoidKill::_poldimagedata!=NULL)
	{
		free(x86FixupAvoidKill::_poldimagedata);
		x86FixupAvoidKill::_poldimagedata = NULL;
	}
	if(x86FixupAvoidKill::_pnewimagedata!=NULL)
	{
		free(x86FixupAvoidKill::_pnewimagedata);
		x86FixupAvoidKill::_pnewimagedata = NULL;
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupAvoidKill::x86FixupGenFuzzResult()
{
	FILE* fp = fopen("mutate.exe","wb+");
	if(fp!=NULL)
	{
		fwrite(x86FixupAvoidKill::_pnewimagedata,1,x86FixupAvoidKill::_nfilesize,fp);
		fclose(fp);
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupAvoidKill::logFuzzme(char* pkey)
{
#ifndef NDEBUG
	char pimagepath[MAX_PATH] = {0};
	wcstombs(pimagepath,x86FixupAvoidKill::_pimagepath,wcslen(x86FixupAvoidKill::_pimagepath));
	x86FixupAvoidKill::logFuzzme(pimagepath,pkey);
#endif
}
//---------------------------------------------------------------------------------------------
void x86FixupAvoidKill::logFuzzme(char* ppath,char* pkey)
{
#ifndef NDEBUG
	time_t t = time(NULL);
	FILE* fp=fopen("fixup.log","ab+");
	fseek(fp,0,SEEK_END);
	fprintf(fp,"¡¾%s¡¿%s¡¾%s¡¿\r\n",ctime(&t),ppath,pkey);
	fflush(fp);
	fclose(fp);
#endif
}
x86FixupAvoidKill::~x86FixupAvoidKill()
{
	x86FixupAvoidKill::x86FixupCleanTarget();
}