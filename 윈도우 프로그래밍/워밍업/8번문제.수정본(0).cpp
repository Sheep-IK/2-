#include<iostream>
#include<random>
#include<windows.h>

using namespace std;



//– 구조체를 사용하여 회원의 데이터를 저장하고 키보드 명령을 수행한다.
//– 3명의 회원 데이터를 미리 저장해 둔다.
//– 최대 15명의 데이터를 저장할 수 있도록 한다.

//
//• 이름 : 최대 5자리 문자열, 예 – song
//• 회원 번호 : 5자리 숫자, 예 - 12345
//• 생일 : 4자리 숫자, 예 - 0302
//• 2023년 출석 현황(1월 ~6월까지) : 2개 정수, 예 1 20 (1월 20회 출석), 2 15 (2월 15회 출석









uniform_int_distribution<int> uid{ 1,29 };
random_device rd;
default_random_engine dre(rd());

class member_inpo
{
public:
	char name[6];
	char member_num[6];
	char birth[5];
	int atendance[6];
};


void initial_saved_member();
void order_assemble();
void new_member();
void print_mem();
void max_mem_print();
void min_mem_print();
void max_month_mem_print();
void name_sort();
void mem_num_sort();
void birth_sort();


member_inpo member[15];

int mem_cnt = 3;
char input;

int main()
{
	initial_saved_member();
	while (1)
	{
		order_assemble();
		cin >> input;
		system("cls");

		switch (input)
		{
		case 'a':
		case 'A':
			new_member();
			break;
		case 'p':
		case 'P':
			print_mem();
			break;
		case 'M':
		case 'm':
			max_mem_print();
			break;
		case 'N':
		case 'n':
			min_mem_print();
			break;
		case '1':
		case '2':
		case '3':
			max_month_mem_print();
			break;
		case 'i':
		case 'I':
			name_sort();
			break;
		case 'j':
		case 'J':
			mem_num_sort();
			break;
		case 'k':
		case 'K':
			birth_sort();
			break;
		case 'q':
		case 'Q':
			cout << "프로그램을 종료합니다";
			exit(0);
		default:
			cout << "잘못된 입력입니다" << endl;
			break;
		}

	}
}

