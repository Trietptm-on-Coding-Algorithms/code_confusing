#include "x86FixupDisassembly.h"

DWORD op_table_1[ 256 ] =
{
	/* 00 */	C_MODRM,
	/* 01 */	C_MODRM,
	/* 02 */	C_MODRM,
	/* 03 */	C_MODRM,
	/* 04 */	C_I8,
	/* 05 */	C_DATA66,
	/* 06 */	C_SINGLE					|	C_ILLEGAL,
	/* 07 */	C_SINGLE					|	C_ILLEGAL,
	/* 08 */	C_MODRM,
	/* 09 */	C_MODRM,
	/* 0A */	C_MODRM,
	/* 0B */	C_MODRM,
	/* 0C */	C_I8,
	/* 0D */	C_DATA66,
	/* 0E */	C_SINGLE					|	C_ILLEGAL,
	/* 0F */	C_MULTI,

	/* 10 */	C_MODRM						|	C_ILLEGAL,
	/* 11 */	C_MODRM,
	/* 12 */	C_MODRM						|	C_ILLEGAL,
	/* 13 */	C_MODRM,
	/* 14 */	C_I8						|	C_ILLEGAL,
	/* 15 */	C_DATA66					|	C_ILLEGAL,
	/* 16 */	C_SINGLE					|	C_ILLEGAL,
	/* 17 */	C_SINGLE					|	C_ILLEGAL,
	/* 18 */	C_MODRM						|	C_ILLEGAL,
	/* 19 */	C_MODRM,
	/* 1A */	C_MODRM,
	/* 1B */	C_MODRM,
	/* 1C */	C_I8						|	C_ILLEGAL,
	/* 1D */	C_DATA66					|	C_ILLEGAL,
	/* 1E */	C_SINGLE					|	C_ILLEGAL,
	/* 1F */	C_SINGLE					|	C_ILLEGAL,

	/* 20 */	C_MODRM,
	/* 21 */	C_MODRM,
	/* 22 */	C_MODRM,
	/* 23 */	C_MODRM,
	/* 24 */	C_I8,
	/* 25 */	C_DATA66,
	/* 26 */	C_PREFIX					|	C_PREFIXSEG	|	C_ILLEGAL,
	/* 27 */	C_SINGLE					|	C_ILLEGAL,
	/* 28 */	C_MODRM,
	/* 29 */	C_MODRM,
	/* 2A */	C_MODRM,
	/* 2B */	C_MODRM,
	/* 2C */	C_I8,
	/* 2D */	C_DATA66,
	/* 2E */	C_PREFIX					|	C_PREFIXSEG	|	C_ILLEGAL,
	/* 2F */	C_SINGLE					|	C_ILLEGAL,

	/* 30 */	C_MODRM,
	/* 31 */	C_MODRM,
	/* 32 */	C_MODRM,
	/* 33 */	C_MODRM,
	/* 34 */	C_I8,
	/* 35 */	C_DATA66,
	/* 36 */	C_PREFIX					|	C_PREFIXSEG	|	C_ILLEGAL,
	/* 37 */	C_SINGLE					|	C_ILLEGAL,
	/* 38 */	C_MODRM,
	/* 39 */	C_MODRM,
	/* 3A */	C_MODRM,
	/* 3B */	C_MODRM,
	/* 3C */	C_I8,
	/* 3D */	C_DATA66,
	/* 3E */	C_PREFIX					|	C_PREFIXSEG	|	C_ILLEGAL,
	/* 3F */	C_SINGLE					|	C_ILLEGAL,

	/* 40 */	C_SINGLE,
	/* 41 */	C_SINGLE,
	/* 42 */	C_SINGLE,
	/* 43 */	C_SINGLE,
	/* 44 */	C_SINGLE					|	C_ILLEGAL,
	/* 45 */	C_SINGLE,
	/* 46 */	C_SINGLE,
	/* 47 */	C_SINGLE,
	/* 48 */	C_SINGLE,
	/* 49 */	C_SINGLE,
	/* 4A */	C_SINGLE,
	/* 4B */	C_SINGLE,
	/* 4C */	C_SINGLE					|	C_ILLEGAL,
	/* 4D */	C_SINGLE,
	/* 4E */	C_SINGLE,
	/* 4F */	C_SINGLE,

	/* 50 */	C_SINGLE,
	/* 51 */	C_SINGLE,
	/* 52 */	C_SINGLE,
	/* 53 */	C_SINGLE,
	/* 54 */	C_SINGLE,
	/* 55 */	C_SINGLE,
	/* 56 */	C_SINGLE,
	/* 57 */	C_SINGLE,
	/* 58 */	C_SINGLE,
	/* 59 */	C_SINGLE,
	/* 5A */	C_SINGLE,
	/* 5B */	C_SINGLE,
	/* 5C */	C_SINGLE					|	C_ILLEGAL,
	/* 5D */	C_SINGLE,
	/* 5E */	C_SINGLE,
	/* 5F */	C_SINGLE,

	/* 60 */	C_SINGLE					|	C_ILLEGAL,
	/* 61 */	C_SINGLE					|	C_ILLEGAL,
	/* 62 */	C_MODRM						|	C_ILLEGAL,
	/* 63 */	C_MODRM						|	C_ILLEGAL,
	/* 64 */	C_PREFIX					|	C_PREFIXSEG,
	/* 65 */	C_PREFIX					|	C_PREFIXSEG	|	C_ILLEGAL,
	/* 66 */	C_PREFIX					|	C_PREFIX66,
	/* 67 */	C_PREFIX					|	C_PREFIX67,
	/* 68 */	C_DATA66,
	/* 69 */	C_MODRM		|	CH_DATA66,
	/* 6A */	C_I8,
	/* 6B */	C_MODRM		|	CH_I8,
	/* 6C */	C_SINGLE					|	C_ILLEGAL,
	/* 6D */	C_SINGLE					|	C_ILLEGAL,
	/* 6E */	C_SINGLE					|	C_ILLEGAL,
	/* 6F */	C_SINGLE					|	C_ILLEGAL,

	/* 70 */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* 71 */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* 72 */	C_I8						|	C_REL,
	/* 73 */	C_I8						|	C_REL,
	/* 74 */	C_I8						|	C_REL,
	/* 75 */	C_I8						|	C_REL,
	/* 76 */	C_I8						|	C_REL,
	/* 77 */	C_I8						|	C_REL,
	/* 78 */	C_I8						|	C_REL,
	/* 79 */	C_I8						|	C_REL,
	/* 7A */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* 7B */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* 7C */	C_I8						|	C_REL,
	/* 7D */	C_I8						|	C_REL,
	/* 7E */	C_I8						|	C_REL,
	/* 7F */	C_I8						|	C_REL,

	/* 80 */	C_MODRM		|	CH_I8,
	/* 81 */	C_MODRM		|	CH_DATA66,
	/* 82 */	C_MODRM		|	CH_I8		|	C_ILLEGAL,
	/* 83 */	C_MODRM		|	CH_I8,
	/* 84 */	C_MODRM,
	/* 85 */	C_MODRM,
	/* 86 */	C_MODRM,
	/* 87 */	C_MODRM,
	/* 88 */	C_MODRM,
	/* 89 */	C_MODRM,
	/* 8A */	C_MODRM,
	/* 8B */	C_MODRM,
	/* 8C */	C_MODRM						|	C_ILLEGAL,
	/* 8D */	C_MODRM,
	/* 8E */	C_MODRM						|	C_ILLEGAL,
	/* 8F */	C_MODRM,

	/* 90 */	C_SINGLE,
	/* 91 */	C_SINGLE,
	/* 92 */	C_SINGLE,
	/* 93 */	C_SINGLE					|	C_ILLEGAL,
	/* 94 */	C_SINGLE					|	C_ILLEGAL,
	/* 95 */	C_SINGLE					|	C_ILLEGAL,
	/* 96 */	C_SINGLE					|	C_ILLEGAL,
	/* 97 */	C_SINGLE					|	C_ILLEGAL,
	/* 98 */	C_SINGLE					|	C_ILLEGAL,
	/* 99 */	C_SINGLE,
	/* 9A */	C_DATA66	|	CH_I16		|	C_ILLEGAL,
	/* 9B */	C_SINGLE,
	/* 9C */	C_SINGLE					|	C_ILLEGAL,
	/* 9D */	C_SINGLE					|	C_ILLEGAL,
	/* 9E */	C_SINGLE					|	C_ILLEGAL,
	/* 9F */	C_SINGLE					|	C_ILLEGAL,

	/* A0 */	C_ADDR67,
	/* A1 */	C_ADDR67,
	/* A2 */	C_ADDR67,
	/* A3 */	C_ADDR67,
	/* A4 */	C_SINGLE					|	C_ALLOWREP,
	/* A5 */	C_SINGLE					|	C_ALLOWREP,
	/* A6 */	C_SINGLE					|	C_ALLOWREP,
	/* A7 */	C_SINGLE					|	C_ALLOWREP,
	/* A8 */	C_I8,
	/* A9 */	C_DATA66,
	/* AA */	C_SINGLE					|	C_ALLOWREP,
	/* AB */	C_SINGLE					|	C_ALLOWREP,
	/* AC */	C_SINGLE					|	C_ALLOWREP,
	/* AD */	C_SINGLE					|	C_ALLOWREP	|	C_ILLEGAL,
	/* AE */	C_SINGLE					|	C_ALLOWREP,
	/* AF */	C_SINGLE					|	C_ALLOWREP	|	C_ILLEGAL,

	/* B0 */	C_I8,
	/* B1 */	C_I8,
	/* B2 */	C_I8,
	/* B3 */	C_I8,
	/* B4 */	C_I8,
	/* B5 */	C_I8,
	/* B6 */	C_I8						|	C_ILLEGAL,
	/* B7 */	C_I8						|	C_ILLEGAL,
	/* B8 */	C_DATA66,
	/* B9 */	C_DATA66,
	/* BA */	C_DATA66,
	/* BB */	C_DATA66,
	/* BC */	C_DATA66					|	C_ILLEGAL,
	/* BD */	C_DATA66,
	/* BE */	C_DATA66,
	/* BF */	C_DATA66,

	/* C0 */	C_MODRM		|	CH_I8,
	/* C1 */	C_MODRM		|	CH_I8,
	/* C2 */	C_I16						|	C_STOP,
	/* C3 */	C_STOP,
	/* C4 */	C_MODRM						|	C_ILLEGAL,
	/* C5 */	C_MODRM						|	C_ILLEGAL,
	/* C6 */	C_MODRM		|	CH_I8,
	/* C7 */	C_MODRM		|	CH_DATA66,
	/* C8 */	C_I16		|	CH_I8,
	/* C9 */	C_SINGLE,
	/* CA */	C_I16						|	C_STOP		|	C_ILLEGAL,
	/* CB */	C_STOP						|	C_ILLEGAL,
	/* CC */	C_SINGLE					|	C_ILLEGAL,
	/* CD */	C_I8,
	/* CE */	C_SINGLE					|	C_ILLEGAL,
	/* CF */	C_STOP						|	C_ILLEGAL,

	/* D0 */	C_MODRM,
	/* D1 */	C_MODRM,
	/* D2 */	C_MODRM,
	/* D3 */	C_MODRM,
	/* D4 */	C_I8						|	C_ILLEGAL,
	/* D5 */	C_I8						|	C_ILLEGAL,
	/* D6 */	C_SINGLE					|	C_ILLEGAL,
	/* D7 */	C_SINGLE					|	C_ILLEGAL,
	/* D8 */	C_MODRM,
	/* D9 */	C_MODRM,
	/* DA */	C_MODRM,
	/* DB */	C_MODRM,
	/* DC */	C_MODRM,
	/* DD */	C_MODRM,
	/* DE */	C_MODRM,
	/* DF */	C_MODRM,

	/* E0 */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* E1 */	C_I8						|	C_REL		|	C_ILLEGAL,
	/* E2 */	C_I8						|	C_REL,
	/* E3 */	C_I8						|	C_REL,
	/* E4 */	C_I8						|	C_ILLEGAL,
	/* E5 */	C_I8						|	C_ILLEGAL,
	/* E6 */	C_I8						|	C_ILLEGAL,
	/* E7 */	C_I8						|	C_ILLEGAL,
	/* E8 */	C_DATA66					|	C_REL,
	/* E9 */	C_DATA66					|	C_REL		|	C_STOP,
	/* EA */	C_DATA66	|	CH_I16		|	C_ILLEGAL,
	/* EB */	C_I8						|	C_REL		|	C_STOP,
	/* EC */	C_SINGLE					|	C_ILLEGAL,
	/* ED */	C_SINGLE					|	C_ILLEGAL,
	/* EE */	C_SINGLE					|	C_ILLEGAL,
	/* EF */	C_SINGLE					|	C_ILLEGAL,

	/* F0 */	C_PREFIX					|	C_PREFIXLOCK|	C_ILLEGAL,
	/* F1 */	C_SINGLE					|	C_ILLEGAL,
	/* F2 */	C_PREFIX					|	C_PREFIXREP,
	/* F3 */	C_PREFIX					|	C_PREFIXREP,
	/* F4 */	C_SINGLE					|	C_ILLEGAL,
	/* F5 */	C_SINGLE					|	C_ILLEGAL,
	/* F6 */	C_ROPCODEF6,
	/* F7 */	C_ROPCODEF7,
	/* F8 */	C_SINGLE,
	/* F9 */	C_SINGLE,
	/* FA */	C_SINGLE					|	C_ILLEGAL,
	/* FB */	C_SINGLE					|	C_ILLEGAL,
	/* FC */	C_SINGLE,
	/* FD */	C_SINGLE,
	/* FE */	C_MODRM,
	/* FF */	C_MODRM
};

