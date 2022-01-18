#include <Windows.h>
#include <iostream>

using namespace std;

// 동기 입출력 동시에 (큰파일ver.)

DWORD SectorsPerCluster;
DWORD BytesPerSector;
DWORD NumberOfFreeClusters;
DWORD TotalNumberOfClusters;

void main()
{
	WCHAR lpRootPathName[] = L"c:\\";
	GetDiskFreeSpace(lpRootPathName, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters);

	setlocale(LC_ALL, "KOREAN"); // cout 유니코드로 출력 가능하게 함

	WCHAR* g_buffer = NULL;
	LARGE_INTEGER filesize;// 큰용량 파일을 받을때
	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };

	HANDLE hFileReadAsync = CreateFile(L"Test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); // 파일생성
	// CreateFile(파일을 생성하거나 열 경로, 파일을 열거나 쓸때 권한(플래그 중복가능), 파일의 공유모드, 보안속성, 해당위치에 파일이 존재할경우 행동, 생성될 파일의 속성,파일에 대한 속성을 제공)
	// FILE_FLAG_NO_BUFFERING을 가지고 CreateFile했을경우 ReadFile,WriteFile사용시 sector단위가 512byte 정수배가 되어야한다 그렇지 않을경우 오류발생

	if (hFileReadAsync != INVALID_HANDLE_VALUE) // 파일이 만들어졌으면 작업하자
	{
		GetFileSizeEx(hFileReadAsync, &filesize); // GetFileSize는 WORD버전 4.2GB넘어서 더 큰건 GetFileSizeEx쓰자
		DWORD dwSize = 0;
		if (filesize.QuadPart % BytesPerSector != 0)
		{
			dwSize = filesize.QuadPart / BytesPerSector;
			dwSize = (dwSize + 1) * BytesPerSector;
		}
		g_buffer = new WCHAR[filesize.QuadPart];
		DWORD dwRead;
		BOOL ret = ReadFile(hFileReadAsync, g_buffer, dwSize, &dwRead, &g_ReadOV);
		BOOL bPanding = FALSE; // 대기
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				wcout << L"로드 중" << endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"로드 실패" << endl;
			}
		}
		else
		{
			wcout << L"로드 완료" << endl;
		}
		// 로드 중인 경우
		DWORD dwTrans = 0;
		BOOL bReturn;
		while (bPanding)
		{
			bReturn = GetOverlappedResult(hFileReadAsync, &g_ReadOV, &dwTrans, FALSE);
			if (bReturn == TRUE)
			{
				wcout << L"로드 완료" << endl;
				bPanding = FALSE;
			}
			else
			{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					wcout << L".";
				}
				else
				{
					wcout << L"로드 실패" << endl;
				}
			}
			Sleep(1000); // 1초에 한번 로드되게
		}
	}
	CloseHandle(hFileReadAsync);
	wcout << L"복사시작" << endl;

	HANDLE hFileWriteAsync = CreateFile(L"TestCopy.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, NULL); // 파일생성
	// CreateFile(파일을 생성하거나 열 경로, 파일을 열거나 쓸때 권한(플래그 중복가능), 파일의 공유모드, 보안속성, 해당위치에 파일이 존재할경우 행동, 생성될 파일의 속성,파일에 대한 속성을 제공)

	if (hFileWriteAsync != NULL) // 파일이 만들어졌으면 작업하자
	{
		DWORD dwLength = filesize.QuadPart;
		DWORD dwWrite;
		BOOL ret = WriteFile(hFileWriteAsync, g_buffer, dwLength, &dwWrite, &g_WriteOV);
		BOOL bPanding = FALSE;
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				wcout << L"출력 중" << endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"출력 실패" << endl;
			}
		}
		else
		{
			wcout << L"출력 완료" << endl;
			bPanding = FALSE;
		}
		// 로드 중인 경우
		DWORD dwTrans = 0;
		BOOL bReturn;
		while (bPanding)
		{
			bReturn = GetOverlappedResult(hFileWriteAsync, &g_WriteOV, &dwTrans, FALSE);
			if (bReturn == TRUE)
			{
				wcout << L"출력 완료" << endl;
				bPanding = FALSE;
			}
			else
			{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_IO_INCOMPLETE)
				{
					wcout << L".";
				}
				else
				{
					wcout << L"실패";
					bPanding = FALSE;
				}
			}
			Sleep(1000);
		}
		CloseHandle(hFileWriteAsync);
	}
}