//---------------------------------------------------------------------------------------------
#ifndef X86FIXUPAVOIDKILL_H
#define X86FIXUPAVOIDKILL_H
#include "main.h"
#include "x86FixupAssembler.h"
#include "x86FixupPeDefined.h"
//---------------------------------------------------------------------------------------------
class x86FixupAvoidKill:public x86FixupAssembler,public x86FixupPeDefined
{
public:
    bool x86FixupFullTarget(wchar_t *pfilename);
    bool x86FixupFuzzAssembly();
    void x86FixupGenFuzzResult();
    void logFuzzme(char *pkey);
private:
	void x86FixupCleanTarget();
    void logFuzzme(char *ppath, char *pkey);
public:
	~x86FixupAvoidKill();
private:
    unsigned char *_poldimagedata;
    unsigned char *_pnewimagedata;
    unsigned long  _nfilesize;
    wchar_t _pimagepath[MAX_PATH];
};
//---------------------------------------------------------------------------------------------
#endif