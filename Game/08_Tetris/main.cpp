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
#define UP				72		// 회전
#define DOWN			80		// 빠른 하단 이동 (Soft Move)
#define SPACE			32		// 바로 바닥으로 순간 이동 (Hatd Move)
#define p				112		// p키를 입력 (소문자)
#define P				80		// P키를 입력 (대문자)
#define ESC				27		// 게임 종료

// 게임 보드 상태 & Status 정보
// 블록이 이동할 수 있는 공간은 0 또는 음수로 표현 (0, -1, -2)
// 블록이 이동할 수 없는 공간은 양수로 표현 (1, 2)
#define ACTIVE_BLOCK	-2
#define CELLING			-1
#define EMPTY			 0
#define WALL			 1
#define INACTIVE_BLOCK	 2		// 이동이 완료된 블록 값

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
int b_type;						// 블록 종류
int b_rotation;					// 블록 회전값
int b_type_next;				// 다음 블록값

int main_org[MAIN_Y][MAIN_X];	// 게임판의 정보를 저장하는 배열. 화면에 표시 후 Main_cpy로 복사된다.
int main_cpy[MAIN_Y][MAIN_X];	// 게임판이 화면에 표기되기 전의 정보를 가지고 있음.
// main의 전체를 비교하지 않고 (계속 표기하면 화면이 깜빡거린다.)
// main_cpy와 배열을 비교하여 값이 달라진 부분만 모니터에 새로 고침!!!

int bx, by;						// 이동 중인 블록의 게임판 상의 x, y 좌표

int key;						// 키보드로 받은 키값

int speed;						// 게임 진행 속도
int level;						// 현재 레벨
int level_goal;					// 목표 점수 : 다음 레벨로 넘어가기 위한 점수
int cnt;						// 현재 레벨에서 제거한 줄 수
int score;						// 현재 점수
int last_score = 0;				// 마지막 점수
int best_score = 0;				// 최고 점수

int new_block_on = 0;			// 새로운 블록이 필요함을 알리는 플래그 (flag)
int crush_on = 0;				// 현재 이동 중인 블록이 충돌 상태인지 알려주는 플래그 (flag)
int level_up_on = 0;			// 다음 레벨로 진행하라고 알리는 플래그 (현재 레벨 목표가 완료되었음) (flag)
int space_key_on = 0;			// 하드 드롭 상태임을 알리는 플래그 (flag) (= bool 형)

// 커서 숨기기 열거형 데이터
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

// 게임 함수 ------------------------------
void title();				
void reset();				
void reset_main();			
void reset_main_cpy();		
void draw_map();			
void draw_main();			
void new_block();			
void check_key();			
void drop_block();			
int check_crush(int bx, int by, int b_rotation);	
void move_block(int dir);	
void check_line();			
void check_level_up();		
void check_game_over();		
void pause();				

void gotoxy(int x, int y);	
void set_cursor_type(CURSOR_TYPE c);	

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
		// 블록이 한칸씩 떨어지는 동안 5번 키 입력을 받을 수 있도록 제어하기! (난이도 설정 가능)
		for (i = 0; i < 5; i++)
		{
			// 키 입력하기
			check_key();

			// 화면 그리기
			draw_main();

			// 게임 속도 조절하기
			Sleep(speed);

			// 블록 회전 예외 처리하기 (기본)
			// 블록이 충돌 중인 경우, 추가로 이동 및 회전할 시간 제공...
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false)
				Sleep(100);

			// 스페이스 바를 누른 경우, 바로 하드 드롭 처리하기
			if (space_key_on == 1)
			{
				space_key_on = 0;
				break;
			}
		}

		// 블록 한칸 내리기
		drop_block();

		// 레벨업 확인하기
		check_level_up();

		// 게임 오버 확인하기
		check_game_over();

		// 새로운 블록 생성하기 (뉴블록 플래그가 있는 경우)
		if (new_block_on == 1)
			new_block();
	}

	return 0;
}

// ------------------------------

