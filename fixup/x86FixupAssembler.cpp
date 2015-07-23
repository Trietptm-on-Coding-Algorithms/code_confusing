//---------------------------------------------------------------------------------------------
#include "x86FixupAssembler.h"
#pragma code_seg(".shell")
//---------------------------------------------------------------------------------------------
void x86FixupAssembler::fixAssemblyPushImm(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset)
{
	unsigned char *pdst =  static_cast<unsigned char*>(dst);
	unsigned char *psrc =  static_cast<unsigned char*>(src);
	unsigned long nsrcnextaddr = ((unsigned long)psrc+5+nrva2+noffset);
	if(*psrc==0x68)
	{
		x86FixupAssembler::fixAssemblyRandomPushReg(pdst);
		x86FixupAssembler::fixAssemblyMovImmToEspPtr(pdst+1,psrc+1);
		x86FixupAssembler::fixAssemblyJmpImm(psrc,(unsigned long)(pdst+nrva1+noffset-nsrcnextaddr));
		x86FixupAssembler::fixAssemblyJmpImm(pdst+8,(nsrcnextaddr-((unsigned long)pdst+8+nrva1+noffset+5)));
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupAssembler::fixAssemblyCallMem(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset)
{
	unsigned char *pdst = static_cast<unsigned char*>(dst);
	unsigned char *psrc = static_cast<unsigned char*>(src);
	unsigned long nsrcnextaddr = ((unsigned long)psrc+5+nrva2+noffset);
	if(*(unsigned short*)psrc==0x15FF)
	{
		x86FixupAssembler::fixAssemblyRandomPushReg(pdst);
		unsigned char* paddress = pdst+14+nrva1+noffset;
		x86FixupAssembler::fixAssemblyMovImmToEspPtr(pdst+1,(unsigned char*)&paddress);
		x86FixupAssembler::fixAssemblyJmpMem(pdst+8,psrc+2);
		x86FixupAssembler::fixAssemblyJmpImm(psrc,(unsigned long)(pdst+nrva1+noffset-nsrcnextaddr));
		x86FixupAssembler::fixAssemblyNop(psrc+5);
		x86FixupAssembler::fixAssemblyJmpImm(pdst+14,(nsrcnextaddr-((unsigned long)pdst+14+nrva1+noffset+5)));
	}
}
//---------------------------------------------------------------------------------------------
void x86FixupAssembler::fixAssemblyCallRva(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset)
{
	unsigned char *pdst =  static_cast<unsigned char*>(dst);
	unsigned char *psrc =  static_cast<unsigned char*>(src);
	unsigned long nsrcnextaddr = ((unsigned long)psrc+5+nrva2+noffset);
	if(*psrc==0xE8)
	{
		unsigned long nmemrvaaddr = ((unsigned long)psrc+(*(unsigned long *)(psrc+1))+6+nrva2+noffset);
		x86FixupAssembler::fixAssemblyRandomPushReg(pdst);
		x86FixupAssembler::fixAssemblyJmpImm(psrc,(unsigned long)(pdst+nrva1+noffset-nsrcnextaddr));
		unsigned char* paddress = pdst+13+nrva1+noffset;
		x86FixupAssembler::fixAssemblyMovImmToEspPtr(pdst+1,(unsigned char*)&paddress);
		x86FixupAssembler::fixAssemblyJmpImm(pdst+8,(nmemrvaaddr -= ((unsigned long)pdst+9+nrva1+noffset+5)));
		x86FixupAssembler::fixAssemblyJmpImm(pdst+13,(nsrcnextaddr-((unsigned long)pdst+13+nrva1+noffset+5)));
	}
}