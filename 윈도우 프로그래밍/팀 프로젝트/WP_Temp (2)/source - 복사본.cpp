#include <windows.h> //--- ������ ��� ����
#include <tchar.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment(lib, "msimg32.lib") //TransparentBlt ���� �ʼ�
//#pragma comment (lib, "fmod_vc.lib")

#define STAGELEN 10


//ĳ���� Ŭ���� ID
#define ID_USER_Crusader 1
#define ID_USER_Vestal 2
#define ID_USER_Highway 3
#define ID_USER_Musketeer 4

//���� Ŭ���� ID
#define ID_ENEMY_NAME1 101
#define ID_ENEMY_NAME2 102
#define ID_ENEMY_NAME3 103
#define ID_ENEMY_NAME4 104

	//��ų ID
#define noskill -1
//Crusader
#define SKILL_Crusader_Smite 1
#define SKILL_Crusader_HolyLance 2
#define SKILL_Crusader_Zealous_Speech 3
#define SKILL_Crusader_Block 4
//Vestal
#define SKILL_Vestal_Mace 11
#define SKILL_Vestal_Lectionary 12
#define SKILL_Vestal_Miracle 13
#define SKILL_Vestal_Heal 14
//Highway
#define SKILL_Highway_Slash 21
#define SKILL_Highway_Pistol 22
#define SKILL_Highway_Hipshot 23
#define SKILL_Highway_Lunge 24
//Musketeer
#define SKILL_Musketeer_rifle 31
#define SKILL_Musketeer_pistol 32
#define SKILL_Musketeer_Buckshot 33
#define SKILL_Musketeer_Selfheal 34

//�� ��ų
#define SKILL_Monster_attack 41
#define SKILL_Monster_defend 42
#define SKILL_Monster_healing 43
#define SKILL_Monster_allattack 44
#define SKILL_Monster_seldefend 45
#define SKILL_Monster_selhealing 46

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

	int Maxhp;	//�ִ� ü��
	int hp;		//ü��
	int AtkBuff;	//�߰�������
	int def;	//��(���ظ� ��Ź���)
	int speed;	//�ӵ�(�������� ���� �ൿ)

	int skill[4];	//4���� ��ų ID�� ������.
	//int animate;	//ĳ���� �ִϸ��̼� ��¿�

	struct UNIT* next;
	struct UNIT* turnnext;
}Unit;
/*
typedef struct NODE {
	Unit* unit;
	struct NODE* next;
}Node;
*/
typedef struct LIST {
	Unit* head;	//���Ḯ��Ʈ ���
	int n;		//���Ḯ��Ʈ ��� ��
}List;

typedef struct Mapset {
	int num;		//���� ���� ( 0 = ���� , 1 = ���� , 2 = �޽� )
	int seed;		//������ ������ ���� ���� (0~4)
}Mapset;

typedef struct SKILL {
	int ID;
	int target;	//0: ����, 1: ��ü
	int num;	//0: ����, 1: �Ʊ�, 2: ����
	TCHAR sentence[100];
	HBITMAP* icon;
}Skill;

//------------------
//���� ���� ����
//------------------

List* userlist = NULL;
List* enemylist = NULL;
List* turnlist = NULL;

	//������ ��ġ ��� ũ��&��ġ ���� ����ü
Shape unitShape[4];	//ĳ���� ũ��&��ġ
Shape enemyShape[3];	//��ĳ���� ũ��&��ġ
Shape skillShape[6];	//��ų��ư ũ��&��ġ
Shape turnShape[7];	//�ϼ��� ũ��&��ġ

	//������ ���ݽ� ��ġ ���
Shape unitAttack[4];	//ĳ���� ũ��&��ġ
Shape enemyAttack[3];	//��ĳ���� ũ��&��ġ

Unit* unit[4];	//ĳ���� ���� ����ü
Unit* enemy[3];	//������ ���� ����ü

Skill skill[10][4];
//------------------
//�Լ� ���� �� ����
//------------------

//���콺�� ��ü���� �浹üũ
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
	//���Ḯ��Ʈ ����
void MakeList(List** list) {
	List* tmp = (List*)malloc(sizeof(List));

	tmp->head = NULL;
	tmp->n = 0;

	*list = tmp;
}
	//���Ḯ��Ʈ ����
void DeleteList(List** list) {
	Unit* tmp = (*list)->head;
	Unit* prev = NULL;

	while (tmp != NULL) {
		prev = tmp;
		tmp = tmp->next;
		free(prev);
	}

	List* del = *list;
	free(del);

	*list = NULL;
}

	//���� ����
void MakeUnit(List* list,int Unit_ID) {
	Unit* unit = (Unit*)malloc(sizeof(Unit));
	//������ ���� ID�� ���� unit �ʱ�ȭ
	/*	
	//ĳ���� Ŭ���� ID
	#define ID_USER_Crusader 1
	#define ID_USER_Vestal 2
	#define ID_USER_Highway 3
	#define ID_USER_Musketeer 4
	*/
	switch (Unit_ID) {
	case ID_USER_Crusader:

		unit->hp = 70;
		unit->AtkBuff = 5;
		unit->def = 20;
		unit->speed = 100;
		unit->skill[0] = 1;
		unit->skill[1] = 2;
		unit->skill[2] = 3;
		unit->skill[3] = 4;

		break;
	case ID_USER_Vestal:

		unit->hp = 50;
		unit->AtkBuff = 5;
		unit->def = 0;
		unit->speed = 90;

		unit->skill[0] = 11;
		unit->skill[1] = 12;
		unit->skill[2] = 13;
		unit->skill[3] = 14;

		break;
	case ID_USER_Highway:

		unit->hp = 50;
		unit->AtkBuff = 5;
		unit->def = 0;
		unit->speed = 110;

		unit->skill[0] = 21;
		unit->skill[1] = 22;
		unit->skill[2] = 23;
		unit->skill[3] = 24;

		break;
	case ID_USER_Musketeer:
		unit->hp = 50;
		unit->AtkBuff = 5;
		unit->def = 10;
		unit->speed = 100;

		unit->skill[0] = 31;
		unit->skill[1] = 32;
		unit->skill[2] = 33;
		unit->skill[3] = 34;

		break;
	case ID_ENEMY_NAME1:

		unit->hp = 40;
		unit->AtkBuff = 0;
		unit->def = 5;
		unit->speed = 70;

		
		unit->skill[0] = SKILL_Monster_attack;
		unit->skill[1] = SKILL_Monster_healing;
		unit->skill[2] = SKILL_Monster_healing;
		unit->skill[3] = noskill;

		break;
	case ID_ENEMY_NAME2:
		unit->hp = 30;
		unit->AtkBuff = 10;
		unit->def = 5;
		unit->speed = 70;

		unit->skill[0] = SKILL_Monster_attack;
		unit->skill[1] = SKILL_Monster_attack;
		unit->skill[2] = SKILL_Monster_attack;
		unit->skill[3] = noskill;

		break;
	case ID_ENEMY_NAME3:
		unit->hp = 50;
		unit->AtkBuff = 15;
		unit->def = 5;
		unit->speed = 70;

		unit->skill[0] = SKILL_Monster_attack;
		unit->skill[1] = SKILL_Monster_healing;
		unit->skill[2] = SKILL_Monster_healing;
		unit->skill[3] = noskill;

		break;
	case ID_ENEMY_NAME4:
		unit->hp = 50;
		unit->AtkBuff = 0;
		unit->def = 5;
		unit->speed = 70;

		unit->skill[0] = SKILL_Monster_attack;
		unit->skill[1] = SKILL_Monster_healing;
		unit->skill[2] = SKILL_Monster_healing;
		unit->skill[3] = noskill;

		break;
	}
	unit->Maxhp = unit->hp;
	//unit->hp /= 2;	//debug ����
	unit->CLASS = Unit_ID;
	unit->next = NULL;
	//������ �� ������ �־��� head�� ���� ����.
	Unit* tmp = list->head;

	if (list->head == NULL) {
		list->head = unit;
	}
	else {
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = unit;
	}	
	list->n++;
}
	//���� ����
Unit* FindUnit(List* list, int num) {
	Unit* tmp = list->head;

	for (int i = 0; i < num - 1; i++) {
		tmp = tmp->next;
	}

	return tmp;
}

	//���� ����
void DeleteUnit(List* list, Unit* target) {
	Unit* tmp = list->head;
	Unit* prev = NULL;
	Unit* turnprev = NULL;

	while (tmp != target && tmp != NULL) {
		prev = tmp;
		tmp = tmp->next;
	}

	if (tmp == NULL) {
		//�̰� �����߻� �κ�
	}
	else {
		if (prev == NULL) {
			list->head = tmp->next;			
		}
		else {
			prev->next = tmp->next;			
		}
	}
	free(target);
	list->n--;
}

	//������ ���� ����
void ActDeleteUnit(List* list, Unit* target) {
	Unit* tmp = list->head;	
	Unit* turnprev = NULL;

	while (tmp != target && tmp != NULL) {
		turnprev = tmp;
		tmp = tmp->turnnext;
	}

	if (tmp == NULL) {
		//�̰� �����߻� �κ�
	}
	else {
		if (turnprev == NULL) {
			list->head = tmp->turnnext;			
		}
		else {
			turnprev->turnnext = tmp->turnnext;
		}
	}
	//free(target);
	list->n--;
}

//�ϸ���Ʈ ���� �Լ�
	//�ϸ���Ʈ�� unitlist ���� �������� �� turnnext �ʱ�ȭ
void MakeTurnList(List* destination, List* source) {
	Unit* dest = destination->head;
	Unit* tmp = source->head;
	if (dest != NULL) {
		while (dest->turnnext != NULL) {
			dest = dest->turnnext;
		}
	}
	while (tmp != NULL) {
		if (dest == NULL) {
			destination->head = tmp;
		}
		else {
			dest->turnnext = tmp;
		}
		tmp->turnnext = NULL;
		destination->n++;
		dest = tmp;
		tmp = tmp->next;
	}
}

//�ϸ���Ʈ ���� �Լ�
void DeleteTurnList(List** list) {
	Unit* tmp = (*list)->head;
	Unit* prev = NULL;

	while (tmp != NULL) {
		prev = tmp;		
		tmp = tmp->turnnext;		
		prev->turnnext = NULL;
	}
	List* del = *list;
	free(del);
	*list = NULL;
}
//�� ���� 
Unit* getTail(Unit* head) {
	Unit* tmp = head;
	while (tmp->turnnext != NULL)
		tmp = tmp->turnnext;
	return tmp;
}

Unit* partition(Unit* head, Unit* tail, Unit** newHead, Unit** newTail) {
	Unit* pivot = tail;
	Unit* prev = NULL;
	Unit* current = head;
	Unit* end = pivot;

	while (current != pivot) {
		if (current->speed > pivot->speed) {
			if (*newHead == NULL)
				*newHead = current;
			prev = current;
			current = current->turnnext;
		}
		else {
			if (prev)
				prev->turnnext = current->turnnext;

			Unit* temp = current->turnnext;
			current->turnnext = NULL;
			end->turnnext = current;
			end = current;
			current = temp;
		}
	}

	if (*newHead == NULL)
		*newHead = pivot;

	*newTail = end;

	return pivot;
}

Unit* QuickSort_SL(Unit* head, Unit* tail) {
	//��尡 ������ �� ���� : Unit>1
	if (head == NULL || head == tail)
		return head;

	Unit* newHead = NULL;
	Unit* newTail = NULL;

	Unit* pivot = partition(head, tail, &newHead, &newTail);

	if (newHead != pivot) {
		Unit* temp = newHead;
		while (temp->turnnext != pivot) {
			temp = temp->turnnext;
		}
		temp->turnnext = NULL;

		newHead = QuickSort_SL(newHead, temp);

		temp = getTail(newHead);
		temp->turnnext = pivot;
	}

	pivot->turnnext = QuickSort_SL(pivot->turnnext, newTail);

	return newHead;
}

