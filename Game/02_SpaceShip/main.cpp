
/// [ 컨셉 ] 동작 정의 ==============================

/// 1. 최고점에서 최저점 (높이는 20칸)
/// 2. 자동으로 아래로 떨어진다. (0.2초 또는 0.15초마다 하락)
/// 3. 착률 지점 랜덤
/// 4. 좌우 키보드를 이용하여 플레이 (입력 처리)
/// 5. 결과 : 정확히 착륙하는 경우		> 성공! ("착륙에 성공하였습니다!" 메시지 출력)
/// 6. 결과 : 걸쳐서 착륙하는 경우		> 절반 성공! ("완벽한 착륙에는 성공하지 못하였습니다." 메시지 출력)
/// 7. 결과 : 다른 곳에 착륙하는 경우	> 실패! ("착륙에 실패하였습니다!" 메시지 출력)
/// 8. UI - 게임 시작 전 게임 설명 출력

/// 필요 함수, 라이브러리 체크 ==============================

/// time();		// <time.h>
/// rand();		// <stdlib.h>
/// srand();	// <stdlib.h>
/// sleep();	// <Windows.h>	// 1/1000초 사용
/// getch();	// 키 입력, 일시 지연
/// system();	// 콘솔 명령어

/// ==================================================

#include <stdio.h>		// printf
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

// 함수 정의 ------------------------------

void Intro_game();
void Ending_game(int num);

void gotoxy(int x, int y);
void Print_SpaceShip(int x, int y);
void Del_SpaceShip(int x, int y);
void Move_ArrowKey(char key, int* x1, int x_b);
int Move_Down(int x);
int Print_Platform();
int Game_Control(int x);

// 데이터 정의 ------------------------------

// 우주선 데이터
int Spaceship[3][3] = { 0,1,0,
						1,1,1,
						1,0,1 };

// 착륙 지점 데이터
int Platform[3] = { 1,1,1 };

// Main 함수 구현 ------------------------------

int main()
{
	// 0. 초기화 & 시스템 ------------------------------
	
	// 색 설정
	//system("color e");
	system("color 9e");		// 첫자리 : 배경색, 두번째 : 글자색

	// 화면 사이즈 & 제목 조정 
	system("cls");
	system("mode con:cols=75 lines=30");
	system("title SpaceShip");

	// 착륙 지점 랜덤 처리
	srand((unsigned)time(NULL));

	// 시작 지점 x값 설정
	// (우주선 시작 랜덤값)
	int ShipX = 0;
	do {
		ShipX = rand() % 30;
	} while (ShipX % 2 == 0);

	// 0. 테스트 ------------------------------

	// Draw Test : 캐릭터, 액터(mob), 오브젝트 출력
	//Print_SpaceShip(1, 1);	Sleep(2000); Del_SpaceShip(1, 1);
	//Print_Platform();			system("pause");
	///// ??? ///// printf("A\n");

	// 1. 게임 인트로 ------------------------------
	Intro_game();

	// 2. 게임 플레이 & 콘트롤 ------------------------------
	Game_Control(ShipX);

	// 3. 게임 종료 ------------------------------
	gotoxy(20, 28);
	system("pause");

	return 0;
}

// 함수 구현 ------------------------------

/// <summary>
/// 인트로 (게임 설명) 출력 함수
/// </summary>
void Intro_game()
{
	system("cls");

	printf("\t\t \n\n");
	printf("\t\t ========== 우주선 착륙 게임 ==========\n\n");
	printf("\t\t    우주선을 착륙시키는 게임입니다.\n\n");
	printf("\t\t  우주선은 화면 위에 위치하는데\n\n");
	printf("\t\t  좌, 우 화살표 키를 이용하여\n\n");
	printf("\t\t  착륙 지점에 정확히 착륙시켜야 합니다.\n\n");
	printf("\t\t \n\n");
	printf("\t\t  우주선 모양\t\t착륙 위치\n\n");
	printf("\t\t      ▲\n\n");
	printf("\t\t    ▲▲▲\n\n");
	printf("\t\t    ▲  ▲\t\t □□□\n\n");
	printf("\t\t \n\n");
	printf("\t\t     < 아무 키나 누르면 시작합니다. > \n\n");

	gotoxy(20, 27);
	system("pause");
	system("cls");
}

/// <summary>
/// 결과 출력 함수
/// </summary>
void Ending_game(int num)
{
	switch (num)
	{
		case 1:		// 성공인 경우
			gotoxy(1, 27);
			printf("\t ========== 착륙에 성공하였습니다! ==========");
			break;
		case 2:		// 절반의 성공인 경우
			gotoxy(1, 27);
			printf("\t ===== 아쉽게도 완벽한 착륙에는 성공하지 못하였습니다! =====");
			break;
		case 3:		// 실패인 경우
			gotoxy(1, 27);
			printf("\t ========== 착륙에 실패하였습니다! ==========");
			break;
	}

	//gotoxy(20, 27);
	//system("pause");
	//system("cls");
}

