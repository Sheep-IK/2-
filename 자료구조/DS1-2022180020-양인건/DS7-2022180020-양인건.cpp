#include <stdio.h>

#define SIZE 19


int board[SIZE][SIZE];
int x, y;
int count;
int black = 0;
int white = 0;
int menu = 0;
void Print();
void Hold();
void NumCheck();
void scan();
void change();

//
//
int main()
{
	board[5][3] = '0';
	board[5][4] = '0';
	board[5][5] = '0';
	board[6][3] = '0';
	board[3][2] = '0';
	board[3][1] = '0';
	board[3][3] = '@';
	board[3][4] = '@';
	board[4][3] = '0';
	board[4][5] = '@';
	board[4][4] = '@';


	while (1)
	{
		Print();
		printf("1�� : hold 2�� : �� ���� 3�� : ������ ��");
		scanf_s("%d", &menu);
		switch (menu) {
		case 1:
			Hold();
			printf("�� ���� ���� : %d, �� ���� ���� : %d \n", black, white);
			Print();
			break;

		case 2:
			NumCheck();
			break;

		case 3:
			scan();
			break;

		case 5:
			change();
			break;
		}
		
	}
}

void Print()
{
	
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (board[i][j] == '@' || board[i][j] == '0' || board[i][j] == '&' || board[i][j] == '#' )
			{}
			else
			{
				board[i][j] = '+';
			}
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}


}

void Hold()
{
	printf("���� ���� ��ġ�� �Է����ּ���. ( X , Y )");
	scanf_s(" %d %d", &x, &y);
	if (x >= SIZE || x < 0 || y >= SIZE || y < 0)
	{
		printf("�ٵ��� �ȿ��� ���� �����ּ���.\n");
	}
	else
	{
		if (board[y][x] == '@' || board[y][x] == '0')
		{
			printf("�̹� ���� ���� �ֽ��ϴ�.");
		}
		else
		{
			++count;
			if (count % 2 == 1)
			{
				board[y][x] = '@';
				++black;
			}
			else
			{
				board[y][x] = '0';
				++white;
			}
		}
	}
}
	
void NumCheck()
{
	int cntB = 0;
	int cntW = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == '@')
				++cntB;
			else if (board[i][j] == '0')
				++cntW;
		}
		printf("%2d��° �� ���� �� : %d, ������ : %d\n", i+1, cntW, cntB);
		cntB = 0;
		cntW = 0;
	}

		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				if (board[j][i] == '@')
					++cntB;
				else if (board[j][i] == '0')
					++cntW;
			}
			printf("%2d��° �� ���� �� : %d, ������ : %d\n", i+1, cntW, cntB);
			cntB = 0;
			cntW = 0;
		}
	
}

void scan()
{

	int cntB = 0;
	int cntW = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == '@')
				++cntB;
			else if (board[i][j] == '0')
				++cntW;
		}
		if (cntB > cntW)
			printf("%2d�� ���� �� : %d\n", i + 1, cntB);
		else if (cntW > cntB)
			printf("%2d�� �� �� : %d\n",i+1, cntW);
		else
			printf("%2d�� ���� ��, �� �� : %d\n",i+1, cntB);
		cntB = 0;
		cntW = 0;


	}
}

void scan2()
{
	int cntB = 0;
	int cntW = 0;
	

}



void change()
{

	int cntB = 0;
	int cntW = 0;
	int i;
	int j;
	for (i = 0; i < SIZE; ++i) {
		for ( j = 0; j < SIZE; ++j) {
			if (board[i][j] == '@')
				++cntB;
			else if (board[i][j] == '0')
				++cntW;
		}
		if (cntB > cntW)
			for (j = 0; j < SIZE; j++) {
				if (board[i][j] == '@')
					board[i][j] = '&';
			}
		else if (cntW > cntB) 
			for (j = 0; j < SIZE; j++) {
			if (board[i][j] == '0')
				board[i][j] = '#';
			}
		cntB = 0;
		cntW = 0;


	}
}