#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

#define SIZE 100





int x;
int y;
int num = 20;

struct unit {
	int spotx;
	int spoty;
	char ut;
	char id;
	int hp;
	int mp;
};

void MakeUL();
void ShowUL();
void ShowUL_R();
void Select_UL();
void Select_UL_R();
void Destroy_UL();

struct unit List[SIZE];

int main()
{
	MakeUL();
	Destroy_UL();
	ShowUL();
}


void MakeUL() {

	List[0] = { 0, 0, 'M', 1, 30, 0 };
	List[1] = { 0, 1, 'M',2, 30, 0 };
	List[2] = { 0, 1, 'M', 3, 30, 0 };
	List[3] = { 0, 1, 'M', 4, 30, 0 };
	List[4] = { 0, 1, 'M', 5, 30,0 };
	List[5] = { 0, 1, 'M', 6, 30, 0 };
	List[6] = { 1, 2, 'V',7, 80, 140 };
	List[7] = { 1, 2, 'V',7, 80, 140 };
	List[8] = { 1, 2, 'V',7, 80, 140 };
	List[9] = { 2, 2, 'V',8, 80, 140 };
	List[10] = { 0, 3, 'V',9, 120, 0 };
	List[11] = { 0, 4,'V', 10, 120, 0 };
	List[12] = { 39, 19,'H', 11, 50, 0 };
	List[13] = { 39, 18, 'H', 12, 50, 0 };
	List[14] = { 37, 17,'H', 13, 50, 0 };
	List[15] = { 38, 16, 'H',14, 50, 0 };
	List[16] = { 38, 15, 'H',15, 50, 0 };
	List[17] = { 39, 14, 'H',16, 50, 0 };
	List[18] = { 38, 17, 'Q',17, 70, 150 };
	List[19] = { 39, 17,'Q', 18, 70, 150 };
	List[20] = { 39, 16, 'D',19, 60, 200 };
	List[21] = { 39, 15,'D', 20, 60, 200 };

}

void ShowUL() {
	for (int i = 0; i < SIZE; ++i) {
		printf(" À¯´Ö : %c, ÁÂÇ¥ : %d %d, ID : %d, HP : %d , MP : %d\n",
			List[i].ut, List[i].spotx, List[i].spoty, List[i].id, List[i].hp, List[i].mp);
	}
}

void ShowUL_R() {
	int menu;
	printf("°è¼Ó Ãâ·ÂÀº '1' Á¾·á´Â '2'\n");
	scanf_s("%d", &menu);
	if (menu == 1) {
		for (int i = 0; i < SIZE; ++i) {
			printf(" À¯´Ö : %c, ÁÂÇ¥ : %d %d, ID : %d, HP : %d , MP : %d\n",
				List[i].ut, List[i].spotx, List[i].spoty, List[i].id, List[i].hp, List[i].mp);
		}
		ShowUL_R();
	}
}

void Select_UL() {
	printf("¹Þ°í½ÍÀº ÁÂÇ¥¸¦ ÀÔ·Â");
	scanf_s("%d %d", &x,&y);
	for (int i = 0; i < SIZE; ++i) {
		if (List[i].spotx == x && List[i].spoty == y) {
			printf("À¯´Ö : %c, ÁÂÇ¥ : %d %d, ID : %d, HP : %d , MP : %d\n", List[i].ut, List[i].spotx, List[i].spoty, List[i].hp, List[i].mp);
			break;
		}
	}
}

void Select_UL_R() {
	int menu;
	printf("°è¼Ó Ãâ·ÂÀº '1' Á¾·á´Â '2'\n");
	scanf_s("%d", &menu);
	if (menu == 1) {
		printf("¹Þ°í½ÍÀº ÁÂÇ¥¸¦ ÀÔ·Â");
		scanf_s("%d %d", &x, &y);
		for (int i = 0; i < SIZE; ++i) {
			if (List[i].spotx == x && List[i].spoty == y) {
				printf("À¯´Ö : %c, ÁÂÇ¥ : %d %d, ID : %d, HP : %d , MP : %d\n", List[i].ut, List[i].spotx, List[i].spoty, List[i].hp, List[i].mp);
				break;
			}
		}
		Select_UL_R();
	}
}

void Destroy_UL() {

	printf("Áö¿ì°í½ÍÀº ÁÂÇ¥¸¦ ÀÔ·Â");
	scanf_s("%d %d", &x, &y);
	for (int i = 0; i < SIZE; ++i) {
		if (List[i].spotx == x && List[i].spoty == y) {
			List[i].ut = NULL;
			List[i].spotx = 0; 
			List[i].spoty = 0;
			List[i].hp = 0;
			List[i].mp = 0;
			List[i].id = 0;
			break;
		}
	}

}
