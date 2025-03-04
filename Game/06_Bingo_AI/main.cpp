
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>               // Sleep 함수 사용

void BingoGameStart(int*);         // Bingo 게임 함수 = play();     // 전적 추가!

/// 컴파일러를 위한 선언... (개인 개발 시 사용)
void Init(int*);                   // 초기화
void PrintBingoBoard(int*);        // Bingo 판 그리기
void PrintBingoBoard2(int*);       // Bingo 판 (AI용) 그리기
void InputNum(int*, int*);         // 입력 처리 (입력값 추가 : AI 의 빙고)
int CheckBingo(int*);              // Bingo 게임 결과 처리 부분 (!!)

void PrintRecord(int*);            // 전적(기록) 출력 함수
void RandNum(int*, int*);          // 랜덤 숫자 지정 함수           // (AI, NPC, COM) = AI 착수 개선 가능!

/// 사람을 위한 선언... (동료와 함께 개발 시 사용 (추천!!!))
//void Init(int* bingo);
//void PrintBingoBoard(int* bingo, int count);
//void InputNum(int* bingo);
//int CheckBingo(int* bingo);

int main()
{
    // 화면 사이즈 조정
    system("mode CON COLS=50 LINES=30");
    system("color 3e");

    // 메뉴 입력 변수
    int menu = 0;
    int record[3] = { 0 };  // 전적을 보관하는 배열 (0: 전적, 1: 승리, 2: 패배)

    // 반복문
    while (true)
    {
        // 메뉴 출력
        printf("\n\n");
        printf(" 1. 빙고게임 시작\n\n");
        printf(" 2. 게임전적 확인\n\n");
        printf(" 3. 프로그램 종료\n\n");
        scanf_s("%d", &menu);

        // 메뉴 선택
        switch (menu)
        {
        case 1:
            BingoGameStart(record);
            break;
        case 2:
            PrintRecord(record);
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

void BingoGameStart(int* record)
{
    int UserBingo[25];      // 유저의 빙고판
    int UserCount = 0;      // 유저의 빙고수

    int ComBingo[25];       // 컴퓨터의 빙고판
    int ComCount = 0;       // 컴퓨터의 빙고수

    int isTurn = 0;         // 현재 유저의 턴인지, 컴퓨터의 턴인지 구분 (1이면 유저의 턴이다.)

    // 여기에서 한번만 처리! (랜덤 초기화)
    srand((unsigned int)time(NULL));

    Init(UserBingo);        // 배열 초기화 (유저)
    Init(ComBingo);         // 배열 초기화 (컴퓨터)

    // 게임 반복 부분
    while (true)
    {
        /////// 빙고수 체크 = 카운트 (빙고수)!
        UserCount = CheckBingo(UserBingo);      // 유저의 빙고 카운트
        ComCount = CheckBingo(ComBingo);        // 컴퓨터의 빙고 카운트

        // 화면에 빙고판 그려주기
        PrintBingoBoard(UserBingo);             // 유저의 빙고판 그려주기
        PrintBingoBoard2(ComBingo);             // 컴퓨터의 빙고판 그려주기 (개발 버전)

        // 빙고판 출력 테스트 (일시 정지)
        //system("pause");

        // 승패 판단 & 출력 & 판정하기 ------------------------------

        // 각각의 빙고수 출력하기
        printf("\t사용자 : %d 줄 빙고\n\n", UserCount);
        printf("\t컴퓨터 : %d 줄 빙고\n\n", ComCount);

        // 승패 판정 & 출력
        // 1-1. 유저의 빙고수가 5를 넘고, 현재 유저의 턴인 경우 (유저의 턴 발생 상황)
        // 1-2. 유저의 빙고수가 5를 넘고, 컴퓨터의 빙고수가 5를 넘지 못한 경우 (컴퓨터의 턴 발생 상황)
        if ((UserCount >= 5 && isTurn) || (UserCount >= 5 && ComCount < 5))
        {
            printf("\t플레이어의 %d 줄 빙고로 승리하였습니다!\n\n", UserCount); // 유저의 승리!

            record[0]++;                // 전적 증가
            record[1]++;                // 승리 증가

            printf(" 게임 종료!\n");    // 게임 종료
            break;
        }
        // 2-1. 컴퓨터의 빙고수가 5를 넘으면 패배
        else if (ComCount >= 5)
        {
            printf("\t컴퓨터의 %d 줄 빙고로 패배하였습니다!\n\n", ComCount); // 유저의 패배!

            record[0]++;                // 전적 증가
            record[2]++;                // 패배 증가

            printf(" 게임 종료!\n");    // 게임 종료
            break;
        }

        // 턴 변경 (전환)
        isTurn = !isTurn;

        // 인풋 (입력) 처리하기
        // (변수가 두개인 이유는 입력된 숫자와 같은 숫자가 있는 경우, 두 빙고판을 체크하기 위함.)
        if (isTurn)
            InputNum(UserBingo, ComBingo);      // 유저가 입력
        else
            RandNum(UserBingo, ComBingo);       // 컴퓨터가 입력
    }
}

void Init(int* bingo)
{
    int i;

    // 랜덤 초기화 (함수로 따로 빼냄)
    //srand((unsigned int)time(NULL));

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

void PrintBingoBoard(int* bingo)
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
    //if (count >= 5)
    //    printf("\t%d 줄 빙고로 승리 하였습니다.\n\n", count);
    //else
    //    printf("\t%d 줄 빙고!\n\n", count);
}

/// <summary>
/// 컴퓨터의 보드판을 화면에 출력해주는 함수
/// </summary>
/// <param name="bingo"></param>
void PrintBingoBoard2(int* bingo)
{
    // 화면 설명
    printf(" ▼ 컴퓨터 디버깅 ▼ \n");

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
}

// 숫자 처리 함수
// 1. 새로운 수가 이미 체크되었는지 확인 기능
// 2. 체크가 안되어있으면 체크해주는 기능
// 3. 정상적인 숫자가 들어오면 저장 기능
void InputNum(int* UserBingo, int* ComBingo)
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
                if (UserBingo[i] == num)
                {
                    UserBingo[i] = 35;      // # = ASCII 32 // bingo[i] = '#';
                    isCheck = 1;
                }

                // 컴퓨터도 동일하게 처리하기
                if (ComBingo[i] == num)
                {
                    ComBingo[i] = 35;
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
int CheckBingo(int* bingo)
{
    // 최대 12개의 빙고 체크 !!
    // (가로 5개, 세로 5개, 대각선 2개 = 12개)
    int bingoCheck[12] = { 0 };
    int count = 0;                // 빙고 줄의 수
    int i, j;

    // 가로 5줄 확인
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (bingo[(i * 5) + j] == 35)
                bingoCheck[i]++;
        }
    }

    // 세로 5줄 확인
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (bingo[(j * 5) + i] == 35)
                bingoCheck[i + 5]++;
        }
    }

    // 대각선 : 왼쪽 위에서 오른쪽 아래로 1줄 확인
    for (int i = 0; i < 5; i++)
    {
        for (int j = i; j <= i; j++)
        {
            if (bingo[(i * 5) + j] == 35)
                bingoCheck[10]++;
        }
    }

    // 대각선 : 왼쪽 아래에서 오른쪽 위로 1줄 확인
    for (int i = 4; i >= 0; i--)
    {
        for (int j = 4 - i; j >= 4 - i; j--)
        {
            if (bingo[(i * 5) + j] == 35)
                bingoCheck[11]++;
        }
    }

    // 확인한 빙고수를 센 후 출력
    for (int i = 0; i < 12; i++)
    {
        if (bingoCheck[i] == 5)
            count++;
    }

    return count;  // 빙고수 반환
}

