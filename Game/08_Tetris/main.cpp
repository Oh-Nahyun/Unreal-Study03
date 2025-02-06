#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

// 1. 속성 > C/C++ > 전처리기 > 전처리기 정의 (_CRT_SECURE_NO_WARNINGS (추가))
// 2. 속성 > C/C++ > 일반 > SDL 검사 (아니오 (선택))
// 3. 속성 > C/C++ > 언어 > 준수 모드 (아니오 (선택))
// 4. 속성 > 플랫폼 (Win32 (선택))
// 4. 속성 > 구성 관리자 > (위) x64 & (아래) Win32 (선택)
#define _CRT_SECURE_NO_WARNINGS

// Define ------------------------------
#define LEFT			75		// 좌로 이동
#define RIGHT			77		// 우로 이동
#define UP			72		// 회전
#define DOWN			80		// 빠른 하단 이동 (Soft Move)
#define SPACE			21		// 바로 바닥으로 순간 이동 (Hatd Move)
#define p			112		// p키를 입력 (소문자)
#define P			80		// P키를 입력 (대문자)
#define ESC			27		// 게임 종료

// 게임 보드 상태 & Status 정보
// 블록이 이동할 수 있는 공간은 0 또는 음수로 표현 (0, -1, -2)
// 블록이 이동할 수 없는 공간은 양수로 표현 (1, 2)
#define ACTIVE_BLOCK		-2
#define CELLING			-1
#define EMPTY			 0
#define WALL			 1
#define INACTIVE_BLOCK	 	 2		// 이동이 완료된 블록 값

#define MAIN_X			11		// 게임판 가로 크기
#define MAIN_Y			23		// 게임판 세로 크기
#define MAIN_X_ADJ		 3		// 게임판 위치 조정 x (여유공간)
#define MAIN_Y_ADJ		 1		// 게임판 위치 조정 y (여유공간)

#define STATUS_X_ADJ	MAIN_X_ADJ + MAIN_X + 1		// 게임 정보 위치

int STATUS_Y_GOAL;				// GOAL  정보 표시 위치, Y 좌표 저장
int STATUS_Y_LEVEL;				// LEVEL 정보 표시 위치, Y 좌표 저장
int STATUS_Y_SCORE;				// SCORE 정보 표시 위치, Y 좌표 저장

// 블록 데이터 ★ ★ ★
// 블록 모양 저장, 4 * 4의 공간에 블록을 표현한다.
// block[type][rotation][4][4] <= 버퍼 잡기 [7][4][4][4]
int blocks[7][4][4][4] = { 
	// O 미노 
	{{0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0}, {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}}, 
	// I 미노 
	{{0,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}}, 
	// Z 미노 
	{{0,0,0,0, 1,1,0,0, 0,1,1,0, 0,0,0,0}, {0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}}, 
	// S 미노 
	{{0,0,0,0, 0,1,1,0, 1,1,0,0, 0,0,0,0}, {0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}}, 
	// L 미노 
	{{0,0,0,0, 0,0,1,0, 1,1,1,0, 0,0,0,0}, {0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},{0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}}, 
	// J 미노 
	{{0,0,0,0, 1,0,0,0, 1,1,1,0, 0,0,0,0}, {0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},{0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}}, 
	// T 미노 
	{{0,0,0,0, 0,1,0,0, 1,1,1,0, 0,0,0,0}, {0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},{0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}} 
};

// 게임 변수 ------------------------------
int b_type;			// 블록 종류
int b_rotation;			// 블록 회전값
int b_type_next;		// 다음 블록값

int main_org[MAIN_Y][MAIN_X];	// 게임판의 정보를 저장하는 배열. 화면에 표시 후 Main_cpy로 복사된다.
int main_cpy[MAIN_Y][MAIN_X];	// 게임판이 화면에 표기되기 전의 정보를 가지고 있음.
				// main의 전체를 비교하지 않고 (계속 표기하면 화면이 깜빡거린다.)
				// main_cpy와 배열을 비교하여 값이 달라진 부분만 모니터에 새로 고침!!!

