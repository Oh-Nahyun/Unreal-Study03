
/// [ 컨셉 ] 동작 정의 ==============================

/// 1. 보스 출력 (함수)
/// 2. 플레이어 출력 (함수)
/// 3. 플레이어 이동 처리 (상하좌우)
/// 4. 플레이어 총알 처리 (스페이스 키 발사, 총알 갯수 한정 : 3개)
/// 5. 보스 이동 처리 (AI : 간단한 패턴 처리)
/// 6. 보스 총알 발사 (1초당 한 발씩)

/// 게임 화면 (씬) ==============================

/// 1. 게임 메뉴
/// 2. 게임 설명
/// 3. 게임 플레이
/// 4. 게임 종료

/// 게임이 아닌, 프로토타입!! (조작 및 게임의 형태만 구축할 예정)

/// ==================================================

#include <stdio.h>		// 기본 출력 함수 (C Base)
#include <Windows.h>	// 커서 관련 (콘솔)
#include <time.h>		// 시간
#include <stdlib.h>		// 랜덤 etc...
#include <conio.h>		// (콘솔의) 출력

// 데이터 정의 ------------------------------

// 좌표 (디파인)
#define BOSS_X 12		// 보스의 기본 좌표
#define BOSS_Y 7
#define USER_X 14		// 유저의 기본 좌표
#define USER_Y 40

// 총알
typedef struct bullet
{
	int x, y;
};

// 총알 총 수량 (화면에 출력되는 최대 숫자)
struct
{
	BOOL exist;
	int x, y;
} Bullet[3];

// 플레이어 데이터
typedef struct user_stat
{
	int x, y;			// 위치
	int life;			// 생명
	int chk;			// 총알의 발사 여부
	bullet bullet;		// 각각의 총알
} user_stat;

// 보스 데이터
typedef struct boss_stat
{
	int x, y;			// 위치
	int life;			// 생명
	int chk;			// 총알의 발사 여부
	bullet bullet;		// 각각의 총알
} boss_stat;

// 적 데이터
// ...

// 객체(플레이어, 보스) 데이터 선언
struct user_stat User;
struct boss_stat Boss;

// 함수 정의 ------------------------------

void gotoxy(int x, int y);
void cursor(int n);

void boss(int x, int y);
void bossDelete(int x, int y);
void player(int x, int y);
void playerDelete(int x, int y);

void bulletMove();
void DrawBullet(int i);
void EraseBullet(int i);

void menu();
void game();
void help();

/*
menu() = UI 처리, 게임 로비
{
	game();		// 플레이 함수
	help();		// 도움말
	exit();		// 종료
}

game()
{
	// 보스 출력, 랜덤 좌우 이동
	// 플레이어 출력, 조작 이동 (상하좌우), 공격 (space key)
	// 총알 발사 처리
	// 총알 이동 처리
}
*/

// Main 함수 구현 ------------------------------

int main()
{
	// 시스템 처리
	// cursor(true);	// (커서 처리 : 보이도록)
	cursor(false);		// (커서 처리 : 안보이도록)

	// 랜덤 초기화
	srand((unsigned)time(NULL));

	// 메뉴 (게임 플레이 본체) & 화면 초기화
	menu();

	// 게임 종료 (test:일시 정지)
	system("pause");

	return 0;
}

// 함수 구현 ------------------------------

/// <summary>
/// 커서 이동 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

/// <summary>
/// 커서를 숨기거나 보이도록 처리하는 함수
/// </summary>
/// <param name="n">커서 보이기 = 1 (true), 커서 숨기기 = 0 (false)</param>
void cursor(int n)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;		// 숫자가 커질 수록 커서가 커진다.

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

/// <summary>
/// 보스 출력 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void boss(int x, int y)
{
	gotoxy(x	, y);		printf("■■      ■■");
	gotoxy(x + 4, y + 1);	printf("■■■");
	gotoxy(x + 2, y + 2);	printf("■■■■■");
	gotoxy(x	, y + 3);	printf("■  ■■■  ■");
	gotoxy(x - 2, y + 4);	printf("■■■■■■■■■");
	gotoxy(x	, y + 5);	printf("■■      ■■");
	gotoxy(x + 2, y + 6);	printf("■■  ■■");
}

/// <summary>
/// 보스 지우는 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void bossDelete(int x, int y)
{

	gotoxy(x	, y);		printf("              ");
	gotoxy(x + 4, y + 1);	printf("      ");
	gotoxy(x + 2, y + 2);	printf("          ");
	gotoxy(x	, y + 3);	printf("              ");
	gotoxy(x - 2, y + 4);	printf("                  ");
	gotoxy(x	, y + 5);	printf("              ");
	gotoxy(x + 2, y + 6);	printf("          ");
}

