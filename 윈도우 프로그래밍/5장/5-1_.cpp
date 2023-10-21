#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "resource.h"
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
	hwnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1150, 800, NULL, (HMENU)NULL, hInstance, NULL);
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
	HDC hdc, memdc;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hBitmap, OldBitmap;
	static RECT window, rect;
	static BOOL Command_a = FALSE;
	static int mx, my, Selection = 0, location[4] = { 0, 0, 0, 0 };
	static double  x, y, ratio;
	static DWORD dwRop = SRCCOPY;
	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Command_a) {
			memdc = CreateCompatibleDC(hdc); //--- 3) 메모리 DC 만들기
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			OldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);// 메모리DC에 리소스 로딩

			ratio = 350 / x;
			if (location[0] >= 0) {
				StretchBlt(hdc, location[0], 0, x - location[0], y, memdc, 0, 0, (x - location[0]) * ratio, 233, dwRop);
				StretchBlt(hdc, 0, 0, location[0], y, memdc, (x - location[0]) * ratio, 0, location[0] * ratio, 233, dwRop);
			}
			else if (location[0] < 0) {
				StretchBlt(hdc, 0, 0, x + location[0], y, memdc, -location[0] * ratio, 0, (x + location[0]) * ratio, 233, dwRop);
				StretchBlt(hdc, x + location[0], 0, -location[0], y, memdc, 0, 0, -location[0] * ratio, 233, dwRop);
			}

			ratio = 350 / (window.right - x);
			if (location[1] >= 0) {
				StretchBlt(hdc, x + location[1], 0, window.right - x - location[1], y, memdc, 0, 0, (window.right - x - location[1]) * ratio, 233, dwRop);
				StretchBlt(hdc, x, 0, location[1], y, memdc, (window.right - x - location[1]) * ratio, 0, location[1] * ratio, 233, dwRop);
			}
			else if (location[1] < 0) {
				StretchBlt(hdc, x, 0, window.right - x + location[1], y, memdc, -location[1] * ratio, 0, (window.right - x + location[1]) * ratio, 233, dwRop);
				StretchBlt(hdc, window.right + location[1], 0, -location[1], y, memdc, 0, 0, -location[1] * ratio, 233, dwRop);
			}

			ratio = 350 / x;
			if (location[2] >= 0) {
				StretchBlt(hdc, location[2], y, x - location[2], window.bottom - y, memdc, 0, 0, (x - location[2]) * ratio, 233, dwRop);
				StretchBlt(hdc, 0, y, location[2], window.bottom - y, memdc, (x - location[2]) * ratio, 0, location[2] * ratio, 233, dwRop);
			}
			else if (location[2] < 0) {
				StretchBlt(hdc, 0, y, x + location[2], window.bottom - y, memdc, -location[2] * ratio, 0, (x + location[2]) * ratio, 233, dwRop);
				StretchBlt(hdc, x + location[2], y, -location[2], window.bottom - y, memdc, 0, 0, -location[2] * ratio, 233, dwRop);
			}

			ratio = 350 / (window.right - x);
			if (location[3] >= 0) {
				StretchBlt(hdc, x + location[3], y, window.right - x - location[3], window.bottom - y, memdc, 0, 0, (window.right - x - location[3]) * ratio, 233, dwRop);
				StretchBlt(hdc, x, y, location[3], window.bottom - y, memdc, (window.right - x - location[3]) * ratio, 0, location[3] * ratio, 233, dwRop);
			}
			else if (location[3] < 0) {
				StretchBlt(hdc, x, y, window.right - x + location[3], window.bottom - y, memdc, -location[3] * ratio, 0, (window.right - x + location[3]) * ratio, 233, dwRop);
				StretchBlt(hdc, window.right + location[3], y, -location[3], window.bottom - y, memdc, 0, 0, -location[3] * ratio, 233, dwRop);
			}

			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			switch (Selection) {
			case 1:
				rect = { 0, 0, (int)x, (int)y };
				break;
			case 2:
				rect = { (int)x, 0, window.right, (int)y };
				break;
			case 3:
				rect = { 0, (int)y, (int)x, window.bottom };
				break;
			case 4:
				rect = { (int)x, (int)y, window.right, window.bottom };
				break;
			}
			FrameRect(hdc, &rect, hBrush);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			SelectObject(memdc, OldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(memdc);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		switch (wParam) {
		case 'a':
			Command_a = TRUE;
			GetClientRect(hwnd, &window);
			x = window.right; y = window.bottom;
			break;
		case 'r':
			dwRop = dwRop == SRCCOPY ? NOTSRCCOPY : SRCCOPY;
			break;
		case '+':
			switch (Selection) {
			case 1:
				y += 10; x += 10;
				break;
			case 2:
				y += 10; x -= 10;
				break;
			case 3:
				y -= 10; x += 10;
				break;
			case 4:
				y -= 10; x -= 10;
				break;
			}
			break;
		case '=':
			switch (Selection) {
			case 1:
				y -= 10; x -= 10;
				break;
			case 2:
				y -= 10; x += 10;
				break;
			case 3:
				y += 10; x -= 10;
				break;
			case 4:
				y += 10; x += 10;
				break;
			}
			break;
		case 'p':
			for (int i = 1; i <= 4; i++) {
				if (i % 2 == 1) location[i - 1] = location[i - 1] - 10 < -x ? 0 : location[i - 1] - 10;
				if (i % 2 == 0) location[i - 1] = location[i - 1] - 10 < x - window.right ? 0 : location[i - 1] - 10;
			}
			break;
		case 's':
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 0, 0, window.right, window.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			x = window.right; y = window.bottom;
			rect = { 0, 0, 0, 0 };
			Command_a = FALSE;
			Selection = location[0] = location[1] = location[2] = location[3] = 0;
			dwRop = SRCCOPY;
			break;
		case 'q':
			PostQuitMessage(0);
			break;
		case VK_LEFT:
		case VK_RIGHT:
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (Selection % 2 == 1) location[Selection - 1] = location[Selection - 1] - 10 < -x ? 0 : location[Selection - 1] - 10;
			else location[Selection - 1] = location[Selection - 1] - 10 < x - window.right ? 0 : location[Selection - 1] - 10;
			break;
		case VK_RIGHT:
			if (Selection % 2 == 1) location[Selection - 1] = location[Selection - 1] + 10 > x ? 0 : location[Selection - 1] + 10;
			else location[Selection - 1] = location[Selection - 1] + 10 > window.right - x ? 0 : location[Selection - 1] + 10;
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (mx < x && my < y) Selection = 1;
		else if (mx > x && my < y) Selection = 2;
		else if (mx < x && my > y) Selection = 3;
		else if (mx > x && my > y) Selection = 4;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}