/// <summary>
/// 게임 시작 화면 출력 함수
/// </summary>
void title()
{
	int x = 5;	// 타이틀 화면이 표시되는 x 좌표
	int y = 4;	// 타이틀 화면이 표시되는 y 좌표
	int cnt;	// 타이틀 프레임을 세는 변수 (약간의 효과를 위한 변수)

	gotoxy(x, y + 0);	printf("■□■□■□■□■□■□■□■");			Sleep(200);
	gotoxy(x, y + 1);	printf("□■□■□■□■□■□■□■□");			Sleep(200);
	gotoxy(x, y + 2);	printf("■□■□              □■□■");			Sleep(200);
	gotoxy(x, y + 3);	printf("□■□■□■□■□■□■□■□");			Sleep(200);
	gotoxy(x, y + 4);	printf("■□■□■□■□■□■□■□■");			Sleep(200);

	gotoxy(x + 5, y + 2);	printf("T E T R I S");							Sleep(200);

	gotoxy(x, y + 7);	printf("Please Enter Any Key to Start..");			Sleep(200);

	gotoxy(x, y + 9);	printf("  △    : Shift");
	gotoxy(x, y + 10);	printf("◁  ▷  : Left / Right");
	gotoxy(x, y + 11);	printf("  ▽    : Soft Drop");
	gotoxy(x, y + 12);	printf(" SPACE  : Hard Drop");
	gotoxy(x, y + 13);	printf("   P    : Pause");
	gotoxy(x, y + 14);	printf("  ESC   : Quit");

	gotoxy(x, y + 16);	printf("BONUS FOR HARD DROPS / COMBOS");

	// (연출) 재미는 있지만 하나도 안중요한 별 애니메이션 효과.. ------------------------------
	for (cnt = 0; ; cnt++)
	{
		if (_kbhit())				// 키 입력이 있다면 무한 루프 종료
			break;

		if (cnt % 200 == 0)			// 200으로 나누어 떨어질 때는 별 표시
		{
			gotoxy(x + 2, y + 1);	printf("★");
		}
		if ((cnt % 200 - 100) == 0)	// 위 카운트에서 100 카운트 간격으로 별 지우기
		{
			gotoxy(x + 2, y + 1);	printf("  ");
		}

		if (cnt % 350 == 0)			// 350으로 나누어 떨어질 때는 별 표시
		{
			gotoxy(x + 13, y + 2);	printf("★");
		}
		if ((cnt % 350 - 100) == 0)	// 위 카운트에서 100 카운트 간격으로 별 지우기
		{
			gotoxy(x + 13, y + 2);	printf("  ");
		}

		// 딜레이
		Sleep(10);
	}

	// 버퍼에 기록된 키값 버리기 (((이 부분은 중요!)))
	while (_kbhit())
	{
		_getch();
	}
}

/// <summary>
/// 게임판 초기화 함수
/// </summary>
void reset()
{
	// 최고 점수는 파일로 저장
	FILE* file = fopen("score.dat", "rt");	// score.dat 파일을 연결

	if (file == 0)
	{
		best_score = 0;						// 파일이 없으면 그냥 최고 점수에 0을 넣는다.
	}
	else
	{
		fscanf(file, "%d", &best_score);	// 파일이 열리면 최고 점수를 불러온다.
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
	reset_main();				// main_org 을 초기화 (천장, 좌우 벽돌, 바닥 등 초기화)
	draw_map();					// 게임 화면 그리기 (화면 오른쪽 정보 내용 등 출력) - UI
	draw_main();				// 게임판 그리기 (플레이 화면 내용)

	b_type_next = rand() % 7;	// 다음에 나올 블록 종류를 랜덤하게 생성하기
	new_block();				// 새로운 블록 하나 만들기
	//draw_main();				// test...
}

/// <summary>
/// 메인 게임판 초기화 함수 (data)
/// (main_orh[][])
/// </summary>
void reset_main()
{
	int i, j;

	// 게임판을 0으로 초기화
	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			main_org[i][j] = 0;
			main_cpy[i][j] = 0;
		}
	}

	// 천장 (y값이 3인 위치) : Celling
	for (j = 1; j < MAIN_X; j++)
	{
		main_org[3][j] = CELLING;
	}

	// 좌우 벽돌 만들기 : Wall
	for (i = 0; i < MAIN_Y - 1; i++)
	{
		main_org[i][0] = WALL;				// 왼쪽
		main_org[i][MAIN_X - 1] = WALL;		// 오른쪽
	}

	// 바닥 벽돌 만들기 : Wall
	for (j = 0; j < MAIN_X; j++)
	{
		main_org[MAIN_Y - 1][j] = WALL;
	}
}

