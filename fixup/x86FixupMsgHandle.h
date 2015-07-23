#ifndef X86IFXUPMSGHANDLE_H
#define X86IFXUPMSGHANDLE_H
#include "main.h"
//---------------------------------------------------------------------------------------------
class x86FixupMsgHandle
{
public:
	bool InstanceMsgLoop(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
#endif