/// <summary>
/// 플레이어 출력 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void player(int x, int y)
{
	gotoxy(x	, y);		printf("▲");
	gotoxy(x - 2, y + 1);	printf("▲■▲");
	gotoxy(x - 2, y + 2);	printf("◎  ◎");
}

/// <summary>
/// 플레이어 지우는 함수
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void playerDelete(int x, int y)
{
	gotoxy(x	, y);		printf("  ");
	gotoxy(x - 2, y + 1);	printf("      ");
	gotoxy(x - 2, y + 2);	printf("      ");
}

/// <summary>
/// 유저의 총알 이동 함수
/// </summary>
void bulletMove()
{
	// 총알 이동
	for (int i = 0; i < 3; i++)
	{
		// 총알 활성화 확인
		if (Bullet[i].exist)	// Bullet[i].exist == true
		{
			// 총알 지우기
			EraseBullet(i);

			// 총알 범위 계산 (화면을 벗어났는지 체크)
			if (Bullet[i].y <= 0)
			{
				Bullet[i].exist = false;
			}
			else
			{
				// 총알 출력 (위치 변경 포함)
				Bullet[i].y--;
				DrawBullet(i);
			}
		}
	}
}

/// <summary>
/// 유저의 총알 출력 함수
/// </summary>
/// <param name="i">총알 인덱스</param>
void DrawBullet(int i)
{
	gotoxy(Bullet[i].x, Bullet[i].y);
	printf("↑");
}

/// <summary>
/// 유저의 총알 지우는 함수
/// </summary>
/// <param name="i">총알 인덱스</param>
void EraseBullet(int i)
{
	gotoxy(Bullet[i].x, Bullet[i].y);
	printf("  ");
}

/// <summary>
/// 메뉴 함수
/// (게임 메뉴 선택 : 로비 (1. 게임시작 / 2. 게임방법 / 3. 게임종료))
/// </summary>
void menu()
{
	// 메뉴 선택 번호
	int select = 0;

	// 화면 비율 변경
	system("mode CON COLS=60 LINES=50");	// Windows11은 다르다.
	system("color 1e");						// 첫자리 : 배경색, 두번째 : 글자색 (0 ~ f)

	// 메뉴 반복 부분
	while (true)
	{
		// 메뉴 출력
		system("cls");
		gotoxy(16, 7);	printf("******************************");
		gotoxy(16, 8);	printf("*                            *");
		gotoxy(16, 9);	printf("*       1. 게임 시작         *");
		gotoxy(16, 10);	printf("*                            *");
		gotoxy(16, 11);	printf("*       2. 게임 방법         *");
		gotoxy(16, 12);	printf("*                            *");
		gotoxy(16, 13);	printf("*       3. 게임 종료         *");
		gotoxy(16, 14);	printf("*                            *");
		gotoxy(16, 15);	printf("******************************");

		gotoxy(22, 35);	printf("번호를 입력하세요. : ");
		scanf_s("%d", &select);

		// 메뉴 선택
		if (select == 1)
		{
			gotoxy(20, 37);
			printf("게임을 시작하겠습니다.");
			Sleep(1000);

			game();
		}
		else if (select == 2)
		{
			help();
		}
		else
		{
			gotoxy(20, 37);
			printf("게임을 종료하겠습니다.");
			Sleep(1000);
			gotoxy(15, 39);		// pause 위치..

			break;
		}
	}
}

