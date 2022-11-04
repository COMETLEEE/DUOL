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
	TRACE("CLexcer���� Close(); \n");
	Close();
}

CHAR CLexer::GetNextChar()
{
	// CASE 1) Buffer�� ���� ������� �ٽ� File�� ���� Buffer��ŭ�� �о���δ�.
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
	// CASE 2) Buffer�� ���� �ʾ��� ��� �׳� ���� ���ڸ� ����������.
	else
	{
		m_NowProgress++;

		m_Pointer++;
		return	m_Char[m_Pointer - 1];
	}
}

BOOL CLexer::Open(LPSTR p_FileName, DWORD p_SizeOfBuffer)
{
	// 1) FileName String�� NULL�̸� �׳� ����������.
	if (p_FileName == NULL)
	{
		return	FALSE;
	}

	// 2) Buffer�� Size�� 0�� ����������.
	if (p_SizeOfBuffer == 0)
	{
		return	FALSE;
	}


	// 1) File�� Open�Ѵ�.
	m_File = CreateFileA(p_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_File == INVALID_HANDLE_VALUE)
	{
		TRACE("Cant open file");
		return FALSE;
	}

	// 2) �⺻���� �������� �����Ѵ�.
	m_TotalFileSize = GetFileSize(m_File, NULL);
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;

	// 3) Buffer�� �����.
	m_SizeOfBuffer = p_SizeOfBuffer;
	m_Char = (CHAR*)malloc(m_SizeOfBuffer);

	return	TRUE;
}

VOID CLexer::Close()
{
	// 1) ������ ���� ���������� �˻��Ͽ� �������°� �ƴ϶�� �׳� ����������.
	if (m_File == NULL)
	{
		TRACE("File is not Opened\n");
		return;
	}

	// 2) File�� �ݴ´�.
	CloseHandle(m_File);
	m_File = NULL;

	// 3) Buffer�� ����������.
	m_SizeOfBuffer = 0;
	//SAFE_FREE(m_Char);
	/// � �����ε� ���� free�� ���� �Ѵ�.
	free(m_Char);
}

BOOL CLexer::Reset()
{
	// 1) ������ ���� ���������� �˻��Ѵ�.
	if (m_File == NULL)
	{
		TRACE("File is not Opened\n");
		return	FALSE;
	}

	// 2) �⺻ ������ Reset�Ѵ�.
	m_NowProgress = 0;
	m_Pointer = 0;
	m_NowLoaded = 0;

	// 3) File�� Pointer�� ���� ó������ �ǵ�����.
	SetFilePointer(m_File, 0, 0, FILE_BEGIN);

	return	TRUE;
}

LONG CLexer::GetToken(LPSTR /*p_TokenString*/)
{
	return	0;
}