int bx, by;			// 이동 중인 블록의 게임판 상의 x, y 좌표

int key;			// 키보드로 받은 키값

int speed;			// 게임 진행 속도
int level;			// 현재 레벨
int level_goal;			// 목표 점수 : 다음 레벨로 넘어가기 위한 점수
int cnt;			// 현재 레벨에서 제거한 줄 수
int score;			// 현재 점수
int last_score = 0;		// 마지막 점수
int best_score = 0;		// 최고 점수

int new_block_on = 0;		// 새로운 블록이 필요함을 알리는 플래그 (flag)
int crush_on = 0;		// 현재 이동 중인 블록이 충돌 상태인지 알려주는 플래그 (flag)
int level_up_on = 0;		// 다음 레벨로 진행하라고 알리는 플래그 (현재 레벨 목표가 완료되었음) (flag)
int space_key_on = 0;		// 하드 드롭 상태임을 알리는 플래그 (flag) (= bool 형)

// 커서 숨기기 열거형 데이터
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

// 게임 함수 ------------------------------
void title();			// 게임 시작 화면 출력 함수
void reset();			// 게임판 초기화 함수
void reset_main();		// 메인 게임판 초기화 함수 (main_orh[][])
void reset_main_cpy();		// 복사 게임판 초기화 함수 (main_cpy[][])
void draw_map();		// 게임 전체 인터페이스를 표시하는 함수
void draw_main();		// 게임판 그리는 함수
void new_block();		// 새로운 블록 하나를 만드는 함수
void check_key();		// 키보드로 키 입력받는 함수
void drop_block();		// 블록을 아래로 떨어트리는 함수
int check_crush(int bx, int by, int b_rotation);	// 충돌 판단 함수 (rotation 시에 위치 보정)
void move_block(int dir);	// dir 방향으로 블록을 움직이는 함수
void check_line();		// 줄이 가득 찼는지 판단하고 지우는 함수
void check_level_up();		// 레벨 목표를 달성했는지 판단하고 레벨업을 시키는 함수
void check_game_over();		// 게임 오버인지 판단하고 게임 오버 시키는 함수
void pause();			// 게임 일시 정지 시키는 함수

void gotoxy(int x, int y);	// 커서 이동 함수
void set_cursor_type(CURSOR_TYPE c);	// 커서 숨기는 함수

// ------------------------------

int main()
{
	// 반복자
	int i;

	// 난수 초기화
	srand((unsigned)time(NULL));

	// 화면 크기
	system("mode con:cols=80 lines=30");
	system("title [TETRIS]");
	system("color 3e");

	// 커서 숨기기
	set_cursor_type(NOCURSOR);

	// 메인 타이틀 출력
	title();

	// 게임판 리셋
	reset();

	// 게임 반복 루프
	while (true)
	{
		// 키 입력하기
		
		// 블록 한칸 내리기
		
		// 레벨업 확인하기
		
		// 게임 오버 확인하기
		
		// 새로운 블록 생성하기 (뉴블록 플래그가 있는 경우)
	}
	
	return 0;
}

// ------------------------------

