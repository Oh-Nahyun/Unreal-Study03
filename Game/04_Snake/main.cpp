
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

// 게임 정보 (데이터) 정의 ------------------------------

// 방향키의 아스키 값
#define LEFT		75		// ← : 224, 75
#define RIGHT		77		// → : 224, 77
#define UP		72		// ↑ : 224, 72
#define DOWN		80		// ↓ : 224, 80

#define PAUSE		112		// p키를 입력
#define STATUS		115		// s키를 입력
#define ESC		27

#define MAP_X		3
#define MAP_Y		2
#define MAP_WIDTH	30
#define MAP_HEIGHT	20

// 게임 요소, 변수 정의 ------------------------------

int x[100], y[100];			// 좌표값 x, y 100개. [0][0] = 뱀머리...
int food_x, food_y;			// 음식의 좌표값
int length;				// 몸통의 길이
int speed;				// 게임 속도
int score;				// 점수 저장
int best_score;				// 최고 점수 저장
int last_score;				// 마지막 플레이 점수 저장
int dir;				// 이동 방향
int key;				// 입력 받는 키 값 저장
int status_on;				// 개발자용 (디버그용) 상세 정보 보여주기 (s키를 눌러 활성화)

// 게임 함수 정의 ------------------------------

void gotoxy(int x, int y);					
void gotoxy(int x, int y, const char* s);	
void cursor(int n);							

void title();								
void reset();								
void draw_map();							
void move(int dir);							
void pause();								
void game_over();							
void food();								

void status();								
void status_off();							

// Main 함수 구현 ------------------------------

int main()
{
	// 화면 사이즈 조절 & 색 변경
	system("mode CON COLS=100 LINES=25");	// Windows11은 다르다.
	system("color 7c");			// 첫자리 : 배경색, 두번째 : 글자색 (0 ~ f)

	// 커서 숨기기
	cursor(false);

	// 타이틀 출력
	title();

	// 반복문
	while (true)
	{
		// 키 입력
		if (_kbhit())
		{
			do {
				key = _getch();
			} while (key == 224);
		}

		// 게임 속도 조절
		Sleep(speed);

		// 이동 처리 (키 입력)
		switch (key)
		{
			case LEFT:
			case RIGHT:
			case UP:
			case DOWN:
				dir = key;
				key = 0;
				break;
			case PAUSE:			// 일시 정지 (P Key)
				pause();
				break;
			case STATUS:			// 게임 정보 보기 (S Key)
				if (status_on == 0) status_on = 1;
				else status_on = 0;
				// status_on = (!status_on);	// 위 코드와 같은 코드1
				// status_on != status_on;	// 위 코드와 같은 코드2
				key = 0;
				break;
			case ESC:			// 프로그램 종료
				exit(0);
				break;
			default:
				break;
		}

		// 스네이크 이동 처리
		move(dir);

		// 개발자용 게임 정보 표시
		if (status_on) status();
		else status_off();
	}

	// 게임 종료 전 일시 정지
	system("pause");

	return 0;
}

// 함수 구현 ------------------------------

/// <summary>
/// 좌표 이동 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/// <summary>
/// 좌표 이동 함수
/// (문자열 추가)
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="s">문자열</param>
void gotoxy(int x, int y, const char* s)
{
	COORD Pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	printf("%s", s);
}

/// <summary>
/// 커서 숨기는 함수
/// </summary>
/// <param name="n">커서 보이기 = 1 (true), 커서 숨기기 = 0 (false)</param>
void cursor(int n)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

/// <summary>
/// 게임 시작 화면 출력 함수
/// </summary>
void title()
{
	int i, j;

	// 키값 초기화 (버퍼에 있는 키값을 버린다.)
	while (_kbhit())
	{
		_getch();
	}

	// 맵 테두리 그리기
	draw_map();

	// 맵 테두리 안쪽만 지우기 (빈칸으로 채우기) - (나중에 게임 플레이 시 화면 초기화)
	
	// 맵 테두리 안쪽에 내용 출력
	int _x = MAP_X + (MAP_WIDTH / 2) - 7;
	gotoxy(_x, MAP_Y + 5, "+------------------------+");
	gotoxy(_x, MAP_Y + 6, "+        S N A K E       +");
	gotoxy(_x, MAP_Y + 7, "+------------------------+");

	gotoxy(_x, MAP_Y + 9, "< PRESS ANY KEY TO START >");

	gotoxy(_x, MAP_Y + 11, "  ▶ ←,→,↑,↓ : Move");
	gotoxy(_x, MAP_Y + 12, "  ▶    P Key    : Pause");
	gotoxy(_x, MAP_Y + 13, "  ▶   ESC Key   : Quit");

	// 타이틀 (키 입력)
	while (true)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == ESC)		// ESC Key --> 종료
			{
				exit(0);	// 0이면 정상 종료, 1이면 강제 종료
			}
			else
			{
				break;
			}
		}

		// 시작 키 알림 문구 반짝임... (0.4초씩)
		gotoxy(_x, MAP_Y + 9, "< PRESS ANY KEY TO START >");
		Sleep(400);
		gotoxy(_x, MAP_Y + 9, "                          ");
		Sleep(400);
	}

	// 게임 초기화 (준비) ==> 게임 플레이(로 이어지도록 준비)
	reset();
}