/// <summary>
/// 게임 실행 부분 (게임 매니저) 함수
/// </summary>
void game()
{
	// 화면 초기화
	system("cls");

	// 데이터 초기화
	User.x = USER_X;
	User.y = USER_Y;
	User.chk = 0;

	Boss.x = BOSS_X;
	Boss.y = BOSS_Y;
	Boss.chk = 0;			// 보스는 화면에 1발만 처리 (총알이 화면을 벗어나면 발사)
	Boss.bullet.y = 0;

	// 시간 기록용 변수
	// (((타이머 (스레드) : 컴퓨터마다 1초의 시간이 달라지면 안되기 때문)))
	unsigned int prev_time = 0;

	// 보스 이동 플래그 (좌우 이동)
	int b_move = 0;

	// 총알 인덱스
	int i = 1;

	// 게임 반복 구간
	while (true)
	{
		// 게임 종료 (ESC Key)
		if (GetAsyncKeyState(VK_ESCAPE) & 0x01) break;

		// 보스, 플레이어 출력
		boss(Boss.x * 2, Boss.y);
		player(User.x * 2, User.y);

		// 총알 처리 & 출력
		bulletMove();

		// 입력 처리 (플레이어 이동 처리 : 상하좌우)
		if ((GetAsyncKeyState(VK_UP) & 0x0001) && (2 < User.y))		// 위로 이동 + 화면 이동 제약
		{
			playerDelete(User.x * 2, User.y);

			// 좌표 이동
			User.y--;
			player(User.x * 2, User.y);
		}
		if ((GetAsyncKeyState(VK_DOWN) & 0x0001) && (46 > User.y))	// 아래로 이동 + 화면 이동 제약
		{
			playerDelete(User.x * 2, User.y);

			// 좌표 이동
			User.y++;
			player(User.x * 2, User.y);
		}
		if ((GetAsyncKeyState(VK_LEFT) & 0x0001) && (2 < User.x))	// 왼쪽으로 이동 + 화면 이동 제약
		{
			playerDelete(User.x * 2, User.y);

			// 좌표 이동
			User.x--;
			player(User.x * 2, User.y);
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x0001) && (27 > User.x))	// 오른쪽으로 이동 + 화면 이동 제약
		{
			playerDelete(User.x * 2, User.y);

			// 좌표 이동
			User.x++;
			player(User.x * 2, User.y);
		}

		// 유저 총알 쏘는 부분 (Space Key)
		if (GetAsyncKeyState(VK_SPACE) & 0x0001)					// 스페이스 키가 눌린 경우
		{
			// 3개의 총알이 발사 중인지 체크
			//for (i = 0; i < 3; i++)
			//{
			//	if (Bullet[i].exist == false) break;
			//}
			for (i = 0; i < 3 && Bullet[i].exist; i++)
			{
				break;
			}

			// 3개 중 남은 배열 인덱스가 있을 때만 총알 발사
			if (i != 3)
			{
				Bullet[i].x = User.x * 2;
				Bullet[i].y = User.y - 2;
				Bullet[i].exist = true;
			}
		}

		// 보스 처리 (1초마다 처리 (NPC))
		if (time(NULL) - prev_time >= 1)	// 1초마다 실행된다.
		{
			prev_time = time(NULL);

			// 1초 테스트
			gotoxy(14, 1);
			printf("1970/01/01 이후 초 : %d", prev_time);

			// 보스의 이동 방향 선정 가능 (2(+1)가지 패턴 : 1. 좌로 이동 / 2. 우로 이동 / (3. 총알 발사 = 1초 마다(화면에 총알이 없으면 발사)))
			// (((State Machine)))
			b_move = rand() % 2;			// 0이나 1만 나온다. (좌우 선택)

			// 보스 이동 처리 (잔상 지우기 & 외각 영역 처리)
			if (!b_move && Boss.x > 2)		// 왼쪽으로 이동
			{
				bossDelete(Boss.x * 2, Boss.y);
				Boss.x -= 2;
				boss(Boss.x * 2, Boss.y);
			}
			if (b_move && Boss.x < 22)		// 오른쪽으로 이동
			{
				bossDelete(Boss.x * 2, Boss.y);
				Boss.x += 2;
				boss(Boss.x * 2, Boss.y);
			}

			// 보스의 총알 처리
			if (!Boss.chk)					// 총알이 발사되지 않은 경우 -> 총알 발사!
			{
				// 총알의 위치 초기화 (보스의 바로 아래)
				Boss.bullet.x = Boss.x * 2 + (3 * 2);
				Boss.bullet.y = BOSS_Y + 7;

				// 총알 출력
				gotoxy(Boss.bullet.x, Boss.bullet.y);
				printf("▽");

				// 총알 발사 처리
				Boss.chk = true;
			}
			else if (Boss.chk)				// 총알이 발사된 경우
			{
				if (Boss.bullet.y > 46)		// 총알이 화면을 벗어난 경우
				{
					// 총알 지우기
					gotoxy(Boss.bullet.x, Boss.bullet.y);
					printf("  ");

					// 총알 발사 초기화
					Boss.chk = false;
				}
				else                        // 총알이 계속 아래로 하강
				{
					// 총알 지우기
					gotoxy(Boss.bullet.x, Boss.bullet.y);
					printf("  ");

					// 위치 증가
					Boss.bullet.y += 3;

					// 총알 출력
					gotoxy(Boss.bullet.x, Boss.bullet.y);
					printf("▽");
				}
			}
		}

		// 게임 딜레이 (전체적인 게임 속도)
		Sleep(10);
	}
}

/// <summary>
/// 게임 설명 (도움말) 함수
/// </summary>
void help()
{
	system("cls");

	// 보스 출력
	boss(BOSS_X * 2, BOSS_Y);

	// 유저 출력
	player(USER_X * 2, USER_Y);

	// 설명 출력
	gotoxy(19, 17);	printf("위와 같은 보스가 나타나면");
	gotoxy(7, 19);	printf("↑,↓,←,→ 방향키를 이용하여 적의 공격을 피하고");
	gotoxy(11, 21);	printf("스페이스 키를 이용하여 적을 공격하세요.");

	gotoxy(13, 25);	system("pause");
}
