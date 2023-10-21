#include <iostream>
#include <format>



//
//5번문제
// 수정본
// 수정
// 수정 완료
// 
//

std::string days[] = { "Sunday","Monday","Tuesday" ,"Wednesday", "Thursday", "Friday", "Saturday" };

int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

int day_day(int yyyy, int mm, int dd)
{
	int day = 3;
	for (int i = 2020; i < yyyy; ++i)
	{
		day += ((0 == i % 4 && i % 100) || 0 == i % 400) ? 366 : 365;
	}
	for (int i = 1; i < mm; ++i)
	{
		day += months[i - 1];
		if (((0 == yyyy % 4 && yyyy % 100) || 0 == yyyy % 400) && 2 == i)
			++day;
	}
	day += dd - 1;
	return day % 7;
}


void print_day(int yyyy, int mm, int dd)
{
	std::cout << yyyy << '/' << std::format("{:02d}", mm) << '/' << std::format("{:02d}", dd) << ": " << days[day_day(yyyy, mm, dd)] << std::endl;
	std::cout << yyyy << '-' << mm << std::endl;
	for (std::string& day : days)
		std::cout << std::format("{:12s}", day);
	std::cout << std::endl;
	for (int i = 1 - day_day(yyyy, mm, 01); i <= ((((0 == yyyy % 4 && yyyy % 100) || 0 == yyyy % 400) && mm == 2) ? 29 : months[mm - 1]); ++i)
	{
		if (i > 0)
			std::cout << std::format("{:<12d}", i);
		else
			std::cout << std::format("{:12c}", ' ');
		if ((7 - day_day(yyyy, mm, 01)) % 7 == i % 7)
			std::cout << std::endl;
	}
}

int main()
{
	bool running = true;
	char input;
	int year, month, day;
	print_day(2023, 03, 01);
	while (running)
	{
		int cnt = 0;
		std::cout << std::endl;
		std::cout << "명령어 : c/C - 요일 계산기 로드" << std::endl;
		std::cout << "명령어 : f/F - 모든 달의 마지막 날의 날짜와 요일 출력" << std::endl;
		std::cout << "명령어 : q/Q - 프로그램 종료" << std::endl;
		std::cin >> input;
		switch (input)
		{
		case 'c':
		case 'C':
			std::cout << "계산할 년, 월, 일을 입력하세요" << std::endl;
			std::cout << "(2020 01 01 ~ 2030 12 31)" << std::endl;
			std::cin >> year;
			std::cin >> month;
			std::cin >> day;
			if (year < 2020 || year>2030 || month < 1 || month>12 || day<1 || day>months[month - 1] + ((0 == year % 4 && year % 100) || 0 == year % 400) ? 1 : 0)
			{
				std::cout << "올바르지 않은 날짜를 입력하였습니다" << std::endl;
				system("pause");
				system("cls");
				print_day(2022, 03, 01);
			}
			else
			{
				system("cls");
				print_day(year, month, day);
			}
			break;
		case 'f':
		case 'F':
			std::cout << "계산할 연도를 입력하세요" << std::endl;
			std::cin >> year;
			for (int i = 2; i < 14; ++i)
				std::cout << year << '/' << std::format("{:02d}", i - 1) << '/' << std::format("{:02d}", (((0 == year % 4 && year % 100) || 0 == year % 400) && i == 3) ? 29 : months[i - 2]) << ": " << days[day_day(year, i, 0)] << std::endl;
			system("pause");
			system("cls");
			print_day(2022, 03, 01);
			break;
		case'q':
		case 'Q':
			running = false;
			break;
		default:
			std::cout << "올바른 값을 입력해주세요!." << std::endl;
			system("pause");
			system("cls");
			print_day(2022, 03, 01);
			break;
		}
	}
	system("cls");
	std::cout << "프로그램 종료~" << std::endl;
	system("pause");
}