/// <summary>
/// 좌표 이동 (커서 이동) 함수
/// </summary>
void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/// <summary>
/// 우주선 출력 함수
/// </summary>
void Print_SpaceShip(int x, int y)
{
	int count = 1;
	gotoxy(x, y);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (Spaceship[i][j])
			{
				printf("▲");
			}
			else
			{
				printf("  ");
			}
		}
		gotoxy(x, y + count++);
	}
}

/// <summary>
/// 우주선 지우는 함수
/// (Windows 방식)
/// </summary>
void Del_SpaceShip(int x, int y)
{
	int count = 1;
	gotoxy(x, y);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("  ");
		}
		gotoxy(x, y + count++);
	}
}

/// <summary>
/// 우주선 이동키 (좌, 우) 함수
/// (화살표 키를 이용한 문자의 이동 함수)
/// </summary>
void Move_ArrowKey(char key, int* x1, int x_b)
{
	switch (key)
	{
		case 75:	// 왼쪽 화살표 키
			*x1 = *x1 - 2;
			if (*x1 < 1) *x1 = 1;		// 화면을 벗어나지 않도록 설정
			Beep(1318.510, 10);			// #define _E 1318.510
			break;
		case 77:	// 오른쪽 화살표 키
			*x1 = *x1 + 2;
			if (*x1 > x_b) *x1 = x_b;	// 화면을 벗어나지 않도록 설정
			Beep(1396.913, 10);			// #define _F 1396.913
			break;
		default:
			break;
	}
}

/// <summary>
/// 우주선 아래로 이동시키는 함수
/// </summary>
/// <returns>우주선의 이동 위치</returns>
int Move_Down(int x)
{
	int y = 1;
	char key;

	for (y = 1; y < 20; y++)
	{
		gotoxy(x, y);

		// 우주선 출력
		Print_SpaceShip(x, y);
		Sleep(150);

		// 우주선 삭제
		//system("cls");		// 전체 화면을 지우는 방식 ==> 플랫폼이 지워지는 에러 발생
		Del_SpaceShip(x, y);	// 우주선 부분만 지우는 방식

		// 키보드 입력받기
		if (_kbhit())
		{
			key = _getch();
			Move_ArrowKey(key, &x, 30);
		}

		// 게임 중간 출력 (디버그)
		gotoxy(14, 25);
		printf("우주선 : %d", x);
	}

	// 최종 위치
	gotoxy(x, y);

	// 우주선 다시 출력
	Print_SpaceShip(x, y);

	// 우주선의 이동 위치 반환
	return x;
}

/// <summary>
/// 도착점 출력 함수
/// (위치는 랜덤)
/// </summary>
/// <returns>착륙장 시작 위치</returns>
int Print_Platform()
{
	// 위치
	int number = 0;
	do {
		number = rand() % 30;
	} while (number % 2 == 0);

	// 커서 이동
	gotoxy(number, 23);

	// 출력
	for (int i = 0; i < 3; i++)
	{
		if (Platform[i])
		{
			printf("□");
		}
	}

	// 착륙장 시작 위치 반환 (결과 확인용)
	return number;
}

/// <summary>
/// 결과 관리 함수
/// (점수 측정 - 게임 콘트롤)
/// </summary>
/// <returns></returns>
int Game_Control(int x)
{
	// 1. 착륙 위치 출력
	// 2. 우주선 출력 & 이동 & 하강
	// 3. 게임의 결과 처리

	// 기준 좌표값
	int result = 0;		// 플랫폼[0] = 맨 왼쪽 x값 □□□
	int sResult = 0;	// 우주선[0] = 맨 왼쪽 x값 ▲  ▲

	// 플랫폼 출력
	result = Print_Platform();

	// 게임 결과 출력 (확인)
	gotoxy(1, 25);
	printf("도착점 : %d", result);

	// 내려오는 우주선 출력
	sResult = Move_Down(x);

	// 게임 결과 출력 (재확인)
	gotoxy(1, 25);
	printf("도착점 : %d, 우주선 : %d", result, sResult);

	// 결과 출력
	int GameResult = 0;

	if (sResult == result)
		GameResult = 1;	// 성공
	else if (sResult + 2 == result || sResult + 4 == result || sResult - 2 == result || sResult - 4 == result)
		GameResult = 2;	// 걸친 상태
	else
		GameResult = 3; // 실패

	Ending_game(GameResult);

	return 0;
}
