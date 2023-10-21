#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "resource2.h"
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
	HDC hdc, memdc, mdc;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hBitmap, OldBitmap, HBitmap;
	static RECT rt, rect;
	static POINT pt;
	static DWORD dwRop = SRCCOPY;
	static BOOL Drag = FALSE, Timer = FALSE;
	static int Paint = 0, view = 0, x, y, dir, t, cnt = 0, downx, downy, upx, upy, timer, dis, voidx, voidy, remx, remy, temp,
		view12[3] = {0, 1, 2},
		divide3[3][3] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
		divide4[4][4] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
		divide5[5][5] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &rt);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (Paint != 0) {
			mdc = CreateCompatibleDC(hdc); //--- 3) 메모리 DC 만들기
			HBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			SelectObject(mdc, (HBITMAP)HBitmap);

			memdc = CreateCompatibleDC(hdc); //--- 3) 메모리 DC 만들기
			if (Paint == 1) hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			else hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
			OldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);// 메모리DC에 리소스 로딩

			FillRect(mdc, &rt, WHITE_BRUSH);
			switch (view) {
			case 0:		//전체보기
				StretchBlt(mdc, 0, 0, 1150, 800, memdc, 0, 0, 1050, 700, dwRop);
				break;
			case 1:		//세로로 등분
				for (int i = 0; i < 3; i++) {
					if (Timer) {
						if (remx == i) StretchBlt(mdc, 383 * i + dir * timer * 38, 0, 383, 800, memdc, 350 * view12[i], 0, 350, 700, dwRop);
						else if (voidx == i) StretchBlt(mdc, 383 * i - dir * timer * 38, 0, 383, 800, memdc, 350 * view12[i], 0, 350, 700, dwRop);
						else StretchBlt(mdc, 383 * i, 0, 383, 800, memdc, 350 * view12[i], 0, 350, 700, dwRop);
					}
					else StretchBlt(mdc, 383 * i, 0, 383, 800, memdc, 350 * view12[i], 0, 350, 700, dwRop);
				}
				break;
			case 2:		//가로로 등분
				for (int i = 0; i < 3; i++) {
					if (Timer) {
						if (remx == i) StretchBlt(mdc, 0, 266 * i + dir * timer * 26, 1150, 266, memdc, 0, 233 * view12[i], 1050, 233, dwRop);
						else if (voidx == i) StretchBlt(mdc, 0, 266 * i - dir * timer * 26, 1150, 266, memdc, 0, 233 * view12[i], 1050, 233, dwRop);
						else StretchBlt(mdc, 0, 266 * i, 1150, 266, memdc, 0, 233 * view12[i], 1050, 233, dwRop);
					}
					else StretchBlt(mdc, 0, 266 * i, 1150, 266, memdc, 0, 233 * view12[i], 1050, 233, dwRop);
				}
				break;
			case 3:		//3 * 3
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {//383 266 / 350 233
						x = divide3[i][j] % 3; y = divide3[i][j] / 3;
						if (Timer && remy == i && remx == j) {
							switch (dir) {
							case 0:
								StretchBlt(mdc, 383 * j, 266 * i - dis * timer, 383, 266, memdc, 350 * x, 233 * y, 350, 233, dwRop);
								break;
							case 1:
								StretchBlt(mdc, 383 * j - dis * timer, 266 * i, 383, 266, memdc, 350 * x, 233 * y, 350, 233, dwRop);
								break;
							case 2:
								StretchBlt(mdc, 383 * j, 266 * i + dis * timer, 383, 266, memdc, 350 * x, 233 * y, 350, 233, dwRop);
								break;
							case 3:
								StretchBlt(mdc, 383 * j + dis * timer, 266 * i, 383, 266, memdc, 350 * x, 233 * y, 350, 233, dwRop);
								break;
							}
						}
						else if (divide3[i][j] != 0) StretchBlt(mdc, 383 * j, 266 * i, 383, 266, memdc, 350 * x, 233 * y, 350, 233, dwRop);
					}
				}
				break;
			case 4:		//4 * 4
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {//287 200 / 262 175
						x = divide4[i][j] % 4; y = divide4[i][j] / 4;
						if (Timer && remy == i && remx == j) {
							switch (dir) {
							case 0:
								StretchBlt(mdc, 287 * j, 200 * i - dis * timer, 287, 200, memdc, 262 * x, 175 * y, 262, 175, dwRop);
								break;
							case 1:
								StretchBlt(mdc, 287 * j - dis * timer, 200 * i, 287, 200, memdc, 262 * x, 175 * y, 262, 175, dwRop);
								break;
							case 2:
								StretchBlt(mdc, 287 * j, 200 * i + dis * timer, 287, 200, memdc, 262 * x, 175 * y, 262, 175, dwRop);
								break;
							case 3:
								StretchBlt(mdc, 287 * j + dis * timer, 200 * i, 287, 200, memdc, 262 * x, 175 * y, 262, 175, dwRop);
								break;
							}
						}
						else if (divide4[i][j] != 0) StretchBlt(mdc, 287 * j, 200 * i, 287, 200, memdc, 262 * x, 175 * y, 262, 175, dwRop);
					}
				}
				break;
			case 5:		//5 * 5
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {//230 160 / 210 140
						x = divide5[i][j] % 5; y = divide5[i][j] / 5;
						if (Timer && remy == i && remx == j) {
							switch (dir) {
							case 0:
								StretchBlt(mdc, 230 * j, 160 * i - dis * timer, 230, 160, memdc, 210 * x, 140 * y, 210, 140, dwRop);
								break;
							case 1:
								StretchBlt(mdc, 230 * j - dis * timer, 160 * i, 230, 160, memdc, 210 * x, 140 * y, 210, 140, dwRop);
								break;
							case 2:
								StretchBlt(mdc, 230 * j, 160 * i + dis * timer, 230, 160, memdc, 210 * x, 140 * y, 210, 140, dwRop);
								break;
							case 3:
								StretchBlt(mdc, 230 * j + dis * timer, 160 * i, 230, 160, memdc, 210 * x, 140 * y, 210, 140, dwRop);
								break;
							}
						}
						else if (divide5[i][j] != 0) StretchBlt(mdc, 230 * j, 160 * i, 230, 160, memdc, 210 * x, 140 * y, 210, 140, dwRop);
					}
				}
				break;
			}
			BitBlt(hdc, 0, 0, rt.right, rt.bottom, mdc, 0, 0, SRCCOPY);
			SelectObject(memdc, OldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(memdc);
			DeleteObject(HBitmap);
			DeleteDC(mdc);
		}
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
	{
		hdc = GetDC(hwnd);
		switch (LOWORD(wParam)) { //--- 선택된 메뉴의 id가 저장되어 있다.
		case ID_PHOTO_KARINA:
			Paint = 1;
			break;
		case ID_PHOTO_WINTER:
			Paint = 2;
			break;
		case ID_DIVIDE_3:
		case ID_GAME_START:
		{
			if (LOWORD(wParam) == ID_GAME_START && Paint == 0) Paint = 1;
			view = 3;
			voidx = voidy = cnt = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					divide3[i][j] = cnt++;
				}
			}
			for (int i = 0; i < 50; i++) {
				t = 0;
				while (t == 0) {
					dir = rand() % 4;
					switch (dir) {
					case 0:		//상
						if (voidy < 2) {
							divide3[voidy][voidx] = divide3[voidy + 1][voidx];
							divide3[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 2) {
							divide3[voidy][voidx] = divide3[voidy][voidx + 1];
							divide3[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide3[voidy][voidx] = divide3[voidy - 1][voidx];
							divide3[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide3[voidy][voidx] = divide3[voidy][voidx - 1];
							divide3[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
			}
		}
		break;
		case ID_DIVIDE_4:
		{
			view = 4;
			voidx = voidy = cnt = 0;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					divide4[i][j] = cnt++;
				}
			}
			for (int i = 0; i < 75; i++) {
				t = 0;
				while (t == 0) {
					dir = rand() % 4;
					switch (dir) {
					case 0:		//상
						if (voidy < 3) {
							divide4[voidy][voidx] = divide4[voidy + 1][voidx];
							divide4[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 3) {
							divide4[voidy][voidx] = divide4[voidy][voidx + 1];
							divide4[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide4[voidy][voidx] = divide4[voidy - 1][voidx];
							divide4[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide4[voidy][voidx] = divide4[voidy][voidx - 1];
							divide4[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
			}
		}
		break;
		case ID_DIVIDE_5:
		{
			view = 5;
			voidx = voidy = cnt = 0;
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					divide5[i][j] = cnt++;
				}
			}
			for (int i = 0; i < 100; i++) {
				t = 0;
				while (t == 0) {
					dir = rand() % 4;
					switch (dir) {
					case 0:		//상
						if (voidy < 4) {
							divide5[voidy][voidx] = divide5[voidy + 1][voidx];
							divide5[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 4) {
							divide5[voidy][voidx] = divide5[voidy][voidx + 1];
							divide5[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide5[voidy][voidx] = divide5[voidy - 1][voidx];
							divide5[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide5[voidy][voidx] = divide5[voidy][voidx - 1];
							divide5[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
			}
		}
		break;
		case ID_GAME_ORIGINAL:
			view = 0;
			break;
		case ID_GAME_INVERSION:
			dwRop = dwRop == SRCCOPY ? NOTSRCCOPY : SRCCOPY;
			break;
		case ID_GAME_QUIT:
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
	}
	break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		switch (wParam) {
		case 's':
		{
			if (Paint == 0) Paint = 1;
			view = 3;
			voidx = voidy = cnt = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					divide3[i][j] = cnt++;
				}
			}
			for (int i = 0; i < 50; i++) {
				t = 0;
				while (t == 0) {
					dir = rand() % 4;
					switch (dir) {
					case 0:		//상
						if (voidy < 2) {
							divide3[voidy][voidx] = divide3[voidy + 1][voidx];
							divide3[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 2) {
							divide3[voidy][voidx] = divide3[voidy][voidx + 1];
							divide3[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide3[voidy][voidx] = divide3[voidy - 1][voidx];
							divide3[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide3[voidy][voidx] = divide3[voidy][voidx - 1];
							divide3[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
			}
		}
		break;
		case 'f':
			view = 0;
			break;
		case 'q':
			PostQuitMessage(0);
			break;
		case 'v':
			view = 1;
			for (int i = 0; i < 3; i++) {
				view12[i] = i;
			}
			break;
		case 'h':
			view = 2;
			for (int i = 0; i < 3; i++) {
				view12[i] = i;
			}
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		downx = LOWORD(lParam); downy = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		upx = LOWORD(lParam);
		upy = HIWORD(lParam);
		if (view == 3 || view == 4 || view == 5) {
			if (abs(downx - upx) > abs(downy - upy)) {
				dir = downx - upx > 0 ? 1 : 3;
				dis = 115 / view;
			}
			else {
				dir = downy - upy > 0 ? 0 : 2;
				dis = 80 / view;
			}
			switch (dir) {
			case 0:		//상
				if (voidy < view - 1) {
					timer = 0;
					SetTimer(hwnd, 100, 1, NULL);
					Timer = TRUE;
					remx = voidx; remy = voidy + 1;
				}
				break;
			case 1:		//좌
				if (voidx < view - 1) {
					timer = 0;
					SetTimer(hwnd, 100, 1, NULL);
					Timer = TRUE;
					remx = voidx + 1; remy = voidy;
				}
				break;
			case 2:		//하
				if (voidy > 0) {
					timer = 0;
					SetTimer(hwnd, 100, 1, NULL);
					Timer = TRUE;
					remx = voidx; remy = voidy - 1;
				}
				break;
			case 3:		//우
				if (voidx > 0) {
					timer = 0;
					SetTimer(hwnd, 100, 1, NULL);
					Timer = TRUE;
					remx = voidx - 1; remy = voidy;
				}
				break;
			}
		}
		else if (view == 1) {
			pt = { downx, downy };
			for (int i = 0; i < 3; i++) {
				rect = { 383 * i, 0, 383 * (i + 1), 800 };
				if (PtInRect(&rect, pt)) {
					remx = i;
					break;
				}
			}
			pt = { upx, upy };
			for (int i = 0; i < 3; i++) {
				rect = { 383 * i, 0, 383 * (i + 1), 800 };
				if (PtInRect(&rect, pt)) {
					voidx = i;
					break;
				}
			}
			if (remx != voidx) {
				timer = 0;
				SetTimer(hwnd, 100, 1, NULL);
				Timer = TRUE;
				dir = (voidx - remx) / abs(voidx - remx);
			}
		}
		else if (view == 2) {
			pt = { downx, downy };
			for (int i = 0; i < 3; i++) {
				rect = { 0, 266 * i, 1150, 266 * (i + 1) };
				if (PtInRect(&rect, pt)) {
					remx = i;
					break;
				}
			}
			pt = { upx, upy };
			for (int i = 0; i < 3; i++) {
				rect = { 0, 266 * i, 1150, 266 * (i + 1) };
				if (PtInRect(&rect, pt)) {
					voidx = i;
					break;
				}
			}
			if (remx != voidx) {
				timer = 0;
				SetTimer(hwnd, 100, 1, NULL);
				Timer = TRUE;
				dir = (voidx - remx) / abs(voidx - remx);
			}
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_TIMER:
		if (Timer) {
			timer++;
			if (timer == 10) {
				switch (view) {
				case 1:
				case 2:
					temp = view12[remx];
					view12[remx] = view12[voidx];
					view12[voidx] = temp;
					break;
				case 3:
				{
					switch (dir) {
					case 0:		//상
						if (voidy < 2) {
							divide3[voidy][voidx] = divide3[voidy + 1][voidx];
							divide3[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 2) {
							divide3[voidy][voidx] = divide3[voidy][voidx + 1];
							divide3[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide3[voidy][voidx] = divide3[voidy - 1][voidx];
							divide3[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide3[voidy][voidx] = divide3[voidy][voidx - 1];
							divide3[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
				break;
				case 4:
				{
					switch (dir) {
					case 0:		//상
						if (voidy < 3) {
							divide4[voidy][voidx] = divide4[voidy + 1][voidx];
							divide4[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 3) {
							divide4[voidy][voidx] = divide4[voidy][voidx + 1];
							divide4[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide4[voidy][voidx] = divide4[voidy - 1][voidx];
							divide4[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide4[voidy][voidx] = divide4[voidy][voidx - 1];
							divide4[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
				break;
				case 5:
				{
					switch (dir) {
					case 0:		//상
						if (voidy < 4) {
							divide5[voidy][voidx] = divide5[voidy + 1][voidx];
							divide5[++voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 1:		//좌
						if (voidx < 4) {
							divide5[voidy][voidx] = divide5[voidy][voidx + 1];
							divide5[voidy][++voidx] = 0;
							t = 1;
						}
						break;
					case 2:		//하
						if (voidy > 0) {
							divide5[voidy][voidx] = divide5[voidy - 1][voidx];
							divide5[--voidy][voidx] = 0;
							t = 1;
						}
						break;
					case 3:		//우
						if (voidx > 0) {
							divide5[voidy][voidx] = divide5[voidy][voidx - 1];
							divide5[voidy][--voidx] = 0;
							t = 1;
						}
						break;
					}
				}
				break;
				}
				KillTimer(hwnd, 1);
				Timer = FALSE;
			}
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}