/// <summary>
/// 게임 초기화 함수 ★★★★★
/// (게임 플레이 준비)
/// </summary>
void reset()
{
	int i;

	// 화면 지우기
	system("cls");

	// 맵 테두리 그리기
	draw_map();

	// 키 값 초기화	 ★★★★★
	while (_kbhit())
	{
		_getch();
	}

	// 데이터 초기화 ★★★★★
	dir = LEFT;		// 방향 초기화
	speed = 100;		// 속도 초기화 (딜레이 값)
	length = 5;		// 뱀 몸통의 길이 초기화
	score = 0;		// 점수 초기화

	// 시작 시 뱀의 몸통값 입력
	for (i = 0; i < length; i++)
	{
		x[i] = MAP_WIDTH / 2 + i;
		y[i] = MAP_HEIGHT / 2;
		gotoxy(MAP_X + x[i], MAP_Y + y[i], "○");
	}

	// 뱀의 머리 그리기
	gotoxy(MAP_X + x[0], MAP_Y + y[0], "●");

	// 음식 생성 (랜덤)
	food();
}

/// <summary>
/// 맵 그리는 함수
/// (게임 맵 테두리)
/// </summary>
void draw_map()
{
	int i;

	// 상단 테두리
	for (i = 0; i < MAP_WIDTH; i++)
	{
		gotoxy(MAP_X + i, MAP_Y, "■");
	}

	// 세로 테두리
	for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++)
	{
		gotoxy(MAP_X, i, "■");
		gotoxy(MAP_X + MAP_WIDTH - 1, i, "■");
	}

	// 하단 테두리
	for (i = 0; i < MAP_WIDTH; i++)
	{
		gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■");
	}
}

/// <summary>
/// 뱀 이동 함수
/// (키 입력 및 출력) --> 게임의 메인 부분!
/// </summary>
/// <param name="dir"></param>
void move(int dir)
{
	/// [이벤트 처리] ------------------------------

	// 음식을 먹은 경우
	// (food와 충돌 => 몸통의 길이가 늘어난다.)

	// 벽과 충돌한 경우
	// (게임 오버) : 뱀의 머리

	// 자기 몸과 충돌한 경우
	// (게임 오버)
	
	/// [뱀 그리기 처리] ------------------------------

	// 몸통의 마지막 지우기
	

	// 몸통의 좌표를 한칸씩 옮기기

	// 머리가 있던 부분을 몸통으로 변경
	gotoxy(MAP_X + x[0], MAP_Y + y[0], "○");

	// 방향에 따라 새로운 머리 좌표를 변경
	if (dir == LEFT)	--x[0];
	if (dir == RIGHT)	++x[0];
	if (dir == UP)		--y[0];
	if (dir == DOWN)	++y[0];

	// 새로운 머리 좌표에 머리 그리기
	gotoxy(MAP_X + x[0], MAP_Y + y[0], "●");	// test...
}

/// <summary>
/// 게임 일시 정지 함수
/// </summary>
void pause()
{
}

/// <summary>
/// 게임 결과 처리 함수
/// (게임 오버)
/// </summary>
void game_over()
{
}

/// <summary>
/// 음식 생성 함수
/// </summary>
void food()
{
}

/// <summary>
/// 개발자용 status (상태 보기) 함수
/// (각종 스탯 개발 중 정보를 볼 수 있다.)
/// </summary>
void status()
{
	// 이동 좌표 (머리 좌표 기준)
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "Head   : ");
	printf("%3d, %3d", x[0], y[0]);

	// 음식 좌표
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "Food   : ");
	printf("%3d, %3d", food_x, food_y);

	// 몸통의 길이
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "Length : ");
	printf("%3d", length);
	
	// 현재의 키 값 (방향)
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "Key    : ");
	printf("%3d", dir);

	// 이동 속도
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "Speed  : ");
	printf("%3d", speed);

	// 현재 점수
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 5, "Score  : ");
	printf("%3d", score);
}

/// <summary>
/// 개발자용 status (상태 숨기기) 함수
/// </summary>
void status_off()
{
	// 이동 좌표 (머리 좌표 기준)
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "                    ");

	// 음식 좌표
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "                    ");

	// 몸통의 길이
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "                    ");

	// 현재의 키 값 (방향)
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "                    ");

	// 이동 속도
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "                    ");

	// 현재 점수
	gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 5, "                    ");
}

