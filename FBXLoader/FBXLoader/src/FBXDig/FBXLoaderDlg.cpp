// FBXLoaderDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FBXConverter/FBXLoader.h"
#include "FBXDig/FBXLoaderDlg.h"
#include "afxdialogex.h"

#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <tchar.h>
#include <atlstr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFBXLoaderDlg 대화 상자


CFBXLoaderDlg::CFBXLoaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FBXLOADER_DIALOG, pParent)
	, _strPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// init parser
	fbxparser = new DUOLParser::DUOLFBXParser();
}

CFBXLoaderDlg::~CFBXLoaderDlg()
{
	fbxparser->Destory();
}

void CFBXLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROCESS_COUNT, _edCount);
	DDX_Control(pDX, IDC_SPIN_PROCESS_COUNT, _spProcessCount);
	DDX_Text(pDX, IDC_FILE_PATH, _strPath);
}

BEGIN_MESSAGE_MAP(CFBXLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFBXLoaderDlg::OnBnClickedOk)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN_PROCESS_COUNT, &CFBXLoaderDlg::OnNMThemeChangedScrollbar1)
	ON_EN_CHANGE(IDC_EDIT_PROCESS_COUNT, &CFBXLoaderDlg::OnEnChangeEditProcessCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PROCESS_COUNT, &CFBXLoaderDlg::OnDeltaposSpinProcessCount)
	ON_BN_CLICKED(IDC_BUTTON1, &CFBXLoaderDlg::AllFbxLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CFBXLoaderDlg::OnClick)
	ON_BN_CLICKED(IDC_RUN, &CFBXLoaderDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// CFBXLoaderDlg 메시지 처리기

BOOL CFBXLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE); // 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE); // 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	_edCount.SetWindowText(_T("0"));

	// Process를 10개까지만 만들 수 있게 세팅
	_spProcessCount.SetRange(0, 10);
	_spProcessCount.SetPos(0);

	return TRUE; // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFBXLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFBXLoaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFBXLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFBXLoaderDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CFBXLoaderDlg::OnNMThemeChangedScrollbar1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CFBXLoaderDlg::OnEnChangeEditProcessCount()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFBXLoaderDlg::OnDeltaposSpinProcessCount(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iVal = pNMUpDown->iPos + pNMUpDown->iDelta;
	// 값 = 현재값 +증/감

	if ((0 <= iVal) && (iVal <= 10))
	{
		CString sValue;
		sValue.Format(_T("%d\n"), iVal);
		_edCount.SetWindowText(sValue);
		_processCount = iVal;
	}

	*pResult = 0;
}

/**
 * \brief 지정된 Process 수만큼 돌면서 Load를 한다.
 */
void CFBXLoaderDlg::AllFbxLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 모든 file의 이름을 넣어줍니다. 
	auto path = std::filesystem::current_path();

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Mesh/UseMesh"))
	{
		if (entry.path().extension().string() == ".fbm")
			continue;

		_fbxNames.emplace_back(entry.path().string());
	}

	// 프로

	// 생성하는 프로세스의 속성을 지정할 때 사용하는 구조체
	STARTUPINFO si;

	// 새로 생성된 프로세스와 스레드 정보가 있음
	PROCESS_INFORMATION pi;

}


void CFBXLoaderDlg::OnClick()
{
	// 창을 띄워준다. TRUE 열기 / FALSE 저장
	CFileDialog dlg(TRUE);

	// open file window
	// stop
	int ok = dlg.DoModal();

	if (ok == IDOK)
	{
		// get file path 
		_strPath = dlg.GetPathName();

		UpdateData(FALSE);
	}
}

void CFBXLoaderDlg::OnBnClickedRun()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox(_T("이벤트 처리 완료"));

}
