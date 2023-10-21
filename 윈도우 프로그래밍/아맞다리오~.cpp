#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


typedef struct figure
{
	int x1;
	int x2;
	int y1;
	int y2;

} figure;

typedef struct color
{
	int R;
	int G;
	int B;
} color;

int goalspotx;
int goalspoty;
int ballspeed = 70;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 1";
LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)wndProc;
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{


	HDC hdc;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	RECT rect;
	static RECT rectView;
	static figure mainball = {400, 410, 300, 310};
	static color maincolor = { 102, 255, 102 }; // R / G / B
	static double middleX = (mainball.x1 + mainball.x2) / 2;
	static double middleY = (mainball.y1 + mainball.y2) / 2;
	static double dis;
	static double ddis = dis*dis;

	ddis = goalspotx* goalspotx + goalspoty * goalspoty;
	dis = sqrt(ddis);

	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, ballspeed, NULL);
		break;

	//case WM_MOUSEMOVE:


	//	InvalidateRect(hwnd, NULL, TRUE);
	//	break;

	//case WM_LBUTTONDOWN:
	//	goalspotx = LOWORD(lParam);
	//	goalspoty = HIWORD(lParam);
	//	if (goalspotx - middleX > 0 && goalspoty - middleY < 0) {
	//		ddis = goalspotx * goalspotx + goalspoty * goalspoty;
	//		dis = sqrt(ddis);
	//		mainball.x1 += dis / goalspotx;
	//		mainball.x2 += dis / goalspotx;
	//		mainball.y1 -= dis / goalspoty;
	//		mainball.y2 -= dis / goalspoty;

	//	}
	//	else if (goalspotx - middleX < 0 && goalspoty - middleY < 0) {
	//		ddis = goalspotx * goalspotx + goalspoty * goalspoty;
	//		dis = sqrt(ddis);
	//		mainball.x1 -= dis / goalspotx;
	//		mainball.x2 -= dis / goalspotx;
	//		mainball.y1 -= dis / goalspoty;
	//		mainball.y2 -= dis / goalspoty;

	//	}
	//	else if (goalspotx - middleX < 0 && goalspoty - middleY > 0) {
	//		ddis = goalspotx * goalspotx + goalspoty * goalspoty;
	//		dis = sqrt(ddis);
	//		mainball.x1 -= dis / goalspotx;
	//		mainball.x2 -= dis / goalspotx;
	//		mainball.y1 += dis / goalspoty;
	//		mainball.y2 += dis / goalspoty;

	//	}
	//	else if (goalspotx - middleX > 0 && goalspoty - middleY > 0) {
	//		ddis = goalspotx * goalspotx + goalspoty * goalspoty;
	//		dis = sqrt(ddis);
	//		mainball.x1 += dis / goalspotx;
	//		mainball.x2 += dis / goalspotx;
	//		mainball.y1 += dis / goalspoty;
	//		mainball.y2 += dis / goalspoty;

	//	}
	//	else if (goalspotx - middleX == 0 && goalspoty - middleY < 0) {
	//		mainball.y1 -= dis / goalspoty;
	//		mainball.y2 -= dis / goalspoty;
	//	}
	//	else if (goalspotx - middleX == 0 && goalspoty - middleY > 0) {
	//		mainball.y1 += dis / goalspoty;
	//		mainball.y2 += dis / goalspoty;
	//	}
	//	else if (goalspotx - middleX > 0 && goalspoty - middleY == 0) {
	//		mainball.x1 += dis / goalspotx;
	//		mainball.x2 += dis / goalspotx;
	//	}
	//	else if (goalspotx - middleX < 0 && goalspoty - middleY == 0) {
	//		mainball.x1 -= dis / goalspotx;
	//		mainball.x2 -= dis / goalspotx;
	//	}
	//	InvalidateRect(hwnd, NULL, TRUE);
	//	break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			goalspotx = LOWORD(lParam);
			goalspoty = HIWORD(lParam);

			if (goalspotx - middleX > 0 && goalspoty - middleY < 0) {
				ddis = goalspotx * goalspotx + goalspoty * goalspoty;
				dis = sqrt(ddis);
				mainball.x1 += dis / goalspotx;
				mainball.x2 += dis / goalspotx;
				mainball.y1 -= dis / goalspoty;
				mainball.y2 -= dis / goalspoty;

			}
			else if (goalspotx - middleX < 0 && goalspoty - middleY < 0) {
				ddis = goalspotx * goalspotx + goalspoty * goalspoty;
				dis = sqrt(ddis);
				mainball.x1 -= dis / goalspotx;
				mainball.x2 -= dis / goalspotx;
				mainball.y1 -= dis / goalspoty;
				mainball.y2 -= dis / goalspoty;

			}
			else if (goalspotx - middleX < 0 && goalspoty - middleY > 0) {
				ddis = goalspotx * goalspotx + goalspoty * goalspoty;
				dis = sqrt(ddis);
				mainball.x1 -= dis / goalspotx;
				mainball.x2 -= dis / goalspotx;
				mainball.y1 += dis / goalspoty;
				mainball.y2 += dis / goalspoty;

			}
			else if (goalspotx - middleX > 0 && goalspoty - middleY > 0) {
				ddis = goalspotx * goalspotx + goalspoty * goalspoty;
				dis = sqrt(ddis);
				mainball.x1 += dis / goalspotx;
				mainball.x2 += dis / goalspotx;
				mainball.y1 += dis / goalspoty;
				mainball.y2 += dis / goalspoty;

			}
			else if (goalspotx - middleX == 0 && goalspoty - middleY < 0) {
				mainball.y1 -= dis / goalspoty;
				mainball.y2 -= dis / goalspoty;
			}
			else if (goalspotx - middleX == 0 && goalspoty - middleY > 0) {
				mainball.y1 += dis / goalspoty;
				mainball.y2 += dis / goalspoty;
			}
			else if (goalspotx - middleX > 0 && goalspoty - middleY == 0) {
				mainball.x1 += dis / goalspotx;
				mainball.x2 += dis / goalspotx;
			}
			else if (goalspotx - middleX < 0 && goalspoty - middleY == 0) {
				mainball.x1 -= dis / goalspotx;
				mainball.x2 -= dis / goalspotx;
			}

			break;
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		COLORREF SetBkColor(HDC hdc, COLORREF crColor);
		//SetBkColor(hdc, RGB(0, 0, 0));
		//COLORREF text_color;
		//text_color = RGB(0, 0, 0);
		//SetTextColor(hdc, text_color);
		hBrush = CreateSolidBrush(RGB(maincolor.R, maincolor.G, maincolor.B));
		SelectObject(hdc, hBrush); 
		Ellipse(hdc, mainball.x1, mainball.y1, mainball.x2, mainball.y2);
		DeleteObject(hBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}