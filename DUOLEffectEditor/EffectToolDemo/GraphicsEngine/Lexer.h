#pragma once
#include <windows.h>

///#define TRACE __noop
#include <atltrace.h>
#define TRACE ATLTRACE

#define	MAX_SIZE_OF_BUFFER					65536

// �⺻ Identifier
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
	// ������ �о �۾��ϱ� ���� ������
	HANDLE m_File;			// ������ �ڵ�

	DWORD m_Pointer;		// ���� Buffer�� �а� �ִ� ���ڿ��� Pointer
	DWORD m_NowLoaded;		// ���� �о� �鿩���� ũ��

	DWORD m_SizeOfBuffer;	// Buffer�� ũ��
	CHAR* m_Char;			// ������ ������ �о� ���� Buffer

	DWORD m_TotalFileSize;	// ������ ��ü ũ��
	DWORD m_NowProgress;	// ���� ����.

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
