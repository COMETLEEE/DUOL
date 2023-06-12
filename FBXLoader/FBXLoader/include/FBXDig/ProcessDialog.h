#pragma once
#include "afxdialogex.h"


// ProcessDialog 대화 상자

class ProcessDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDialog)

public:
	ProcessDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ProcessDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl _processBar;
};