/// <summary>
/// 복사 게임판 초기화 함수 
/// (main_cpy[][])
/// (main_cpy를 초기화)
/// </summary>
void reset_main_cpy()
{
	int i, j;

	// 게임판에서 게임에 사용하지 않는 숫자를 넣는다. (-1, 999)
	// main_org 과 같은 숫자가 없게 하기 위함이다.
	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			main_cpy[i][j] = 100;
		}
	}
}

/// <summary>
/// 게임 전체 인터페이스 (상태) 를 표시하는 함수 (UI)
/// </summary>
void draw_map()
{
	int y = 3;	// 게임 상태 표시 위치가 바뀌어도, 안바뀌어도 되도록 저장

	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y);		printf(" LEVEL : %5d", level);
	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL  = y + 1);	printf(" GOAL  : %5d", 10 - cnt);

	gotoxy(STATUS_X_ADJ, y + 2);					printf("+-  N E X T  -+");
	gotoxy(STATUS_X_ADJ, y + 3);					printf("|             |");
	gotoxy(STATUS_X_ADJ, y + 4);					printf("|             |");
	gotoxy(STATUS_X_ADJ, y + 5);					printf("|             |");
	gotoxy(STATUS_X_ADJ, y + 6);					printf("|             |");
	gotoxy(STATUS_X_ADJ, y + 7);					printf("+-- -  -  - --+");
	gotoxy(STATUS_X_ADJ, y + 8);					printf(" YOUR SCORE : ");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9);	printf("		%6d", score);
	gotoxy(STATUS_X_ADJ, y + 10);					printf(" LAST SCORE : ");
	gotoxy(STATUS_X_ADJ, y + 11);					printf("		%6d", last_score);
	gotoxy(STATUS_X_ADJ, y + 12);					printf(" BEST SCORE : ");
	gotoxy(STATUS_X_ADJ, y + 13);					printf("		%6d", best_score);
						   
	gotoxy(STATUS_X_ADJ, y + 15);					printf("  △   : Shift          SPACE : Hard Drop");
	gotoxy(STATUS_X_ADJ, y + 16);					printf("◁  ▷ : Left / Right	P   : Pause");
	gotoxy(STATUS_X_ADJ, y + 17);					printf("  ▽   : Soft Drop       ESC  : Quit");
}

