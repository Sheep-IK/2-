#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define SIZEX 40
#define SIZEY 20

int x;
int y;
int i = 0;
int count = 0;
char list[30];
char Map[SIZEY][SIZEX];
void Display();
void ShowOrderList();
void Order();
void DestroyAll();
void GetDistance();
void SortByDistance();

struct Unit {
	int spotx;
	int spoty;
	char id;
	int hp;
	int mp;
};

int main() {

	//유닛 정보 입력
	struct Unit m = { 0, 0, 1, 30, 0 };
	struct Unit m1 = { 0, 1, 2, 30, 0 };
	struct Unit m2 = { 0, 2, 3, 30, 0 };
	struct Unit m3 = { 1, 3, 4, 30, 0 };
	struct Unit m4 = { 1, 4, 5, 30, 0 };
	struct Unit m5 = { 0, 5, 6, 30, 0 };
	struct Unit v = { 1, 2, 7, 80, 140 };
	struct Unit v1 = { 2, 2, 8, 80, 140 };
	struct Unit t = { 0, 3, 9, 120, 0 };
	struct Unit t1 = { 0, 4, 10, 120, 0 };
	struct Unit h = { 39, 19, 11, 50, 0 };
	struct Unit h1 = { 39, 18, 12, 50, 0 };
	struct Unit h2 = { 37, 17, 13, 50, 0 };
	struct Unit h3 = { 38, 16, 14, 50, 0 };
	struct Unit h4 = { 38, 15, 15, 50, 0 };
	struct Unit h5 = { 39, 14, 16, 50, 0 };
	struct Unit q = { 38, 17, 17, 70, 150 };
	struct Unit q1 = { 39, 17, 18, 70, 150 };
	struct Unit d = { 39, 16, 19, 60, 200 };
	struct Unit d1 = { 39, 15, 20, 60, 200 };

	Map[m.spoty][m.spotx] = 'M';
	Map[m1.spoty][m1.spotx] = 'M';
	Map[m2.spoty][m2.spotx] = 'M';
	Map[m3.spoty][m3.spotx] = 'M';
	Map[m4.spoty][m4.spotx] = 'M';
	Map[m5.spoty][m5.spotx] = 'M';
	Map[v.spoty][v.spotx] = 'V';
	Map[v1.spoty][v1.spotx] = 'V';
	Map[t.spoty][t.spotx] = 'T';
	Map[t1.spoty][t1.spotx] = 'T';
	Map[h.spoty][h.spotx] = 'H';
	Map[h1.spoty][h1.spotx] = 'H';
	Map[h2.spoty][h2.spotx] = 'H';
	Map[h3.spoty][h3.spotx] = 'H';
	Map[h4.spoty][h4.spotx] = 'H';
	Map[h5.spoty][h5.spotx] = 'H';
	Map[q.spoty][q.spotx] = 'Q';
	Map[q1.spoty][q1.spotx] = 'Q';
	Map[d.spoty][d.spotx] = 'D';
	Map[d1.spoty][d1.spotx] = 'D';
	int menu = 1;

	Display();

	while (1)
	{	
		printf("\n메뉴를 선택해주세요. \n 1. ShowOrderList  2. Order 3. Display 4. DestroyAll 5. GetDistance 6.SortByDistance 7. 종료\n");
		scanf("%d", &menu);
		switch (menu) {
		case 1:
			ShowOrderList();
			break;
		case 2:
			Order();
			break;
		case 3:
			Display();
			break;
		case 4:
			DestroyAll();
			break;
		case 5:
			GetDistance();
			break;
		case 6:
			SortByDistance();
			break;
		case 7:
			return 0;
		}
	}
}


void Display() {
	for (int i = 0; i < SIZEY; ++i)
	{
		for (int j = 0; j < SIZEX; ++j)
		{
			if (Map[i][j] == 'M' || Map[i][j] == 'V' || Map[i][j] == 'T' || Map[i][j] == 'H' || Map[i][j] == 'Q' || Map[i][j] == 'D')
			{
			}
			else
				Map[i][j] = ' ';
			printf("%c", Map[i][j]);
		}
		printf("\n");
	}
}

void ShowOrderList()
{
	for (i=0; i < 30; ++i) {
		printf(" %c", list[i]);
	}
}

void Order()
{	
	printf("명령을 입력해주세요\n");
	scanf(" %c", &list[count]);
	++count;
}

void DestroyAll()
{
	int x1, x2, y1, y2;
	int k = 0;
	printf("삭제할 범위를 입력해주세요. ex) (x1) (y1) (x2) (y2)");
	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
	for (x1; x1 <= x2; ++x1) {
		for (k = y1; k <= y2; ++k) {
			Map[k][x1] = ' ';
		}
	}
}

void GetDistance()
{
	int x1, x2, y1, y2;
	int dis;
	printf("거리를 구하고 싶은 두 좌표를 입력하시오 ex) (x1) (y1) (x2) (y2)\n");
	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
	if (x2 - x1 > y2 - y1)
		dis = x2 - x1;
	else
		dis = y2 - y1;
	printf("두 좌표의 거리 : %d\n", dis);
}

void SortByDistance()
{
	printf("좌표를 입력해주세요 ex) 0 0\n");
	scanf("%d %d", &x, &y);
	for (int i = x/2; i < 6; ++i) {
		for (int j = y/2; j < 6; ++j) {
			if(Map[i][j] != ' ')
			printf("%c\n", Map[i][j]);
		}
	}
}
