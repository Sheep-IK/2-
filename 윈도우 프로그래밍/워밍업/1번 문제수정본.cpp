#include <iostream>
#include <random>


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_int_distribution<int> uid{ 0,3 };


//TOGGLE
//GONGBACKGUMSA



//e / E: 단어에 e 또는 E자가 있는 경우에는 그 단어를 모두 대문자로 바꾸고 출력
//– f / F : 모든 공백에 랜덤한 문자(@, %, # 또는 .)를 넣고 출력
//– l / L : 단어의 개수를 출력(공백을 기준으로 단어 분리한다)
//– c / C : 대문자는 소문자로, 소문자는 대문자로 바꾸기
//– a / A : 문장의 문자를 좌측 / 우측으로 한 문자씩 이동한다.
//– q / Q : 프로그램을 종료



char char_table[]{ '@', '%', '#', '.' };

const int max_size = 60;

int main()
{
	char* sentence = new char(max_size);
	std::cout << "최대 " << max_size << "자의 문장을 입력하세요: ";
	std::cin >> std::noskipws;
	char temp = NULL;
	for (int i = 0; i < max_size; ++i)
	{
		std::cin >> sentence[i];
		if (' ' == temp && ' ' == sentence[i])
			--i;
		temp = sentence[i];
		if ('\n' == sentence[i])
		{
			sentence[i] = NULL;
			break;
		}
	}
	int sentence_size = 0;
	for (int i = 0; NULL != sentence[i]; ++i)
		++sentence_size;
	bool running = true;
	int word_num;
	bool to_upper = false;
	int word_start;
	while (running)
	{
		system("cls");
		std::cout << "현재 문장: " << sentence << std::endl << std::endl;
		std::cout << "사용할 명령어를 입력하세요" << std::endl;
		std::cout << "	-e/E: 단어에 e 또는 E자가 있는 경우에는 그 단어를 모두 대문자로 변환" << std::endl;
		std::cout << "	-f/F: 모든 공백에 랜덤한 문자 (@, %, # 또는 .)를 삽입" << std::endl;
		std::cout << "	-l/L: 단어의 개수를 출력" << std::endl;
		std::cout << "	-c/C: 대문자는 소문자로, 소문자는 대문자로 변환" << std::endl;
		std::cout << "	-a/A: 문장의 문자를 좌측/우측으로 한 문자씩 이동" << std::endl;
		std::cout << "	-q/Q: 프로그램을 종료" << std::endl;
		char input;
		if (std::cin >> input && '\n' != input)
		{
			switch (input)
			{
			case 'e':
			case 'E':
				word_start = 0;
				for (int i = 0; i < sentence_size; ++i)
				{
					if ('e' == sentence[i] || 'E' == sentence[i])
						to_upper = true;
					if ((' ' == sentence[i] || i + 1 == sentence_size) && to_upper)
					{
						if (NULL == sentence[i + 1])
							++i;
						for (int j = word_start; j < i; ++j)
						{
							if (islower(sentence[j]))
								sentence[j] -= 32;
						}
						to_upper = false;
					}
					if (' ' == sentence[i])
						word_start = i + 1;
				}
				break;
			case 'f':
			case 'F':
				for (int i = 0; i < sentence_size; ++i)
				{
					if (' ' == sentence[i])
					{
						sentence[i] = char_table[uid(dre)];
					}
				}
				break;
			case 'l':
			case 'L':
				word_num = 1;
				for (int i = 0; i < sentence_size; ++i)
				{
					if (' ' == sentence[i])
					{
						++word_num;
					}
				}
				std::cout << "Result: " << word_num << " words" << std::endl;
				system("pause");
				break;
			case 'c':
			case 'C':
				for (int i = 0; i < sentence_size; ++i)
				{
					if (islower(sentence[i]))
						sentence[i] -= 32;
					else if (isupper(sentence[i]))
						sentence[i] += 32;
				}
				break;
			case 'a':
				temp = sentence[0];
				for (int i = 1; i < sentence_size; ++i)
				{
					sentence[i - 1] = sentence[i];
					if (NULL == sentence[i + 1])
						sentence[i] = temp;
				}
				break;
			case 'A':
				temp = sentence[sentence_size - 1];
				for (int i = sentence_size - 1; i > 0; --i)
					sentence[i] = sentence[i - 1];
				sentence[0] = temp;
				break;
			case 'q':
			case 'Q':
				running = false;
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