#include "pch.h"
#include "Lexer.h"


CLexer::CLexer(LPSTR p_FileName, DWORD p_SizeOfBuffer)
{
	m_File = NULL;

	m_TotalFileSize = 0;
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;
	m_SizeOfBuffer = 0;
	m_Char = NULL;

	if (p_FileName != NULL)
	{
		Open(p_FileName, p_SizeOfBuffer);
	}
}

CLexer::~CLexer()
{
	TRACE("CLexcer에서 Close(); \n");
	Close();
}

CHAR CLexer::GetNextChar()
{
	// CASE 1) Buffer의 끝에 닿았으면 다시 File로 부터 Buffer만큼을 읽어들인다.
	if (m_Pointer == m_NowLoaded)
	{
		if (ReadFile(m_File, m_Char, m_SizeOfBuffer, &m_NowLoaded, NULL) != TRUE)
		{
			return	0;
		}

		if (!m_NowLoaded)
		{
			return	0;
		}

		m_NowProgress++;

		m_Pointer = 1;
		return	m_Char[0];
	}
	// CASE 2) Buffer가 차지 않았을 경우 그냥 다음 글자를 돌려보낸다.
	else
	{
		m_NowProgress++;

		m_Pointer++;
		return	m_Char[m_Pointer - 1];
	}
}

BOOL CLexer::Open(LPSTR p_FileName, DWORD p_SizeOfBuffer)
{
	// 1) FileName String이 NULL이면 그냥 돌려보낸다.
	if (p_FileName == NULL)
	{
		return	FALSE;
	}

	// 2) Buffer의 Size가 0면 돌려보낸다.
	if (p_SizeOfBuffer == 0)
	{
		return	FALSE;
	}


	// 1) File을 Open한다.
	m_File = CreateFileA(p_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_File == INVALID_HANDLE_VALUE)
	{
		TRACE("Cant open file");
		return FALSE;
	}

	// 2) 기본적인 변수들을 설정한다.
	m_TotalFileSize = GetFileSize(m_File, NULL);
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;

	// 3) Buffer를 만든다.
	m_SizeOfBuffer = p_SizeOfBuffer;
	m_Char = (CHAR*)malloc(m_SizeOfBuffer);

	return	TRUE;
}

VOID CLexer::Close()
{
	// 1) 파일이 열린 상태인지를 검사하여 열린상태가 아니라면 그냥 돌려보낸다.
	if (m_File == NULL)
	{
		TRACE("File is not Opened\n");
		return;
	}

	// 2) File을 닫는다.
	CloseHandle(m_File);
	m_File = NULL;

	// 3) Buffer를 지워분진다.
	m_SizeOfBuffer = 0;
	//SAFE_FREE(m_Char);
	/// 어떤 식으로든 여기 free가 들어가야 한다.
	free(m_Char);
}

BOOL CLexer::Reset()
{
	// 1) 파일이 열린 상태인지를 검사한다.
	if (m_File == NULL)
	{
		TRACE("File is not Opened\n");
		return	FALSE;
	}

	// 2) 기본 변수를 Reset한다.
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;

	// 3) File의 Pointer를 제일 처음으로 되돌린다.
	SetFilePointer(m_File, 0, 0, FILE_BEGIN);

	return	TRUE;
}

LONG CLexer::GetToken(LPSTR /*p_TokenString*/)
{
	return	0;
}