/// <summary>
/// 게임판을 그리는 함수 (Main Image)
/// </summary>
void draw_main()
{
	int i, j;

	// 천장은 계속 새로운 블록이 지나가서 지워지면 새로 그려주기
	for (j = 1; j < MAIN_X - 1; j++)
	{
		if (main_org[3][j] == EMPTY)
			main_org[3][j] = CELLING;
	}

	// cpy 와 비교하여 값이 달라진 부분만 새로 그려주기
	// (전체를 다시 모두 그리면 계속 그려져서 느려지고 화면이 껌뻑거린다..)
	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			if (main_org[i][j] != main_cpy[i][j])	// 다를 경우에만..
			{
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);

				switch (main_org[i][j])
				{
				case EMPTY:				// 빈칸 모양
					printf("  ");
					break;
				case CELLING:			// 천장 모양
					printf(". ");
					break;
				case WALL:				// 벽 모양
					printf("▩");
					break;
				case INACTIVE_BLOCK:	// 굳은 블록 모양
					printf("□");
					break;
				case ACTIVE_BLOCK:		// 움직이고 있는 블록 모양
					printf("■");
					break;
				}
			}
		}
	}

	// 게임판을 그린 후 main_cpy 에 복사하기
	for (i = 0; i < MAIN_Y; i++)
	{
		for (j = 0; j < MAIN_X; j++)
		{
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

/// <summary>
/// 새로운 블록 하나를 만드는 함수 (생성)
/// </summary>
void new_block()
{
	int i, j;

	bx = (MAIN_X / 2) - 1;		// 블록 생성 위치 (x 좌표) (게임판의 가운데)
	by = 0;						// 블록 생성 위치 (y 좌표) (게임판의 제일 위)
	b_type = b_type_next;		// 다음 블록 값 가져오기
	b_type_next = rand() % 7;	// 다음 블록 만들기
	b_rotation = 0;				// 회전 값을 0으로 초기화하여 가져오기
	
	new_block_on = 0;			// 새로운 블록 플래그 off

	// 게임판의 bx, by 위치에 블록 생성하기 (block data) (4 * 4)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[b_type][b_rotation][i][j] == 1)
			{
				main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
	}

	// 게임 상태 표시판에 다음에 나올 블록 그리기 (4 * 3)
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[b_type_next][0][i][j] == 1)
			{
				// 예외로 바로 그리기..
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("■");
			}
			else
			{
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

/// <summary>
/// 키보드로 키 입력받는 함수
/// (키 입력 확인)
/// </summary>
void check_key()
{
	// 키 값 초기화
	key = 0;

	// 키 값이 있는 경우
	if (_kbhit())
	{
		// 키 값 받아오기
		key = _getch();

		// 방향 키인 경우
		if (key == 224)
		{
			// 방향 키 지시값 버리기
			do {
				key = _getch();
			} while (key == 224);

			switch (key)
			{
				case LEFT:		// 왼쪽으로 갈 수 있는지 확인 후 가능하다면 이동하기
					if (check_crush(bx - 1, by, b_rotation) == true)
						move_block(LEFT);
					break;

				case RIGHT:		// 오른쪽으로 갈 수 있는지 확인 후 가능하다면 이동하기
					if (check_crush(bx + 1, by, b_rotation) == true)
						move_block(RIGHT);
					break;

				case DOWN:		// 아래쪽으로 갈 수 있는지 확인 후 가능하다면 이동하기
					if (check_crush(bx, by + 1, b_rotation) == true)
						move_block(DOWN);
					break;

				case UP:		// 블록 회전하기 (회전할 수 있는지 확인 후 가능하면 회전하기 or 바닥에 닿은 경우, 예외 처리)
					// 회전할 수 있는지 확인하기! (check_crush()로 확인) (가능하다면 회전하기)
					if (check_crush(bx, by, (b_rotation + 1) % 4) == true)	// check!
						move_block(UP);

					// 예외 처리
					// 바닥에 닿은 경우, 위쪽으로 한칸 띄워서 회전이 가능하다면 회전한다. (특수 동작)
					else if (crush_on == 1 && check_crush(bx, by, (b_rotation + 1) % 4) == true)
						move_block(100);
					break;

				default:
					break;
			}
		}

		// 방향 키가 아닌 경우
		else
		{
			switch (key)
			{
				case SPACE:
					// 스페이스 키 플래그 처리하기
					space_key_on = 1;

					// 바닥에 닿을 때까지 이동시키기
					while (crush_on == 0)
					{
						drop_block();

						// 보너스 점수 추가 처리하기
						score += level;							// 보너스 하드드롭
						gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE);
						printf("         %6d", score);			// 점수 표시 갱신하기
					}
					break;

				case P:
				case p:
					pause();									// 일시 정지...
					break;

				case ESC:
					system("cls");
					exit(0);									// 게임 종료...
					break;
			}
		}
	}

	// 키 버퍼 지우기
	while (_kbhit())
	{
		_getch();
	}
}

/// <summary>
/// 블록을 아래로 떨어트리는 함수
/// </summary>
void drop_block()
{
	int i, j;

	// 블록의 밑이 비어있으면 Crush Flag 끄기
	if (crush_on && check_crush(bx, by + 1, b_rotation) == true)
		crush_on = 0;

	// 블록의 밑이 비어있지 않고, Crush Flag 가 켜져있는 경우
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false)
	{
		// 현재 조작중인 블록 굳히기
		for (i = 0; i < MAIN_Y; i++)
		{
			for (j = 0; j < MAIN_Y; j++)
			{
				if (main_org[i][j] == ACTIVE_BLOCK)
					main_org[i][j] = INACTIVE_BLOCK;
			}
		}

		// Flag 끄기 (초기화)
		crush_on = 0;

		// 라인 확인하기
		check_line();

		// 새로운 블록 생성 Flag 켜기
		new_block_on = 1;

		// 함수 종료
		return;
	}

	// 블록의 밑이 비어있으면 아래로 한 칸 이동하기
	if (check_crush(bx, by + 1, b_rotation) == true)
		move_block(DOWN);

	// 블록의 밑으로 이동이 안되면 Crush Flag 켜기
	if (check_crush(bx, by + 1, b_rotation) == false)
		crush_on++;
}

/// <summary>
/// 충돌 판단 함수 
/// (rotation 시에 위치 보정)
/// (지정된 좌표와 회전값으로 충돌이 있는지 검사한다.)
/// </summary>
/// <param name="bx">블록의 x좌표</param>
/// <param name="by">블록의 y좌표</param>
/// <param name="b_rotation">블록의 회전값</param>
/// <returns>블록의 충돌 여부</returns>
int check_crush(int bx, int by, int b_rotation)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			// 지정된 위치의 게임판(main_org)과 블록 모양(blocks)을 비교하여 겹치면 false 리턴!
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j] > 0)
			{
				return false;
			}
		}
	}

	// 하나도 안겹치면 true 리턴
	return true;
}

