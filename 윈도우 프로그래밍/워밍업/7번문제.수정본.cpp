#include <iostream>

int main()
{
	bool running = true;
	while (running)
	{
		system("cls");
		int input, width;
		std::cout << "����� ������ ����� �����ϼ���" << std::endl;
		std::cout << "1 - X" << std::endl;
		std::cout << "2 - ���ﰢ��" << std::endl;
		std::cout << "3 - ������" << std::endl;
		std::cout << "4 - ����" << std::endl;
		std::cout << "5 - �𷡽ð�" << std::endl;
		std::cout << "6 - �׵θ� �簢�� �� ��" << std::endl;
		std::cout << "7 - ���α׷� ����" << std::endl;
		std::cin >> input;
		if (7 == input)
			running = false;
		else
		{
			std::cout << "����� ������ �ʺ� �Է��ϼ��� (2~20 ������ ¦����)" << std::endl;
			std::cin >> width;
			if (0 != width % 2 || width < 2 || width > 20 || input < 1 || input > 6)
				std::cout << "�߸��� �Է��Դϴ�." << std::endl;
			else
			{
				switch (input)
				{
				case 1:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width / 2; ++j)
							if (i == j || i + j == width / 2 - 1)
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				case 2:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width; ++j)
							if (i + j < width - 1 && j - i >= 0)
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				case 3:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width; ++j)
							if (i * 2 + j >= width / 2 - 1 && i * 2 + j <= width * 1.5 - 3 && i * 2 - j <= width / 2 - 1 && j - i * 2 <= width / 2 - 1)
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				case 4:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width - 1; ++j)
							if (!(i * 2 + j <= width - 3 && i * 2 - j <= -1) && !(i * 2 + j >= width - 1 && i * 2 - j >= 1))
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				case 5:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width - 1; ++j)
							if ((i * 2 + j <= width - 2 && i * 2 - j <= 0) || (i * 2 + j >= width - 2 && i * 2 - j >= 0))
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				case 6:
					for (int i = 0; i < width / 2; ++i)
					{
						for (int j = 0; j < width * 2; ++j)
							if (i == 0 || i == width / 2 - 1 || j == 0 || j == width - 1 || j == width || j == width * 2 - 1)
								std::cout << '*';
							else
								std::cout << ' ';
						std::cout << std::endl;
					}
					break;
				default:
					break;
				}
			}
		}
		system("pause");
	}
	std::cout << "���α׷��� �����մϴ�." << std::endl;
}