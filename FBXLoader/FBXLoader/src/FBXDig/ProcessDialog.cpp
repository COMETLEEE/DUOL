// ProcessDialog.cpp: 구현 파일
//

#include "pch.h"
#include "afxdialogex.h"
#include "ProcessDialog.h"

#include "resource.h"


// ProcessDialog 대화 상자

IMPLEMENT_DYNAMIC(ProcessDialog, CDialogEx)

ProcessDialog::ProcessDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

ProcessDialog::~ProcessDialog()
{
}

void ProcessDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, _processBar);
}


BEGIN_MESSAGE_MAP(ProcessDialog, CDialogEx)
END_MESSAGE_MAP()


// ProcessDialog 메시지 처리기
