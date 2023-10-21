#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#pragma comment(lib, "msimg32.lib") //TransparentBlt 사용시 필수


//------------------
//구조체 정의
//------------------
typedef struct SHAPE {
	int px, py;	//중심점(x,y)
	int width, height;	///좌우폭, 세로폭
}Shape;

typedef struct UNIT {
	int space;	//서있는 위치를 저장
	int CLASS;	//직업군

	int hp;		//체력
	int atk;	//추가데미지
	int def;	//방어막(피해를 대신받음)
	int speed;	//속도(빠를수록 먼저 행동)

	int skill[4];	//4개의 스킬 ID를 저장함.
	int animate;	//캐릭터 애니메이션 출력용

	struct UNIT* next;
}Unit;




//------------------
//고정 변수 선언
//------------------

Unit* charhead = NULL;
Unit* enemyhead = NULL;

	//전투시 위치 출력 크기&위치 저장 구조체
Shape unitShape[4];	//캐릭터 크기&위치
Shape enemyShape[3];	//적캐릭터 크기&위치
Shape skillShape[6];	//스킬버튼 크기&위치
Shape turnShape[6];	//턴순서 크기&위치

Unit* unit[4];	//캐릭터 저장 구조체
Unit* enemy[3];	//적유닛 저장 구조체

//------------------
//함수 선언 및 정의
//------------------

//주어진 Shape 를 RECT 구조체로 변형하는 함수
RECT DrawShape(Shape* target) {
	RECT tmp = { target->px - target->width / 2 ,target->py - target->height / 2, target->px + target->width / 2, target->py + target->height / 2 };
	return tmp;
}

//유닛저장 연결리스트 조작관련 함수
	//유닛 생성
void MakeUnit(Unit** head,int Unit_ID) {
	Unit* unit = malloc(sizeof(Unit));
	//생성할 유닛 ID에 따라 unit 초기화
	switch (Unit_ID) {
	case 0:

		break;
	}
	
	//생성된 새 유닛을 주어진 head의 끝에 삽입.
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
	//유닛 삭제
void DeleteUnit(Unit** head, Unit* target) {
	Unit* tmp = *head;
	Unit* prev = NULL;

	while (tmp != target && tmp != NULL) {
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp == NULL) {
		//이건 오류발생 부분
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
//헤더용 끝
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
	
	static int menu = 1;	//현재 화면 출력선택
	
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hwnd, &size);
		srand(GetTickCount64());
		//--------------------
		//이미지 로드
		//--------------------
		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Crusader_combat_%04d.bmp", i);
			knightBitmap[i] = LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		knightBitmap[31] = LoadImage(g_hInst, L"Crusader_sword.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
		//--------------------
		//이미지 출력 크기 초기화
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
		case 1: //애니메이션 

			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_PAINT:
		GetClientRect(hwnd, &size);
		hdc = BeginPaint(hwnd, &ps);

		//더블버퍼링
		mdc = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
		SelectObject(mdc, (HBITMAP)hBitmap);
		//----------------------------
		// 아래에 WM_PAINT 메세지 입력 (더블버퍼링 사이에는 mdc로만 넣어야함.)
		//----------------------------
		// 
		//----------------------------
		//뒷배경 칠하기
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
		//내용 칠하기
		//----------------------------
		switch (menu) {
		case 0://메인 메뉴

			break;
		case 1:	//파티 설정
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
						wsprintf(str, L"기사");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER|DT_VCENTER);
						break;
					case 1:
						wsprintf(str, L"도적");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					case 2:
						wsprintf(str, L"이단심판관");
						DrawText(mdc, str, lstrlen(str), &tmp, DT_CENTER | DT_VCENTER);
						break;
					case 3:
						wsprintf(str, L"총잡이");
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
		case 2:	//지도

			break;
		case 3: //전투 시퀸스
			//용병 출력
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
			//for /f "delims=_레이어." %a in ('dir /b *.*') do ren "%~a_레이어*.*" "%~a.*"
			//적 출력
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
			//스킬 출력
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
			//턴 순서 출력
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
		case 4: //상점

			break;
		case 5: //휴식

			break;

		}
		//더블버퍼링
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
