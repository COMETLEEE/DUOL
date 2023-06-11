
// FBXLoaderDlg.h: 헤더 파일
//

#pragma once
#include <string>
#include <vector>

#include "Serialize/BinarySerialize.h"

namespace DUOLParser
{
	class DUOLFBXParser;
}

// CFBXLoaderDlg 대화 상자
class CFBXLoaderDlg : public CDialogEx
{
// 생성입니다.
public:
	CFBXLoaderDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CFBXLoaderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FBXLOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult);
	// 프로세스의 카운터를 출력하는 edit
	CEdit _edCount;
	// ProcessCount를 관리하는 버튼
	CSpinButtonCtrl _spProcessCount;
	afx_msg void OnEnChangeEditProcessCount();
	afx_msg void OnDeltaposSpinProcessCount(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void AllFbxLoad();

	afx_msg void OnClick();

private:
	int _processCount;
	std::vector<std::string> _fbxNames;
	DUOLParser::DUOLFBXParser* fbxparser;
	DUOLFBXSerialize::BinarySerialize* binaryExporter;

public:
	// file Path string	
	CString _strPath;
	afx_msg void OneLoad();
};
