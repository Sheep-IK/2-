#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "resource3.h"
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Program 1";
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((unsigned int)time(NULL));
	HWND hwnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1050, 700, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, mdc;
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	static HBITMAP hBitmap, OldBitmap, HBitmap;
	static RECT window, rect, copy, rem;
	static DWORD dwRop = SRCCOPY;
	static BOOL Drag = FALSE, Command_f = FALSE, Command_a = FALSE, Command_b = FALSE, Selection = FALSE;
	static int downx, downy, upx, upy, Paint = 1, xratio = 100, yratio = 100, pcnt = 0, temp, dir, timer;
	static RECT paste[5] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
	static POINT mp;
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &window);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		mdc = CreateCompatibleDC(hdc); //더블버퍼링 위한 메모리 DC
		HBitmap = CreateCompatibleBitmap(hdc, window.right, window.bottom);//더블버퍼링 위한 비트맵
		SelectObject(mdc, (HBITMAP)HBitmap);//메모리 DC와 비트맵 연결

		memdc = CreateCompatibleDC(hdc); //사진 출력 위한 비트맵
		if (Paint == 1) hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		else hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		OldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);// 메모리DC에 리소스 로딩
		
		if (Command_f) {
			StretchBlt(mdc, 0, 0, 1050, 700, memdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, dwRop);//전체
		}
		else {
			StretchBlt(mdc, 0, 0, 1050, 700, memdc, 0, 0, 1050, 700, dwRop);//전체
			for (int i = 0; i < 5; i++) {//붙여넣기
				StretchBlt(mdc, paste[i].left, paste[i].top, paste[i].right, paste[i].bottom, memdc, copy.left, copy.top, copy.right - copy.left, copy.bottom - copy.top, dwRop);
			}
			//돋보기
			StretchBlt(mdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memdc, rect.left, rect.top, (rect.right - rect.left) * xratio / 100, (rect.bottom - rect.top) * yratio / 100, dwRop);
			FrameRect(mdc, &rect, hBrush);//테두리
		}

		BitBlt(hdc, 0, 0, window.right, window.bottom, mdc, 0, 0, SRCCOPY);//비트맵 복사

		SelectObject(memdc, OldBitmap);//비트맵, 메모리DC 삭제
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		DeleteObject(HBitmap);
		DeleteDC(mdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		switch (wParam) {
		case '1':
		case '2':
			Paint = wParam - 48;
			break;
		case 'e':
			xratio -= 10;
			if (xratio == 0) xratio = 100;
			yratio -= 10;
			if (yratio == 0) yratio = 100;
			break;
		case 's':
			xratio += 10;
			if (xratio == 200) xratio = 100;
			yratio += 10;
			if (yratio == 200) yratio = 100;
			break;
		case 'o':
			xratio = 100;
			yratio = 100;
			break;
		case 'c':
			copy = { rect.left, rect.top, rect.right, rect.bottom };
			break;
		case 'p':
			paste[pcnt++] = { rand() % (1050 - copy.right + copy.left), rand() % (700 - copy.bottom + copy.top), copy.right - copy.left, copy.bottom - copy.top};
			if (pcnt == 5) pcnt = 0;
			break;
		case 'f':
			Command_f = Command_f == TRUE ? FALSE : TRUE;
			break;
		case 'r':
			for (int i = 0; i < 5; i++) {
				paste[i] = { 0, 0, 0, 0 };
			}
			Paint = 1, xratio = yratio = 100, pcnt = 0;
			Drag = FALSE, Command_f = FALSE;
			copy = rect = { 0, 0, 0, 0 };
			break;
		case 'q':
			PostQuitMessage(0);
			break;
		case 'm':
			rect.left = rect.left - 5 < 0 ? 0 : rect.left - 5;
			rect.top = rect.top - 5 < 0 ? 0 : rect.top - 5;
			rect.right = rect.right + 5 > 1049 ? 1049 : rect.right + 5;
			rect.bottom = rect.bottom + 5 > 700 ? 700 : rect.bottom + 5;
			break;
		case 'n':
			rect.left += 5;
			rect.top += 5;
			rect.right -= 5;
			rect.bottom -= 5;
			if (rect.right < rect.left) rect.right = rect.left;
			if (rect.bottom < rect.top) rect.bottom = rect.top;
			break;
		case 'h':
			temp = copy.left;
			copy.left = copy.right;
			copy.right = temp;
			break;
		case 'v':
			temp = copy.top;
			copy.top = copy.bottom;
			copy.bottom = temp;
			break;
		case 'x':
			xratio += 10;
			if (xratio == 200) xratio = 100;
			break;
		case 'X':
			xratio -= 10;
			if (xratio == 0) xratio = 100;
			break;
		case 'y':
			yratio += 10;
			if (yratio == 200) yratio = 100;
			break;
		case 'Y':
			yratio -= 10;
			if (yratio == 0) yratio = 100;
			break;
		case 'a':
			SetTimer(hwnd, 100, 1, NULL);
			Command_a = TRUE;
			timer = 30; dir = 1;
			break;
		case 'b':
			if (Command_b) {
				Command_b = FALSE;
				rect = { rem.left, rem.top, rem.right, rem.bottom };
				KillTimer(hwnd, 1);
			}
			else {
				SetTimer(hwnd, 100, 1, NULL);
				timer = 30; dir = 1;
				Command_b = TRUE;
				rem = { rect.left, rect.top, rect.right, rect.bottom };
			}
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_TIMER:
		if (Command_a) {
			timer--;
			if (timer == 0) {
				dir = dir + 1 == 5 ? 1 : dir + 1;
				timer = 30;
			}
			if (dir % 2 == 0) {
				rect.top += 5;
				rect.bottom += 5;
			}
			else if (dir == 1) {
				rect.left += 5;
				rect.right += 5;
			}
			else if (dir == 3) {
				rect.left -= 5;
				rect.right -= 5;
			}
			if (rect.left < 0 || rect.top < 0 || rect.right > 1050 || rect.bottom > 700) {
				Command_a = FALSE;
				KillTimer(hwnd, 1);
			}
		}
		else if (Command_b) {
			timer--;
			if (timer == 0) {
				dir = dir == 1 ? 2 : 1;
				timer = 30;
			}
			if (dir == 1) {
				rect.left -= 5;
				rect.top -= 5;
				rect.right += 5;
				rect.bottom += 5;
			}
			else {
				rect.left += 5;
				rect.top += 5;
				rect.right -= 5;
				rect.bottom -= 5;
			}
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_UP:
			rect.top -= 10;
			rect.bottom -= 10;
			break;
		case VK_LEFT:
			rect.left -= 10;
			rect.right -= 10;
			break;
		case VK_DOWN:
			rect.top += 10;
			rect.bottom += 10;
			break;
		case VK_RIGHT:
			rect.left += 10;
			rect.right += 10;
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
	}
		break;
	case WM_RBUTTONDOWN:
		mp = { LOWORD(lParam), HIWORD(lParam) };
		if (PtInRect(&rect, mp)) Selection = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < 5; i++) {
			paste[i] = { 0, 0, 0, 0 };
		}
		downx = LOWORD(lParam);
		downy = HIWORD(lParam);
		Drag = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hwnd);
		if (Drag) {
			upx = LOWORD(lParam);
			upy = HIWORD(lParam);
			rect.left = downx < upx ? downx : upx;
			rect.top = downy < upy ? downy : upy;
			rect.right = downx > upx ? downx : upx;
			rect.bottom = downy > upy ? downy : upy;
			InvalidateRect(hwnd, NULL, FALSE);
		}
		else if (Selection) {
			upx = LOWORD(lParam);
			upy = HIWORD(lParam);
			rect.left += upx - mp.x;
			rect.top += upy - mp.y;
			rect.right += upx - mp.x;
			rect.bottom += upy - mp.y;
			InvalidateRect(hwnd, NULL, FALSE);
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONUP:
		Selection = FALSE;
		break;
	case WM_LBUTTONUP:
		upx = LOWORD(lParam);
		upy = HIWORD(lParam);
		rect.left = downx < upx ? downx : upx;
		rect.top = downy < upy ? downy : upy;
		rect.right = downx > upx ? downx : upx;
		rect.bottom = downy > upy ? downy : upy;
		Drag = FALSE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}