DWORD op_table_2[ 256 ] =
{
	/* 00 */	C_MODRM,
	/* 01 */	C_MODRM,
	/* 02 */	C_MODRM,
	/* 03 */	C_MODRM,
	/* 04 */	C_SINGLE,
	/* 05 */	C_SINGLE,
	/* 06 */	C_SINGLE,
	/* 07 */	C_SINGLE,
	/* 08 */	C_SINGLE,
	/* 09 */	C_SINGLE,
	/* 0A */	C_SINGLE,
	/* 0B */	C_SINGLE,
	/* 0C */	C_SINGLE,
	/* 0D */	C_MODRM,
	/* 0E */	C_SINGLE,
	/* 0F */	C_MODRM		|	CH_I8,

	/* 10 */	C_MODRM						|	C_ALLOWREP,
	/* 11 */	C_MODRM						|	C_ALLOWREP,
	/* 12 */	C_MODRM,
	/* 13 */	C_MODRM,
	/* 14 */	C_MODRM,
	/* 15 */	C_MODRM,
	/* 16 */	C_MODRM,
	/* 17 */	C_MODRM,
	/* 18 */	C_MODRM,
	/* 19 */	C_SINGLE,
	/* 1A */	C_SINGLE,
	/* 1B */	C_SINGLE,
	/* 1C */	C_SINGLE,
	/* 1D */	C_SINGLE,
	/* 1E */	C_SINGLE,
	/* 1F */	C_SINGLE,

	/* 20 */	C_MODRM,
	/* 21 */	C_MODRM,
	/* 22 */	C_MODRM,
	/* 23 */	C_MODRM,
	/* 24 */	C_MODRM,
	/* 25 */	C_SINGLE,
	/* 26 */	C_MODRM,
	/* 27 */	C_SINGLE,
	/* 28 */	C_MODRM,
	/* 29 */	C_MODRM,
	/* 2A */	C_MODRM						|	C_ALLOWREP,
	/* 2B */	C_MODRM,
	/* 2C */	C_MODRM						|	C_ALLOWREP,
	/* 2D */	C_MODRM						|	C_ALLOWREP,
	/* 2E */	C_MODRM,
	/* 2F */	C_MODRM,

	/* 30 */	C_SINGLE,
	/* 31 */	C_SINGLE,
	/* 32 */	C_SINGLE,
	/* 33 */	C_SINGLE,
	/* 34 */	C_SINGLE,
	/* 35 */	C_SINGLE,
	/* 36 */	C_SINGLE,
	/* 37 */	C_SINGLE,
	/* 38 */	C_SINGLE,
	/* 39 */	C_SINGLE,
	/* 3A */	C_SINGLE,
	/* 3B */	C_SINGLE,
	/* 3C */	C_SINGLE,
	/* 3D */	C_SINGLE,
	/* 3E */	C_SINGLE,
	/* 3F */	C_SINGLE,

	/* 40 */	C_MODRM,
	/* 41 */	C_MODRM,
	/* 42 */	C_MODRM,
	/* 43 */	C_MODRM,
	/* 44 */	C_MODRM,
	/* 45 */	C_MODRM,
	/* 46 */	C_MODRM,
	/* 47 */	C_MODRM,
	/* 48 */	C_MODRM,
	/* 49 */	C_MODRM,
	/* 4A */	C_MODRM,
	/* 4B */	C_MODRM,
	/* 4C */	C_MODRM,
	/* 4D */	C_MODRM,
	/* 4E */	C_MODRM,
	/* 4F */	C_MODRM,

	/* 50 */	C_MODRM,
	/* 51 */	C_MODRM						|	C_ALLOWREP,
	/* 52 */	C_MODRM						|	C_ALLOWREP,
	/* 53 */	C_MODRM						|	C_ALLOWREP,
	/* 54 */	C_MODRM,
	/* 55 */	C_MODRM,
	/* 56 */	C_MODRM,
	/* 57 */	C_MODRM,
	/* 58 */	C_MODRM						|	C_ALLOWREP,
	/* 59 */	C_MODRM						|	C_ALLOWREP,
	/* 5A */	C_MODRM,
	/* 5B */	C_MODRM,
	/* 5C */	C_MODRM						|	C_ALLOWREP,
	/* 5D */	C_MODRM						|	C_ALLOWREP,
	/* 5E */	C_MODRM						|	C_ALLOWREP,
	/* 5F */	C_MODRM						|	C_ALLOWREP,

	/* 60 */	C_MODRM,
	/* 61 */	C_MODRM,
	/* 62 */	C_MODRM,
	/* 63 */	C_MODRM,
	/* 64 */	C_MODRM,
	/* 65 */	C_MODRM,
	/* 66 */	C_MODRM,
	/* 67 */	C_MODRM,
	/* 68 */	C_MODRM,
	/* 69 */	C_MODRM,
	/* 6A */	C_MODRM,
	/* 6B */	C_MODRM,
	/* 6C */	C_MODRM,
	/* 6D */	C_MODRM,
	/* 6E */	C_MODRM,
	/* 6F */	C_MODRM,

	/* 70 */	C_MODRM		|	CH_I8,
	/* 71 */	C_MODRM		|	CH_I8,
	/* 72 */	C_MODRM		|	CH_I8,
	/* 73 */	C_MODRM		|	CH_I8,
	/* 74 */	C_MODRM,
	/* 75 */	C_MODRM,
	/* 76 */	C_MODRM,
	/* 77 */	C_SINGLE,
	/* 78 */	C_SINGLE,
	/* 79 */	C_SINGLE,
	/* 7A */	C_SINGLE,
	/* 7B */	C_SINGLE,
	/* 7C */	C_MODRM,
	/* 7D */	C_MODRM,
	/* 7E */	C_MODRM,
	/* 7F */	C_MODRM,

	/* 80 */	C_DATA66					|	C_REL,
	/* 81 */	C_DATA66					|	C_REL,
	/* 82 */	C_DATA66					|	C_REL,
	/* 83 */	C_DATA66					|	C_REL,
	/* 84 */	C_DATA66					|	C_REL,
	/* 85 */	C_DATA66					|	C_REL,
	/* 86 */	C_DATA66					|	C_REL,
	/* 87 */	C_DATA66					|	C_REL,
	/* 88 */	C_DATA66					|	C_REL,
	/* 89 */	C_DATA66					|	C_REL,
	/* 8A */	C_DATA66					|	C_REL,
	/* 8B */	C_DATA66					|	C_REL,
	/* 8C */	C_DATA66					|	C_REL,
	/* 8D */	C_DATA66					|	C_REL,
	/* 8E */	C_DATA66					|	C_REL,
	/* 8F */	C_DATA66					|	C_REL,

	/* 90 */	C_MODRM,
	/* 91 */	C_MODRM,
	/* 92 */	C_MODRM,
	/* 93 */	C_MODRM,
	/* 94 */	C_MODRM,
	/* 95 */	C_MODRM,
	/* 96 */	C_MODRM,
	/* 97 */	C_MODRM,
	/* 98 */	C_MODRM,
	/* 99 */	C_MODRM,
	/* 9A */	C_MODRM,
	/* 9B */	C_MODRM,
	/* 9C */	C_MODRM,
	/* 9D */	C_MODRM,
	/* 9E */	C_MODRM,
	/* 9F */	C_MODRM,

	/* A0 */	C_SINGLE,
	/* A1 */	C_SINGLE,
	/* A2 */	C_SINGLE,

	/* A3 */	C_MODRM,
	/* A4 */	C_MODRM		|	CH_I8,
	/* A5 */	C_MODRM,
	/* A6 */	C_SINGLE,
	/* A7 */	C_SINGLE,
	/* A8 */	C_SINGLE,
	/* A9 */	C_SINGLE,
	/* AA */	C_SINGLE,
	/* AB */	C_MODRM,
	/* AC */	C_MODRM		|	CH_I8,
	/* AD */	C_MODRM,
	/* AE */	C_MODRM,
	/* AF */	C_MODRM,

	/* B0 */	C_MODRM,
	/* B1 */	C_MODRM,
	/* B2 */	C_MODRM,
	/* B3 */	C_MODRM,
	/* B4 */	C_MODRM,
	/* B5 */	C_MODRM,
	/* B6 */	C_MODRM,
	/* B7 */	C_MODRM,
	/* B8 */	C_SINGLE,
	/* B9 */	C_SINGLE,
	/* BA */	C_MODRM		|	CH_I8,
	/* BB */	C_MODRM,
	/* BC */	C_MODRM,
	/* BD */	C_MODRM,
	/* BE */	C_MODRM,
	/* BF */	C_MODRM,

	/* C0 */	C_MODRM,
	/* C1 */	C_MODRM,
	/* C2 */	C_MODRM		|	CH_I8		|	C_ALLOWREP,
	/* C3 */	C_MODRM,
	/* C4 */	C_MODRM		|	CH_I8,
	/* C5 */	C_MODRM		|	CH_I8,
	/* C6 */	C_MODRM		|	CH_I8,
	/* C7 */	C_MODRM,
	/* C8 */	C_SINGLE,
	/* C9 */	C_SINGLE,
	/* CA */	C_SINGLE,
	/* CB */	C_SINGLE,
	/* CC */	C_SINGLE,
	/* CD */	C_SINGLE,
	/* CE */	C_SINGLE,
	/* CF */	C_SINGLE,

	/* D0 */	C_MODRM,
	/* D1 */	C_MODRM,
	/* D2 */	C_MODRM,
	/* D3 */	C_MODRM,
	/* D4 */	C_MODRM,
	/* D5 */	C_MODRM,
	/* D6 */	C_MODRM,
	/* D7 */	C_MODRM,
	/* D8 */	C_MODRM,
	/* D9 */	C_MODRM,
	/* DA */	C_MODRM,
	/* DB */	C_MODRM,
	/* DC */	C_MODRM,
	/* DD */	C_MODRM,
	/* DE */	C_MODRM,
	/* DF */	C_MODRM,

	/* E0 */	C_MODRM,
	/* E1 */	C_MODRM,
	/* E2 */	C_MODRM,
	/* E3 */	C_MODRM,
	/* E4 */	C_MODRM,
	/* E5 */	C_MODRM,
	/* E6 */	C_MODRM,
	/* E7 */	C_MODRM,
	/* E8 */	C_MODRM,
	/* E9 */	C_MODRM,
	/* EA */	C_MODRM,
	/* EB */	C_MODRM,
	/* EC */	C_MODRM,
	/* ED */	C_MODRM,
	/* EE */	C_MODRM,
	/* EF */	C_MODRM,

	/* F0 */	C_MODRM,
	/* F1 */	C_MODRM,
	/* F2 */	C_MODRM,
	/* F3 */	C_MODRM,
	/* F4 */	C_MODRM,
	/* F5 */	C_MODRM,
	/* F6 */	C_MODRM,
	/* F7 */	C_MODRM,
	/* F8 */	C_MODRM,
	/* F9 */	C_MODRM,
	/* FA */	C_MODRM,
	/* FB */	C_MODRM,
	/* FC */	C_MODRM,
	/* FD */	C_MODRM,
	/* FE */	C_MODRM,
	/* FF */	C_SINGLE
};

