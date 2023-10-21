#include <iostream>
#include <format>
#include <random>
#include <algorithm>

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<int> uid{ 1, 50 };

int matrix[4][5];
int original_matrix[4][5];

void print_matrix()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; ++j)
			std::cout << std::format("{:4d}", matrix[i][j]);
		std::cout << std::endl;
	}
}

void copy_matrix()
{
	for (int i = 0; i < 20; ++i)
		(*matrix)[i] = (*original_matrix)[i];
}

void input_matrix()
{
	for (int i = 0; i < 20; ++i)
	{
		(*original_matrix)[i] = uid(dre);
		for (int j = 0; j < i; ++j)
			if ((*original_matrix)[i] == (*original_matrix)[j])
				--i;
	}
	copy_matrix();
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main()
{
	input_matrix();
	bool running = true;
	char last_input = NULL;
	while (running)
	{
		system("cls");
		std::cout << "현재 행렬: " << std::endl;
		print_matrix();
		std::cout << std::endl;
		std::cout << "사용할 명령어를 입력하세요" << std::endl;
		std::cout << "	-a: 1행1열을 시작으로 행을 따라 오름차순으로 정렬" << std::endl;
		std::cout << "	-d: 1행5열을 시작으로 열을 따라 내림차순으로 정렬" << std::endl;
		std::cout << "	-e: 짝수만 출력하기" << std::endl;
		std::cout << "	-o: 홀수만 출력하기" << std::endl;
		std::cout << "	-m: 최대값 출력" << std::endl;
		std::cout << "	-n: 최소값 출력" << std::endl;
		std::cout << "	-p: 한 행의 값을 모두 더해 각 행의 1열에 저장" << std::endl;
		std::cout << "	-r: 랜덤하게 재정렬" << std::endl;
		std::cout << "	-s: 새로운 랜덤한 숫자를 다시 자동 입력 (리셋)" << std::endl;
		std::cout << "	-q: 프로그램 종료" << std::endl;
		char input;
		int max = 0;
		size_t maxindex = 0;
		int min = 51;
		size_t minindex = 0;
		if (std::cin >> input && '\n' != input && last_input != input)
		{
			switch (input)
			{
			case 'a':
				std::sort((*matrix), (*matrix) + 20);
				break;
			case 'd':
				for (int i = 0; i < 19; ++i)
					for (int j = i + 1; j < 20; ++j)
						if (matrix[i % 4][4 - i / 4] < matrix[j % 4][4 - j / 4])
							swap(matrix[i % 4] + 4 - i / 4, matrix[j % 4] + 4 - j / 4);
				break;
			case 'e':
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 5; ++j)
						if (0 == matrix[i][j] % 2)
							std::cout << std::format("{:4d}", matrix[i][j]);
						else
							std::cout << std::format("{:4d}", 0);
					std::cout << std::endl;
				}
				system("pause");
				break;
			case 'o':
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 5; ++j)
						if (1 == matrix[i][j] % 2)
							std::cout << std::format("{:4d}", matrix[i][j]);
						else
							std::cout << std::format("{:4d}", 0);
					std::cout << std::endl;
				}
				system("pause");
				break;
			case 'm':
				for (int i = 0; i < 20; ++i)
					if ((*matrix)[i] > max)
					{
						max = (*matrix)[i];
						maxindex = i;
					}
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 5; ++j)
						if (maxindex == i * 5 + j)
							std::cout << std::format("{:4d}", matrix[i][j]);
						else
							std::cout << std::format("{:4d}", 0);
					std::cout << std::endl;
				}
				system("pause");
				break;
			case 'n':
				for (int i = 0; i < 20; ++i)
					if ((*matrix)[i] < min)
					{
						min = (*matrix)[i];
						minindex = i;
					}
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 5; ++j)
						if (minindex == i * 5 + j)
							std::cout << std::format("{:4d}", matrix[i][j]);
						else
							std::cout << std::format("{:4d}", 0);
					std::cout << std::endl;
				}
				system("pause");
				break;
			case 'p':
				for (int i = 0; i < 4; ++i)
				{
					int sum = 0;
					for (int j = 0; j < 5; ++j)
					{
						sum += matrix[i][j];
					}
					matrix[i][0] += sum;
				}
				break;
			case 'r':
				qsort(matrix[0], 20, 4,
					[](const void* a, const void* b)
					{
						return uid(dre) - 25;
					});
				break;
			case 's':
				input_matrix();
				break;
			case 'q':
				running = false;
				break;
			default:
				std::cout << "입력된 명령어가 올바르지 않습니다" << std::endl;
				system("pause");
				break;
			}
			last_input = input;
		}
		else if (last_input == input)
		{
			copy_matrix();
			last_input = NULL;
		}
	}
	std::cout << "프로그램이 종료되었습니다." << std::endl;
}