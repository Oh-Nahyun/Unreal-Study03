
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
    /////// 빙고수 체크 = 카운트 (빙고수)!
    count = CheckBingo(bingo);

    // 화면에 빙고판 그려주기
    PrintBingoBoard(bingo, count);

    // 빙고판 출력 테스트 (일시 정지)
    //system("pause");

    // 승패 확인하기

    // 인풋 (입력) 처리하기
    InputNum(bingo);
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

  /////// 섞어주기 (100번)
  for (i = 0; i < 100; i++)
  {
    int sour = rand() % 25;  // 임의의 인덱스 값
    int dest = rand() % 25;  // 임의의 인덱스 값 (같아도 상관없음)

    int temp = bingo[sour];
    bingo[sour] = bingo[dest];
    bingo[dest] = temp;
  }
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
      // 선택된 숫자를 #으로 표현하기 위해 조건 처리하기
      if (bingo[(i * 5) + j] == 35)
      {
        printf("%c #", 5); // 왼쪽 라인 + # 출력
      }
      else
      {
        printf("%c%2d", 5, bingo[(i * 5) + j]);  // 해당 숫자 출력 /////// bingo[y][x] == bingo[y * x] (이런 방식으로 배열을 많이 사용한다!!!)
      }
    }
    printf("%c\n", 5);                         // 맨끝 오른쪽 세로 라인
    
    if (i != 4)                                // 맨 마지막 출력을 안하도록 처리 (중간 라인 (16개))
      printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 25, 6, 6, 16, 6, 6, 16, 6, 6, 16, 6, 6, 16, 6, 6, 23);
  }

  // 3. 최하단 라인 (16개)
  printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", 3, 6, 6, 21, 6, 6, 21, 6, 6, 21, 6, 6, 21, 6, 6, 4);

  // 승리 판단하기
  printf("\t%d 줄 빙고!\n\n", count);
}

// 숫자 처리 함수
// 1. 새로운 수가 이미 체크되었는지 확인 기능
// 2. 체크가 안되어있으면 체크해주는 기능
// 3. 정상적인 숫자가 들어오면 저장 기능
void InputNum(int* bingo)
{
  // 입력 저장 변수
  int num;

  // 이미 입력되어 체크되어있는지 확인 변수 (플래그)
  int isCheck;

  // 정상적인 수(새롭고 & 체크 안된)가 입력될 때까지 반복
  while (true)
  {
    // 초기화
    isCheck = 0;

    // 화면 출력
    printf(" 입력 : ");
    scanf_s("%d", &num);

    // 입력받은 수의 범위 체크 (1 ~ 25)
    if (num < 1 || num > 25)
    {
      printf(" 1 ~ 25 사이의 숫자를 입력해주세요.\n");
    }
    else // 정상적인 수가 들어온 경우
    {
      // 빙고 배열을 25개 모두 검사해서 같은 수가 저장되어있다면...
      for (int i = 0; i < 25; i++)
      {
        // 입력받은 수가 있다면...
        if (bingo[i] == num)
        {
          bingo[i] = 35;      // # = ASCII 32 // bingo[i] = '#';
          isCheck = 1;
        }
      }

      // 체크되어있다면...
      if (isCheck) break;
      // 체크가 안되어있다면...
      else printf(" 이미 입력한 숫자입니다.\n");
    }
  }
}

// 매개 변수로 넘어온 배열 (빙고판)을 체크하여 빙고수를 리턴해주는 함수 !!!
int CheckBingo(int* bingo);
{
  // 최대 12개의 빙고 체크 !!
  // (가로 5개, 세로 5개, 대각선 2개 = 12개)
  int bingoCheck[12] = { 0 };
  int count = 0;                // 빙고 줄의 수
  int i, j;

  // 가로 5줄 확인
  // 세로 5줄 확인
  // 대각선 : 왼쪽 위에서 오른쪽 아래로 1줄 확인
  // 대각선 : 왼쪽 아래에서 오른쪽 위로 1줄 확인
  // 확인한 빙고수를 센 후 출력
  
  for (int i = 0; i < 25; i++)
  {
    
  }

  return count;  // 빙고수 반환
}