DWORD op_table_ex[ 2 ][ 8 ] =
{
	/*	F6,0 */	C_MODRM		|	CH_I8,
	/*	F6,1 */	C_SINGLE					|	C_ILLEGAL,
	/*	F6,2 */	C_MODRM,
	/*	F6,3 */	C_MODRM,
	/*	F6,4 */	C_MODRM,
	/*	F6,5 */	C_MODRM,
	/*	F6,6 */	C_MODRM,
	/*	F6,7 */	C_MODRM,

	/*	F7,0 */	C_MODRM		|	CH_DATA66,
	/*	F7,1 */	C_SINGLE					|	C_ILLEGAL,
	/*	F7,2 */	C_MODRM,
	/*	F7,3 */	C_MODRM,
	/*	F7,4 */	C_MODRM,
	/*	F7,5 */	C_MODRM,
	/*	F7,6 */	C_MODRM,
	/*	F7,7 */	C_MODRM,
};

unsigned long __fastcall decodeOpcodeSize( LPCVOID Opcode )
{
	DWORD	Prefix = 0;
	DWORD	Modrm;

	DWORD	Opcodelen = 1;
	LPBYTE	cPtr = ( LPBYTE )Opcode;
	DWORD	Flags = op_table_1[ *cPtr ];

	for(;;)
	{
		switch( Flags & 0xFF )
		{
		case	C_SINGLE:
			return	Opcodelen;
			break;

		case	C_I8:
			return	Opcodelen + 1;
			break;

		case	C_I16:
			return	Opcodelen + 2;
			break;

		case	C_DATA66:
			return Opcodelen  + ( ( Prefix & C_PREFIX66 ) ? 2 : 4 );
			break;

		case	C_I32:
			return	Opcodelen + 4;
			break;

		case	C_ADDR67:
			return Opcodelen + ( ( Prefix & C_PREFIX67 ) ? 2 : 4 );
			break;

		case	C_ROPCODE:
			Flags = op_table_ex[ ( Flags >> 8 ) & 0xFF ][ ( cPtr[ 1 ] & 0x38 ) >> 3 ];
			continue;

		case	C_PREFIX:
			{
				while ( Flags = op_table_1[ *cPtr ] & C_PREFIXANY ) 
				{
					DWORD tPrefix = Flags & C_PREFIXANY;

					if( Prefix & tPrefix )
					{
						return 1;
					}

					Prefix |= tPrefix;
					cPtr++;
					if(	Flags & C_PREFIXREP	)
					{
						DWORD NextOp = cPtr[ 0 ];
						if( ( NextOp != 0x0F ) ? !( op_table_1[ NextOp ] & C_ALLOWREP ) : !( op_table_2[ cPtr[ 1 ] ] & C_ALLOWREP ) )						
						{
							return 1;
						}
					}
				}
				Opcodelen += cPtr - LPBYTE( Opcode );
				Flags = op_table_1[ *cPtr ];
				continue;
			}
			break;

		case	C_MODRM:
			Modrm = cPtr[ 1 ];
			if( Prefix & C_PREFIX67 )
			{
				switch ( Modrm >> 6 )
				{
				case 0:
					return ( ( Modrm & 7 ) == 6 ) ? Opcodelen + 3 : Opcodelen + 1;
					break;

				case 1:
					return Opcodelen + 2;
					break;

				case 2:
					return Opcodelen + 3;
					break;

				case 3:
					return	Opcodelen + 1;
				}
			}
			else
			{
				switch ( Modrm >> 6 )
				{
				case 0:
					switch( Modrm & 7 )
					{
					case	4:
						return ( ( cPtr[ 2 ] & 7 ) == 5 ) ? Opcodelen + 6 : Opcodelen + 2;

					case	5:
						return Opcodelen + 5;

					default:
						return Opcodelen + 1;
					}
					break;

				case 1:
					if( ( Modrm & 7 ) == 4 )
					{
						return	Opcodelen + 3;
					}
					else
					{
						return	Opcodelen + 2;
					}
					break;

				case 2:
					if( ( Modrm & 7 ) == 4 )
					{
						return Opcodelen + 6;
					}
					else
					{
						return	Opcodelen + 5;
					}
					break;

				case 3:
					return	Opcodelen + 1;
				}
			}
			break;

		case	C_MULTI:
			Flags = op_table_2[ * ++cPtr ];
			Opcodelen ++;
			continue;

		default:
			switch( ( Flags >> 4 ) & 0xFF )
			{
			case	C_I8:
				Opcodelen ++;
				break;

			case	C_I16:
				Opcodelen += 2;
				break;

			case	C_DATA66:
				Opcodelen  += ( ( Prefix & C_PREFIX66 ) ? 2 : 4 );
				break;

			case	C_I32:
				Opcodelen += 4;
				break;

			case	C_ADDR67:
				Opcodelen += ( ( Prefix & C_PREFIX67 ) ? 2 : 4 );
				break;
			}
			Flags &= ~0xF0;
			continue;
		}
	}
	return Opcodelen;
}

