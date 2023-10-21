#include <windows.h> //--- ������ ��� ����
#include <tchar.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#pragma comment(lib, "msimg32.lib") //TransparentBlt ���� �ʼ�


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
	int animate;	//ĳ���� �ִϸ��̼� ��¿�

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
	switch (Unit_ID) {
	case 0:

		break;
	}
	
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

HBITMAP knightBitmap[32];



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
	HDC hdc, mdc;
	HBITMAP hBitmap, memBitmap;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	
	static int menu = 1;	//���� ȭ�� ��¼���
	
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &size);
		srand(GetTickCount64());
		//--------------------
		//�̹��� �ε�
		//--------------------
		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Crusader_combat_%04d.bmp", i);
			knightBitmap[i] = LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		knightBitmap[31] = LoadImage(g_hInst, L"Crusader_sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
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
			enemyShape[i].px = size.right-75 - 150 * i;
			enemyShape[i].py = 300;
			enemyShape[i].width = 140;
			enemyShape[i].height = 300;
		}

		for (int i = 0; i < 6; i++) {
			skillShape[i].px = 200 + 100*i;
			skillShape[i].py = 540;
			skillShape[i].width = 80;
			skillShape[i].height = 80;
		}

		for (int i = 0; i < 6; i++) {
			turnShape[i].px = size.right - 60 * (i+1);
			turnShape[i].py = 40;			
			turnShape[i].width = 50;
			turnShape[i].height = 50;		
		}

		break;
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
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		oldPen = (HPEN)SelectObject(mdc, hPen);
		hBrush = GetStockObject(WHITE_BRUSH);
		oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

		Rectangle(mdc, 0, 0, size.right, size.bottom);		

		SelectObject(mdc, oldPen);
		SelectObject(mdc, oldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
		//----------------------------
		//���� ĥ�ϱ�
		//----------------------------
		switch (menu) {
		case 0://���� �޴�

			break;
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
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER|DT_VCENTER);
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
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
