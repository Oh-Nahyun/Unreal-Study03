#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <fstream>

using namespace std;

void gotoxy(int x, int y);
void SetConsoleView();
int GetKeyDown();
void DrawReadyGame();
void DrawStartGame(int life, int score, vector<string>& pastWord);
void SetDictionary(vector<string>& strArr);
bool ReadyGame();
void StartGame();

// ------------------------------

int main()
{
	// 콘솔 화면 세팅
	SetConsoleView();

	bool isStart = false;

	// 반복문
while (true)
{
	// 게임 준비
	isStart = ReadyGame();	// s, q => Start 키가 입력되면 true

	// 게임 시작
	if (isStart)
	{
		// 게임 시작
		StartGame();

		// 테스트용
		//system("pause");
	}
	// 게임 종료
	else
	{
		// 다음 줄로 넘김
		cout << endl << endl;

		// 게임 종료
		break;
	}
}

return 0;
}

// ------------------------------

/// <summary>
/// 커서 이동 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/// <summary>
/// 콘솔 화면 세팅 함수
/// (화면 사이즈 조절 & 색 변경)
/// </summary>
void SetConsoleView()
{
	system("mode CON COLS=80 LINES=25");	// Windows11은 다르다.
	system("title [HangManGame]");			// 게임 제목
	system("color 7c");						// 첫자리 : 배경색, 두번째 : 글자색 (0 ~ f)
}

/// <summary>
/// 키보드 입력 함수
/// </summary>
/// <returns>입력된 키 값</returns>
int GetKeyDown()
{
	// 키보드에 무언가 입력이 오면...
	if (_kbhit() != 0)
	{
		return _getch();
	}

	return 0;
}

/// <summary>
/// 시작 화면 그리는 함수 (메뉴)
/// </summary>
void DrawReadyGame()
{
	system("cls");

	gotoxy(5, 2);	cout << "==============================";
	gotoxy(5, 3);	cout << "=====    HANG MAN GAME   =====";
	gotoxy(5, 4);	cout << "==============================";

	gotoxy(5, 6);	cout << "시작하려면 'S'키를 눌러주세요.";

	gotoxy(5, 8);	cout << "종료하려면 'Q'키를 눌러주세요.";
}

/// <summary>
/// 게임 화면 그리는 함수
/// </summary>
/// <param name="life">생명력</param>
/// <param name="score">점수</param>
/// <param name="pastWord">사용한 단어</param>
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
	system("cls");

	gotoxy(5, 1);	cout << "Life = " << life;
	gotoxy(5, 2);	cout << "Score = " << score;

	gotoxy(5, 8);	cout << "Past = ";
	for (int i = 0; i < static_cast<int>(pastWord.size()); ++i)
	{
		cout << pastWord[i] << " ";
	}

	gotoxy(5, 12);	cout << "Input = ";
	gotoxy(5, 14);	cout << "(메인화면으로 가려면 'QQ'를 입력해주세요.)" << endl;
}

/// <summary>
/// 사전(단어)을 세팅하는 함수 ★ ★ ★ ★ ★
/// </summary>
/// <param name="strArr"></param>
void SetDictionary(vector<string>& strArr)
{
	static const int INIT_NUM = 4;
	static const string str[INIT_NUM] = { "apple", "banana", "code", "program" };	// 샘플 영어 단어들

	ifstream readFromFile("words.txt");						// 읽기 전용으로 파일 오픈

	if (!readFromFile.is_open())							// 오픈되지 않는다는 건 파일이 존재하지 않는다는 의미!
	{
		ofstream writeToFile("words.txt");					// 쓰기 전용으로 파일 오픈 (파일이 없으면 자동으로 생성)

		for (int i = 0; i < INIT_NUM; ++i)
		{
			string temp = str[i];

			if (i != INIT_NUM - 1)
			{
				temp += "\n";
			}

			writeToFile.write(temp.c_str(), temp.size());	// 파일에 쓰는 함수
			strArr.push_back(str[i]);						// 단어장(strArr)에 단어를 넣는다.
		}

		writeToFile.close();								// 쓰기 파일 닫기
		return;
	}

	// 읽기 전용으로 성공한 경우 => 파일을 끝까지 읽어옴
	while (!readFromFile.eof())			// 파일 끝까지...
	{
		string temp;
		getline(readFromFile, temp);	// 한줄씩 읽기
		strArr.push_back(temp);			// 단어장에 저장하기
	}

	readFromFile.close();				// 읽기 파일 닫기
	return;
}