/// <summary>
/// dir 방향으로 블록을 움직이는 함수
/// (방향 값으로 블록을 이동시킨다.)
/// </summary>
/// <param name="dir">방향 값</param>
void move_block(int dir)
{
	int i, j;

	switch (dir)
	{
		case LEFT:
			// 현재 블록 지우기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = EMPTY;
					}
				}
			}
			// 왼쪽으로 한칸 이동한 블록 위치에 Active Block을 찍어주기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
					}
				}
			}
			// 좌표값 이동시키기
			bx--;
			break;

		case RIGHT:
			// 현재 블록 지우기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = EMPTY;
					}
				}
			}
			// 오른쪽으로 한칸 이동한 블록 위치에 Active Block을 찍어주기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
					}
				}
			}
			// 좌표값 이동시키기
			bx++;
			break;

		case DOWN:
			// 현재 블록 지우기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = EMPTY;
					}
				}
			}
			// 아래쪽으로 한칸 이동한 블록 위치에 Active Block을 찍어주기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
					}
				}
			}
			// 좌표값 이동시키기
			by++;
			break;

		case UP:
			// 현재 블록 지우기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = EMPTY;
					}
				}
			}
			// 회전 값을 1 증가시키기 (3이 넘어가면 0으로 되돌리기)
			b_rotation = (b_rotation + 1) % 4;
			// 회전한 블록 위치에 Active Block을 찍어주기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = ACTIVE_BLOCK;
					}
				}
			}
			break;

		case 100:
			// 블록이 바닥 혹은 다른 블록과 닿은 상태에서 한칸 위로 올려 회전이 가능한 경우, 특수 동작 허용!
			// 현재 블록 지우기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i][bx + j] = EMPTY;
					}
				}
			}
			// 회전 값을 1 증가시키기 (3이 넘어가면 0으로 되돌리기)
			b_rotation = (b_rotation + 1) % 4;
			// 회전한 블록 위치에 Active Block을 찍어주기 (data)
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (blocks[b_type][b_rotation][i][j] == 1)
					{
						main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
					}
				}
			}
			// 좌표값 이동하기
			by--;
			break;

		default:
			break;
	}
}

