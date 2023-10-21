#include <iostream>

int main()
{
	bool running = true;
	while (running)
	{
		system("cls");
		int input, width;
		std::cout << "출력할 도형의 모양을 선택하세요" << std::endl;
		std::cout << "1 - X" << std::endl;
		std::cout << "2 - 역삼각형" << std::endl;
		std::cout << "3 - 마름모" << std::endl;
		std::cout << "4 - 나비" << std::endl;
		std::cout << "5 - 모래시계" << std::endl;
		std::cout << "6 - 테두리 사각형 두 개" << std::endl;
		std::cout << "7 - 프로그램 종료" << std::endl;
		std::cin >> input;
		if (7 == input)
			running = false;
		else
		{
			std::cout << "출력할 도형의 너비를 입력하세요 (2~20 사이의 짝수로)" << std::endl;
			std::cin >> width;
			if (0 != width % 2 || width < 2 || width > 20 || input < 1 || input > 6)
				std::cout << "잘못된 입력입니다." << std::endl;
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
	std::cout << "프로그램을 종료합니다." << std::endl;
}