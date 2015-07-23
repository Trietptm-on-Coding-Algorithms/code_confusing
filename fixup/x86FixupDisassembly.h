#ifndef X86FIXUPDISASSEMBLY_H
#define X86FIXUPDISASSEMBLY_H
//---------------------------------------------------------------------------------------------
#include <Windows.h>
//
//	指令特征,	0-3 位 指令的第一操作对像，		4-7位 指令的第二操作对像，		8-15 扩展表索引，	16-31 特征值
//
#define C_PREFIXLOCK    0x00100000		//	加锁前缀
#define C_PREFIX66      0x00200000		//	操作数大小修饰前缀(16/32位)
#define C_PREFIX67      0x00400000		//	地址大小修饰前缀(16/32位)
#define C_PREFIXREP     0x00800000		//	重复前缀
#define C_PREFIXSEG     0x01000000		//	段寄存器前缀
#define C_PREFIXANY		(C_PREFIX66+C_PREFIX67+C_PREFIXLOCK+C_PREFIXREP+C_PREFIXSEG)

#define C_ALLOWREP		0x02000000		//	支持重复前载

#define C_REL			0x10000000		//	这是跳转指令jxx或者call
#define C_STOP			0x20000000		//	这是回跳指令，ret或者jmp
#define C_WORD			0x40000000
#define C_ILLEGAL		0x80000000

#define C_SINGLE		0x00000000		//	简单指令，不带有 MOD/RM 字节
#define C_I8			0x00000001		//	操作码中数据大小的位字段
#define C_I16			0x00000002
#define C_DATA66		0x00000003		//	带有指令字节长的数据项
#define C_I32			0x00000004
#define C_ADDR67		0x00000005		//	带有指令字节长的地址项
#define C_ROPCODE		0x00000006		//	直接数被	REG/OPCODE	影响，需要查询扩展表
#define C_PREFIX		0x00000007		//	前缀
#define C_MODRM			0x00000008		//	带有 MOD/RM 字节
#define C_MULTI			0x00000009		//	多字节指令，由下一个字节决定指令的信息

#define CH_I8			(	C_I8 << 4		)
#define CH_I16			(	C_I16 << 4		)
#define CH_I32			(	C_I32 << 4		)
#define CH_DATA66		(	C_DATA66 << 4	)
#define CH_ADDR67		(	C_ADDR67 << 4	)

#define C_ROPCODEF6		(	0x00000000	|	C_ROPCODE)
#define C_ROPCODEF7		(	0x00000100	|	C_ROPCODE)

typedef struct _DetailedOfSize
{
	union
	{
		struct
		{
			USHORT	prefix;
			USHORT	opcode;
		};
		ULONG		LowPart;
	};

	union
	{
		USHORT		data[ 2 ];
		ULONG		HighPart;		
	};
}DETAILED_SIZE, FAR* LPDETAILED_SIZE;
unsigned long __fastcall decodeOpcodeSize( LPCVOID Opcode );
unsigned long __fastcall decodeOpcodeSize( LPCVOID Opcode, DETAILED_SIZE& Detailed );
#endif