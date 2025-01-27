
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void BingoGameStart();             // Bingo 게임 함수 = play();

// 컴파일러를 위한 선언...
void Init(int*);                   // 초기화
void PrintBingoBoard(int*, int);   // Bingo 판 그리기
void InputNum(int*);               // 입력 처리
int CheckBingo(int*);              // Bingo 게임 결과 처리 부분 (!!)

// 사람(동료)을 위한 선언...
//void Init(int* bingo);
//void PrintBingoBoard(int* bingo, int count);
//void InputNum(int* bingo);
//int CheckBingo(int* bingo);