void title()
{
	int x = 5;	// 타이틀 화면이 표시되는 x 좌표
	int y = 4;	// 타이틀 화면이 표시되는 y 좌표
	int cnt;	// 타이틀 프레임을 세는 변수 (약간의 효과를 위한 변수)

	gotoxy(x, y + 0);	printf("◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎");			Sleep(200);
	gotoxy(x, y + 1);	printf("◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎");			Sleep(200);
	gotoxy(x, y + 2);	printf("◼︎◻︎◼︎◻︎              ◻︎◼︎◻︎◼︎");		Sleep(200);
	gotoxy(x, y + 3);	printf("◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎");			Sleep(200);
	gotoxy(x, y + 4);	printf("◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎◻︎◼︎");			Sleep(200);

	gotoxy(x + 5, y + 2);	print("T E T R I S");				Sleep(200);
	
	gotoxy(x, y + 7);	print("Please Enter Any Key to Start..");	Sleep(200);
	
	gotoxy(x, y + 9);	print("  △    : Shift");
	gotoxy(x, y + 10);	print("◁   ▷  : Left / Right");
	gotoxy(x, y + 11);	print("  ▽    : Soft Drop");
	gotoxy(x, y + 12);	print(" SPACE : Hard Drop");
	gotoxy(x, y + 13);	print("   P   : Pause");
	gotoxy(x, y + 14);	print("  ESC  : Quit");
	
	gotoxy(x, y + 16);	print("BONUS FOR HARD DROPS / COMBOS");

	// (연출) 재미는 있지만 하나도 안중요한 별 애니메이션 효과.. ------------------------------
	for (cnt = 0; ; cnt++)
	{
		if (_kbhit())			// 키 입력이 있다면 무한 루프 종료
			break;

		if (cnt % 200 == 0)		// 200으로 나누어 떨어질 때는 별 표시
			gotoxy(x + 2, y + 1);	printf("★");
		if ((cnt % 200 - 100) == 0)	// 위카운트에서 100 카운트 간격으로 별 지우기
			gotoxy(x + 2, y + 1);	printf("  ");

		if (cnt % 350 == 0)		// 350으로 나누어 떨어질 때는 별 표시
			gotoxy(x + 13, y + 2);	printf("★");
		if ((cnt % 350 - 100) == 0)	// 위카운트에서 100 카운트 간격으로 별 지우기
			gotoxy(x + 13, y + 2);	printf("  ");

		// 딜레이
		Sleep(10);
	}
	
	// 버퍼에 기록된 키값 버리기 (((이 부분은 중요!)))
	while (_kbhit())
	{
		_getch();
	}
}

void reset()
{
	// 최고 점수는 파일로 저장
	FILE* file = fopen("score.dat", "rt");	// score.dat 파일을 연결

	if (file)
	{
		best_score = 0;			// 파일이 없으면 그냥 최고 점수에 0을 넣는다.
	}
	else
	{
		fscanf(file, "%d", &best_score); // 파일이 열리면 최고 점수를 불러온다.
		fclose(file);
	}

	// 각종 변수 초기화
	level = 1;
	score = 0;
	level_goal = 10;
	key = 0;
	crush_on = 0;
	cnt = 0;
	speed = 100;

	// 화면 지우기
	system("cls");
	
	// 데이터 초기화
	reset_main();			// main_org 을 초기화 (천장, 좌우 벽돌, 바닥 등 초기화)
	draw_map();			// 게임 화면 그리기 (화면 오른쪽 정보 내용 등 출력) - UI
	draw_main();			// 게임판 그리기 (플레이 화면 내용)

	b_type_next = rand() % 7;	// 다음에 나올 블록 종류를 랜덤하게 생성하기
	new_block();			// 새로운 블록 하나 만들기
}

void reset_main()
{
}

void reset_main_cpy()
{
}

void draw_map()
{
}

void draw_main()
{
}

void new_block()
{
}

void check_key()
{
}

void drop_block()
{
}

int check_crush(int bx, int by, int b_rotation)
{
}

void move_block(int dir)
{
}

void check_line()
{
}

void check_level_up()
{
}

void check_game_over()
{
}

void pause()
{
}

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void set_cursor_type(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c)
	{
		case NOCURSOR:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = FALSE;
			break;
		case SOLIDCURSOR:
			CurInfo.dwSize = 100;
			CurInfo.bVisible = TRUE;
			break;
		case NORMALCURSOR:
			CurInfo.dwSize = 20;
			CurInfo.bVisible = TRUE;
			break;
		default:
			break;
	}

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