/// <summary>
/// 시작 화면 기능 함수 (s, q)
/// </summary>
/// <returns></returns>
bool ReadyGame()
{
	// 시작 화면 그리기
	DrawReadyGame();

	// 키 입력 처리하기
	while (true)
	{
		// 키 입력 부분
		int key = GetKeyDown();

		// 키 확인
		if (key == 's' || key == 'S')
		{
			return true;
		}
		else if (key == 'q' || key == 'Q')
		{
			break;
		}
	}

	return false;
}

/// <summary>
/// 게임 시작 함수
/// </summary>
void StartGame()
{
	// 게임 데이터
	int score = 0;

	// 입력한 영단어 저장
	vector<string> pastWord;

	// 맞출 단어장
	vector<string> strArr;

	// 파일 읽기
	SetDictionary(strArr);

	// 게임 루프 (반복)
	while (true)
	{
		// Play (단어 문제 랜덤 선택)
		int num = 0;
		srand((unsigned) time(NULL));

		//num = rand() % strArr.size();
		//num = (int)(rand() % strArr.size());				// 타입캐스팅01 : int 형 변환
		num = rand() % static_cast<int>(strArr.size());		// 타입캐스팅02 : static_cast<바꾸고자 하는 타입>(대상)

		// _ _ _ _ _ _ 형태로 표현할 변수
		string strQuestion;

		// 정답 확인용으로 저장하기 (string)
		const string strOriginal = strArr[num];

		// 정답 확인용으로 저장하기 (length)
		//int OriginalLen = strOriginal.length();
		int OriginalLen = static_cast<int>(strOriginal.length());

		// 초기화 (Init)
		for (int i = 0; i < OriginalLen; ++i)
		{
			strQuestion += "_";			// 정답의 길이 만큼 "_" 넣기
		}

		int life = OriginalLen + 2;		// 생명력은 정답 길이 + 2

		// 질문 (하나의 단어를 맞추는 루프!)
		while (true)
		{
			// 생명력, 점수, 사용한 알파벳 & 단어 표기하기
			DrawStartGame(life, score, pastWord);

			// 문제 그리기
			gotoxy(5, 5);
			for (int i = 0; i < OriginalLen; ++i)
			{
				cout << strQuestion[i] << " ";
			}
			cout << endl;

			// 입력 처리하기
			gotoxy(10, 12);
			string strInput;
			cin >> strInput;

			// 종료 입력 처리하기 (나가기)
			if (strInput == "qq" || strInput == "QQ")
			{
				return;
			}

			// 한번 입력한 단어는 pastWord에서 표기하기
			pastWord.push_back(strInput);

			// ------------------------------

			// 입력값 확인하기 (판별)

			// 1. 입력받은 스트링의 길이가 1인 경우 (단어의 알파벳을 찾음)
			if (strInput.length() == 1)
			{
				// 알파벳 검색
				for (int i = 0; i < OriginalLen; ++i)
				{
					// 오리지널 단어에 입력한 알파벳이 있는 경우 확인하기!
					if (strOriginal[i] == strInput[0])
					{
						// 해당 위치에 "_"를 알파벳으로 변경한다.
						strQuestion[i] = strInput[0];
					}
				}
			}

			// 2. 입력받은 스트링의 길이가 1보다 긴 경우 (단어 => 정답인지 확인)
			else if (strInput.length() > 1)
			{
				// 단어 체크, 오리지널 단어와 입력 단어가 같은 경우 (정답)
				if (strOriginal == strInput)
				{
					// 정답인 경우, 정답 점수에 5점을 더한다.
					score += 5;
					pastWord.clear();

					// 하나의 단어를 맞추는 루프를 나가게 되고, 다음 턴으로 넘어간다.
					break;
				}
			}

			// 라이프 수 조정하기 (틀리거나 맞거나 한자리 입력이 들어오면 => 무조건 life가 하나씩 깎인다.)
			life -= 1;
			if (life < 0)
			{
				score -= 5;
				if (score < 0)
				{
					// 사실상 게임 오버
					score = 0;	// 문제를 틀려도 계속 플레이 가능하도록 처리
				}
				pastWord.clear();
				break;
			}
		}
	}
}
