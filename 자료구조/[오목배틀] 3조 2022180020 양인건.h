#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


char board[19][19];

int board_2022180020[19][19];

#define BLACK -1
#define WHITE 1
#define SIZE 19

int many[SIZE];
int many2[SIZE];


int x, y;
int count;
int black = 0;
int white = 0;

int randX = rand() % 19;
int randY = rand() % 19;


int type;

int randCnt = 1;
int startX = 3;
int startY = 3;
int cntturn = 1;
int WcntTurn = 1;


int BImport = 0;

int WImport = 0;

void sideCheck();

void Bscan();
void Wscan();
void RandHold();


void WhiteAttack_2022180020(int* X, int* Y)
{

	srand(time(NULL));

	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (board_2022180020[i][j] == BLACK)
				board[i][j] = '@';
			else if (board_2022180020[i][j] == WHITE)
				board[i][j] = '0';
			else
				board[i][j] = '+';
		}
	}

	Wscan();
	if (board[y][x] != '+' || y < 0 || x < 0 || y > 18 || x > 18) {
		RandHold();
	}

	*X = x;
	*Y = y;
	board_2022180020[*Y][*X] = WHITE;

}

void WhiteDefence_2022180020(int X, int Y)
{

	board[Y][X] = '@';
	board_2022180020[Y][X] = BLACK;
}

void BlackAttack_2022180020(int* X, int* Y)
{

	srand(time(NULL));

	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (board_2022180020[i][j] == BLACK)
				board[i][j] = '@';
			else if (board_2022180020[i][j] == WHITE)
				board[i][j] = '0';
			else
				board[i][j] = '+';
		}
	}

	Bscan();
	if (board[y][x] != '+' || y < 0 || x < 0 || y > 18 || x > 18) {
		RandHold();
	}

	/*while (board[y][x] != '+' && y >= 1 && x >= 1 && y <= 17 && x <= 1)
	{
		RandHold();
	}*/

	*X = x;
	*Y = y;
	board_2022180020[*Y][*X] = BLACK;
}

void BlackDefence_2022180020(int X, int Y)
{

	board[Y][X] = '0';
	board_2022180020[Y][X] = WHITE;
}






//void Hold()
//{
//	printf("돌을 놓을 위치를 입력해주세요. ( X , Y )");
//	scanf_s(" %d %d", &x, &y);
//	if (x >= SIZE || x < 0 || y >= SIZE || y < 0)
//	{
//		printf("바둑판 안에만 돌을 놓아주세요.\n");
//	}
//	else
//	{
//		if (board[y][x] == '@' || board[y][x] == '0')
//		{
//			printf("이미 돌이 놓여 있습니다.\n");
//		}
//		else
//		{
//			++count;
//			if (count % 2 == 1)
//			{
//				board[y][x] = '@';
//				++black;
//			}
//			else
//			{
//				board[y][x] = '0';
//				++white;
//			}
//		}
//	}
//}



