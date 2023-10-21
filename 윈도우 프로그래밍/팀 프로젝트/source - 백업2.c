#include <windows.h> //--- ������ ��� ����
#include <tchar.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#pragma comment(lib, "msimg32.lib") //TransparentBlt ���� �ʼ�

//ĳ���� Ŭ���� ID
#define ID_KNIGHT 1
#define ID_HEALER 2
#define ID_THIEF 3
#define ID_SNIPER 4


//------------------
//����ü ����
//------------------
typedef struct SHAPE {
	int px, py;	//�߽���(x,y)
	int width, height;	///�¿���, ������
}Shape;

typedef struct UNIT {
	int space;	//���ִ� ��ġ�� ����
	int CLASS;	//������

	int hp;		//ü��
	int atk;	//�߰�������
	int def;	//��(���ظ� ��Ź���)
	int speed;	//�ӵ�(�������� ���� �ൿ)

	int skill[4];	//4���� ��ų ID�� ������.
	//int animate;	//ĳ���� �ִϸ��̼� ��¿�

	struct UNIT* next;
}Unit;




//------------------
//���� ���� ����
//------------------

Unit* charhead = NULL;
Unit* enemyhead = NULL;

	//������ ��ġ ��� ũ��&��ġ ���� ����ü
Shape unitShape[4];	//ĳ���� ũ��&��ġ
Shape enemyShape[3];	//��ĳ���� ũ��&��ġ
Shape skillShape[6];	//��ų��ư ũ��&��ġ
Shape turnShape[6];	//�ϼ��� ũ��&��ġ

Unit* unit[4];	//ĳ���� ���� ����ü
Unit* enemy[3];	//������ ���� ����ü

//------------------
//�Լ� ���� �� ����
//------------------

BOOLEAN CheckMouseRect(int mx, int my,int left,int top,int right,int bottom) {
	if (mx >= left && mx <= right && my >= top && my <= bottom) {
		return TRUE;
	}
	else
		return FALSE;
}

//�־��� Shape �� RECT ����ü�� �����ϴ� �Լ�
RECT DrawShape(Shape* target) {
	RECT tmp = { target->px - target->width / 2 ,target->py - target->height / 2, target->px + target->width / 2, target->py + target->height / 2 };
	return tmp;
}

//�������� ���Ḯ��Ʈ ���۰��� �Լ�
	//���� ����
void MakeUnit(Unit** head,int Unit_ID) {
	Unit* unit = malloc(sizeof(Unit));
	//������ ���� ID�� ���� unit �ʱ�ȭ
	/*	
	//ĳ���� Ŭ���� ID
	#define ID_KNIGHT 1
	#define ID_HEALER 2
	#define ID_THIEF 3
	#define ID_SNIPER 4
	*/
	switch (Unit_ID) {
	case ID_KNIGHT:
		unit->CLASS= ID_KNIGHT;

		unit->hp = 50;
		unit->atk = 5;
		unit->def = 5;
		unit->speed = 90;

		unit->skill[0] = NULL;
		unit->skill[1] = NULL;
		unit->skill[2] = NULL;
		unit->skill[3] = NULL;

		break;
	case ID_HEALER:

		unit->CLASS = ID_HEALER;

		unit->hp = 50;
		unit->atk = 5;
		unit->def = 5;
		unit->speed = 90;

		unit->skill[0] = NULL;
		unit->skill[1] = NULL;
		unit->skill[2] = NULL;
		unit->skill[3] = NULL;

		break;
	case ID_THIEF:

		unit->CLASS = ID_THIEF;

		unit->hp = 50;
		unit->atk = 5;
		unit->def = 5;
		unit->speed = 90;

		unit->skill[0] = NULL;
		unit->skill[1] = NULL;
		unit->skill[2] = NULL;
		unit->skill[3] = NULL;

		break;
	case ID_SNIPER:

		unit->CLASS = ID_SNIPER;

		unit->hp = 50;
		unit->atk = 5;
		unit->def = 5;
		unit->speed = 90;

		unit->skill[0] = NULL;
		unit->skill[1] = NULL;
		unit->skill[2] = NULL;
		unit->skill[3] = NULL;

		break;
	}
	unit->next = NULL;
	//������ �� ������ �־��� head�� ���� ����.
	Unit* tmp = *head;

	if (*head == NULL) {
		*head = unit;
	}
	else {
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = unit;
	}	
}
	//���� ����
