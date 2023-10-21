#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

#define SIZE 100


int x;
int y;
int num = 20;
int i = 0;
int menu = 0;

struct unit {
	int spotx;
	int spoty;
	char ut;
	char id;
	int hp;
	int mp;
};

void MakeUL();
void ShowUL_R();

struct unit List[SIZE];

int main()
{
	MakeUL();
	ShowUL_R();
}


void MakeUL() {

	List[0] = { 0, 0, 'M', 1, 25, 0 };
	List[1] = { 0, 1, 'M',2, 20, 0 };
	List[2] = { 0, 2, 'M', 3, 30, 0 };
	List[3] = { 1, 3, 'M', 4, 15, 0 };
	List[4] = { 0, 5, 'M', 6, 27,0 };
	List[5] = { 1, 4, 'M', 5, 30, 0 };
	List[6] = { 1, 2, 'V',7, 80, 140 };
	List[7] = { 2, 2, 'V',8, 75, 140 };
	List[8] = { 0, 3, 'T',9, 120, 0 };
	List[9] = { 0, 4,'T', 10, 50, 0 };
	List[10] = { 39, 14,'H', 11, 50, 0 };
	List[11] = { 38, 15, 'H', 12, 45, 0 };
	List[12] = { 38, 16,'H', 13, 20, 0 };
	List[13] = { 37, 17, 'H',14, 30, 0 };
	List[14] = { 39, 18, 'H',15, 47, 0 };
	List[15] = { 39, 19, 'H',16, 37, 0 };
	List[16] = { 38, 17, 'Q',17, 70, 150 };
	List[17] = { 39, 17,'Q', 18, 35, 150 };
	List[18] = { 39, 16, 'D',19, 15, 200 };
	List[19] = { 39, 15,'D', 20, 40, 200 };

}

void ShowUL_R()
{
		printf("계속 하려면 '1' , 종료하려면 '2'");
		scanf_s("%d", &menu);
		if(menu == 1)
			for (i; i != SIZE; i++)
				printf("%c\n", List[i].ut);
			break;

		ShowUL_R();

}