void initial_saved_member()
{
	member[0] = { "kim" ,"00004","0530" };
	member[1] = { "lee" ,"00002","0227" };
	member[2] = { "park","00003","1224" };

	for (int i = 0; i < 6; ++i) member[0].atendance[i] = uid(dre);
	for (int i = 0; i < 6; ++i) member[1].atendance[i] = uid(dre);
	for (int i = 0; i < 6; ++i) member[2].atendance[i] = uid(dre);
}
void order_assemble()
{
	cout << "A/a: 새로운 회원 데이터를 입력받고 저장" << endl;
	cout << "P/p: 모든 회원 데이터를 출력" << endl;
	cout << "M/m: 회원 중 출석의 숫자가 가장 큰 회원을 찾아 데이터를 출력" << endl;
	cout << "N/n: 회원 중 출석의 숫자가 가장 작은 회원을 찾아 데이터를 출력" << endl;
	cout << "1/2/3: 1월/2월/3월 중 출석의 숫자가 가장 큰 회원을 찾아 데이터를 출력" << endl;
	cout << "I/i: 회원 이름에 따라 오름차순으로 정렬" << endl;
	cout << "J/j: 회원 번호에 따라 오름차순으로 정렬" << endl;
	cout << "K/k: 회원 생년월일에 따라 오름차순으로 정렬" << endl;
	cout << "Q/q: 프로그램 종료" << endl;
}
void new_member()
{
	cout << "회원 데이터를 입력하세요." << endl;
	cout << "이름 : ";
	do {
		cin >> member[mem_cnt].name;
		cout << endl;
		if (strlen(member[mem_cnt].name) > 5) cout << "5자 이내로 적어주세요." << endl;
	} while (strlen(member[mem_cnt].name) > 5);

	cout << "회원번호 : ";
	do {
		cin >> member[mem_cnt].member_num;
		cout << endl;
		if (strlen(member[mem_cnt].member_num) != 5) cout << "5자리로 적어주세요." << endl;
	} while (strlen(member[mem_cnt].member_num) != 5);


	cout << "생일: ";
	do {
		cin >> member[mem_cnt].birth;
		cout << endl;
		if (strlen(member[mem_cnt].birth) != 4) cout << "4자리로 적어주세요.  ex) 0421" << endl;
	} while (strlen(member[mem_cnt].birth) != 4);

	cout << "출석 일수: ";
	cout << endl;
	for (int i = 1; i <= 6; ++i)
	{
		cout << i << " 월 출석 일 수 : ";
		cin >> member[mem_cnt].atendance[i - 1];
		cout << endl;
	}
	++mem_cnt;
	system("cls");
}
void print_mem()
{
	for (int i = 0; i < mem_cnt; ++i)
	{
		cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "  생일 : " << member[i].birth;

		cout << " 1~6월 까지의 출석 횟수 : ";
		for (int j = 0; j < 6; ++j)
			cout << member[i].atendance[j] << "  ";

		cout << endl;
	}
}
void max_mem_print()
{
	int max = 0;
	int sum[15];
	for (int i = 0; i < 15; ++i)sum[i] = 0;

	for (int i = 0; i < mem_cnt; ++i)
		for (int j = 0; j < 6; ++j) sum[i] += member[i].atendance[j];

	for (int i = 0; i < mem_cnt; ++i)
	{
		if (max < sum[i])
		{
			max = sum[i];
			system("cls");
			cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "생일 : " << member[i].birth;
			cout << " 1~6월 까지의 출석 횟수 : ";
			for (int j = 0; j < 6; ++j)
				cout << member[i].atendance[j] << "  ";

			cout << endl;
		}
	}
}
void min_mem_print()
{
	int min = 100;
	int sum[15];
	for (int i = 0; i < 15; ++i)sum[i] = 0;

	for (int i = 0; i < mem_cnt; ++i)
		for (int j = 0; j < 6; ++j) sum[i] += member[i].atendance[j];

	for (int i = 0; i < mem_cnt; ++i)
	{
		if (min > sum[i])
		{
			min = sum[i];
			system("cls");
			cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "생일 : " << member[i].birth;
			cout << " 1~6월 까지의 출석 횟수 : ";
			for (int j = 0; j < 6; ++j)
				cout << member[i].atendance[j] << "  ";

			cout << endl;
		}
	}
}
void max_month_mem_print()
{
	int max = 0;
	switch (input)
	{
	case '1':
		for (int i = 0; i < mem_cnt; ++i)
		{
			if (max < member[i].atendance[0])
			{
				max = member[i].atendance[0];
				system("cls");
				cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "생일 : " << member[i].birth;
				cout << " 1월 출석 횟수 : ";

				cout << member[i].atendance[0] << "  ";

				cout << endl;
			}
		}
		break;
	case '2':
		for (int i = 0; i < mem_cnt; ++i)
		{
			if (max < member[i].atendance[1])
			{
				max = member[i].atendance[1];
				system("cls");
				cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "생일 : " << member[i].birth;
				cout << " 2월 출석 횟수 : ";

				cout << member[i].atendance[1] << "  ";

				cout << endl;
			}
		}
		break;
	case'3':
		for (int i = 0; i < mem_cnt; ++i)
		{
			if (max < member[i].atendance[2])
			{
				max = member[i].atendance[2];
				system("cls");
				cout << "회원 이름 : " << member[i].name << "  회원 번호 : " << member[i].member_num << "생일 : " << member[i].birth;
				cout << " 3월 출석 횟수 : ";
				cout << member[i].atendance[2] << "  ";

				cout << endl;
			}
		}
		break;
	}
}
void name_sort()
{
	char temp[6];
	int ntemp[6];

	for (int i = 0; i < mem_cnt; ++i)
	{
		for (int j = i + 1; j < mem_cnt; ++j)
		{
			if (strcmp(member[i].name, member[j].name) > 0)
			{
				strcpy_s(temp, member[i].name);
				strcpy_s(member[i].name, member[j].name);
				strcpy_s(member[j].name, temp);

				strcpy_s(temp, member[i].birth);
				strcpy_s(member[i].birth, member[j].birth);
				strcpy_s(member[j].birth, temp);

				strcpy_s(temp, member[i].member_num);
				strcpy_s(member[i].member_num, member[j].member_num);
				strcpy_s(member[j].member_num, temp);

				for (int k = 0; k < 6; ++k)
				{
					ntemp[k] = member[i].atendance[k];
					member[i].atendance[k] = member[j].atendance[k];
					member[j].atendance[k] = ntemp[k];
				}
			}
		}
	}
	print_mem();
}
void mem_num_sort()
{
	char temp[6];
	int ntemp[6];
	for (int i = 0; i < mem_cnt; ++i)
	{
		for (int j = i + 1; j < mem_cnt; ++j)
		{
			int num_i = atoi(member[i].member_num);
			int num_j = atoi(member[j].member_num);

			if (num_i > num_j)
			{
				strcpy_s(temp, member[i].name);
				strcpy_s(member[i].name, member[j].name);
				strcpy_s(member[j].name, temp);

				strcpy_s(temp, member[i].birth);
				strcpy_s(member[i].birth, member[j].birth);
				strcpy_s(member[j].birth, temp);

				strcpy_s(temp, member[i].member_num);
				strcpy_s(member[i].member_num, member[j].member_num);
				strcpy_s(member[j].member_num, temp);

				for (int k = 0; k < 6; ++k)
				{
					ntemp[k] = member[i].atendance[k];
					member[i].atendance[k] = member[j].atendance[k];
					member[j].atendance[k] = ntemp[k];
				}
			}

		}
	}
	print_mem();
}
void birth_sort()
{
	char temp[6];
	int ntemp[6];
	for (int i = 0; i < mem_cnt; ++i)
	{
		for (int j = i + 1; j < mem_cnt; ++j)
		{
			int num_i = atoi(member[i].birth);
			int num_j = atoi(member[j].birth);

			if (num_i > num_j)
			{
				strcpy_s(temp, member[i].name);
				strcpy_s(member[i].name, member[j].name);
				strcpy_s(member[j].name, temp);

				strcpy_s(temp, member[i].birth);
				strcpy_s(member[i].birth, member[j].birth);
				strcpy_s(member[j].birth, temp);

				strcpy_s(temp, member[i].member_num);
				strcpy_s(member[i].member_num, member[j].member_num);
				strcpy_s(member[j].member_num, temp);

				for (int k = 0; k < 6; ++k)
				{
					ntemp[k] = member[i].atendance[k];
					member[i].atendance[k] = member[j].atendance[k];
					member[j].atendance[k] = ntemp[k];
				}
			}
		}
	}
	print_mem();
}