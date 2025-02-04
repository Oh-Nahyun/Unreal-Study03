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
		isStart = ReadyGame();	// s, q

		// 게임 시작
		if (isStart)
		{
			// 게임 시작
			StartGame();

			// 테스트용
			system("pause");
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
}

/// <summary>
/// 콘솔 화면 세팅 함수
/// </summary>
void SetConsoleView()
{
}

/// <summary>
/// 키보드 입력 함수
/// </summary>
/// <returns></returns>
int GetKeyDown()
{
	return 0;
}

/// <summary>
/// 시작 화면 그리는 함수 (메뉴)
/// </summary>
void DrawReadyGame()
{
}

/// <summary>
/// 게임 화면 그리는 함수
/// </summary>
/// <param name="life"></param>
/// <param name="score"></param>
/// <param name="pastWord"></param>
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
}

/// <summary>
/// 사전(단어)을 세팅하는 함수
/// </summary>
/// <param name="strArr"></param>
void SetDictionary(vector<string>& strArr)
{
}

/// <summary>
/// 시작 화면 기능 함수 (s, q)
/// </summary>
/// <returns></returns>
bool ReadyGame()
{
	return false;
}

/// <summary>
/// 게임 시작 함수
/// </summary>
void StartGame()
{
}
