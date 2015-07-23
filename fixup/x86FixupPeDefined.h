#ifndef X86FIXUPPEDEFINED_H
#define X86FIXUPPEDEFINED_H
#include "main.h"
//---------------------------------------------------------------------------------------------
class x86FixupPeDefined
{
public:
	bool setPeMemoryImage(unsigned char* pimagebase);
	unsigned long getBaseAddress();
	unsigned long getRvaImageBaseMax();
	unsigned long getRvaImageBaseMin();
	unsigned long getNumberOfSections();
	unsigned long getSectionAlignment();
	unsigned long getFileAlignment();
	void* getDosHeader();
	void* getNtHeader();
	void* getFileHeader();
	void* getOptionHeader();
	void* getSectionHeader();
	size_t alignmentCalc(size_t nsize, size_t nvalue);
private:
	PIMAGE_DOS_HEADER         _dos_header;
	PIMAGE_NT_HEADERS32       _nt32_header;
	PIMAGE_FILE_HEADER        _file_header;
	PIMAGE_OPTIONAL_HEADER32  _optional32_header;
	PIMAGE_SECTION_HEADER     _section_header;
};
//---------------------------------------------------------------------------------------------
#pragma pack(push)
#pragma pack(1)
//---------------------------------------------------------------------------------------------
typedef struct mzstruct
{
	unsigned short  mz_id;
	unsigned short  mz_last512;
	unsigned short  mz_num512;
	unsigned short  mz_relnum;
	unsigned short  mz_headersize;
	unsigned short  mz_minmem;
	unsigned short  mz_maxmem;
	unsigned short  mz_ss;
	unsigned short  mz_sp;
	unsigned short  mz_checksum;
	unsigned short  mz_ip;
	unsigned short  mz_cs;
	unsigned short  mz_relofs;
	unsigned short  mz_ovrnum;
	unsigned char   mz_reserved[32];
	unsigned long   mz_neptr;
} mzheader,*pmzheader;
//---------------------------------------------------------------------------------------------
typedef struct pestruct
{
	unsigned long   pe_id;                  // 00 01 02 03
	unsigned short  pe_cputype;             // 04 05
	unsigned short  pe_numofobjects;        // 06 07
	unsigned long   pe_datetime;            // 08 09 0A 0B
	unsigned long   pe_coffptr;             // 0C 0D 0E 0F
	unsigned long   pe_coffsize;            // 10 11 12 13
	unsigned short  pe_ntheadersize;        // 14 15
	unsigned short  pe_flags;               // 16 17
	// NT_Header {
	unsigned short  pe_magic;               // 18 19
	unsigned char   pe_linkmajor;           // 1A
	unsigned char   pe_linkminor;           // 1B
	unsigned long   pe_sizeofcode;          // 1C 1D 1E 1F
	unsigned long   pe_sizeofidata;         // 20 21 22 23
	unsigned long   pe_sizeofudata;         // 24 25 26 27
	unsigned long   pe_entrypointrva;       // 28 29 2A 2B
	unsigned long   pe_baseofcode;          // 2C 2D 2E 2F
	unsigned long   pe_baseofdata;          // 30 31 32 33
	unsigned long   pe_imagebase;           // 34 35 36 37
	unsigned long   pe_objectalign;         // 38 39 3A 3B
	unsigned long   pe_filealign;           // 3C 3D 3E 3F
	unsigned short  pe_osmajor;             // 40 41
	unsigned short  pe_osminor;             // 42 43
	unsigned short  pe_usermajor;           // 44 45
	unsigned short  pe_userminor;           // 46 47
	unsigned short  pe_subsysmajor;         // 48 49
	unsigned short  pe_subsysminor;         // 4A 4B
	unsigned long   pe_reserved;            // 4C 4D 4E 4F
	unsigned long   pe_imagesize;           // 50 51 52 53
	unsigned long   pe_headersize;          // 54 55 56 56
	unsigned long   pe_checksum;            // 58 59 5A 5B
	unsigned short  pe_subsystem;           // 5C 5D
	unsigned short  pe_dllflags;            // 5E 5F
	unsigned long   pe_stackreserve;        // 60 61 62 63
	unsigned long   pe_stackcommit;         // 64 65 66 67
	unsigned long   pe_heapreserve;         // 68 69 6A 6B
	unsigned long   pe_heapcommit;          // 6C 6D 6E 6F
	unsigned long   pe_loaderflags;         // 70 71 72 73
	unsigned long   pe_numofrvaandsizes;    // 74 75 76 77
	// rva and sizes
	unsigned long   pe_exportrva;           // 78 79 7A 7B
	unsigned long   pe_exportsize;          // 7C 7D 7E 7F
	unsigned long   pe_importrva;           // 80 81 82 83
	unsigned long   pe_importsize;          // 84 85 86 87
	unsigned long   pe_resourcerva;         // 88 89 8A 8B
	unsigned long   pe_resourcesize;        // 8C 8D 8E 8F
	unsigned long   pe_exceptionrva;        // 90 91 92 93
	unsigned long   pe_exceptionsize;       // 94 95 96 97
	unsigned long   pe_securityrva;         // 98 99 9A 9B
	unsigned long   pe_securitysize;        // 9C 9D 9E 9F
	unsigned long   pe_fixuprva;            // A0 A1 A2 A3
	unsigned long   pe_fixupsize;           // A4 A5 A6 A7
	unsigned long   pe_debugrva;            // A8 A9 AA AB
	unsigned long   pe_debugsize;           // AC AD AE AF
	unsigned long   pe_descriptionrva;      // B0 B1 B2 B3
	unsigned long   pe_descriptionsize;     // B4 B5 B6 B7
	unsigned long   pe_machinerva;          // B8 B9 BA BB
	unsigned long   pe_machinesize;         // BC BD BE BF
	unsigned long   pe_tlsrva;              // C0 C1 C2 C3
	unsigned long   pe_tlssize;             // C4 C5 C6 C7
	unsigned long   pe_loadconfigrva;       // C8 C9 CA CB
	unsigned long   pe_loadconfigsize;      // CC CD CE CF
	unsigned char   pe_reserved_1[8];       // D0 D1 D2 D3  D4 D5 D6 D7
	unsigned long   pe_iatrva;              // D8 D9 DA DB
	unsigned long   pe_iatsize;             // DC DD DE DF
	unsigned char   pe_reserved_2[8];       // E0 E1 E2 E3  E4 E5 E6 E7
	unsigned char   pe_reserved_3[8];       // E8 E9 EA EB  EC ED EE EF
	unsigned char   pe_reserved_4[8];       // F0 F1 F2 F3  F4 F5 F6 F7
	// ---- total size == 0xF8 ---------
} peheader,*ppeheader;
//---------------------------------------------------------------------------------------------
typedef struct peobjentrystruct
{
	unsigned char   oe_name[8];             // 00 01 02 03  04 05 06 07
	unsigned long   oe_virtsize;            // 08 09 0A 0B
	unsigned long   oe_virtrva;             // 0C 0D 0E 0F
	unsigned long   oe_physsize;            // 10 11 12 13
	unsigned long   oe_physoffs;            // 14 15 16 17
	unsigned char   oe_reserved[0x0C];      // 18 19 1A 1B   1C 1D 1E 1F  20 21 22 23
	unsigned long   oe_objectflags;         // 24 25 26 27
	// ---- total size == 0x28 ---------
} peobjentry,*ppeobjentry;
//---------------------------------------------------------------------------------------------
typedef struct peexportstruct
{
	unsigned long   ex_flags;               // 00 01 02 03
	unsigned long   ex_datetime;            // 04 05 06 07
	unsigned short  ex_major_ver;           // 08 09
	unsigned short  ex_minor_ver;           // 0A 0B
	unsigned long   ex_namerva;             // 0C 0D 0E 0F
	unsigned long   ex_ordinalbase;         // 10 11 12 13
	unsigned long   ex_numoffunctions;      // 14 15 16 17
	unsigned long   ex_numofnamepointers;   // 18 19 1A 1B
	unsigned long   ex_addresstablerva;     // 1C 1D 1E 1F
	unsigned long   ex_namepointersrva;     // 20 21 22 23
	unsigned long   ex_ordinaltablerva;     // 24 25 26 27
	// ---- total size == 0x28 ---------
} peexport,*ppeexport;
//---------------------------------------------------------------------------------------------
typedef struct peimportstruct
{
	unsigned long   im_lookup;              // 00
	unsigned long   im_datetime;            // 04  ?
	unsigned long   im_forward;             // 08  -1
	unsigned long   im_name;                // 0C
	unsigned long   im_addresstable;        // 10
	// ---- total size == 0x14 ---------
} peimport,*ppeimport;
//---------------------------------------------------------------------------------------------
typedef struct perelocstruct
{
	unsigned long   fx_pagerva;             // 00 01 02 03
	unsigned long   fx_blocksize;           // 04 05 06 07
	unsigned short  fx_typeoffs[1];         // 08 09 .. ..
} pereloc,*ppereloc;
//---------------------------------------------------------------------------------------------
#pragma pack(pop)
//---------------------------------------------------------------------------------------------
#endif