#include <iostream>

double nums[6]{};
double copy_nums[6]{};
int oper_riv[6]{};
int copy_riv[6]{};
char oper[5]{};
char copy_oper[5]{};

void expressInput()
{
	char input;
	for (int i = 0; i < 5; ++i)
	{
		if ('(' == std::cin.peek() && i < 4)
		{
			std::cin >> input;
			for (int j = i; j < 4; ++j)
				oper_riv[j + 1] += 2;
			--i;
			continue;
		}
		std::cin >> nums[i];

		while (')' == std::cin.peek())
		{
			std::cin >> input;
			for (int j = i; j < 4; ++j)
				oper_riv[j + 1] -= 2;
		}
		if (i < 4)
		{
			std::cin >> oper[i];
			if ('*' == oper[i] || '/' == oper[i])
				++oper_riv[i + 1];
		}
	}
}

void operate()
{
	for (int i = 0; i < 6; ++i)
		copy_nums[i] = nums[i];
	for (int i = 0; i < 5; ++i)
		copy_oper[i] = oper[i];
	for (int i = 0; i < 5; ++i)
	{
		for (int j = copy_riv[i]; j < copy_riv[i + 1]; j += 2)
			std::cout << '(';
		std::cout << copy_nums[i];

		for (int j = copy_riv[i]; j > copy_riv[i + 1]; j -= 2)
			std::cout << ')';
		if (i < 4)
			std::cout << copy_oper[i];
	}

	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (copy_riv[j + 1] == 9 - i)
			{
				switch (copy_oper[j])
				{
				case '+':
					copy_nums[j] += copy_nums[j + 1];
					for (int k = j + 1; k < 5; ++k)
					{
						copy_nums[k] = copy_nums[k + 1];
						copy_oper[k - 1] = copy_oper[k];
						copy_riv[k] = copy_riv[k + 1];
					}
					--j;
					break;
				case '-':
					copy_nums[j] -= copy_nums[j + 1];
					for (int k = j + 1; k < 5; ++k)
					{
						copy_nums[k] = copy_nums[k + 1];
						copy_oper[k - 1] = copy_oper[k];
						copy_riv[k] = copy_riv[k + 1];
					}
					--j;
					break;
				case '*':
					copy_nums[j] *= copy_nums[j + 1];
					for (int k = j + 1; k < 5; ++k)
					{
						copy_nums[k] = copy_nums[k + 1];
						copy_oper[k - 1] = copy_oper[k];
						copy_riv[k] = copy_riv[k + 1];
					}
					--j;
					break;
				case '/':
					if (0 == copy_nums[j + 1])
					{
						std::cout << "0���� ���� �� �����ϴ�" << std::endl;
						system("pause");
						return;
					}
					copy_nums[j] /= copy_nums[j + 1];
					for (int k = j + 1; k < 5; ++k)
					{
						copy_nums[k] = copy_nums[k + 1];
						copy_oper[k - 1] = copy_oper[k];
						copy_riv[k] = copy_riv[k + 1];
					}
					--j;
					break;
				default:
					break;
				}
			}
		}
	std::cout << '=' << copy_nums[0];
	system("pause");
}

int main()
{
	char input;
	std::cout << "����� ���ϴ� ���� ���� ���� �Է��Ͻÿ� (�Ұ�ȣ ��� ����)" << std::endl;
	expressInput();
	bool running = true;
	while (running)
	{
		system("cls");
		std::cout << "��ɾ �Է��ϼ���" << std::endl;
		std::cout << "1: ������ �켱 ������� ����" << std::endl;
		std::cout << "2: �տ������� ���" << std::endl;
		std::cout << "3: �ڿ������� ���" << std::endl;
		std::cout << "4: �� �ٽ� �Է�" << std::endl;
		std::cout << "q: ���α׷� ����" << std::endl;
		std::cin >> input;
		switch (input)
		{
		case '1':
			for (int i = 0; i < 5; ++i)
				copy_riv[i] = oper_riv[i];
			operate();
			break;
		case '2':
			for (int i = 1; i < 5; ++i)
				copy_riv[i] = 10 - i * 2;
			operate();
			break;
		case '3':
			for (int i = 0; i < 5; ++i)
				copy_riv[i] = i * 2;
			operate();
			break;
		case '4':
			expressInput();
			break;
		case 'q':
			running = false;
			break;
		default:
			break;
		}
	}
	std::cout << "���α׷��� ����Ǿ����ϴ�" << std::endl;
}