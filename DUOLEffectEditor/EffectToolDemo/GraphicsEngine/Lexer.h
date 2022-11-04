#pragma once
#include <windows.h>

///#define TRACE __noop
#include <atltrace.h>
#define TRACE ATLTRACE

#define	MAX_SIZE_OF_BUFFER					65536

// 기본 Identifier
#define	TOKEND_NUMBER						1001
#define	TOKEND_STRING						1002
#define	TOKEND_BLOCK_START					1003
#define	TOKEND_BLOCK_END					1004

#define	TOKEND_END							65534
#define	TOKEND_IDENTIFIER					65535
#define	TOKEND_NOTDEFINED					65536

class CLexer
{
protected:
	// 파일을 읽어서 작업하기 위한 변수들
	HANDLE m_File;			// 파일의 핸들

	DWORD m_Pointer;		// 현재 Buffer상에 읽고 있는 문자열의 Pointer
	DWORD m_NowLoaded;		// 현재 읽어 들여놓은 크기

	DWORD m_SizeOfBuffer;	// Buffer의 크기
	CHAR* m_Char;			// 파일의 내용을 읽어 들인 Buffer

	DWORD m_TotalFileSize;	// 파일의 전체 크기
	DWORD m_NowProgress;	// 현재 진행.

protected:
	CHAR GetNextChar();

public:
	BOOL Open(LPSTR p_FileName, DWORD p_SizeOfBuffer = MAX_SIZE_OF_BUFFER);
	VOID Close();
	BOOL Reset();

	virtual	LONG GetToken(LPSTR p_TokenString);

public:
	CLexer(LPSTR p_FileName = NULL, DWORD p_SizeOfBuffer = MAX_SIZE_OF_BUFFER);
	virtual ~CLexer();
};
