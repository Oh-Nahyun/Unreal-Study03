
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
void Ending_game();

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

	// 착륙 지점 랜덤 처리
	srand((unsigned)time(NULL));

	// 시작 지점 x값 설정
	// (우주선 시작 랜덤값)
	int x = 0;
	do {
		x = rand() % 30;
	} while (x % 2 == 0);

	// 0. 테스트 ------------------------------

	// Draw Test : 캐릭터, 액터(mob), 오브젝트 출력
	Print_SpaceShip(1, 1);
	Print_Platform();

	// 1. 게임 인트로 ------------------------------
	Intro_game();

	// 2. 게임 플레이 & 콘트롤 ------------------------------
	Game_Control(x);

	// 3. 게임 종료 ------------------------------
	system("pause");

	return 0;
}

// 함수 구현 ------------------------------

/// <summary>
/// 인트로 (게임 설명) 출력 함수
/// </summary>
void Intro_game()
{

}

/// <summary>
/// 결과 출력 함수
/// </summary>
void Ending_game()
{

}

/// <summary>
/// 좌표 이동 (커서 이동) 함수
/// </summary>
void gotoxy(int x, int y)
{

}

/// <summary>
/// 우주선 출력 함수
/// </summary>
void Print_SpaceShip(int x, int y)
{

}

/// <summary>
/// 우주선 지우는 함수
/// </summary>
void Del_SpaceShip(int x, int y)
{

}

/// <summary>
/// 우주선 이동키 (좌, 우) 함수
/// </summary>
void Move_ArrowKey(char key, int* x1, int x_b)
{

}

/// <summary>
/// 우주선 아래로 이동시키는 함수
/// </summary>
/// <returns></returns>
int Move_Down(int x)
{
	return 0;
}

/// <summary>
/// 도착점 출력 함수
/// (위치는 랜덤)
/// </summary>
/// <returns></returns>
int Print_Platform()
{
	return 0;
}

/// <summary>
/// 결과 관리 함수
/// (점수 측정 - 게임 콘트롤)
/// </summary>
/// <returns></returns>
int Game_Control(int x)
{
	return 0;
}