void DeleteUnit(Unit** head, Unit* target) {
	Unit* tmp = *head;
	Unit* prev = NULL;

	while (tmp != target && tmp != NULL) {
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL) {
		//�̰� �����߻� �κ�
	}
	else {
		if (prev == NULL) {
			*head = tmp->next;
			free(tmp);
		}
		else {
			prev->next = tmp->next;
			free(tmp);
		}
	}
}


//------------------
//����� ��
//------------------

//HBITMAP knightBitmap[32];
HBITMAP TankerBitmap;
HBITMAP HealerBitmap;
HBITMAP ThiefBitmap;
HBITMAP SniperBitmap;
HBITMAP BkBitmap;


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	hwnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);
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
	static RECT size;
	PAINTSTRUCT ps;
	HDC hdc, mdc, imgdc;
	HBITMAP hBitmap, imgBitmap;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HFONT hFont, oldFont;
	BITMAP bmp;

	static int menu = 0;			//���� ȭ�� ��¼���
	static int SelectCursor = 0;	//���� ���콺�� ����Ű�� ��ü��ȣ
	static int mx, my;				//���콺 Ŀ�� ��ġ



	//Ÿ��Ʋ ȭ���� ���
	if (menu == 0) {
		TCHAR str[100];
		switch (uMsg) {
		case WM_CREATE:
			GetClientRect(hwnd, &size);
			srand(GetTickCount64());
			//--------------------
			//�̹��� �ε�
			//--------------------
			/*
			for (int i = 0; i < 31; i++) {
				TCHAR str[100];
				wsprintf(str, L"Crusader_combat_%04d.bmp", i);
				knightBitmap[i] = LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			}
			*/
			//knightBitmap[31] = LoadImage(g_hInst, L"Crusader_sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			TankerBitmap = LoadImage(g_hInst, L"Image\\Tanker.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			HealerBitmap = LoadImage(g_hInst, L"Image\\Healer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			ThiefBitmap = LoadImage(g_hInst, L"Image\\Thief.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			SniperBitmap = LoadImage(g_hInst, L"Image\\Sniper.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			BkBitmap = LoadImage(g_hInst, L"Image\\Background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			//--------------------
			//�̹��� ��� ũ�� �ʱ�ȭ
			//--------------------
			for (int i = 0; i < 4; i++) {
				unitShape[i].px = 75 + 150 * i;
				unitShape[i].py = 300;
				unitShape[i].width = 140;
				unitShape[i].height = 300;
			}

			for (int i = 0; i < 3; i++) {
				enemyShape[i].px = size.right - 75 - 150 * i;
				enemyShape[i].py = 300;
				enemyShape[i].width = 140;
				enemyShape[i].height = 300;
			}

			for (int i = 0; i < 6; i++) {
				skillShape[i].px = 200 + 100 * i;
				skillShape[i].py = 540;
				skillShape[i].width = 80;
				skillShape[i].height = 80;
			}

			for (int i = 0; i < 6; i++) {
				turnShape[i].px = size.right - 60 * (i + 1);
				turnShape[i].py = 40;
				turnShape[i].width = 50;
				turnShape[i].height = 50;
			}

			break;
		case WM_PAINT:
			GetClientRect(hwnd, &size);
			hdc = BeginPaint(hwnd, &ps);

			//������۸�
			mdc = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
			SelectObject(mdc, (HBITMAP)hBitmap);
			//----------------------------
			// �Ʒ��� WM_PAINT �޼��� �Է� (������۸� ���̿��� mdc�θ� �־����.)
			//----------------------------
			// 
			//----------------------------
			//�޹�� ĥ�ϱ�
			//----------------------------
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = GetStockObject(BLACK_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

			Rectangle(mdc, 0, 0, size.right, size.bottom);

			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
			//----------------------------
			//Ÿ��Ʋ�� �޴��� ���
			//----------------------------
				//Ÿ��Ʋ ���
			SetTextColor(mdc, RGB(255, 255, 255));	//���� ����
			SetBkMode(mdc, TRANSPARENT);			//���� ��� ����ȭ
			//SetBkMode(mdc, OPAQUE);
			hFont = CreateFont(150, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "����");
			oldFont = (HFONT)SelectObject(mdc, hFont);

			_tcscpy(str, L"����");
			TextOut(mdc, 450, 100, str, lstrlen(str));

			SelectObject(mdc, oldFont);
			DeleteObject(hFont);
				//Ÿ��Ʋ �޴� 1
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			
			if (SelectCursor == 1) {
				SetTextColor(mdc, RGB(255, 255, 0));	//���� ����
				hFont = CreateFont(130, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "����");}
			else{ 
				SetTextColor(mdc, RGB(255, 255, 255));	//���� ����
			}

			hFont = CreateFont(100, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, "����");
			oldFont = (HFONT)SelectObject(mdc, hFont);

			_tcscpy(str, L"Start");
			TextOut(mdc, 150, 400, str, lstrlen(str));
			//Rectangle(mdc, 150, 400, 380, 500);		

			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);

				//Ÿ��Ʋ �޴� 2
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

			if (SelectCursor == 2) {
				SetTextColor(mdc, RGB(255, 255, 0));	//���� ����
			}
			else {
				SetTextColor(mdc, RGB(255, 255, 255));	//���� ����
			}

			_tcscpy(str, L"Exit");
			TextOut(mdc, 150, 550, str, lstrlen(str));
			//Rectangle(mdc, 150, 550, 330, 650);

			SelectObject(mdc, oldFont);
			DeleteObject(hFont);

			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);

			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);
			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_MOUSEMOVE:
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if (SelectCursor == 0) {
				//Rectangle(mdc, 150, 400, 380, 500); - ù��° �޴� Rect
				if (CheckMouseRect(mx, my, 150, 400, 380, 500)) {
					SelectCursor = 1;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
				//Rectangle(mdc, 150, 550, 330, 650); - �ι�° �޴� Rect

				if (CheckMouseRect(mx, my, 150, 550, 330, 650)) {
					SelectCursor = 2;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
			}
			if (SelectCursor == 1) {
				//Rectangle(mdc, 150, 400, 380, 500); - ù��° �޴� Rect
				if (CheckMouseRect(mx, my, 150, 400, 380, 500)) {
					//SelectCursor = 1;
					break;
				}
				else {
					SelectCursor = 0;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
			}
			if (SelectCursor == 2) {
				//Rectangle(mdc, 150, 400, 380, 500); - ù��° �޴� Rect
				if (CheckMouseRect(mx, my, 150, 550, 330, 650)) {
					//SelectCursor = 2;
					break;
				}
				else {
					SelectCursor = 0;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (SelectCursor != 0) {
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				if (SelectCursor == 1) {			
					menu = 1;
					SelectCursor = 0;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
				//Rectangle(mdc, 150, 550, 330, 650); - �ι�° �޴� Rect
				if (SelectCursor == 2) {
					DestroyWindow(hwnd);
					break;
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	//ĳ���� ����ȭ�� ���
	if (menu == 1) {
		
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 1: //�ִϸ��̼� 

				break;
			}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_PAINT:
			GetClientRect(hwnd, &size);
			hdc = BeginPaint(hwnd, &ps);

			//������۸�
			mdc = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
			SelectObject(mdc, (HBITMAP)hBitmap);

			imgdc = CreateCompatibleDC(hdc);		

			//----------------------------
			// �Ʒ��� WM_PAINT �޼��� �Է� (������۸� ���̿��� mdc�θ� �־����.)
			//----------------------------
			// 
			//----------------------------
			//�޹�� ĥ�ϱ�
			//----------------------------
			SelectObject(imgdc, (HBITMAP)BkBitmap);
			GetObject(BkBitmap, sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------
		
			//�����Ǵ� ĳ���� ���
			for (int i = 0; i < 4; i++) {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(mdc, hPen);
				hBrush = CreateSolidBrush(RGB(128, 128, 128));
				oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
				/*
				HBITMAP TankerBitmap;
				HBITMAP HealerBitmap;
				HBITMAP ThiefBitmap;
				HBITMAP SniperBitmap;
				*/
				switch (i) {
				case 0:
					SelectObject(imgdc, (HBITMAP)TankerBitmap);
					GetObject(TankerBitmap, sizeof(BITMAP), &bmp);
					break;
				case 1:
					SelectObject(imgdc, (HBITMAP)HealerBitmap);
					GetObject(HealerBitmap, sizeof(BITMAP), &bmp);
					break;
				case 2:
					SelectObject(imgdc, (HBITMAP)ThiefBitmap);
					GetObject(ThiefBitmap, sizeof(BITMAP), &bmp);
					break;
				case 3:
					SelectObject(imgdc, (HBITMAP)SniperBitmap);
					GetObject(SniperBitmap, sizeof(BITMAP), &bmp);
					break;
				}
				if (SelectCursor == i + 1) {
					Rectangle(mdc, (size.right / 4)* i, 0, (size.right / 4)* (i + 1), 500);
				}

				RECT tmp = { (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500 };
				TransparentBlt(mdc, (size.right / 4)* i, 0, size.right / 4, 500, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
				//switch()

				SelectObject(mdc, oldPen);
				SelectObject(mdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);
				
			}
			//���õ� ĳ���� ����
			for (int i = 0; i < 4; i++) {
				hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(mdc, hPen);
				hBrush = CreateSolidBrush(RGB(128, 128, 128));
				oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

				RECT tmp = { 300 + 150 * i,700,450 + 150 * i,750 };
				Ellipse(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
				/*
				if (unit[i] != NULL) {
					TCHAR str[100];
					switch (unit[i]->CLASS) {
					case 0:
						wsprintf(str, L"���");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					case 1:
						wsprintf(str, L"����");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					case 2:
						wsprintf(str, L"����");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					case 3:
						wsprintf(str, L"������");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					}
				}
				*/

				SelectObject(mdc, oldPen);
				SelectObject(mdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);
			}	

			//ĳ���� ���
			Unit* tmp = charhead;
			int i = 0;
			hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			while(tmp != NULL) {
				switch (tmp->CLASS) {
				case ID_KNIGHT:
					SelectObject(imgdc, (HBITMAP)TankerBitmap);
					GetObject(TankerBitmap, sizeof(BITMAP), &bmp);
					break;
				case ID_HEALER:
					SelectObject(imgdc, (HBITMAP)HealerBitmap);
					GetObject(HealerBitmap, sizeof(BITMAP), &bmp);
					break;
				case ID_THIEF:
					SelectObject(imgdc, (HBITMAP)ThiefBitmap);
					GetObject(ThiefBitmap, sizeof(BITMAP), &bmp);
					break;
				case ID_SNIPER:
					SelectObject(imgdc, (HBITMAP)SniperBitmap);
					GetObject(SniperBitmap, sizeof(BITMAP), &bmp);
					break;
				}
				if (SelectCursor == i + 5) {
					Rectangle(mdc, 300 + 150 * i, 550, 450 + 150 * i, 750);
				}
				TransparentBlt(mdc, 300 + 150 * i, 550, 150, 170, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
				i++;
				tmp = tmp->next;
			}
			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
			TCHAR str[100];

			wsprintf(str, L"SelectCursor : %d", SelectCursor);
			TextOut(mdc, 0, 0, str, lstrlen(str));
			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);

			DeleteDC(imgdc);

			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_MOUSEMOVE:
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			// �����ϴ� ĭ�� ����
			//	RECT tmp = { (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500 };			
			for (int i = 0; i < 4; i++) {								
				if (CheckMouseRect(mx, my, (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500)) {
					if (SelectCursor == i + 1) {
						break;
					}
					SelectCursor = i + 1;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
				if (SelectCursor <= 4) {
					SelectCursor = 0;
					InvalidateRect(hwnd, NULL, FALSE);
				}
			}			
			// RECT tmp = { 300 + 150 * i,700,450 + 150 * i,750 };
			for (int i = 5; i < 8; i++) {
				if (CheckMouseRect(mx, my, 300 + 150 * (i-5), 550, 450 + 150 * (i-5), 720)) {
					if (SelectCursor == i) {
						break;
					}
					SelectCursor = i;
					InvalidateRect(hwnd, NULL, FALSE);
					break;
				}
				
				if (SelectCursor >= 5) {
					SelectCursor = 0;
					InvalidateRect(hwnd, NULL, FALSE);
				}
				
			}
			break;
		case WM_LBUTTONDOWN:
			if (SelectCursor != 0) {
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				switch (SelectCursor) {
				case ID_KNIGHT:
					MakeUnit(&charhead, ID_KNIGHT);
					break;
				case ID_HEALER:
					MakeUnit(&charhead, ID_HEALER);
					break;
				case ID_THIEF:
					MakeUnit(&charhead, ID_THIEF);
					break;
				case ID_SNIPER:
					MakeUnit(&charhead, ID_SNIPER);
					break;

				case 5:
					DeleteUnit(&charhead, charhead);
					break;
				case 6:
					DeleteUnit(&charhead, charhead->next);
					break;
				case 7:
					DeleteUnit(&charhead, charhead->next->next);
					break;
				case 8:
					DeleteUnit(&charhead, charhead->next->next->next);
					break;
				}
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	//ĳ���� ����ȭ�� ���
	if (menu == 2) {
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 1: //�ִϸ��̼� 

				break;
			}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_PAINT:
			GetClientRect(hwnd, &size);
			hdc = BeginPaint(hwnd, &ps);

			//������۸�
			mdc = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
			SelectObject(mdc, (HBITMAP)hBitmap);
			//----------------------------
			// �Ʒ��� WM_PAINT �޼��� �Է� (������۸� ���̿��� mdc�θ� �־����.)
			//----------------------------
			// 
			//----------------------------
			//�޹�� ĥ�ϱ�
			//----------------------------
			
			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------
			switch (menu) {
			case 1:	//��Ƽ ����
				for (int i = 0; i < 4; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					switch (i) {
					case 0:
						hBrush = CreateSolidBrush(RGB(0, 255, 255));
						break;
					case 1:
						hBrush = CreateSolidBrush(RGB(255, 255, 0));
						break;
					case 2:
						hBrush = CreateSolidBrush(RGB(255, 0, 255));
						break;
					case 3:
						hBrush = CreateSolidBrush(RGB(100, 50, 100));
						break;
					}
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = { (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500 };
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
					//switch()

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}

				for (int i = 0; i < 4; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = { (size.right / 4) * i, 500, (size.right / 4) * (i + 1), 800 };
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
					if (unit[i] != NULL) {
						TCHAR str[100];
						switch (unit[i]->CLASS) {
						case 0:
							wsprintf(str, L"���");
							DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
							break;
						case 1:
							wsprintf(str, L"����");
							DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
							break;
						case 2:
							wsprintf(str, L"�̴ܽ��ǰ�");
							DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
							break;
						case 3:
							wsprintf(str, L"������");
							DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
							break;
						}
					}

					//switch()

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}

				break;
			case 2:	//����

				break;
			case 3: //���� ������
				//�뺴 ���
				for (int i = 0; i < 4; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(0, 255, 255));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = DrawShape(&unitShape[i]);
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
					//switch()

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
				//for /f "delims=_���̾�." %a in ('dir /b *.*') do ren "%~a_���̾�*.*" "%~a.*"
				//�� ���
				for (int i = 0; i < 4; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(255, 255, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = DrawShape(&enemyShape[i]);
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
				//��ų ���
				for (int i = 0; i < 6; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(128, 128, 128));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = DrawShape(&skillShape[i]);
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
				//�� ���� ���
				for (int i = 0; i < 6; i++) {
					hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(3, 200, 50));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					RECT tmp = DrawShape(&turnShape[i]);
					Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
				break;
			case 4: //����

				break;
			case 5: //�޽�

				break;

			}
			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);
			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