void SortTurnList(List* list) {
	list->head = QuickSort_SL(list->head, getTail(list->head));
}

int Attack(Unit* target, int damage) {
	if (target == NULL)
		return 0;
	int left = damage;
	if (target->def != 0) {
		if (target->def > damage) {
			target->def -= damage;
			return 0;	//���п� ����
		}
		else {
			damage -= target->def;
			target->def = 0;
			target->hp -= damage;
			return damage;	//���а� �μ���
		}
	}
	else {
		target->hp -= damage;
		return damage;
	}	
}


int Shield(Unit* target, int shieldvalue) {
	int left = shieldvalue;
	target->def += left;
	return left;
}

int Heal(Unit* target, int value) {
	target->hp = target->hp + value > target->Maxhp ? target->Maxhp: target->hp + value;
	return value;
}


void Map(Mapset base[][STAGELEN]) {
	int cnt = 0;
	for (int i = 0; i < 3; ) {
		for (int j = 0; j < STAGELEN; ++j) {
			if (j == 0) 			//ù��° ���������� ������ ������ ������ ��
				base[i][j].num = 0;
			else
				base[i][j].num = rand() % 3; //���� ���� ����


			base[i][j].seed = rand() % 5; // ���� �� ����

			if (base[i][j].num == 1 || base[i][j].num == 2)
				++cnt;
		}
		++i;
		if (cnt > 4) {	// �� �������� (1~10) ���� �޽� or ������ 5�� �̻� ������ �ʰ� ����
			cnt = 0;
			--i;
		}
	}
}

//------------------
//����� ��
//------------------

//HBITMAP knightBitmap[32];

//���� ��Ʈ�� ���� �ڵ�
HBITMAP CrusaderBitmap[34];	//0~30 = �ִϸ��̼�, 31: �ʻ�ȭ, 32: �޽�, 33:����Ʈ(������)
HBITMAP VestalBitmap[34];
HBITMAP HighwayBitmap[34];
HBITMAP MusketeerBitmap[34];
HBITMAP BkBitmap[5];

//���� ��ų ���� �ڵ�
HBITMAP CrusaderSkillBitmap[4];
HBITMAP VestalSkillBitmap[4];
HBITMAP HighwaySkillBitmap[4];
HBITMAP MusketeerSkillBitmap[4];
HBITMAP SkilloptionBitMap[3];
//������ ��Ʈ�� ���� �ڵ�
HBITMAP enemyBitmap[4];

//��ǥ�� ��Ʈ�� ���� �ڵ�
HBITMAP CrusaderTurnBitmap;
HBITMAP VestalTurnBitmap;
HBITMAP HighwayTurnBitmap;
HBITMAP MusketeerTurnBitmap;
HBITMAP enemyTurnBitmap[4];

//������ �ൿ ��Ʈ�� ���� �ڵ�
HBITMAP CrusaderActBitmap[5];
HBITMAP VestalActBitmap[5];
HBITMAP HighwayActBitmap[5];
HBITMAP MusketeerActBitmap[5];
HBITMAP SelectionBitmap[30];
//�׿� ��Ʈ�� ���� �ڵ�
HBITMAP effectBitmap[10];

//���� �� ������ ���� �ڵ�
HBITMAP MapiconBitmap[4];


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Team Project";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


