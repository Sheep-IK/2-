#include <iostream>
#include <format>
#include <random>
#include <algorithm>

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<int> uid_random_bool{ 0,1 };

int arr[30];
int arr_sort[30];

int main()
{
	int limit;
	std::cout << "1 이상의 숫자를 입력하세요: " << std::endl;
	std::cin >> limit;
	if (limit < 1)
	{
		std::cout << "입력한 수가 올바르지 않습니다" << std::endl;
		return 0;
	}
	std::uniform_int_distribution<int> uid{ 1,limit - 1 };
	for (int i = 0; i < 30; ++i)
	{
		arr[i] = uid(dre);
		arr_sort[i] = arr[i];
	}
	std::sort(arr_sort, arr_sort + 30);
	bool running = true;
	while (running)
	{
		system("cls");
		std::cout << "현재 수열: ";
		for (int i = 0; i < 30; ++i)
			std::cout << arr[i] << ' ';
		std::cout << std::endl;
		std::cout << "사용할 명령어를 입력하세요" << std::endl;
		std::cout << "	-a: 오름차순으로 정렬" << std::endl;
		std::cout << "	-d: 내림차순으로 정렬" << std::endl;
		std::cout << "	-r: 랜덤하게 재정렬" << std::endl;
		std::cout << "	-q: 프로그램 종료" << std::endl;
		std::cout << "	-t: 3의 배수 출력하기" << std::endl;
		std::cout << "	-s: 7의 배수 출력하기" << std::endl;
		std::cout << "	-+: 3의 배수이거나 7의 배수 출력하기" << std::endl;
		std::cout << "	--: 3의 배수이고 7의 배수인 숫자 출력하기" << std::endl;
		std::cout << "	-m: 최대값 출력" << std::endl;
		std::cout << "	-n: 최소값 출력" << std::endl;
		std::cout << "	-p: 다시 임의의 숫자 만들고 출력하기" << std::endl;
		char input;
		int temp = 0;
		if (std::cin >> input && '\n' != input)
		{
			switch (input)
			{
			case 'a':
				std::sort(arr, arr + 30);
				break;
			case 'd':
				std::sort(arr, arr + 30,
					[](int a, int b)
					{return a > b; });
				break;
			case 'r':
				qsort(arr, 30, 4,
					[](const void* a, const void* b)
					{return uid_random_bool(dre); });
				break;
			case 'q':
				running = false;
				break;
			case 't':
				for (int i = 0; i < 30; ++i)
					if (0 == arr[i] % 3)
						std::cout << arr[i] << ' ';
				std::cout << std::endl;
				system("pause");
				break;
			case 's':
				for (int i = 0; i < 30; ++i)
					if (0 == arr[i] % 7)
						std::cout << arr[i] << ' ';
				std::cout << std::endl;
				system("pause");
				break;
			case '+':
				for (int i = 0; i < 30; ++i)
					if (0 == arr_sort[i] % 3|| 0 == arr_sort[i] % 7)
						std::cout << arr_sort[i] << ' ';
				std::cout << std::endl;
				system("pause");
				break;
			case '-':
				for (int i = 0; i < 30; ++i)
					if (0 == arr_sort[i] % 3 && 0 == arr_sort[i] % 7)
						std::cout << arr_sort[i] << ' ';
				std::cout << std::endl;
				system("pause");
				break;
			case 'm':
				temp = std::numeric_limits<int>::min();
				for (int i = 0; i < 30; ++i)
					if (arr[i] > temp)
						temp = arr[i];
				std::cout << temp;
				std::cout << std::endl;
				system("pause");
				break;
			case 'n':
				temp = std::numeric_limits<int>::max();
				for (int i = 0; i < 30; ++i)
					if (arr[i] < temp)
						temp = arr[i];
				std::cout << temp;
				std::cout << std::endl;
				system("pause");
				break;
			case 'p':
				for (int i = 0; i < 30; ++i)
				{
					arr[i] = uid(dre);
					arr_sort[i] = arr[i];
				}
				std::sort(arr_sort, arr_sort + 30);
				break;
			default:
				std::cout << "입력된 명령어가 올바르지 않습니다" << std::endl;
				system("pause");
				break;
			}
		}
	}
	std::cout << "프로그램이 종료되었습니다." << std::endl;
}