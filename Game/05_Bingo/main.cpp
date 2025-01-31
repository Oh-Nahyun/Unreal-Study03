
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

void Init(int*)
{

}

void PrintBingoBoard(int*, int)
{

}

void InputNum(int*)
{

}

int CheckBingo(int*);
{

}