FMOD::System* ssystem;
FMOD::Sound* sound1, * sound2;
FMOD::Channel* channel = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	/*
	while (GetMessage(&Message, 0, 0, 0)) {		
		TranslateMessage(&Message);
		DispatchMessage(&Message);		
	}
	*/
	/*
	while (GetMessage(&Message, 0, 0, 0)) {	
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	*/
	
	while (1) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT size;
	PAINTSTRUCT ps;
	HDC hdc, mdc, imgdc;
	HBITMAP hBitmap, imgBitmap;
	static HBITMAP tmpBitmap;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HFONT hFont, oldFont;
	BITMAP bmp;

	//�ִϸ��̼� ����
	static int animate = 0;
	
	//���콺 ���� ����
	static int SelectCursor = 0;	//���� ���콺�� ����Ű�� ��ü��ȣ
	static int mx, my;				//���콺 Ŀ�� ��ġ
	static BOOLEAN MouseLock = FALSE;		//���콺 Ŭ�� ��� ����

	//ȭ����ȯ ���� ����
	static int menu = 0;			//���� ȭ�� ��¼���
	static int nextmenu = 0;		//ȭ����ȯ�� ���� �޴�
	static int len;			//ȭ����ȯ�� ����ȿ�� ����
	static BOOLEAN phase = FALSE;	//ȭ���� �˿����� FALSE, ����ȭ���� ��Ÿ���°�� TRUE
	static BOOLEAN DisplayChange = FALSE;	//ȭ���� ���������� �˸��� ����

	//���� �������� �������� 
	static Mapset base[3][STAGELEN];	//�������� ���	[stage][line]
	static int line = 1;
	static int stage = 0;
	static int now = 1;

	static Unit* turntail = NULL;
	static Unit* SelectUnit = NULL;
	static int selectSkill = -1;
	static BOOLEAN act = FALSE;
	static int damage;
	static int timer;
	
	switch (uMsg) {
		TCHAR str[100];
	case WM_CREATE:
		GetClientRect(hwnd, &size);
		srand(GetTickCount64());
		//--------------------
		//�̹��� �ε�
		//--------------------
		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Crusader\\Crusader_combat_%04d.bmp", i);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			CrusaderBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		CrusaderBitmap[31] = (HBITMAP)LoadImage(g_hInst, L"Image\\Crusader\\Crusader.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		CrusaderBitmap[32] = (HBITMAP)LoadImage(g_hInst, L"Image\\Crusader\\Crusader_camp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		CrusaderBitmap[33] = (HBITMAP)LoadImage(g_hInst, L"Image\\Crusader\\Crusader_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Vestal\\Vestal_combat_%04d.bmp", i);
			//knightBitmap[i] = LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			VestalBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		VestalBitmap[31] = (HBITMAP)LoadImage(g_hInst, L"Image\\Vestal\\Vestal.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		VestalBitmap[32] = (HBITMAP)LoadImage(g_hInst, L"Image\\Vestal\\Vestal_camp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		VestalBitmap[33] = (HBITMAP)LoadImage(g_hInst, L"Image\\Vestal\\Vestal_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Highway\\Highway_combat_%04d.bmp", i);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			HighwayBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		HighwayBitmap[31] = (HBITMAP)LoadImage(g_hInst, L"Image\\Highway\\Highway.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HighwayBitmap[32] = (HBITMAP)LoadImage(g_hInst, L"Image\\Highway\\Highway_camp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HighwayBitmap[33] = (HBITMAP)LoadImage(g_hInst, L"Image\\Highway\\Highway_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		for (int i = 0; i < 31; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Musketeer\\Musketeer_combat_%04d.bmp", i);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			MusketeerBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		MusketeerBitmap[31] = (HBITMAP)LoadImage(g_hInst, L"Image\\Musketeer\\Musketeer.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		MusketeerBitmap[32] = (HBITMAP)LoadImage(g_hInst, L"Image\\Musketeer\\Musketeer_camp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		MusketeerBitmap[33] = (HBITMAP)LoadImage(g_hInst, L"Image\\Musketeer\\Musketeer_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		//---------------
		//��ų �̹��� �ε�
		for (int i = 0; i < 4; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Crusader\\Skill%d.bmp", i + 1);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			CrusaderSkillBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}

		for (int i = 0; i < 4; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Vestal\\Skill%d.bmp", i + 1);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			VestalSkillBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}

		for (int i = 0; i < 4; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Highway\\Skill%d.bmp", i + 1);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			HighwaySkillBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}

		for (int i = 0; i < 4; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Musketeer\\Skill%d.bmp", i + 1);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			MusketeerSkillBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}

		SkilloptionBitMap[0] = (HBITMAP)LoadImage(g_hInst, L"Image\\Trade.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		SkilloptionBitMap[1] = (HBITMAP)LoadImage(g_hInst, L"Image\\Pass.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		SkilloptionBitMap[2] = (HBITMAP)LoadImage(g_hInst, L"Image\\Noskill.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 5; i++) {
			TCHAR str[100];
			wsprintf(str, L"Image\\Background%d.bmp", i);
			//knightBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			BkBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}		

		effectBitmap[0] = (HBITMAP)LoadImage(g_hInst, L"Image\\X.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		effectBitmap[1] = (HBITMAP)LoadImage(g_hInst, L"Image\\Go.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 4; i++) {
			wsprintf(str, L"Image\\Enemy%02d.bmp", i + 1);
			enemyBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		//��ǥ�� �̹���
		CrusaderTurnBitmap = (HBITMAP)LoadImage(g_hInst, L"Image\\Crusader\\CrusaderTurn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		VestalTurnBitmap = (HBITMAP)LoadImage(g_hInst, L"Image\\Vestal\\VestalTurn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HighwayTurnBitmap = (HBITMAP)LoadImage(g_hInst, L"Image\\Highway\\HighwayTurn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		MusketeerTurnBitmap = (HBITMAP)LoadImage(g_hInst, L"Image\\Musketeer\\MusketeerTurn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 4; i++) {
			wsprintf(str, L"Image\\EnemyTurn%02d.bmp", i + 1);
			enemyTurnBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		//���� �̹���
		wsprintf(str, L"Image\\Map\\battle.bmp");
		MapiconBitmap[0] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		wsprintf(str, L"Image\\Map\\shop.bmp");
		MapiconBitmap[1] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		wsprintf(str, L"Image\\Map\\rest.bmp");
		MapiconBitmap[2] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		wsprintf(str, L"Image\\Map\\boss.bmp");
		MapiconBitmap[3] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		//������ �ൿ ��Ʈ��
		for (int i = 0; i < 4; i++) {
			wsprintf(str, L"Image\\Crusader\\Crusader_Skill%d.bmp", i + 1);
			CrusaderActBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			wsprintf(str, L"Image\\Vestal\\Vestal_Skill%d.bmp", i + 1);
			VestalActBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			wsprintf(str, L"Image\\Highway\\Highway_Skill%d.bmp", i + 1);
			HighwayActBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			wsprintf(str, L"Image\\Musketeer\\Musketeer_Skill%d.bmp", i + 1);
			MusketeerActBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		CrusaderActBitmap[4] = (HBITMAP)LoadImage(g_hInst, L"Image\\Crusader\\Crusader_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		VestalActBitmap[4] = (HBITMAP)LoadImage(g_hInst, L"Image\\Vestal\\Vestal_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		HighwayActBitmap[4] = (HBITMAP)LoadImage(g_hInst, L"Image\\Highway\\Highway_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		MusketeerActBitmap[4] = (HBITMAP)LoadImage(g_hInst, L"Image\\Musketeer\\Musketeer_defend.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i = 0; i < 30; i++) {
			wsprintf(str, L"Image\\Loading\\Loading_%04d.bmp", i);
			SelectionBitmap[i] = (HBITMAP)LoadImage(g_hInst, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		}
		//--------------------
		//�̹��� ��� ũ�� �ʱ�ȭ
		//--------------------
		/*
		for (int i = 0; i < 4; i++) {
			unitShape[i].px = 75 + 150 * i;
			unitShape[i].py = 300;
			unitShape[i].width = 140;
			unitShape[i].height = 300;
		}
		*/
		for (int i = 0; i < 3; i++) {
			enemyShape[i].px = size.right - 75 - 150 * i;
			enemyShape[i].py = 300;
			enemyShape[i].width = 140;
			enemyShape[i].height = 300;
		}

		for (int i = 0; i < 6; i++) {
			skillShape[i].px = 350 + 100 * i;
			skillShape[i].py = 540;
			skillShape[i].width = 80;
			skillShape[i].height = 80;
		}

		for (int i = 0; i < 7; i++) {
			turnShape[i].px = size.right - 60 * (i + 1);
			turnShape[i].py = 40;
			turnShape[i].width = 50;
			turnShape[i].height = 50;
		}

		//�׿� ������ �ʱ�ȭ
		len = size.right / 2;
		ClipCursor(&size);

		
		//���� �ý��� ���� �ʱ�ȭ
		result = FMOD::System_Create(&ssystem); //--- ���� �ý��� ����		
		if (result != FMOD_OK)
			exit(0);
		ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata); //--- ���� �ý��� �ʱ�ȭ
		ssystem->createSound("Music\\bgm1.mp3", FMOD_LOOP_NORMAL, 0, &sound1); //--- 1�� ���� ���� �� ����
		ssystem->createSound("sound.mp3", FMOD_LOOP_NORMAL, 0, &sound2); //--- 2�� ���� ���� �� ����
	
		//ĳ���� �ִϸ��̼� ����
		SetTimer(hwnd, 2, 50, NULL);

		//������� ���
		channel->stop();
		ssystem->playSound(sound1, 0, false, &channel);

		break;
		case WM_TIMER:
			switch (wParam) {
			case 0:		//ȭ�� ��ȯ�� ȭ�� ���� Ÿ�̸� 			
				if (phase == FALSE) {
					len -= size.right / 20;
					if (len < 0) {
						KillTimer(hwnd, 2);
						len = 0;
						phase = TRUE;	
						if(menu == 3)
							DeleteTurnList(&turnlist);

						menu = nextmenu;						
					}				
				}
				else {//ȭ�� ��ȯ�� ȭ�� ���� Ÿ�̸� 
					len += size.right  / 20;
					if (len > size.right/2 ) {
						
						len = size.right/2;
						KillTimer(hwnd, 0);
						SetTimer(hwnd, 2, 50, NULL);
						phase = FALSE;
						MouseLock = FALSE;
						DisplayChange = FALSE;
					}
				}
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
		break;
	}

	//Ÿ��Ʋ ȭ���� ���
	if (menu == 0) {
		TCHAR str[100];
		switch (uMsg) {		
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
			SelectObject(imgdc, (HBITMAP)BkBitmap[0]);
			GetObject(BkBitmap[0], sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			/*
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = GetStockObject(BLACK_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

			Rectangle(mdc, 0, 0, size.right, size.bottom);

			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
			*/
			//----------------------------
			//Ÿ��Ʋ�� �޴��� ���
			//----------------------------
				//Ÿ��Ʋ ���
			SetTextColor(mdc, RGB(255, 255, 255));	//���� ����
			SetBkMode(mdc, TRANSPARENT);			//���� ��� ����ȭ
			//SetBkMode(mdc, OPAQUE);
			hFont = CreateFont(150, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");
			oldFont = (HFONT)SelectObject(mdc, hFont);

			_tcscpy(str, L"����");
			TextOut(mdc, 450, 100, str, lstrlen(str));

			SelectObject(mdc, oldFont);
			DeleteObject(hFont);
				//Ÿ��Ʋ �޴� 1
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
			
			if (SelectCursor == 1) {
				SetTextColor(mdc, RGB(255, 255, 0));	//���� ����
				hFont = CreateFont(130, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");}
			else{ 
				SetTextColor(mdc, RGB(255, 255, 255));	//���� ����
			}

			hFont = CreateFont(100, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");
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
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
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


			//ȭ�� ��ȯ��
			if (DisplayChange == TRUE) {
				//imgdc = CreateCompatibleDC(hdc);
				imgBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, imgBitmap);


				SelectObject(imgdc, oldPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(WHITE_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, size.right / 2 - len, size.bottom / 2 - len, size.right / 2 + len, size.bottom / 2 + len);

				BitBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, SRCAND);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

			//	DeleteDC(imgdc);
				DeleteObject(imgBitmap);
			}
			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);

			DeleteDC(imgdc);
			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_MOUSEMOVE:
			if (MouseLock == FALSE) {
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
			}
			break;
		case WM_LBUTTONDOWN:
			if (MouseLock == FALSE) {
				if (SelectCursor != 0) {
					mx = LOWORD(lParam);
					my = HIWORD(lParam);
					if (SelectCursor == 1) {
						MakeList(&userlist);
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						nextmenu = 1;
						MouseLock = TRUE;
						//InvalidateRect(hwnd, NULL, FALSE);
						break;
					}
					//Rectangle(mdc, 150, 550, 330, 650); - �ι�° �޴� Rect
					if (SelectCursor == 2) {
						DestroyWindow(hwnd);
						break;
					}
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
		int charsize = 225;
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 2: //�ִϸ��̼� 
				animate++;
				animate %= 31;
				InvalidateRect(hwnd, NULL, FALSE);
				break;
				/*case 3:	//��ų ������
				InvalidateRect(hwnd, NULL, FALSE); 				
				break;*/
			}
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
			SelectObject(imgdc, (HBITMAP)BkBitmap[0]);
			GetObject(BkBitmap[0], sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------
		
			//���ù�ư ĳ���� ���
			for (int i = 0; i < 4; i++) {
				
				/*
				HBITMAP CrusaderBitmap;
				HBITMAP VestalBitmap;
				HBITMAP HighwayBitmap;
				HBITMAP MusketeerBitmap;
				*/
				switch (i) {
				case 0:
					SelectObject(imgdc, (HBITMAP)CrusaderTurnBitmap);
					GetObject(CrusaderTurnBitmap, sizeof(BITMAP), &bmp);
					break;
				case 1:
					SelectObject(imgdc, (HBITMAP)VestalTurnBitmap);
					GetObject(VestalTurnBitmap, sizeof(BITMAP), &bmp);
					break;
				case 2:
					SelectObject(imgdc, (HBITMAP)HighwayTurnBitmap);
					GetObject(HighwayTurnBitmap, sizeof(BITMAP), &bmp);
					break;
				case 3:
					SelectObject(imgdc, (HBITMAP)MusketeerTurnBitmap);
					GetObject(MusketeerTurnBitmap, sizeof(BITMAP), &bmp);
					break;
				}

				RECT tmp = { 200 + (200 * i), 550, 380 + (200 * i), 730 };

				StretchBlt(mdc, tmp.left, tmp.top, 180, 180, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
				//Rectangle(mdc, 300 + 150 * i, 550, 450 + 150 * i, 750);

				if (SelectCursor == i + 1) {
					hPen = CreatePen(PS_SOLID, 3, RGB(0, 200, 0));
				}
				else {
					hPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 0));
					//hPen = GetStockObject(NULL_PEN);
				}
				//hPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 0));
				oldPen = (HPEN)SelectObject(mdc, hPen);
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

				Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);

				//RECT tmp = { (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500 };
				//TransparentBlt(mdc, (size.right / 4)* i, 0, size.right / 4, 500, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
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

				//RECT tmp = { 300 + 150 * i,700,450 + 150 * i,750 };
				RECT tmp = { charsize * i, 450, charsize * (i + 1), 500};
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
			{
			Unit* tmp = userlist->head;			
			int i = 0; 
			while(tmp != NULL) {
				switch (tmp->CLASS) {
				case ID_USER_Crusader:
					SelectObject(imgdc, (HBITMAP)CrusaderBitmap[animate]);
					GetObject(CrusaderBitmap[animate], sizeof(BITMAP), &bmp);
					break;
				case ID_USER_Vestal:
					SelectObject(imgdc, (HBITMAP)VestalBitmap[animate]);
					GetObject(VestalBitmap[animate], sizeof(BITMAP), &bmp);
					break;
				case ID_USER_Highway:
					SelectObject(imgdc, (HBITMAP)HighwayBitmap[animate]);
					GetObject(HighwayBitmap[animate], sizeof(BITMAP), &bmp);
					break;
				case ID_USER_Musketeer:
					SelectObject(imgdc, (HBITMAP)MusketeerBitmap[animate]);
					GetObject(MusketeerBitmap[animate], sizeof(BITMAP), &bmp);
					break;
				}
				/*
				if (SelectCursor == i + 5) {
					Rectangle(mdc, 300 + 150 * i, 550, 450 + 150 * i, 750);
				}
				*/
				RECT rect = { charsize * i, 450, charsize * (i + 1), 500 };

				//TransparentBlt(mdc, 300 + 150 * i, 550, 150, 170, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
				switch(tmp->CLASS) {
				case ID_USER_Crusader:
					TransparentBlt(mdc, charsize * i, -50, charsize, 520, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
					break;
				default:
					TransparentBlt(mdc, charsize * i, 50, charsize, 420, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
					break;
				}
				//	TransparentBlt(mdc, (size.right / 4)* i, 50, (size.right / 4), 420, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
				i++;
				tmp = tmp->next;
			}

			//���õ� ������ �����ҷ��Ҷ� X�� �̹��� ���
			for (int i = 0; i < 4; i++) {
				if (SelectCursor == i + 5) {
					SelectObject(imgdc, (HBITMAP)effectBitmap[0]);
					GetObject(effectBitmap[0], sizeof(BITMAP), &bmp);					
					//Rectangle(mdc, 1000, 550, 1150, 750);//Rectangle(mdc, (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500);
					TransparentBlt(mdc, charsize* i, (size.right / 8), charsize, (size.right / 4), imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
					//TransparentBlt(mdc, 300 + 150 * i, 550, 150, 170, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
				
				}
			}

			if (userlist->n == 4) {
				SelectObject(imgdc, (HBITMAP)effectBitmap[1]);
				GetObject(effectBitmap[1], sizeof(BITMAP), &bmp);
				//Rectangle(mdc, (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500);
				TransparentBlt(mdc, 1000,250,150,200, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
				if (SelectCursor == 9) {
					hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, 1000, 250, 1150, 450);


					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
			}

			//ȭ�� ��ȯ��
			if (DisplayChange == TRUE) {
				//imgdc = CreateCompatibleDC(hdc);
				imgBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, imgBitmap);
				
				SelectObject(imgdc, oldPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(WHITE_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, size.right / 2 - len, size.bottom / 2 - len, size.right / 2 + len, size.bottom / 2 + len);

				BitBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, SRCAND);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				//DeleteDC(imgdc);
				DeleteObject(imgBitmap);
			}

			//----------------------------
			//debug �����
			//----------------------------
			/*
			TCHAR str[100];

			wsprintf(str, L"SelectCursor : %d", SelectCursor);
			TextOut(mdc, 0, 0, str, lstrlen(str));
			*/
			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);

			DeleteDC(imgdc);

			//-----------
			EndPaint(hwnd, &ps);
			}
			break;
		case WM_MOUSEMOVE:
			if (MouseLock == FALSE) {
				int before = SelectCursor;
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				// �����ϴ� ĭ�� ����
				//	RECT tmp = { (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500 };			
				for (int i = 0; i < 9; i++) {
					if (i == 4)
						continue;
					if (i < 4) {
						//if (CheckMouseRect(mx, my, (size.right / 4) * i, 0, (size.right / 4) * (i + 1), 500)) {
						if (CheckMouseRect(mx, my, 200 + 200 * i, 550, 380 + 200 * i, 720)) {
							if (SelectCursor == i + 1) {
									break;
								}
							SelectCursor = i + 1;
							//InvalidateRect(hwnd, NULL, FALSE);
							break;
						}
						if (SelectCursor <= 4) {
							SelectCursor = 0;
							//InvalidateRect(hwnd, NULL, FALSE);
						}
					}
					else {
						//if (CheckMouseRect(mx, my, 200 + 200 * (i - 5), 550, 380 + 200 * (i - 5), 720)) {
						if (CheckMouseRect(mx, my, charsize * (i - 5), 0, charsize * (i - 4), 500)) {
							if (i - 5 < userlist->n) {
								if (SelectCursor == i) {									
									break;
								}
								SelectCursor = i;
								//InvalidateRect(hwnd, NULL, FALSE);
							}
							break;
						}
						if (SelectCursor >= 5) {
							SelectCursor = 0;
							//InvalidateRect(hwnd, NULL, FALSE);
						}
					}

					if (before != SelectCursor) {
						//InvalidateRect(hwnd, NULL, FALSE);
					}

				}

				// RECT tmp = { 300 + 150 * i,700,450 + 150 * i,750 };
				/*
				for (int i = 5; i < 9; i++) {
					if (CheckMouseRect(mx, my, 300 + 150 * (i - 5), 550, 450 + 150 * (i - 5), 720)) {
							if (i - 5 < userlist->n) {
								if (SelectCursor == i) {
									break;
								}
								SelectCursor = i;
								InvalidateRect(hwnd, NULL, FALSE);
							}
							break;
						}
						if (SelectCursor >= 5) {
							SelectCursor = 0;
							InvalidateRect(hwnd, NULL, FALSE);
						}
					}
				}
				*/
				if (SelectCursor == 0 && userlist->n == 4) {
					if (CheckMouseRect(mx, my, 1000, 250, 1150, 450)) {
						SelectCursor = 9;
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (MouseLock == FALSE) {
				if (SelectCursor != 0) {
					mx = LOWORD(lParam);
					my = HIWORD(lParam);
					Unit* tmp = NULL;
					switch (SelectCursor) {
					case ID_USER_Crusader:
						if (userlist->n < 4)
							MakeUnit(userlist, ID_USER_Crusader);
						break;
					case ID_USER_Vestal:
						if (userlist->n < 4)
							MakeUnit(userlist, ID_USER_Vestal);
						break;
					case ID_USER_Highway:
						if (userlist->n < 4)
							MakeUnit(userlist, ID_USER_Highway);
						break;
					case ID_USER_Musketeer:
						if (userlist->n < 4)
							MakeUnit(userlist, ID_USER_Musketeer);
						break;
					case 5:
						tmp = FindUnit(userlist, 1);
						if (tmp->next == NULL)
							SelectCursor = 0;
						DeleteUnit(userlist, tmp);
						break;
					case 6:
						tmp = FindUnit(userlist, 2);
						if (tmp->next == NULL)
							SelectCursor = 0;
						DeleteUnit(userlist, tmp);
						break;
					case 7:
						tmp = FindUnit(userlist, 3);
						if (tmp->next == NULL)
							SelectCursor = 0;
						DeleteUnit(userlist, tmp);
						break;
					case 8:
						tmp = FindUnit(userlist, 4);
						if (tmp->next == NULL)
							SelectCursor = 0;
						DeleteUnit(userlist, tmp);
						break;
					}
					InvalidateRect(hwnd, NULL, FALSE);
					switch (SelectCursor) {
					case 9:
						//menu = 2;
						nextmenu = 2;	
						SelectCursor = 0;
						tmp = userlist->head;
						for (int i = 0; i < 4; i++) {
							unit[i] = tmp;
							tmp->space = i;
							tmp = tmp->next;
						}					
						for (int i = 0; i < 4; i++) {
							unitShape[i].px = 75 + 150 * i;
							unitShape[i].width = 140;
							switch (unit[i]->CLASS) {
							case ID_USER_Crusader:
								unitShape[i].py = 260;
								unitShape[i].height = 380;
								break;
							case ID_USER_Vestal:
								unitShape[i].py = 300;
								unitShape[i].height = 300;
								break;
							case ID_USER_Highway:
								unitShape[i].py = 300;
								unitShape[i].height = 300;
								break;
							case ID_USER_Musketeer:
								unitShape[i].py = 300;
								unitShape[i].height = 300;
								break;
							}
						}
						Map(base);
						SetTimer(hwnd, 0, 20, NULL);
						KillTimer(hwnd, 2);
						DisplayChange = TRUE;						
						MouseLock = TRUE;
						stage = 1;
						break;
					}
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}

	//����ȭ�� ��½�
	if (menu == 2) {
		int row = 170, col = 50;
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 2: //�ִϸ��̼� 
				break;
			}
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
			SelectObject(imgdc, (HBITMAP)BkBitmap[stage]);
			GetObject(BkBitmap[stage], sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

			SelectObject(imgdc, (HBITMAP)BkBitmap[4]);
			GetObject(BkBitmap[4], sizeof(BITMAP), &bmp);

			TransparentBlt(mdc, 10, 0, size.right - 20, size.bottom * 9 / 10, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------	
			
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < STAGELEN; j++) {
					switch (base[i][j].num) {
					case 0:
						SelectObject(imgdc, (HBITMAP)MapiconBitmap[0]);
						GetObject(MapiconBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case 1:
						SelectObject(imgdc, (HBITMAP)MapiconBitmap[1]);
						GetObject(MapiconBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case 2:
						SelectObject(imgdc, (HBITMAP)MapiconBitmap[2]);
						GetObject(MapiconBitmap[2], sizeof(BITMAP), &bmp);
						break;
					}							
					if (SelectCursor == i * 10 + j + 1 && j >= line-1) {
						TransparentBlt(mdc, row + (1000.0 / (STAGELEN + 2))* j-10, col + 250 * i-10, 70, 70, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
					}
					else
						TransparentBlt(mdc, row + (1000.0/ (STAGELEN + 2)) * j, col + 250 * i, 50, 50, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255,255,255));
				}
			}

			//ȭ�� ��ȯ��
			if (DisplayChange == TRUE) {
				//imgdc = CreateCompatibleDC(hdc);
				imgBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, imgBitmap);
				/*
				SelectObject(imgdc, oldPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);
				*/
				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(WHITE_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, size.right / 2 - len, size.bottom / 2 - len, size.right / 2 + len, size.bottom / 2 + len);

				BitBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, SRCAND);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				//DeleteDC(imgdc);
				DeleteObject(imgBitmap);
			}
			else {
				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(mdc, hPen);
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(mdc, hBrush);


				Ellipse(mdc, row + (1000.0 / (STAGELEN + 2)) * (line - 2) , col + 250 * now , row + (1000.0 / (STAGELEN + 2)) * (line - 2) + 50, col + 250 * now + 50);

				SelectObject(mdc, oldPen);
				DeleteObject(hPen);
				SelectObject(mdc, oldBrush);
				DeleteObject(hBrush);
			}
		
			//----------------------------
			//debug �����
			//----------------------------
			
			TCHAR str[100];

			wsprintf(str, L"SelectCursor : %d", SelectCursor);
			TextOut(mdc, 0, 0, str, lstrlen(str));

			wsprintf(str, L"base[%d][%d]", (SelectCursor-1) / STAGELEN, (SelectCursor - 1) % STAGELEN);
			TextOut(mdc, 0, 30, str, lstrlen(str));

			wsprintf(str, L"Line : %d", line);
			TextOut(mdc, 0, 60, str, lstrlen(str));

			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);

			DeleteDC(imgdc);
			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_MOUSEMOVE:
			if (MouseLock == FALSE) {
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				for (int i = 0; i < 3; i++) {
					int check = 0;
					for (int j = 0; j < STAGELEN; j++) {
						if (CheckMouseRect(mx, my, row + (1000.0 / (STAGELEN + 2)) * j, col + 250 * i, row + (1000.0 / (STAGELEN+2)) * j + 50, col + 50 + 250 * i )) {
							if (SelectCursor != i*10 + j + 1) {
								SelectCursor = i * 10 + j + 1;
								InvalidateRect(hwnd, NULL, FALSE);
								
							}
							check = 1;
							break;
						}
						if (i == 2 && j == 9 && SelectCursor != i * 10 + j + 1) {
							SelectCursor = 0;
							InvalidateRect(hwnd, NULL, FALSE);
						}
					}
					if (check == 1)
						break;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (MouseLock == FALSE && SelectCursor != 0) {				
				if (SelectCursor % STAGELEN == line) {
					int tmp = SelectCursor - 1;
					now = tmp / STAGELEN;
					int seed = base[tmp / STAGELEN][tmp % STAGELEN].seed;
					switch (base[tmp / STAGELEN][tmp % STAGELEN].num + 1) {
					case 1:		//���� �������		
						MakeList(&enemylist);
						switch (stage) {
						case 1:
							switch (seed) {
							case 0:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								break;
							case 1:
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							case 2:
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								break;
							case 3:
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								break;
							case 4:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							}
							break;
						case 2:
							switch (seed) {
							case 0:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								break;
							case 1:
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							case 2:
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								break;
							case 3:
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								break;
							case 4:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							}
							break;
						case 3:
							switch (seed) {
							case 0:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								break;
							case 1:
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							case 2:
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								MakeUnit(enemylist, ID_ENEMY_NAME3);
								break;
							case 3:
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								MakeUnit(enemylist, ID_ENEMY_NAME4);
								break;
							case 4:
								MakeUnit(enemylist, ID_ENEMY_NAME1);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								MakeUnit(enemylist, ID_ENEMY_NAME2);
								break;
							}
							break;
						}
						{
							Unit* tmp = enemylist->head;
							for (int i = 0; i < 3; i++) {
								if (tmp != NULL) {
									enemy[i] = tmp;
									tmp->space = i;
									tmp = tmp->next;
								}
								else {
									enemy[i] = NULL;
								}
							}

							//�ϰ��� 
							MakeList(&turnlist);
							MakeTurnList(turnlist, userlist);
							MakeTurnList(turnlist, enemylist);
							SortTurnList(turnlist);

							tmp = turnlist->head;
							while (tmp->turnnext != NULL) {
								tmp = tmp->turnnext;
							}
							tmp->turnnext = turnlist->head;
						}
						//ȭ����ȯ����
						line++;
						nextmenu = 3;
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						MouseLock = TRUE;
						break;
					case 2:		//���� �������


						//ȭ����ȯ����
						line++;
						nextmenu = 4;
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						MouseLock = TRUE;

						break;
					case 3:		//�޽� �������

						//ȭ����ȯ����
						line++;
						nextmenu = 5;
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						MouseLock = TRUE;
						break;
					}
				

				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}


	//���� ������ ���
	if (menu == 3) {
		TCHAR str[100];
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 2: //�ִϸ��̼� 
				animate++;
				animate %= 31;
				InvalidateRect(hwnd, NULL, FALSE);
				break;

			case 3:	//�ൿ �ִϸ��̼�				
				timer++;
				if (timer > 28) {
					KillTimer(hwnd, 3);					
					//�� �Ʒ��� ��ų�ߵ��� ������ �Ǵ°�(Ÿ�̸ӿ� �־�� �ҵ�?)					
					turnlist->head = turnlist->head->turnnext;
					if (turnlist->head->CLASS <= 4) { //�������� ������ ���ϰ��
						act = FALSE;
						selectSkill = -1;
						SelectUnit = NULL;
						MouseLock = FALSE;
						SetTimer(hwnd, 2, 50, NULL);
					}
					else {		//�������� ���� ���ϰ��
						selectSkill = -1;
						SelectUnit = NULL;
						act = FALSE;
						SelectCursor = turnlist->head->space + 5;

						SetTimer(hwnd, 2, 50, NULL);		
						SetTimer(hwnd, 4, 800, NULL);
					}
					if (enemylist->n == 0) {							
						nextmenu = 2;
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						MouseLock = TRUE;
					}
					if (userlist->n == 0) {
						DeleteList(&userlist);
						DeleteList(&enemylist);					
						for (int i = 0; i < 3; i++) {
							enemy[i] = NULL;
						}

						nextmenu = 0;
						SelectCursor = 0;
						SetTimer(hwnd, 0, 20, NULL);
						DisplayChange = TRUE;
						MouseLock = TRUE;
					}
				}
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			case 4:	//������ ���۱��� ������
				KillTimer(hwnd, 4);
				//���� �ൿ���� ����
				{
					Unit* tmp = turnlist->head;			

					if (tmp->hp > tmp->Maxhp/2) {
						selectSkill = rand() % 2;
					}
					else {
						selectSkill = rand() % 3;
					}

					switch (tmp->skill[selectSkill]) {
					case SKILL_Monster_attack:
					{
						int i = rand() % 4;
						SelectUnit = unit[i];
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
					}	
						break;
					case SKILL_Monster_defend:
					{
						Shield(tmp, 7 + turnlist->head->AtkBuff);
					}
						break;
					case SKILL_Monster_healing:
					{
						Heal(tmp, 7); 
					}
						break;
					case SKILL_Monster_seldefend:
					{						
						int i = rand() % 3;
						SelectUnit = enemy[i];
						Shield(SelectUnit, 7); 
					}
						break;
					case SKILL_Monster_selhealing:
					{
						int i = rand() % 3;
						SelectUnit = enemy[i];
						Shield(SelectUnit, 7); 
					}
						break;
					case SKILL_Monster_allattack:
					{
						tmp = userlist->head;
						for (int i = 0; tmp != NULL; tmp = tmp->next) {
							Attack(tmp, 4 + turnlist->head->AtkBuff);
						}
					}
						break;
					}				
				}
				//�ٽ� act ����
				MouseLock = TRUE;
				act = TRUE;
				timer = 0;
				KillTimer(hwnd, 2);
				SetTimer(hwnd, 3,20,NULL);
				break;
			}
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
			SelectObject(imgdc, (HBITMAP)BkBitmap[stage]);
			GetObject(BkBitmap[stage], sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------
			//user���� ���
			for (int i = 0; i < userlist->n; i++) {
				if (unit[i] != NULL) {				
					RECT rect = DrawShape(&unitShape[i]);					
					switch (unit[i]->CLASS) {
					case ID_USER_Crusader:
						SelectObject(imgdc, (HBITMAP)CrusaderBitmap[animate]);
						GetObject(CrusaderBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Vestal:
						SelectObject(imgdc, (HBITMAP)VestalBitmap[animate]);
						GetObject(VestalBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Highway:
						SelectObject(imgdc, (HBITMAP)HighwayBitmap[animate]);
						GetObject(HighwayBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Musketeer:
						SelectObject(imgdc, (HBITMAP)MusketeerBitmap[animate]);
						GetObject(MusketeerBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					}
					/*
					if (SelectCursor == i + 1) {
						hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = GetStockObject(NULL_BRUSH);
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);
						Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);
						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);
					}
					*/
					TransparentBlt(mdc, unitShape[i].px - unitShape[i].width / 2, unitShape[i].py - unitShape[i].height / 2, unitShape[i].width, unitShape[i].height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

					rect.top += unitShape[i].height;
					rect.bottom += 10;

					//�ǵ尡 ������ ���
					if(unit[i]->def != 0){
						hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = CreateSolidBrush(RGB(200, 200, 200));
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

						int len = unit[i]->def > unit[i]->Maxhp? unit[i]->Maxhp : unit[i]->def;
						Rectangle(mdc, rect.left, rect.top, (int)((double)rect.left + (double)unitShape[i].width * ((double)len / (double)unit[i]->Maxhp)), rect.bottom);

						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);
					}
					rect.top += 10;
					rect.bottom += 10;
					//�ִ�ü���� ��ĭ(����) ���
					if (turnlist->head == unit[i]) {
						hPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 0));
					}
					else
						hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);
					
					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//���� ü�� (����ĭ) ���
					
					hPen = (HPEN)GetStockObject(NULL_PEN);
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(165, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, rect.left, rect.top, (int)((double)rect.left + (double)unitShape[i].width * ((double)unit[i]->hp/(double)unit[i]->Maxhp)), rect.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					/*
					TCHAR str[100];					
					SetBkMode(mdc,TRANSPARENT);
					switch (unit[i]->CLASS) {
					case ID_USER_Crusader:
						wsprintf(str, L"���");
						DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER | DT_VCENTER);
						break;					
					case ID_USER_Vestal:
						wsprintf(str, L"�̴ܽ��ǰ�");
						DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER | DT_VCENTER);
						break;
					case ID_USER_Highway:
						wsprintf(str, L"����");
						DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER | DT_VCENTER);
						break;
					case ID_USER_Musketeer:
						wsprintf(str, L"������");
						DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER | DT_VCENTER);
						break;
					}
					SetBkMode(mdc, OPAQUE);
					*/				
				}
			}
				
			//�� ���
			for (int i = 0; i < enemylist->n; i++) {
				if (enemy[i] != NULL) {
					RECT rect = DrawShape(&enemyShape[i]);
					switch (enemy[i]->CLASS) {
					case ID_ENEMY_NAME1:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[0]);
						GetObject(enemyBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME2:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[1]);
						GetObject(enemyBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME3:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[2]);
						GetObject(enemyBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME4:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[3]);
						GetObject(enemyBitmap[3], sizeof(BITMAP), &bmp);
						break;
					}				
					
					TransparentBlt(mdc, enemyShape[i].px - enemyShape[i].width / 2, enemyShape[i].py - enemyShape[i].height / 2, enemyShape[i].width, enemyShape[i].height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

					//Rectangle(mdc, tmp.left, tmp.top, tmp.right, tmp.bottom);
					if (SelectCursor == i + 5 && selectSkill != -1) {
						int id = turnlist->head->skill[selectSkill];
						if (skill[id / 10][id % 10].num == 0) {
							SelectObject(imgdc, (HBITMAP)SelectionBitmap[animate % 30]);
							GetObject(SelectionBitmap[animate % 30], sizeof(BITMAP), &bmp);

							//enemyShape[i];
							//Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);
							TransparentBlt(mdc, enemyShape[i].px - enemyShape[i].width / 4, enemyShape[i].py - enemyShape[i].width / 4, enemyShape[i].width/2, enemyShape[i].width / 2, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

						}
					}


					rect.top += enemyShape[i].height;
					rect.bottom += 10;

					//�ǵ尡 ������ ���
					if (enemy[i]->def != 0) {
						hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = CreateSolidBrush(RGB(200, 200, 200));
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

						int len = enemy[i]->def > enemy[i]->Maxhp ? enemy[i]->Maxhp : enemy[i]->def;
						Rectangle(mdc, rect.left, rect.top, (int)((double)rect.left + (double)enemyShape[i].width * ((double)len / (double)enemy[i]->Maxhp)), rect.bottom);

						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);
					}

					rect.top += 10;
					rect.bottom += 10;
					//�ִ�ü���� ��ĭ(����) ���
					if (turnlist->head == enemy[i]) {
						hPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 0));
					}
					else
						hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//���� ü�� (����ĭ) ���

					hPen = (HPEN)GetStockObject(NULL_PEN);
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(165, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, rect.left, rect.top, (int)((double)rect.left + (double)enemyShape[i].width * ((double)enemy[i]->hp / (double)enemy[i]->Maxhp)), rect.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

				}
			}
			//���ֵ� ��� �Ʒ� �׸� UI
			//UIĭ ���
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(mdc, hPen);
			hBrush = CreateSolidBrush(RGB(150, 150, 150));
			oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

			Rectangle(mdc, 0, 470, size.right, size.bottom);

			SelectObject(mdc, oldPen);
			SelectObject(mdc, oldBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);

			//��ų ���
			if (1) {
				Unit* tmp = turnlist->head;

				if (SelectCursor > 0 && SelectCursor <= userlist->n) {
					tmp = unit[SelectCursor - 1];
				}

				
				for (int i = 0; i < 6; i++) {
					RECT rect = DrawShape(&skillShape[i]);
					//���� ������� ������ ���� = turnlist�� head;
					if (i < 4) {
						switch (tmp->skill[i]) {						
							//Crusader
						case SKILL_Crusader_Smite:
							SelectObject(imgdc, (HBITMAP)CrusaderSkillBitmap[0]);
							GetObject(CrusaderSkillBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case SKILL_Crusader_HolyLance:
							SelectObject(imgdc, (HBITMAP)CrusaderSkillBitmap[1]);
							GetObject(CrusaderSkillBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case SKILL_Crusader_Zealous_Speech:
							SelectObject(imgdc, (HBITMAP)CrusaderSkillBitmap[2]);
							GetObject(CrusaderSkillBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case SKILL_Crusader_Block:
							SelectObject(imgdc, (HBITMAP)CrusaderSkillBitmap[3]);
							GetObject(CrusaderSkillBitmap[3], sizeof(BITMAP), &bmp);
							break;
							//Vestal
						case 11:
							SelectObject(imgdc, (HBITMAP)VestalSkillBitmap[0]);
							GetObject(VestalSkillBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case 12:
							SelectObject(imgdc, (HBITMAP)VestalSkillBitmap[1]);
							GetObject(VestalSkillBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case 13:
							SelectObject(imgdc, (HBITMAP)VestalSkillBitmap[2]);
							GetObject(VestalSkillBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case 14:
							SelectObject(imgdc, (HBITMAP)VestalSkillBitmap[3]);
							GetObject(VestalSkillBitmap[3], sizeof(BITMAP), &bmp);
							break;
							//Highway
						case 21:
							SelectObject(imgdc, (HBITMAP)HighwaySkillBitmap[0]);
							GetObject(HighwaySkillBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case 22:
							SelectObject(imgdc, (HBITMAP)HighwaySkillBitmap[1]);
							GetObject(HighwaySkillBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case 23:
							SelectObject(imgdc, (HBITMAP)HighwaySkillBitmap[2]);
							GetObject(HighwaySkillBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case 24:
							SelectObject(imgdc, (HBITMAP)HighwaySkillBitmap[3]);
							GetObject(HighwaySkillBitmap[3], sizeof(BITMAP), &bmp);
							break;
							//Musketeer
						case 31:
							SelectObject(imgdc, (HBITMAP)MusketeerSkillBitmap[0]);
							GetObject(MusketeerSkillBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case 32:
							SelectObject(imgdc, (HBITMAP)MusketeerSkillBitmap[1]);
							GetObject(MusketeerSkillBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case 33:
							SelectObject(imgdc, (HBITMAP)MusketeerSkillBitmap[2]);
							GetObject(MusketeerSkillBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case 34:
							SelectObject(imgdc, (HBITMAP)MusketeerSkillBitmap[3]);
							GetObject(MusketeerSkillBitmap[3], sizeof(BITMAP), &bmp);
							break;
						default:
							SelectObject(imgdc, (HBITMAP)SkilloptionBitMap[2]);
							GetObject(SkilloptionBitMap[2], sizeof(BITMAP), &bmp);
							break;
						}
					}
					else {
						SelectObject(imgdc, (HBITMAP)SkilloptionBitMap[i - 4]);
						GetObject(SkilloptionBitMap[i - 4], sizeof(BITMAP), &bmp);
					}

					TransparentBlt(mdc, skillShape[i].px - skillShape[i].width / 2, skillShape[i].py - skillShape[i].height / 2, skillShape[i].width, skillShape[i].height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

					if (SelectCursor == i + userlist->n + enemylist->n + 1) {
						tmpBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
						SelectObject(imgdc, (HBITMAP)tmpBitmap);

						hPen = (HPEN)GetStockObject(NULL_PEN);
						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
						//hBrush = CreateSolidBrush(RGB(128, 128, 128));
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

						Rectangle(imgdc, 0, 0, size.right, size.bottom);


						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);

						BLENDFUNCTION bf;
						bf.AlphaFormat = 0; // ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
						bf.BlendFlags = 0; // ������ 0�̾�� �Ѵ�
						bf.BlendOp = AC_SRC_OVER; // ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
						bf.SourceConstantAlpha = 127; // ����(���� 0 - ������ 255)

						AlphaBlend(mdc, skillShape[i].px - skillShape[i].width / 2, skillShape[i].py - skillShape[i].height / 2, skillShape[i].width, skillShape[i].height, imgdc, 0, 0, skillShape[i].width, skillShape[i].height, bf);

						//Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);
						DeleteObject(tmpBitmap);


					}
					if (i == selectSkill && ((SelectCursor > 0 && SelectCursor <= userlist->n && turnlist->head == unit[SelectCursor - 1]) || SelectCursor == 0 || SelectCursor >= userlist->n + 1)) {
						hPen = CreatePen(PS_SOLID, 3, RGB(0, 150, 0));
					}
					else
						hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
					//hBrush = CreateSolidBrush(RGB(128, 128, 128));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);
				}
			}
			
			//��ų ���� ���
			if (SelectCursor > userlist->n + enemylist->n && SelectCursor <= userlist->n + enemylist->n + 6 || selectSkill != -1) {				
				RECT sentence = { 350,630,950,size.bottom - 10 };
				hFont = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");
				oldFont = (HFONT)SelectObject(mdc, hFont);
				SetBkMode(mdc, TRANSPARENT);
				SetTextColor(mdc, RGB(0, 0, 0));

				int target = selectSkill;
				if (SelectCursor > userlist->n + enemylist->n && SelectCursor <= userlist->n + enemylist->n + 6) {
					target = SelectCursor - (userlist->n + enemylist->n + 1);
				}
				if ((SelectCursor <= userlist->n + enemylist->n+4 && SelectCursor > userlist->n + enemylist->n)|| (selectSkill < 4 && selectSkill >=0)) {
					switch (turnlist->head->skill[target]) {
					default:
						wsprintf(str, L"\0");
						break;
					//Crusader
					case SKILL_Crusader_Smite:
						//Attack(SelectUnit, 7 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 7 + turnlist->head->AtkBuff);
						break;
					case SKILL_Crusader_HolyLance:
						/*
						if (SelectUnit->def > 0)
							Attack(SelectUnit, 10 + turnlist->head->AtkBuff);
						else
							Attack(SelectUnit, 2 + turnlist->head->AtkBuff);*/
						wsprintf(str, L"�ǵ尡 �ִ� ��󿡰� '%d' ���ظ� �����ϴ�. �ǵ尡 ���ٸ� '%d' ���ظ� ��� �����ϴ�.", 10 + turnlist->head->AtkBuff, 2 + turnlist->head->AtkBuff);
						break;
					case SKILL_Crusader_Zealous_Speech:
						/*
						for (Unit* tmp = enemylist->head; tmp != NULL; tmp = tmp->next) {
							Attack(SelectUnit, 1 + turnlist->head->AtkBuff);
						}*/
						//wsprintf(str, L"�Ʊ� ��ο��� '%d'�� �ǵ带 �����մϴ�.", 5);
						wsprintf(str, L"�� ��ü���� '%d' ���ظ� ���մϴ�.", 1 + turnlist->head->AtkBuff);
						break;
					case SKILL_Crusader_Block:
						//Attack(SelectUnit, 5 + turnlist->head->atk);
						/*for (Unit* tmp = userlist->head; tmp != NULL; tmp = tmp->next) {
							Shield(tmp, 5);
						}*/
						wsprintf(str, L"�Ʊ� ��ο��� '%d'�� �ǵ带 �����մϴ�.", 5);
						break;

					//Vestal							
					case SKILL_Vestal_Mace:
						//Attack(SelectUnit, 3 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 3 + turnlist->head->AtkBuff);
						break;
					case SKILL_Vestal_Lectionary:
						//Attack(SelectUnit, 4);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 4);
						break;
					case SKILL_Vestal_Miracle:
						/*
						for (Unit* tmp = userlist->head; tmp != NULL; tmp = tmp->next) {
							Heal(tmp, 3 + turnlist->head->AtkBuff);
						}*/
						wsprintf(str, L"�Ʊ� ��ü�� ü���� '%d' ȸ���մϴ�.", 3 + turnlist->head->AtkBuff);
						break;
					case SKILL_Vestal_Heal:
						//Heal(SelectUnit, 10 + turnlist->head->AtkBuff);
						wsprintf(str, L"����� ü���� '%d' ȸ���մϴ�.", 10 + turnlist->head->AtkBuff);
						break;

					//Highway
					case SKILL_Highway_Slash:
						//Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Pistol:
						//Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Hipshot:
						//Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Lunge:
						//Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� ������ �� ��ĭ���� �̵��մϴ�.", 5 + turnlist->head->AtkBuff);
						break;

					//Musketeer							
					case SKILL_Musketeer_rifle:
						//Attack(SelectUnit, 7 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 7 + turnlist->head->AtkBuff);
						break;
					case SKILL_Musketeer_pistol:
						//Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						wsprintf(str, L"��󿡰� '%d' ���ظ� �����ϴ�.", 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Musketeer_Buckshot:
						/*
						for (Unit* tmp = enemylist->head; tmp != NULL; tmp = tmp->next) {
							Attack(SelectUnit, 3 - timer + turnlist->head->AtkBuff);
							timer++;
						}
						timer = 0;*/
						wsprintf(str, L"�� ��ü���� �ּ��� �������� ������ ���մϴ�. (�ִ� �����: '%d') ", 3 + turnlist->head->AtkBuff);
						break;
					case SKILL_Musketeer_Selfheal:
						//Attack(turnlist->head, 7);
						wsprintf(str, L"�ڽ��� ü���� '7' ȸ���մϴ�.");
						break;
					}
				}				
				else if (SelectCursor == (userlist->n + enemylist->n + 5) || selectSkill == 4) {
					wsprintf(str, L"�Ʊ����ְ� �ڸ��� �ٲߴϴ�.");					
				}
				else {
					wsprintf(str, L"�ƹ��͵� �����ʰ� ���� �ѱ�ϴ�.");
				}
				DrawText(mdc, str, lstrlen(str), &sentence, DT_LEFT | DT_WORDBREAK);

				SetBkMode(mdc, OPAQUE);
				SelectObject(mdc, (HFONT)oldFont);
				DeleteObject(hFont);
			}
			//�� ������ ���� ��(�Ǵ� Ŀ���� �ø� �Ʊ�����)
			if (1) {
				Unit* tmp = turnlist->head;				
				if (SelectCursor > 0 && SelectCursor <= userlist->n) {
					tmp = unit[SelectCursor - 1];
				}

				if (tmp != NULL && tmp->CLASS < 5) {
					switch (tmp->CLASS) {
					case ID_USER_Crusader:
						SelectObject(imgdc, (HBITMAP)CrusaderTurnBitmap);
						GetObject(CrusaderTurnBitmap, sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Vestal:
						SelectObject(imgdc, (HBITMAP)VestalTurnBitmap);
						GetObject(VestalTurnBitmap, sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Highway:
						SelectObject(imgdc, (HBITMAP)HighwayTurnBitmap);
						GetObject(HighwayTurnBitmap, sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Musketeer:
						SelectObject(imgdc, (HBITMAP)MusketeerTurnBitmap);
						GetObject(MusketeerTurnBitmap, sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME1:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[0]);
						GetObject(enemyTurnBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME2:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[1]);
						GetObject(enemyTurnBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME3:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[2]);
						GetObject(enemyTurnBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME4:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[3]);
						GetObject(enemyTurnBitmap[3], sizeof(BITMAP), &bmp);
						break;
					}
					TransparentBlt(mdc, 5, 500, 140, 140, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));


					hPen = CreatePen(PS_SOLID, 3, RGB(255, 150, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, 5, 500, 145, 640);
					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//�ִ�ü���� ��ĭ(����) ���

					hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, 5, 650, 145, 675);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//���� ü�� (����ĭ) ���

					hPen = (HPEN)GetStockObject(NULL_PEN);
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(165, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, 5, 650, (int)((double)5 + (double)140 * ((double)tmp->hp / ((double)tmp->Maxhp + (double)tmp->def))), 675);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					if (tmp->def > 0) {
						hPen = (HPEN)GetStockObject(NULL_PEN);
						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = CreateSolidBrush(RGB(200, 200, 200));
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

						Rectangle(mdc, (int)((double)5 + (double)140 * ((double)tmp->hp / ((double)tmp->Maxhp + (double)tmp->def))), 650, (int)((double)5 + (double)140 * (((double)tmp->hp + (double)tmp->def) / ((double)tmp->Maxhp + (double)tmp->def))), 675);

						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);

					}
					//���� ���� ���
					TCHAR str[100];
					SetBkMode(mdc, TRANSPARENT);
					SetTextColor(mdc, RGB(200, 200, 200));
					hFont = CreateFont(25, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");
					oldFont = (HFONT)SelectObject(mdc, hFont);

					wsprintf(str, L"%d/%d", tmp->hp, tmp->Maxhp);
					RECT rect = { 5, 650, 145, 675 };
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);
					//TextOut(mdc, 5, 650, str,lstrlen(str));

					rect.top += 30;
					rect.bottom += 30;
					wsprintf(str, L"ATK: %d", tmp->AtkBuff);
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);

					rect.top += 30;
					rect.bottom += 30;
					wsprintf(str, L"Shield: %d", tmp->def);
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);
				}
			}
			//�� ���� ���
			if (1) {
				Unit* tmp = turnlist->head;
				for (int i = 0; i < userlist->n + enemylist->n; i++) {
					int t = turnlist->n - 1 - i;
					if (t < 0)
						continue;
					RECT rect = DrawShape(&turnShape[t]);
					if (tmp != NULL) {
						switch (tmp->CLASS) {
						case ID_USER_Crusader:
							SelectObject(imgdc, (HBITMAP)CrusaderTurnBitmap);
							GetObject(CrusaderTurnBitmap, sizeof(BITMAP), &bmp);
							break;
						case ID_USER_Vestal:
							SelectObject(imgdc, (HBITMAP)VestalTurnBitmap);
							GetObject(VestalTurnBitmap, sizeof(BITMAP), &bmp);
							break;
						case ID_USER_Highway:
							SelectObject(imgdc, (HBITMAP)HighwayTurnBitmap);
							GetObject(HighwayTurnBitmap, sizeof(BITMAP), &bmp);
							break;
						case ID_USER_Musketeer:
							SelectObject(imgdc, (HBITMAP)MusketeerTurnBitmap);
							GetObject(MusketeerTurnBitmap, sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME1:
							SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[0]);
							GetObject(enemyTurnBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME2:
							SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[1]);
							GetObject(enemyTurnBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME3:
							SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[2]);
							GetObject(enemyTurnBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME4:
							SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[3]);
							GetObject(enemyTurnBitmap[3], sizeof(BITMAP), &bmp);
							break;
						}
						TransparentBlt(mdc, turnShape[t].px - turnShape[t].width / 2, turnShape[t].py - turnShape[t].height / 2, turnShape[t].width, turnShape[t].height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

						if (i == 0)
							hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
						else
							hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));

						oldPen = (HPEN)SelectObject(mdc, hPen);
						hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
						oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

						Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);

						SelectObject(mdc, oldPen);
						SelectObject(mdc, oldBrush);
						DeleteObject(hPen);
						DeleteObject(hBrush);

						tmp = tmp->turnnext;
					}
				}
			}

			//�������ֿ� Ŀ�� �ø���
			if (1) {
				Unit* tmp = NULL;
				if (SelectCursor > userlist->n && SelectCursor <= userlist->n + enemylist->n) {
					tmp = enemy[SelectCursor - (userlist->n + 1)];
				}
				if (tmp != NULL) {
					switch (tmp->CLASS) {
					case ID_ENEMY_NAME1:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[0]);
						GetObject(enemyTurnBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME2:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[1]);
						GetObject(enemyTurnBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME3:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[2]);
						GetObject(enemyTurnBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME4:
						SelectObject(imgdc, (HBITMAP)enemyTurnBitmap[3]);
						GetObject(enemyTurnBitmap[3], sizeof(BITMAP), &bmp);
						break;
					}
					TransparentBlt(mdc, (size.right - 150), 500, 140, 140, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));

					hPen = CreatePen(PS_SOLID, 3, RGB(255, 150, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, (size.right - 150), 500, size.right, 640);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//�ִ�ü���� ��ĭ(����) ���

					hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, size.right - 150, 650, size.right, 675);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//���� ü�� (����ĭ) ���

					hPen = (HPEN)GetStockObject(NULL_PEN);
					oldPen = (HPEN)SelectObject(mdc, hPen);
					hBrush = CreateSolidBrush(RGB(165, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mdc, hBrush);

					Rectangle(mdc, size.right - 150, 650, (int)((double)(size.right - 150) + (double)150 * ((double)tmp->hp / (double)tmp->Maxhp)), 675);

					SelectObject(mdc, oldPen);
					SelectObject(mdc, oldBrush);
					DeleteObject(hPen);
					DeleteObject(hBrush);

					//���� ���� ���
					TCHAR str[100];
					SetBkMode(mdc, TRANSPARENT);
					SetTextColor(mdc, RGB(200, 200, 200));
					hFont = CreateFont(25, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, L"����");
					oldFont = (HFONT)SelectObject(mdc, hFont);

					wsprintf(str, L"%d/%d", tmp->hp, tmp->Maxhp);
					RECT rect = { size.right - 150, 650, size.right, 675 };
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);
					//TextOut(mdc, 5, 650, str,lstrlen(str));

					rect.top += 30;
					rect.bottom += 30;
					wsprintf(str, L"ATK: %d", tmp->AtkBuff);
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);

					rect.top += 30;
					rect.bottom += 30;
					wsprintf(str, L"DEF: %d", tmp->def);
					DrawText(mdc, str, lstrlen(str), &rect, DT_CENTER);
				}
			}

			//ȭ�� ��ȯ��
			if (DisplayChange == TRUE) {
				//imgdc = CreateCompatibleDC(hdc);
				imgBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, imgBitmap);

				SelectObject(imgdc, oldPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hPen);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(WHITE_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, size.right / 2 - len, size.bottom / 2 - len, size.right / 2 + len, size.bottom / 2 + len);

				BitBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, SRCAND);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				//DeleteDC(imgdc);
				DeleteObject(imgBitmap);
			}
			
			//----------------------------
			//debug �����
			//----------------------------

			TCHAR str[100];

			wsprintf(str, L"SelectCursor : %d", SelectCursor);
			TextOut(mdc, 0, 0, str, lstrlen(str));

			wsprintf(str, L"timer : %d", timer);
			TextOut(mdc, 0, 30, str, lstrlen(str));


			wsprintf(str, L"SelectSkill : %d", selectSkill);
			TextOut(mdc, 0, 60, str, lstrlen(str));

			//�ൿ �����߿� ���
			if (act == TRUE) {
				tmpBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, (HBITMAP)tmpBitmap);

				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				BLENDFUNCTION bf;
				bf.AlphaFormat = 0; // ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
				bf.BlendFlags = 0; // ������ 0�̾�� �Ѵ�
				bf.BlendOp = AC_SRC_OVER; // ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
				bf.SourceConstantAlpha = 127; // ����(���� 0 - ������ 255)

				AlphaBlend(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, size.right, size.bottom, bf);


				//Ÿ�� ���
				if (SelectUnit != NULL) {
					if (SelectUnit->CLASS <= 4) {
						if (turnlist->head->CLASS <= 4) {
							switch (SelectUnit->CLASS) {
							case ID_USER_Crusader:
								SelectObject(imgdc, (HBITMAP)CrusaderBitmap[0]);
								GetObject(CrusaderBitmap[0], sizeof(BITMAP), &bmp);
								break;
								//Vestal
							case ID_USER_Vestal:
								SelectObject(imgdc, (HBITMAP)VestalBitmap[0]);
								GetObject(VestalBitmap[0], sizeof(BITMAP), &bmp);
								break;
								//Highway
							case ID_USER_Highway:
								SelectObject(imgdc, (HBITMAP)HighwayBitmap[0]);
								GetObject(HighwayBitmap[0], sizeof(BITMAP), &bmp);
								break;
								//Musketeer
							case ID_USER_Musketeer:
								SelectObject(imgdc, (HBITMAP)MusketeerBitmap[0]);
								GetObject(MusketeerBitmap[0], sizeof(BITMAP), &bmp);
								break;
							}
							Shape actor = { 540 + timer,400,160,400 };
							TransparentBlt(mdc, actor.px - actor.width / 2, actor.py - actor.height / 2, actor.width, actor.height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
							DeleteObject(tmpBitmap);
							//BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
						}
						else {
							switch (SelectUnit->CLASS) {
							case ID_USER_Crusader:
								SelectObject(imgdc, (HBITMAP)CrusaderBitmap[33]);
								GetObject(CrusaderBitmap[33], sizeof(BITMAP), &bmp);
								break;
								//Vestal
							case ID_USER_Vestal:
								SelectObject(imgdc, (HBITMAP)VestalBitmap[33]);
								GetObject(VestalBitmap[33], sizeof(BITMAP), &bmp);
								break;
								//Highway
							case ID_USER_Highway:
								SelectObject(imgdc, (HBITMAP)HighwayBitmap[33]);
								GetObject(HighwayBitmap[33], sizeof(BITMAP), &bmp);
								break;
								//Musketeer
							case ID_USER_Musketeer:
								SelectObject(imgdc, (HBITMAP)MusketeerBitmap[33]);
								GetObject(MusketeerBitmap[33], sizeof(BITMAP), &bmp);
								break;
							}
							Shape actor = { 540 - timer,400,160,400 };

							TransparentBlt(mdc, actor.px - actor.width / 2, actor.py - actor.height / 2, actor.width, actor.height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
							DeleteObject(tmpBitmap);
							//BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
						}
					}
					else {
						//------------------------
						switch (SelectUnit->CLASS) {
						case ID_ENEMY_NAME1:
							SelectObject(imgdc, (HBITMAP)enemyBitmap[0]);
							GetObject(enemyBitmap[0], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME2:
							SelectObject(imgdc, (HBITMAP)enemyBitmap[1]);
							GetObject(enemyBitmap[1], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME3:
							SelectObject(imgdc, (HBITMAP)enemyBitmap[2]);
							GetObject(enemyBitmap[2], sizeof(BITMAP), &bmp);
							break;
						case ID_ENEMY_NAME4:
							SelectObject(imgdc, (HBITMAP)enemyBitmap[3]);
							GetObject(enemyBitmap[3], sizeof(BITMAP), &bmp);
							break;
						}
						Shape actor;
						if (turnlist->head->CLASS <= 4) {
							actor = { 750 + timer,400,160,400 };
						}
						else {
							actor = { 750 - timer,400,160,400 };
						}
						TransparentBlt(mdc, actor.px - actor.width / 2, actor.py - actor.height / 2, actor.width, actor.height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
					}
				}
			

				//�� ����� -> ����
				if (turnlist->head->CLASS <= 4) {
					Unit* tmp = turnlist->head;
					switch (tmp->skill[selectSkill]) {
					case SKILL_Crusader_Smite:
						SelectObject(imgdc, (HBITMAP)CrusaderActBitmap[0]);
						GetObject(CrusaderActBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case SKILL_Crusader_HolyLance:
						SelectObject(imgdc, (HBITMAP)CrusaderActBitmap[1]);
						GetObject(CrusaderActBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case SKILL_Crusader_Zealous_Speech:
						SelectObject(imgdc, (HBITMAP)CrusaderActBitmap[2]);
						GetObject(CrusaderActBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case SKILL_Crusader_Block:
						SelectObject(imgdc, (HBITMAP)CrusaderActBitmap[3]);
						GetObject(CrusaderActBitmap[3], sizeof(BITMAP), &bmp);
						break;
						//Vestal
					case 11:
						SelectObject(imgdc, (HBITMAP)VestalActBitmap[0]);
						GetObject(VestalActBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case 12:
						SelectObject(imgdc, (HBITMAP)VestalActBitmap[1]);
						GetObject(VestalActBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case 13:
						SelectObject(imgdc, (HBITMAP)VestalActBitmap[2]);
						GetObject(VestalActBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case 14:
						SelectObject(imgdc, (HBITMAP)VestalActBitmap[3]);
						GetObject(VestalActBitmap[3], sizeof(BITMAP), &bmp);
						break;
						//Highway
					case 21:
						SelectObject(imgdc, (HBITMAP)HighwayActBitmap[0]);
						GetObject(HighwayActBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case 22:
						SelectObject(imgdc, (HBITMAP)HighwayActBitmap[1]);
						GetObject(HighwayActBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case 23:
						SelectObject(imgdc, (HBITMAP)HighwayActBitmap[2]);
						GetObject(HighwayActBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case 24:
						SelectObject(imgdc, (HBITMAP)HighwayActBitmap[3]);
						GetObject(HighwayActBitmap[3], sizeof(BITMAP), &bmp);
						break;
						//Musketeer
					case 31:
						SelectObject(imgdc, (HBITMAP)MusketeerActBitmap[0]);
						GetObject(MusketeerActBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case 32:
						SelectObject(imgdc, (HBITMAP)MusketeerActBitmap[1]);
						GetObject(MusketeerActBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case 33:
						SelectObject(imgdc, (HBITMAP)MusketeerActBitmap[2]);
						GetObject(MusketeerActBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case 34:
						SelectObject(imgdc, (HBITMAP)MusketeerActBitmap[3]);
						GetObject(MusketeerActBitmap[3], sizeof(BITMAP), &bmp);
						break;
					}
					Shape actor = { 540 + timer, 400, 400, 400 };
					TransparentBlt(mdc, actor.px - actor.width / 2, actor.py - actor.height / 2, actor.width, actor.height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
					DeleteObject(tmpBitmap);
					//BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);


				}			
				else {
					Unit* tmp = turnlist->head;
					switch (tmp->CLASS) {				
					case ID_ENEMY_NAME1:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[0]);
						GetObject(enemyBitmap[0], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME2:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[1]);
						GetObject(enemyBitmap[1], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME3:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[2]);
						GetObject(enemyBitmap[2], sizeof(BITMAP), &bmp);
						break;
					case ID_ENEMY_NAME4:
						SelectObject(imgdc, (HBITMAP)enemyBitmap[3]);
						GetObject(enemyBitmap[3], sizeof(BITMAP), &bmp);
						break;
					}
					Shape actor = { 750 - timer,400,bmp.bmWidth, bmp.bmHeight };
					TransparentBlt(mdc, actor.px - actor.width / 2, actor.py - actor.height / 2, actor.width, actor.height, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(0, 255, 0));
				}
			}
			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			
			
			DeleteDC(mdc);
			DeleteObject(hBitmap);
		
			DeleteDC(imgdc);
			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_MOUSEMOVE:
			if (MouseLock == FALSE) {
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				for (int i = 0; i < (userlist->n + enemylist->n + 6 + turnlist->n); i++) {
					Shape* rect;
					if (i < userlist->n) {
						rect = &unitShape[i];
					}
					else if (i < userlist->n + enemylist->n) {
						rect = &enemyShape[i- userlist->n];
					}
					else if (i < userlist->n + enemylist->n + 6) {
						rect = &skillShape[i - (userlist->n + enemylist->n)];
					}
					else{
						rect = &turnShape[turnlist->n - 1 + (userlist->n + enemylist->n + 6) - i];
					}

					RECT tmp = DrawShape(rect);
					if (CheckMouseRect(mx, my, tmp.left, tmp.top, tmp.right, tmp.bottom)) {
						if (SelectCursor != i + 1) {
							SelectCursor = i + 1;
							//InvalidateRect(hwnd, NULL, FALSE);
						}
						break;
					}
					if (i == userlist->n + enemylist->n + 6 + turnlist->n - 1 ) { //&& SelectCursor != i + 1
						SelectCursor = 0;
						//InvalidateRect(hwnd, NULL, FALSE);
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (SelectCursor != 0 && MouseLock == FALSE) {
				if (SelectCursor <= userlist->n && SelectCursor > 0 && selectSkill != -1) {
					SelectUnit = unit[SelectCursor - 1];
					switch (turnlist->head->skill[selectSkill]) {
					case SKILL_Vestal_Heal:
						Heal(SelectUnit, 10 + turnlist->head->AtkBuff);

						MouseLock = TRUE;
						act = TRUE;
						timer = 0;
						KillTimer(hwnd, 2);
						SetTimer(hwnd, 3, 20, NULL);
						break;
					}
					if (selectSkill == 4) {
						Unit* tmp = turnlist->head;
						int turnmain = tmp->space;
						int selmain = SelectUnit->space;

						tmp->space = selmain;
						SelectUnit->space = turnmain;

						unit[turnmain] = SelectUnit;
						unit[selmain] = tmp;									

						Shape temp;

						temp.py = unitShape[turnmain].py;
						temp.width = unitShape[turnmain].width;
						temp.height= unitShape[turnmain].height;

						unitShape[turnmain].py = unitShape[selmain].py;
						unitShape[turnmain].width = unitShape[selmain].width;
						unitShape[turnmain].height = unitShape[selmain].height;

						unitShape[selmain].py = temp.py;
						unitShape[selmain].width = temp.width;
						unitShape[selmain].height = temp.height;

						userlist->head = unit[0];
						tmp = userlist->head;
						for (int i = 1; (unit[i + 1] != NULL && i < 4); i++) {
							tmp->next = NULL;
							tmp->next = unit[i];
							tmp = tmp->next;
						}
						turnlist->head = turnlist->head->turnnext;
						if (turnlist->head->CLASS > 4) {
							SetTimer(hwnd, 4, 800, NULL);
						}
						selectSkill = -1;
						SelectUnit = NULL;
					}
					
					//rect = &unitShape[SelectCursor];
				}
				else if (SelectCursor <= userlist->n + enemylist->n && SelectCursor > userlist->n &&  selectSkill != -1) {	

					SelectUnit = enemy[SelectCursor - (userlist->n + 1)];
					//��ų��ȣ�� �ش�Ǵ� ��ų �ߵ�
					switch (turnlist->head->skill[selectSkill]) {
					default:
						break;
						//Crusader
					case SKILL_Crusader_Smite:
						Attack(SelectUnit, 7 + turnlist->head->AtkBuff);
						break;
					case SKILL_Crusader_HolyLance:
						if (SelectUnit->def > 0)
							Attack(SelectUnit, 10 + turnlist->head->AtkBuff);
						else
							Attack(SelectUnit, 2 + turnlist->head->AtkBuff);
						break;
					case SKILL_Crusader_Zealous_Speech:
						for (Unit* tmp = enemylist->head; tmp != NULL; tmp = tmp->next) {
							Attack(SelectUnit, 1 + turnlist->head->AtkBuff);
						}
						break;					
						//Vestal							
					case SKILL_Vestal_Mace:
						Attack(SelectUnit, 3 + turnlist->head->AtkBuff);
						break;
					case SKILL_Vestal_Lectionary:
						Attack(SelectUnit, 4);
						break;					
						//Highway
					case SKILL_Highway_Slash:
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Pistol:
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Hipshot:
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Highway_Lunge:
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						break;
						//Musketeer							
					case SKILL_Musketeer_rifle:
						Attack(SelectUnit, 7 + turnlist->head->AtkBuff);
						break;
					case SKILL_Musketeer_pistol:
						Attack(SelectUnit, 5 + turnlist->head->AtkBuff);
						break;
					case SKILL_Musketeer_Buckshot:
						timer = 1;
						for (Unit* tmp = enemylist->head; tmp != NULL; tmp = tmp->next) {
							Attack(tmp, (3 + turnlist->head->AtkBuff)/timer);
							timer++;
						}						
						timer = 0;
						break;
					
					}
					if (SelectUnit->hp < 1) {
						ActDeleteUnit(turnlist, SelectUnit);
						if (SelectUnit->CLASS <= 4) {
							DeleteUnit(userlist, SelectUnit);
							Unit* tmp = userlist->head;
							for (int i = 0; tmp != NULL; tmp = tmp->next) {
								unit[i] = tmp;
								i++;
							}
						}
						else {
							DeleteUnit(enemylist, SelectUnit);
							Unit* tmp = enemylist->head;
							for (int i = 0; tmp != NULL; tmp = tmp->next) {
								enemy[i] = tmp;
								i++;
							}
						}

					}
					MouseLock = TRUE;
					act = TRUE;
					timer = 0;
					KillTimer(hwnd, 2);
					SetTimer(hwnd, 3, 20, NULL);
				
				}
				else if (SelectCursor <= userlist->n + enemylist->n + 6) {
					selectSkill = SelectCursor - (userlist->n + enemylist->n + 1);
					switch (turnlist->head->skill[selectSkill]) {				
					case SKILL_Musketeer_Selfheal:
						Heal(turnlist->head, 7);		

						MouseLock = TRUE;
						act = TRUE;
						timer = 0;
						KillTimer(hwnd, 2);
						SetTimer(hwnd, 3, 20, NULL);
						break;
					case SKILL_Crusader_Block:
						//Attack(SelectUnit, 5 + turnlist->head->atk);
						for (Unit* tmp = userlist->head; tmp != NULL; tmp = tmp->next) {
							Shield(tmp, 5);
						}

						MouseLock = TRUE;
						act = TRUE;
						timer = 0;
						KillTimer(hwnd, 2);
						SetTimer(hwnd, 3, 20, NULL);
						break;
					case SKILL_Vestal_Miracle:
						for (Unit* tmp = userlist->head; tmp != NULL; tmp = tmp->next) {
							Heal(tmp, 3 + turnlist->head->AtkBuff);
						}

						MouseLock = TRUE;
						act = TRUE;
						timer = 0;
						KillTimer(hwnd, 2);
						SetTimer(hwnd, 3, 20, NULL);
						break;
					}


					if (SelectCursor == userlist->n + enemylist->n + 6) {
						turnlist->head = turnlist->head->turnnext;
						selectSkill = -1;
						if (turnlist->head->CLASS > 4) {
							SetTimer(hwnd, 4, 800, NULL);
						}
					}
					InvalidateRect(hwnd, NULL, FALSE);
				}				
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}



	//���� ������ ���
	if (menu == 4) {
		int charsize = 100;
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 2: //�ִϸ��̼� 
				animate++;
				animate %= 31;
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
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
			
			//----------------------------
			//�޹�� ĥ�ϱ�
			//---------------------
			SelectObject(imgdc, (HBITMAP)BkBitmap[stage]);
			GetObject(BkBitmap[stage], sizeof(BITMAP), &bmp);
			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);			

			//ĳ���� ���
			{
				Unit* tmp = userlist->head;
				int i = 0;
				while (tmp != NULL) {
					switch (tmp->CLASS) {
					case ID_USER_Crusader:
						SelectObject(imgdc, (HBITMAP)CrusaderBitmap[animate]);
						GetObject(CrusaderBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Vestal:
						SelectObject(imgdc, (HBITMAP)VestalBitmap[animate]);
						GetObject(VestalBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Highway:
						SelectObject(imgdc, (HBITMAP)HighwayBitmap[animate]);
						GetObject(HighwayBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					case ID_USER_Musketeer:
						SelectObject(imgdc, (HBITMAP)MusketeerBitmap[animate]);
						GetObject(MusketeerBitmap[animate], sizeof(BITMAP), &bmp);
						break;
					}
					RECT rect = { charsize * i, 450, charsize * (i + 1), 500 };

					switch (tmp->CLASS) {
					case ID_USER_Crusader:
						TransparentBlt(mdc, charsize * i, -50, charsize, 520, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
						break;
					default:
						TransparentBlt(mdc, charsize * i, 50, charsize, 420, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
						break;
					}
					i++;
					tmp = tmp->next;
				}
			}


			//ȭ�� ��ȯ��
			if (DisplayChange == TRUE) {
				//imgdc = CreateCompatibleDC(hdc);
				imgBitmap = CreateCompatibleBitmap(hdc, size.right, size.bottom);
				SelectObject(imgdc, imgBitmap);

				hPen = (HPEN)GetStockObject(BLACK_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, 0, 0, size.right, size.bottom);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				hPen = (HPEN)GetStockObject(WHITE_PEN);
				oldPen = (HPEN)SelectObject(imgdc, hPen);
				hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
				oldBrush = (HBRUSH)SelectObject(imgdc, hBrush);

				Rectangle(imgdc, size.right / 2 - len, size.bottom / 2 - len, size.right / 2 + len, size.bottom / 2 + len);

				BitBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, SRCAND);

				SelectObject(imgdc, oldPen);
				DeleteObject(hPen);
				SelectObject(imgdc, oldBrush);
				DeleteObject(hBrush);

				//DeleteDC(imgdc);
				DeleteObject(imgBitmap);
			}

			//������۸�
			BitBlt(hdc, 0, 0, size.right, size.bottom, mdc, 0, 0, SRCCOPY);
			DeleteDC(mdc);
			DeleteObject(hBitmap);
			DeleteDC(imgdc);

			//-----------
			EndPaint(hwnd, &ps);
			break;
		case WM_LBUTTONDOWN:

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
	}


	//�޽� ������ ���
	if (menu == 5) {
		switch (uMsg) {
		case WM_TIMER:
			switch (wParam) {
			case 2: //�ִϸ��̼� 

				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
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
			

			//----------------------------
			//�޹�� ĥ�ϱ�
			//----------------------------
			SelectObject(imgdc, (HBITMAP)BkBitmap[stage]);
			GetObject(BkBitmap[stage], sizeof(BITMAP), &bmp);

			StretchBlt(mdc, 0, 0, size.right, size.bottom, imgdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			//----------------------------
			//���� ĥ�ϱ�
			//----------------------------
			
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