/// <summary>
/// 전적 출력 함수
/// (전적이 있으면 출력, 없으면 없다고 출력)
/// </summary>
/// <param name=""></param>
void PrintRecord(int* record)
{
    // 전적이 있는 경우
    if (record[0])
    {
        printf("========== 현재 전적 ==========\n");
        printf("=       %d전  %d승  %d패      =\n", record[0], record[1], record[2]);
        printf("===============================\n");
    }
    else
    {
        printf("========== 현재 전적 ==========\n");
        printf("=       전적이 없습니다.      =\n");
        printf("===============================\n");
    }
}

/// <summary>
/// 컴퓨터의 숫자 입력 함수
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void RandNum(int* UserBingo, int* ComBingo)
{
    // 입력 저장 변수
    int num;

    // 이미 입력되어 체크되어있는지 확인 변수 (플래그)
    int isCheck;

    // 화면 출력
    printf("컴퓨터의 차례입니다.\n");
    Sleep(2000);

    // 정상적인 수(새롭고 & 체크 안된)가 입력될 때까지 반복
    while (true)
    {
        // 초기화
        isCheck = 0;

        ////////// AI 처리 (기본 버전) ------------------------------

        // 우선 순위...
        // 1.   점수 득점(빙고)이 가능한 자리
        // 2.   점수 득점에 유리한 자리
        // 2.1. 정가운데!
        // 2.2. (대각선) x 열에서 득점이 가능한 자리 중 모서리 4군데    // (((3점 득점이 가능한 자리)))
        // 2.3. (대각선) x 열에서 득점이 가능한 자리
        // 2.4. (예외처리) 상대편의 점수를 방해할 수 있는 자리 => 전제조건 : 컴퓨터가 유저의 정보를 다 알고 있는 경우!
        //      int CreateNum(); -> 알고리즘을 적용하여 빙고 위치를 반환하는 함수를 만들어서 사용해야 한다!

        ////////// AI 처리 (심플 버전) ------------------------------

        // 랜덤으로 0 ~ 24 결과값, +1로 1 ~ 25 값을 얻어낸다.
        num = rand() % 25 + 1;

        // 빙고 배열을 25개 모두 검사해서 같은 수가 저장되어있다면...
        for (int i = 0; i < 25; i++)
        {
            // 입력받은 수가 있다면...
            if (UserBingo[i] == num)
            {
                UserBingo[i] = 35;      // # = ASCII 32 // bingo[i] = '#';
                isCheck = 1;
            }

            // 컴퓨터도 동일하게 처리하기
            if (ComBingo[i] == num)
            {
                ComBingo[i] = 35;
            }
        }

        // 체크되어있다면...
        if (isCheck) break;
    }

    // 컴퓨터가 입력한 숫자를 유저에게 알려준다.
    printf(" 컴퓨터가 입력한 숫자 : %d\n", num);
    Sleep(2000);
}