unsigned long __fastcall decodeOpcodeSize( LPCVOID Opcode, DETAILED_SIZE& Detailed )
{
	DWORD	Prefix = 0;
	DWORD	Modrm;

	DWORD	Opcodelen = 1;
	LPBYTE	cPtr = ( LPBYTE )Opcode;
	DWORD	Flags = op_table_1[ *cPtr ];
	
	Detailed.LowPart = 0;
	Detailed.HighPart = 0;
	Detailed.opcode = 1;

	for(;;)
	{
		switch( Flags & 0xFF )
		{
		case	C_SINGLE:
			return	Opcodelen;
			break;

		case	C_I8:
			return	Opcodelen + ( Detailed.data[0] = 1 );
			break;

		case	C_I16:
			return	Opcodelen + ( Detailed.data[0] = 2 );
			break;

		case	C_DATA66:
			return Opcodelen  + ( Detailed.data[0] = ( ( Prefix & C_PREFIX66 ) ? 2 : 4 ) );
			break;

		case	C_I32:
			return	Opcodelen + ( Detailed.data[0] = 4 );
			break;

		case	C_ADDR67:
			return Opcodelen + ( Detailed.data[0] = ( ( Prefix & C_PREFIX67 ) ? 2 : 4 ) );
			break;

		case	C_ROPCODE:

			Flags = op_table_ex[ ( Flags >> 8 ) & 0xFF ][ ( cPtr[ 1 ] & 0x38 ) >> 3 ];
			continue;

		case	C_PREFIX:
			{
				while ( Flags = op_table_1[ *cPtr ] & C_PREFIXANY ) 
				{
					DWORD tPrefix = Flags & C_PREFIXANY;

					if( Prefix & tPrefix )
					{
						return Detailed.opcode = 0, Detailed.prefix = 1;
					}

					Prefix |= tPrefix;
					cPtr++;

					if(	Flags & C_PREFIXREP	)
					{
						DWORD NextOp = cPtr[ 0 ];
						if( ( NextOp != 0x0F ) ? !( op_table_1[ NextOp ] & C_ALLOWREP ) : !( op_table_2[ cPtr[ 1 ] ] & C_ALLOWREP ) )						
						{
							return Detailed.opcode = 0, Detailed.prefix = 1;
						}
					}
				}
				Opcodelen += ( Detailed.prefix = cPtr - LPBYTE( Opcode ) );
				Flags = op_table_1[ *cPtr ];

				continue;
			}
			break;

		case	C_MODRM:
			Modrm = cPtr[ 1 ];
			Detailed.opcode ++;

			if( Prefix & C_PREFIX66 )
			{
				switch ( Modrm >> 6 )
				{
				case 0:
					return ( ( Modrm & 7 ) == 6 ) ? Detailed.data[1] = 2, Opcodelen + 3 : Detailed.HighPart >>= 16, Opcodelen + 1;
					break;

				case 1:
					return	Detailed.data[1] = 1, Opcodelen + 2;
					break;

				case 2:
					return	Detailed.data[1] = 2, Opcodelen + 3;
					break;

				case 3:
					return	Detailed.HighPart >>= 16, Opcodelen + 1;
				}
			}
			else
			{
				switch ( Modrm >> 6 )
				{
				case 0:
					switch( Modrm & 7 )
					{
					case	4:
						Detailed.opcode ++;
						return	( ( cPtr[ 2 ] & 7 ) == 5 ) ? Detailed.data[0] = 4, Opcodelen + 6 : Detailed.HighPart >>= 16, Opcodelen + 2;

					case	5:
						return	Detailed.data[0] = 4, Opcodelen + 5;

					default:
						return	Detailed.HighPart >>= 16, Opcodelen + 1;
					}
					break;

				case 1:
					if( ( Modrm & 7 ) == 4 )
					{
						Detailed.opcode ++;
						return	Detailed.data[0] = 1, Opcodelen + 3;
					}
					else
					{
						return	Detailed.data[0] = 1, Opcodelen + 2;
					}
					break;

				case 2:
					if( ( Modrm & 7 ) == 4 )
					{
						Detailed.opcode ++;
						return	Detailed.data[0] = 4, Opcodelen + 6;
					}
					else
					{
						return	Detailed.data[0] = 4, Opcodelen + 5;
					}
					break;

				case 3:
					return	Detailed.HighPart >>= 16, Opcodelen + 1;
				}
			}
			break;

		case	C_MULTI:
			Flags = op_table_2[ * ++cPtr ];
			Opcodelen ++;
			Detailed.opcode ++;
			continue;

		default:
			switch( ( Flags >> 4 ) & 0xFF )
			{
			case	C_I8:
				Opcodelen += ( Detailed.data[ 1 ] = 1 );
				break;

			case	C_I16:
				Opcodelen += ( Detailed.data[ 1 ] = 2 );
				break;

			case	C_DATA66:
				Opcodelen  += ( Detailed.data[ 1 ] = ( ( Prefix & C_PREFIX66 ) ? 2 : 4 ) );
				break;

			case	C_I32:
				Opcodelen += ( Detailed.data[ 1 ] = 4 );
				break;

			case	C_ADDR67:
				Opcodelen += ( Detailed.data[ 1 ] = ( ( Prefix & C_PREFIX67 ) ? 2 : 4 ) );
				break;
			}
			Flags &= ~0xF0;
			continue;
		}
	}
	return Opcodelen;
}