/// <summary>
/// 줄이 가득 찼는지 판단하고 지우는 함수
/// </summary>
void check_line()
{
	int i, j, k, l;

	int block_amount;		// 한 줄의 블록 갯수를 저장하는 변수
	int combo = 0;			// 콤보 갯수를 저장하는 변수 (초기화)

	// 보드판 확인하기
	// 밑쪽 벽의 윗칸 (MAIN_Y - 2) 부터 천장 아래 (3) 까지 검사하기
	for (i = MAIN_Y - 2; i > 3; )
	{
		// 블록의 갯수 저장 변수 초기화하기
		block_amount = 0;

		// 벽과 벽 사이의 블록 갯수 세기
		for (j = 1; j < MAIN_X - 1; j++)
		{
			if (main_org[i][j] > 0)
				block_amount++;
		}

		// 블록이 가득 찬 경우
		if (block_amount == MAIN_X - 2)
		{
			// 레벨업 상태가 아닌 경우, (레벨업이 되면 자동 줄 삭제가 이루어진다.)
			if (level_up_on == 0)
			{
				// 점수 추가하기
				score += 100 * level;
				
				// 지운 줄 수의 카운트 증가하기
				cnt++;

				// 콤보 수 증가하기
				combo++;
			}

			// 윗줄을 한칸씩 모두 내리기 (윗줄이 천장이 아닌 경우에만 실행!)
			for (k = i; k > 1; k--)
			{
				for (l = 1; l < MAIN_X - 1; l++)
				{
					if (main_org[k - 1][l] != CELLING)
						main_org[k][l] = main_org[k - 1][l];

					if (main_org[k - 1][l] == CELLING)
						main_org[k][l] = EMPTY;
				}
			}
		}

		// 블록이 가득 차지 않으면 다음 줄로 넘어가기
		else
		{
			i--;
		}
	}

	// 줄 삭제가 있는 경우, 점수와 레벨 목표 새로 표시하기
	if (combo)
	{
		// 2콤보 이상인 경우, 보너스 및 메시지를 게임판에 띄웠다가 지우기
		if (combo > 1)
		{
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2);
			printf("%d COMBO!", combo);
			Sleep(500);

			score += (combo * level * 100);
			reset_main_cpy();					// 텍스트를 지우기 위해 main_cpy 를 초기화한다.
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL);
		printf(" GOAL : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE);
		printf("        %6d", score);
	}
}

/// <summary>
/// 레벨 목표를 달성했는지 판단하고 레벨업을 시키는 함수
/// </summary>
void check_level_up()
{
	int i, j;

	// 레벨업을 위한 줄 수 (테스트 1줄)
	// int levelup_max = 1;		// test...
	int levelup_max = 10;

	// 레벨별로 10줄씩 업애야 레벨업 가능 -> 10줄 이상 없앤 경우
	if (cnt >= levelup_max)
	{
		// 게임판을 한번 그리기 (refresh)
		draw_main();

		// 레벨업 플래그 띄우기
		level_up_on = 1;

		// 레벨 1 올리기
		level += 1;

		// 지운 줄 수 초기화하기
		cnt = 0;

		// (효과) 레벨업 글자 효과 찍기
		for (i = 0; i < 4; i++)
		{
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);	printf("               ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 6);	printf("               ");
			Sleep(200);

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);	printf("☆ LEVEL UP! ☆");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 6);	printf("★ LEVEL UP! ★");
			Sleep(200);
		}

		// 텍스트를 모두 지우기 위해 초기화하기
		reset_main_cpy();

		// 레벨업 보상 (레벨만큼 아래줄을 지운다.) -> 커스텀 가능!
		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--)
		{
			for (j = 1; j < MAIN_X - 1; j++)
			{
				// 줄을 블록으로 모두 채우기
				main_org[i][j] = INACTIVE_BLOCK;

				// 별을 찍어준다. (보상 효과)
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				printf("★");
				Sleep(20);
			}
		}

		// 딜레이 (꾸미기 = 별을 보여주기 위해 잠시 딜레이)
		Sleep(100);

		// 블록으로 채운 것 지우기
		check_line();

		// (밸런스) 레벨별로 속도 조절하기
		switch (level)
		{
			case 2:		speed = 50;		break;
			case 3:		speed = 25;		break;
			case 4:		speed = 10;		break;
			case 5:		speed = 5;		break;
			case 6:		speed = 4;		break;
			case 7:		speed = 3;		break;
			case 8:		speed = 2;		break;
			case 9:		speed = 1;		break;
			case 10:	speed = 0;		break;
		}
		// speed = 50;		// test speed...

		// 레벨업 플래그 초기화하기 (꺼준다.)
		level_up_on = 0;

		// UI ------------------------------
		
		// 레벨 표시하기
		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL);
		printf(" LEVEL : %5d", level);

		// 다음 레벨 목표 표시하기
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL);
		printf(" GOAL  : %5d", levelup_max - cnt);
		// printf(" GOAL  : %5d", 10 - cnt);		// test...
	}
}

