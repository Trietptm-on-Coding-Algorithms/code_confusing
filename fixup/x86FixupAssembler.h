//---------------------------------------------------------------------------------------------
#ifndef X86FIXUPASSEMBLER_H
#define X86FIXUPASSEMBLER_H
#include <time.h>
//---------------------------------------------------------------------------------------------
class x86FixupAssembler
{
public:
	void fixAssemblyPushImm(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset);
	void fixAssemblyCallMem(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset);
	void fixAssemblyCallRva(void *dst,size_t nrva1,void *src,size_t nrva2,size_t noffset);
private:
	inline void fixAssemblyMovImmToEspPtr(void* dst,void* src)
	{
		unsigned char* pdst =  static_cast<unsigned char*>(dst);
		unsigned char* psrc =  static_cast<unsigned char*>(src);
		unsigned char junkcode[] = {0xC7,0x04,0x24,0x90,0x90,0x90,0x90};
		x86FixupAssembler::inlineMemmove(junkcode+3,psrc,sizeof(unsigned long));
		x86FixupAssembler::inlineMemmove(pdst,junkcode,7);
	}
	inline void fixAssemblyJmpMem(void* dst,void* src)
	{
		unsigned char* pdst =  static_cast<unsigned char*>(dst);
		unsigned char* psrc =  static_cast<unsigned char*>(src);
		unsigned char junkcode[] = {0xFF,0x25,0x00,0x00,0x00,0x00};
		x86FixupAssembler::inlineMemmove(junkcode+2,src,sizeof(unsigned long));
		x86FixupAssembler::inlineMemmove(pdst,junkcode,6);
	}
	inline void fixAssemblyNop(void* dst)
	{
		unsigned char* pdst =  static_cast<unsigned char*>(dst);
		x86FixupAssembler::inlineMemset(pdst,0x90,sizeof(unsigned char));
	}
	inline void fixAssemblyJmpImm(void* src,unsigned long naddr)
	{
		unsigned char* psrc =  static_cast<unsigned char*>(src);
		unsigned long  ndst = naddr;
		x86FixupAssembler::inlineMemset(psrc++,0xE9,sizeof(unsigned char));
		x86FixupAssembler::inlineMemmove(psrc,&ndst,sizeof(unsigned long));
	}
	inline void fixAssemblyRandomPushReg(void* dst)
	{
		unsigned char* pdst = static_cast<unsigned char*>(dst);
		unsigned char preglist[] = {0x50,0x51,0x52,0x53,0x53,0x54,0x55,0x56,0x57};
		x86FixupAssembler::inlineMemset(pdst,preglist[clock()%9],sizeof(unsigned char));
	}
	inline void* inlineMemmove(void* s1,const void* s2,size_t n)
	{ 
		register unsigned char *dst = (unsigned char*) s1;   
		register unsigned char *src = (unsigned char*) s2;   
		register unsigned char *end;
		if(dst<src)
			for(end=src+n;src<end;)  
				*dst++ = *src++;   
		else
			for(end=src,src+=n,dst+=n;src>end;)
				*--dst = *--src;
		return s1;   
	}
	inline void* inlineMemset(void* s,int c,register size_t n)
	{
		register unsigned char  val = (unsigned char)c;
		register unsigned char *ptr = (unsigned char*)s;
		register unsigned char *end = ptr + n;
		while (ptr<end)*ptr++ = val;
		return s;
	}
	inline int inlineMemcmp(const void* s1,const void* s2,size_t n)
	{
		register unsigned char *p1 = (unsigned char*)s1;
		register unsigned char *p2 = (unsigned char*)s2;
		register unsigned char *end = p1 + n;
		while(p1< end)
		{
			if(*p1++!=*p2++)
			{
				if(*--p1<*--p2)      
					return -1;
				else  
					return 1;
			}
		}
		return 0;
	}
	inline void* inlineMemcpy(void* s1,const void* s2,size_t n)
	{   
		register unsigned char *src = (unsigned char *)s2;   
		register unsigned char *dst = (unsigned char *)s1;   
		register unsigned char *end = src + n;   
		while (src<end)*dst++ = *src++;   
		return s1;   
	}
};
#endif