void Bscan() {

	int random = rand();
	BImport = 0;
	WImport = 0;

	if (cntturn == 1) {
		x = 3;
		y = 3;
		++cntturn;
	}
	else if (cntturn == 2 && board[5][1] == '+' && board[6][0] == '+' && board[2][4] == '+' && board[4][2] == '+') {
		x = 1;
		y = 5;
		++cntturn;
	}
	else if (cntturn == 3 && board[6][0] == '+' && board[2][4] == '+' && board[4][2] == '+') {
		x = 0;
		y = 6;
		++cntturn;
	}
	else if (cntturn == 4 && board[2][4] == '+' && board[4][2] == '+') {
		x = 4;
		y = 2;
		++cntturn;
	}
	else if (cntturn == 5 && board[4][2] == '+') {
		x = 2;
		y = 4;
		++cntturn;
	}
	else {

		for (int i = 0; i < SIZE ; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i - 1][j] == '+') {
					x = j;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i + 4][j] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '@') {
					x = j;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '+' && board[i + 3][j] == '@' && board[i + 4][j] == '@') {
					x = j;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '+' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i + 4][j] == '@') {
					x = j;
					y = i + 1;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j - 1] == '+') {
					x = j - 1;
					y = i;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j + 4] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '@') {
					x = j;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '+' && board[i][j + 3] == '@' && board[i][j + 4] == '@') {
					x = j;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '+' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j + 4] == '@') {
					x = j;
					y = i + 1;
					break;
				} // 가로 세로 즉결 체크 완료

				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i - 1][j - 1] == '+') {
					x = j - 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '+') {
					x = j + 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '@') {
					x = j + 3;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '+' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '@') {
					x = j + 2;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '+' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '@') {
					x = j + 1;
					y = i + 1;
					break;
				}


				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i - 1][j + 1] == '+') {
					x = j + 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '+') {
					x = j - 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '@') {
					x = j - 3;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '+' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '@') {
					x = j - 2;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '+' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '@') {
					x = j - 1;
					y = i + 1;
					break;
				}
				//여기까지 4개인 경우 한번에 끝내는 코드

				//여기부터 백돌 4개일 경우 막는 코드
				if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0' && board[i - 1][j] == '+') {
					x = j;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0' && board[i + 4][j] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0' && board[i][j - 1] == '+') {
					x = j - 1;
					y = i;
					break;
				}
				else if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0' && board[i][j + 4] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0' && board[i - 1][j - 1] == '+') {
					x = j - 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0' && board[i + 4][j + 4] == '+') {
					x = j + 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0' && board[i - 1][j + 1] == '+') {
					x = j + 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0' && board[i + 4][j - 4] == '+') {
					x = j - 4;
					y = i + 4;
					break;
				}

				//여기부터 3개일경우 가중치 계산 흑돌
				if ((board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
					(board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '+') ||

					(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
					(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '+') ||

					(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
					(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+') ||

					(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
					(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')
					)
					BImport += 50;
				//여기까지 흑돌 계산

				//여기부터 백돌 계산
				if ((board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
					(board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '+' && board[i + 4][j] == '+') ||

					(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
					(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '+' && board[i][j + 4] == '+') ||

					(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
					(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+') ||

					(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
					(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')
					)
					WImport += 50;

				//여기부터 아무것도 안걸릴때 쓰는 코드
				if (BImport == WImport && BImport == 0) {
					for (int i = 0; i < SIZE; ++i) {
						for (int j = 0; j < SIZE; ++j) {
							if (type != 3) { // 타입이 3이 아닌경우만 들어간다
								if (board[i][j] == '@') {
									switch (type)
									{
									case 0:
										if (board[i][j + 1] == '@') {
											type = 3;
											break;
										}
										x = j + 1;
										y = i;
										type = 0;
										break;

									case 1:
										if (board[i + 1][j] == '@') {
											type = 3;
											break;
										}
										x = j;
										y = i + 1;
										type = 1;
										break;

									case 2:
										if (board[i + 1][j + 1] == '@') {
											type = 3;
											break;
										}
										x = j + 1;
										y = i + 1;
										type = 2;
										break;

									}
								}
							}
							else if (type == 3) { //만약 타입 == 3 이면 다시 돌려주기
								type = random;
							}

						}
					}

				}

				if (BImport >= WImport) { // BLACK 가 더 우선순위인 경우
					if ((board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
						(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+')) {
						x = j + 3;
						y = i + 3;
					}
					else if ((board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
						(board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '+')) {
						x = j;
						y = i + 3;
					}
					else if ((board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
						(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '+')) {
						x = j + 3;
						y = i;
					}
					else if ((board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
						(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')) {
						x = j - 3;
						y = i + 3;
					}

				}
				else
				{
					if ((board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
						(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+')) {
						x = j + 3;
						y = i + 3;
					}
					else if ((board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
						(board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '+' && board[i + 4][j] == '+')) {
						x = j;
						y = i + 3;
					}
					else if ((board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
						(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '+' && board[i][j + 4] == '+')) {
						x = j + 3;
						y = i;
					}
					else if ((board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
						(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')) {
						x = j - 3;
						y = i + 3;
					}
				}
			}
		}
	}

}



void RandHold() {

	/*if (randCnt == 1) {
		randX = rand() % 17 + 1;
		randY = rand() % 17 + 1;

		if (board[randX][randY] != '+')
			RandHold();

		++randCnt;
	}
	else
	{
		int RC = rand() % 2;
		if (RC == 1)
			randY += 1;
		else if (RC == 0)
			randY -= 1;

		if (board[randY][randX] != '+')
			randCnt == 1;
	}*/

	randX = rand() % 18;
	randY = rand() % 18;

	if (board[randY][randX] != '+' && randY >= 0 && randX >= 0 && randY <= 18 && randX <= 18) {
		RandHold();
	}
	else {
		x = randX;
		y = randY;
	}
}

void Wscan() {

	int random = rand();
	BImport = 0;
	WImport = 0;

	if (cntturn == 1) {
		x = 3;
		y = 3;
		++cntturn;
	}
	else if (cntturn == 2 && board[5][1] == '+' && board[6][0] == '+' && board[2][4] == '+' && board[4][2] == '+') {
		x = 1;
		y = 5;
		++cntturn;
	}
	else if (cntturn == 3 && board[6][0] == '+' && board[2][4] == '+' && board[4][2] == '+') {
		x = 0;
		y = 6;
		++cntturn;
	}
	else {
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				//여기까지 4개인 경우 한번에 끝내는 코드

				//여기부터 백돌 4개일 경우 끝내는 코드
				if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0' && board[i - 1][j] == '+') {
					x = j;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0' && board[i + 4][j] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0' && board[i][j - 1] == '+') {
					x = j - 1;
					y = i;
					break;
				}
				else if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0' && board[i][j + 4] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0' && board[i - 1][j - 1] == '+') {
					x = j - 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0' && board[i + 4][j + 4] == '+') {
					x = j + 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0' && board[i - 1][j + 1] == '+') {
					x = j + 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0' && board[i + 4][j - 4] == '+') {
					x = j - 4;
					y = i + 4;
					break;
				}

				//여기부터 흑돌 4개이면 막기
				if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i - 1][j] == '+') {
					x = j;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i + 4][j] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '@') {
					x = j;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '+' && board[i + 3][j] == '@' && board[i + 4][j] == '@') {
					x = j;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j] == '+' && board[i + 2][j] == '@' && board[i + 3][j] == '@' && board[i + 4][j] == '@') {
					x = j;
					y = i + 1;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j - 1] == '+') {
					x = j - 1;
					y = i;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j + 4] == '+') {
					x = j;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '@') {
					x = j;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '+' && board[i][j + 3] == '@' && board[i][j + 4] == '@') {
					x = j;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i][j + 1] == '+' && board[i][j + 2] == '@' && board[i][j + 3] == '@' && board[i][j + 4] == '@') {
					x = j;
					y = i + 1;
					break;
				} // 가로 세로 즉결 체크 완료

				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i - 1][j - 1] == '+') {
					x = j - 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '+') {
					x = j + 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '@') {
					x = j + 3;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '+' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '@') {
					x = j + 2;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j + 1] == '+' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '@' && board[i + 4][j + 4] == '@') {
					x = j + 1;
					y = i + 1;
					break;
				}


				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i - 1][j + 1] == '+') {
					x = j + 1;
					y = i - 1;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '+') {
					x = j - 4;
					y = i + 4;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '@') {
					x = j - 3;
					y = i + 3;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '+' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '@') {
					x = j - 2;
					y = i + 2;
					break;
				}
				else if (board[i][j] == '@' && board[i + 1][j - 1] == '+' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '@' && board[i + 4][j - 4] == '@') {
					x = j - 1;
					y = i + 1;
					break;
				}


				//여기부터 3개일경우 가중치 계산 흑돌
				if ((board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
					(board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '+') ||

					(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
					(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '+') ||

					(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
					(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+') ||

					(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
					(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')
					)
					BImport += 50;
				//여기까지 흑돌 계산

				//여기부터 백돌 계산
				if ((board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
					(board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '+' && board[i + 4][j] == '+') ||

					(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
					(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '+' && board[i][j + 4] == '+') ||

					(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
					(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+') ||

					(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
					(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')
					)
					WImport += 50;

				//여기부터 아무것도 안걸릴때 쓰는 코드
				if (BImport == WImport && BImport == 0) {
					for (int i = 0; i < SIZE; ++i) {
						for (int j = 0; j < SIZE; ++j) {
							if (type != 3) { // 타입이 3이 아닌경우만 들어간다
								if (board[i][j] == '0') {
									switch (type)
									{
									case 0:
										if (board[i][j + 1] == '0') {
											type = 3;
											break;
										}
										x = j + 1;
										y = i;
										type = 0;
										break;

									case 1:
										if (board[i + 1][j] == '0') {
											type = 3;
											break;
										}
										x = j;
										y = i + 1;
										type = 1;
										break;

									case 2:
										if (board[i + 1][j + 1] == '0') {
											type = 3;
											break;
										}
										x = j + 1;
										y = i + 1;
										type = 2;
										break;

									}
								}
							}
							else if (type == 3) { //만약 타입 == 3 이면 다시 돌려주기
								type = random;
							}

						}
					}

				}

				if (BImport <= WImport) { // WHITE 가 더 우선순위인 경우
					if ((board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
						(board[i][j] == '0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+')) {
						x = j + 3;
						y = i + 3;
					}
					else if ((board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
						(board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '+' && board[i + 4][j] == '+')) {
						x = j;
						y = i + 3;
					}
					else if ((board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
						(board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '+' && board[i][j + 4] == '+')) {
						x = j + 3;
						y = i;
					}
					else if ((board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
						(board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')) {
						x = j - 3;
						y = i + 3;
					}
				}
				else
				{
						//

						if ((board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i - 1][j - 1] == '+' && board[i + 3][j + 3] == '+') ||
							(board[i][j] == '@' && board[i + 1][j + 1] == '@' && board[i + 2][j + 2] == '@' && board[i + 3][j + 3] == '+' && board[i + 4][j + 4] == '+')) {
							x = j + 3;
							y = i + 3;
						}
						else if ((board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i - 1][j] == '+' && board[i + 3][j] == '+') ||
							(board[i][j] == '@' && board[i + 1][j] == '@' && board[i + 2][j] == '@' && board[i + 3][j] == '+' && board[i + 4][j] == '+')) {
							x = j;
							y = i + 3;
						}
						else if ((board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j - 1] == '+' && board[i][j + 3] == '+') ||
							(board[i][j] == '@' && board[i][j + 1] == '@' && board[i][j + 2] == '@' && board[i][j + 3] == '+' && board[i][j + 4] == '+')) {
							x = j + 3;
							y = i;
						}
						else if ((board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i - 1][j + 1] == '+' && board[i + 3][j - 3] == '+') ||
							(board[i][j] == '@' && board[i + 1][j - 1] == '@' && board[i + 2][j - 2] == '@' && board[i + 3][j - 3] == '+' && board[i + 4][j - 4] == '+')) {
							x = j - 3;
							y = i + 3;
						}
						//
				}
			}
		}
	}

}