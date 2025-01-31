
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void BingoGameStart();             // Bingo 게임 함수 = play();

/// 컴파일러를 위한 선언... (개인 개발 시 사용)
void Init(int*);                   // 초기화
void PrintBingoBoard(int*, int);   // Bingo 판 그리기
void InputNum(int*);               // 입력 처리
int CheckBingo(int*);              // Bingo 게임 결과 처리 부분 (!!)

/// 사람을 위한 선언... (동료와 함께 개발 시 사용 (추천!!!))
//void Init(int* bingo);
//void PrintBingoBoard(int* bingo, int count);
//void InputNum(int* bingo);
//int CheckBingo(int* bingo);

int main()
{
  // 화면 사이즈 조정
  system("mode CON COLS=50 LINES=25");
  system("color 3e");

  // 메뉴 입력 변수
  int menu = 0;

  // 반복문
  while (true)
  {
    // 메뉴 출력
    printf("\n\n");
    printf(" 1. 빙고게임 시작\n\n");
    printf(" 2. 프로그램 종료\n\n");
    scanf_s("%d", &menu);

    // 메뉴 선택
    switch (menu)
    {
      case 1:
        BingoGameStart();
        break;
      default:
        return 0;
    }

    // 게임 종료
    system("pause");
    system("cls");
  }
  
  return 0;
}

/// -----------------------------

void BingoGameStart()
{
  int gingo[25];  // 유저의 빙고판
  int count = 0;  // 유저의 빙고수

  Init(bingo);    // 배열 초기화

  // 게임 반복 부분
  while (true)
  {
    // 빙고수 체크 = 카운트 (빙고수)!

    // 화면에 빙고판 그려주기
    PrintBingoBoard(bingo, count);

    // 빙고판 출력 테스트 (일시 정지)
    system("pause");

    // 승패 확인하기

    // 인풋 (입력) 처리하기
  }
}

void Init(int*)
{
  int i;

  // 랜덤 초기화
  srand((unsigned int)time(NULL));

  // 기본 값 저장 (test 확인 완료)
  for (i = 0; i < 25; i++)
  {
    bingo[i] = i + 1;
  }

  // 섞어주기 (100번)
  
}

void PrintBingoBoard(int* bingo, int count)
{
  // 화면 클리어
  system("cls");

  // 선 그리기
  // 1. 최상단 라인 (16개)
  printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 1, 6, 6, 22, 6, 6, 22, 6, 6, 22, 6, 6, 22, 6, 6, 2);

  // 2. 중간 라인
  for (int i = 0; i < 5; i++)    // y
  {
    for (int j = 0; j < 5; j++)  // x
    {
      // printf("%c #", 5);                    // 왼쪽 라인 test
      printf("%c%2d", 5, bingo[(i * 5) + j]);  // bingo[y][x] == bingo[y * x] (이런 방식으로 배열을 많이 사용한다!!!)
    }
    printf("%c\n", 5);                         // 맨끝 오른쪽 세로 라인
    
    if (i != 4)                                // 맨 마지막 출력을 안하도록 처리 (중간 라인 (16개))
      printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 25, 6, 6, 16, 6, 6, 16, 6, 6, 16, 6, 6, 16, 6, 6, 23);
  }

  // 3. 최하단 라인 (16개)
  printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 3, 6, 6, 21, 6, 6, 21, 6, 6, 21, 6, 6, 21, 6, 6, 4);

  // 승리 판단하기
}

void InputNum(int*)
{

}

int CheckBingo(int*);
{

}
