// TestCPP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test_variableTemplate.h"
#include "test_new_delete.h"
#include <windows.h>
#pragma comment(lib,"Winmm.lib")
#include <iostream>
#include "test_regex.h"
#include <timeapi.h>
#include "test_compiler.h"

union MyColor
{
    struct
    {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };
    DWORD data;
};
bool iscolor ( int a, int b )
{
    return abs ( a - b ) < 40;
}
int _tmain ( int argc, _TCHAR* argv[] )
{
	test_compiler testcompiler;
	test_regex testregex;
    test_new_delete testnewdelete;
    test_variableTemplate testvariableTemplate;
    return 0;
}

void tpp()
{
	unsigned int t = 0;
	unsigned int frame = 0;
	unsigned int last = timeGetTime();
	unsigned int dpass = 0;
	unsigned int floor = 0;
	static const int gx=970;
	char buffer[2048];
	while ( floor < 1000 )
	{
		t = timeGetTime();
		frame = t - last;
		last = t;
		POINT pt;
		GetCursorPos ( &pt );
		HWND activeWindow = GetActiveWindow();
		GetWindowTextA ( activeWindow, buffer, 2048 );
		HDC dc = GetDC ( activeWindow );
		//COLORREF color = GetPixel ( dc, pt.x, pt.y );

		//136,210,249
		// 116,179,224
		static const unsigned int d = 3000;
		if ( dpass > d )
		{
			COLORREF color = GetPixel ( dc, 970, pt.y );
			MyColor mycolor;
			mycolor.data = color;
			if (
				iscolor ( mycolor.r, 135 )
				&& iscolor ( mycolor.g, 200 )
				&& iscolor ( mycolor.b, 230 )
				)
			{
				std::cout
					<< "Text:" << buffer
					<< "X:" << pt.x
					<< ",Y:" << pt.y
					<< ",Color:" << color
					<< ",A:" << ( int ) mycolor.a
					<< ",R:" << ( int ) mycolor.r
					<< ",G:" << ( int ) mycolor.g
					<< ",B:" << ( int ) mycolor.b
					<< std::endl;
				dpass = 0;
				DWORD param = MAKEWORD ( pt.x, pt.y );
				INPUT input = {0};
				input.type = INPUT_MOUSE;
				////input.mi.dx = pt.x;
				input.mi.dx = 970;
				input.mi.dy = pt.y;
				input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				input.mi.time = GetTickCount();
				input.mi.dwExtraInfo = GetMessageExtraInfo();
				SetCursorPos(970,pt.y);
				SendInput ( 1, &input, sizeof ( input ) );

				input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				input.mi.time = GetTickCount();
				input.mi.dwExtraInfo = GetMessageExtraInfo();
				SendInput ( 1, &input, sizeof ( input ) );
				floor++;
			}
		}
		dpass += frame;
		//黄色,8703737
		Sleep ( 1 );
	}
}