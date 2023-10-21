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
	std::cout << "1 �̻��� ���ڸ� �Է��ϼ���: " << std::endl;
	std::cin >> limit;
	if (limit < 1)
	{
		std::cout << "�Է��� ���� �ùٸ��� �ʽ��ϴ�" << std::endl;
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
		std::cout << "���� ����: ";
		for (int i = 0; i < 30; ++i)
			std::cout << arr[i] << ' ';
		std::cout << std::endl;
		std::cout << "����� ��ɾ �Է��ϼ���" << std::endl;
		std::cout << "	-a: ������������ ����" << std::endl;
		std::cout << "	-d: ������������ ����" << std::endl;
		std::cout << "	-r: �����ϰ� ������" << std::endl;
		std::cout << "	-q: ���α׷� ����" << std::endl;
		std::cout << "	-t: 3�� ��� ����ϱ�" << std::endl;
		std::cout << "	-s: 7�� ��� ����ϱ�" << std::endl;
		std::cout << "	-+: 3�� ����̰ų� 7�� ��� ����ϱ�" << std::endl;
		std::cout << "	--: 3�� ����̰� 7�� ����� ���� ����ϱ�" << std::endl;
		std::cout << "	-m: �ִ밪 ���" << std::endl;
		std::cout << "	-n: �ּҰ� ���" << std::endl;
		std::cout << "	-p: �ٽ� ������ ���� ����� ����ϱ�" << std::endl;
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
				std::cout << "�Էµ� ��ɾ �ùٸ��� �ʽ��ϴ�" << std::endl;
				system("pause");
				break;
			}
		}
	}
	std::cout << "���α׷��� ����Ǿ����ϴ�." << std::endl;
}