#include <Windows.h>
#include <iostream>

using namespace std;

// 동기입출력 예시
void main()
{
	setlocale(LC_ALL, "KOREAN"); // cout 유니코드로 가능하게함
	// 파일쓰기
	HANDLE hFileWrite = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // 파일생성
	if (hFileWrite != NULL) // 파일이 만들어지면 작업
	{
		WCHAR buffer[] = L"lsh";
		DWORD dwLength = sizeof(buffer);
		DWORD dwWritten; // 얼만큼 썼다
		BOOL ret = WriteFile(hFileWrite, buffer, dwLength, &dwWritten, NULL); // 반환값은 성공 실패를 따짐
		// 마지막인자 lpOverlapped가 있어야 비동기 입출력, NULL을 쓰면 비동기 입출력 안하겠다 ㅇㅇ 
		if (ret == TRUE)
		{
			// cout은 유니코드 출력못함, 기본적으로 멀티바이트 지원
			// setlocale을 상단에서 사용해야됨, 기본적으로 영어이기때문에 필요하면 바꿔써라~
			wcout << L"출력성공";
		}
	}
	CloseHandle(hFileWrite);
	// 파일 읽기
	HANDLE hFileRead = CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // 파일생성
	if (hFileRead != NULL) // 파일이 만들어지면 작업
	{
		WCHAR buffer[512] = { 0 , };
		DWORD dwLength = sizeof(buffer);
		DWORD dwRead;
		BOOL ret = WriteFile(hFileRead, buffer, dwLength, &dwRead, NULL); // 반환값은 성공 실패를 따짐
		// 마지막인자 lpOverlapped가 있어야 비동기 입출력, NULL을 쓰면 비동기 입출력 안하겠다 ㅇㅇ 
		if (ret == TRUE)
		{
			// cout은 유니코드 출력못함, 기본적으로 멀티바이트 지원
			// setlocale을 상단에서 사용해야됨, 기본적으로 영어이기때문에 필요하면 바꿔써라~
			wcout << buffer;
		}
	}
	CloseHandle(hFileRead);
}