/// <summary>
/// 게임 오버인지 판단하고 게임 오버 시키는 함수
/// </summary>
void check_game_over()
{
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++)
	{
		// 천장에서 (위에서부터 세번째 줄) INACTIVE 블록이 생성되면 게임 오버!
		if (main_org[3][i] > 0)
		{
			// 게임 오버 메시지 출력하기
			gotoxy(x, y + 0);	printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			gotoxy(x, y + 1);	printf("▤                                    ▤");
			gotoxy(x, y + 2);	printf("▤   +----------------------------+   ▤");
			gotoxy(x, y + 3);	printf("▤   |      G A M E   O V E R     |   ▤");
			gotoxy(x, y + 4);	printf("▤   +----------------------------+   ▤");
			gotoxy(x, y + 5);	printf("▤          YOUR SCORE : %6d          ▤", score);
			gotoxy(x, y + 6);	printf("▤                                    ▤");
			gotoxy(x, y + 7);	printf("▤     Press Any Key to Restart...    ▤");
			gotoxy(x, y + 8);	printf("▤                                    ▤");
			gotoxy(x, y + 9);	printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

			// 게임 점수 저장하기!
			last_score = score;

			// 최고 기록 갱신 시 처리하기
			if (score > best_score)
			{
				FILE* file = fopen("score.dat", "wt");	// score.dat 파일 연결

				gotoxy(x, y + 6);	printf("▤      ☆★  BEST SCORE!  ★☆       ▤");

				if (file == 0)
				{
					gotoxy(0, 0);
					printf("FILE ERROR! \"SCORE.DAT\"");
				}
				else
				{
					fprintf(file, "%d", score);
					fclose(file);
				}
			}

			// 딜레이
			Sleep(1000);

			// 키보드 입력하기 (대기 & 초기화)
			while (_kbhit())
			{
				_getch();
			}
			key = _getch();

			// 초기화하기
			reset();
		}
	}
}

/// <summary>
/// 게임 일시 정지 시키는 함수
/// </summary>
void pause()
{
	int i, j;

	int x = 5;
	int y = 5;

	gotoxy(x, y + 0);	printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(x, y + 1);	printf("▤                                    ▤");
	gotoxy(x, y + 2);	printf("▤   +----------------------------+   ▤");
	gotoxy(x, y + 3);	printf("▤   |          P A U S E         |   ▤");
	gotoxy(x, y + 4);	printf("▤   +----------------------------+   ▤");
	gotoxy(x, y + 5);	printf("▤                                    ▤");
	gotoxy(x, y + 6);	printf("▤     Press Any Key to Resume...     ▤");
	gotoxy(x, y + 7);	printf("▤                                    ▤");
	gotoxy(x, y + 8);	printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	// 키 입력까지 대기하기
	_getch();

	// 화면을 지우고 다시 그리기
	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	// 다음 블록 다시 그리기 (-> 블록만 그리기)
	// (게임 상태 표시판에 다음에 나올 블록 그리기 (4 * 3))
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (blocks[b_type_next][0][i][j] == 1)
			{
				// 예외로 바로 그리기..
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("■");
			}
			else
			{
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}

}

/// <summary>
/// 커서 이동 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/// <summary>
/// 커서 숨기는 함수
/// </summary>
/// <param name="c"></param>
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
