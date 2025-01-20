//
//  Sound.cpp
//  CProject
//
//  Created by 오나현 on 01/17/25.
//

/*
/// beep 함수

#include <stdio.h>
#include <Windows.h>

#define _C 1046.502
#define _D 1174.659
#define _E 1318.510
#define _F 1396.913
#define _G 1567.982
#define _A 1760.000
#define _B 1975.533

int main()
{
	Beep(1046.502, 1000);	// 1000분의 1초
	Sleep(200);				// 0.2초 휴식

	Beep(_C, 500);			// 위와 같은 코드 (도)
	Sleep(200);
	Beep(_D, 100);
	Sleep(200);
	Beep(_E, 300);
	Sleep(200);
	Beep(_F, 1000);
	Sleep(400);
	Beep(_G, 100);
	Sleep(200);
	Beep(_A, 100);
	Sleep(500);
	Beep(_B, 100);

	return 0;
}

// --------------------------------------------------

/// MIDI 파일 (링크 참고)

// --------------------------------------------------

/// WAV 파일 불러오기

#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define _C 1046.502
#define _D 1174.659

int main()
{
	// PlaySound( 경로;사운드 파일 위치, NULL, 파라미터;SND_FILENAME/SND_ASYNC(재생하면서 다음 코드 진행(BGM))/SND_LOOP(반복 재생) )
	PlaySound(TEXT("002.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	Beep(_D, 100);
	Sleep(200);

	system("pause");

	PlaySound(NULL, 0, 0);

	Beep(_C, 500);
	Sleep(200);

	system("pause");

	return 0;
}
*/
