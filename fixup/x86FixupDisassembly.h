#ifndef X86FIXUPDISASSEMBLY_H
#define X86FIXUPDISASSEMBLY_H
//---------------------------------------------------------------------------------------------
#include <Windows.h>
//
//	ָ������,	0-3 λ ָ��ĵ�һ��������		4-7λ ָ��ĵڶ���������		8-15 ��չ��������	16-31 ����ֵ
//
#define C_PREFIXLOCK    0x00100000		//	����ǰ׺
#define C_PREFIX66      0x00200000		//	��������С����ǰ׺(16/32λ)
#define C_PREFIX67      0x00400000		//	��ַ��С����ǰ׺(16/32λ)
#define C_PREFIXREP     0x00800000		//	�ظ�ǰ׺
#define C_PREFIXSEG     0x01000000		//	�μĴ���ǰ׺
#define C_PREFIXANY		(C_PREFIX66+C_PREFIX67+C_PREFIXLOCK+C_PREFIXREP+C_PREFIXSEG)

#define C_ALLOWREP		0x02000000		//	֧���ظ�ǰ��

#define C_REL			0x10000000		//	������תָ��jxx����call
#define C_STOP			0x20000000		//	���ǻ���ָ�ret����jmp
#define C_WORD			0x40000000
#define C_ILLEGAL		0x80000000

#define C_SINGLE		0x00000000		//	��ָ������� MOD/RM �ֽ�
#define C_I8			0x00000001		//	�����������ݴ�С��λ�ֶ�
#define C_I16			0x00000002
#define C_DATA66		0x00000003		//	����ָ���ֽڳ���������
#define C_I32			0x00000004
#define C_ADDR67		0x00000005		//	����ָ���ֽڳ��ĵ�ַ��
#define C_ROPCODE		0x00000006		//	ֱ������	REG/OPCODE	Ӱ�죬��Ҫ��ѯ��չ��
#define C_PREFIX		0x00000007		//	ǰ׺
#define C_MODRM			0x00000008		//	���� MOD/RM �ֽ�
#define C_MULTI			0x00000009		//	���ֽ�ָ�����һ���ֽھ���ָ�����Ϣ

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