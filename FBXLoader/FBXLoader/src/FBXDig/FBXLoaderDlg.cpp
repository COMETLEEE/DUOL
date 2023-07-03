// FBXLoaderDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FBXConverter/FBXLoader.h"
#include "FBXDig/FBXLoaderDlg.h"
#include "FBXDig/ProcessDialog.h"
#include "afxdialogex.h"

#include "DUOLFBXImporter/DUOLFBXImporter.h"
#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <tchar.h>
#include <atlstr.h>
#include <iostream>

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


CFBXLoaderDlg::CFBXLoaderDlg(CString _argv, int _argc,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FBXLOADER_DIALOG, pParent)
	, _strPath(_T(""))
	, _processCount(1)
{
	argv = _argv;

	argc = _argc;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// init parser
	_fbxParser = new DUOLParser::DUOLFBXParser();
	_binaryExporter = new DUOLFBXSerialize::BinarySerialize();

	if(_argv !="" && argc>1)
	{
		ProcessLoad(argv, argc);

		CWnd* pMainWindow = AfxGetApp()->m_pMainWnd;
		if (pMainWindow != nullptr)
		{
			pMainWindow->PostMessageW(WM_CLOSE);
		}
		else
			ExitProcess(0);
	}
}

CFBXLoaderDlg::~CFBXLoaderDlg()
{
	_fbxParser->Destory();
	free(_fbxParser);
	free(_binaryExporter);
}

void CFBXLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROCESS_COUNT, _edCount);
	DDX_Control(pDX, IDC_SPIN_PROCESS_COUNT, _spProcessCount);
	DDX_Text(pDX, IDC_FILE_PATH, _strPath);
	DDX_Control(pDX, IDC_PROGRESS1, progressBar);
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
	ON_BN_CLICKED(IDC_RUN, &CFBXLoaderDlg::OneLoad)
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

	_edCount.SetWindowText(_T("1"));

	// Process를 10개까지만 만들 수 있게 세팅
	_spProcessCount.SetRange(1, 10);
	_spProcessCount.SetPos(1);

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

	if ((1 <= iVal) && (iVal <= 10))
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
	// All Remove
	std::string meshPath = "Asset/BinaryData/Mesh";
	std::string materialPath = "Asset/BinaryData/Materials";
	std::string animationPath = "Asset/BinaryData/Animation";

	for (const auto& entry : std::filesystem::directory_iterator(meshPath))
	{
		std::filesystem::remove(entry.path());
	}

	for (const auto& entry : std::filesystem::directory_iterator(materialPath))
	{
		std::filesystem::remove(entry.path());
	}

	for (const auto& entry : std::filesystem::directory_iterator(animationPath))
	{
		std::filesystem::remove(entry.path());
	}

	// 모든 file의 이름을 넣어줍니다. 
	auto path = std::filesystem::current_path();

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Mesh/UseMesh"))
	{
		if (entry.path().extension().string() == ".fbm")
			continue;

		std::string path = entry.path().string();

		std::wstring wName = std::wstring(path.begin(), path.end());
		LPCWSTR lpName = wName.c_str();

		HANDLE hFile = CreateFile(lpName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

		// get file size
		INT64 dwLowSize = GetFileSize((HWND)hFile, NULL);

		_fbxNames.insert(make_pair(dwLowSize,entry.path().string()));
	}
	int fbxCount = _fbxNames.size();

	int processChangeCount = 0;
	std::vector<std::vector<std::string>> processFbxFileLists(_processCount);

	auto iter = _fbxNames.begin();

	// 각 프로세스에게 할당될 파일 목록을 분할합니다.
	for (int i = 0; i < fbxCount; i++)
	{
		if (_processCount <= processChangeCount)
			processChangeCount = 0;

		processFbxFileLists[processChangeCount].push_back(iter->second);
		processChangeCount++;
		iter++;
	}

	std::vector<HANDLE> processHandles;

	for (int i = 0; i < _processCount; i++)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);

		std::wstring commandLine = L"FBXLoader.exe";

		for (const auto& file : processFbxFileLists[i])
		{
			std::wstring wstr(file.begin(), file.end());

			commandLine += L" " + wstr;
		}

		// 분배받지 못한녀석
		if (commandLine == L"")
		{
			MessageBox(L"이 프로세스는 분배 받지 못함.", L"FBXProcess Converter", MB_OK);
			continue;
		}
		if (!CreateProcess(NULL, (LPWSTR)commandLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			auto error = GetLastError();
			std::cout << "Error: Failed to create process. Error code: " << error << std::endl;
		}
		else
		{
			processHandles.push_back(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	progressBar.SetRange(0, 100);
	progressBar.SetPos(0);

	int percentPerProcess = 100 / _processCount; // 하나의 프로세스가 차지하는 % 비율

	// 모든 자식 프로세스가 종료될 때까지 대기
	while (!processHandles.empty())
	{
		// 하나라도 완료되면..
		DWORD waitResult = WaitForMultipleObjects(processHandles.size(), processHandles.data(), FALSE, INFINITE);

		if (waitResult >= WAIT_OBJECT_0 && waitResult < WAIT_OBJECT_0 + processHandles.size())
		{
			// 종료된 자식 프로세스의 핸들을 벡터에서 제거
			int index = waitResult - WAIT_OBJECT_0;

			CloseHandle(processHandles[index]);

			processHandles.erase(processHandles.begin() + index);

			// 진행 상황 표시
			int completedProcessCount = _processCount - processHandles.size();
			int completedPercent = completedProcessCount * percentPerProcess;

			progressBar.SetPos(completedPercent);
		}
	}

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

void CFBXLoaderDlg::ProcessLoad(CString _argv, int _argc)
{
	ProcessDialog dlg;

	dlg.Create(IDD_DIALOG_PROCESS, this);
	dlg.ShowWindow(SW_SHOW);

	// 대화 상자를 닫을 때까지 다른 작업을 수행하고자 할 때
	while (dlg.GetSafeHwnd() != nullptr)
	{
		std::string argvInfo = std::string(CT2CA(_argv));

		std::stringstream stream;

		stream.str(argvInfo);

		std::string fbxPath = "Asset/Mesh/UseMesh";

		std::vector<std::string> fbxFileNameList;

		std::string tmpStr;

		while (stream >> tmpStr)
		{
			fbxFileNameList.emplace_back(tmpStr);
		}

		dlg._processBar.SetRange(0, fbxFileNameList.size());
		dlg._processBar.SetPos(0);

		for (int i = 0; i < fbxFileNameList.size(); i++)
		{
			std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();

			std::wstring fileName = std::filesystem::path(fbxFileNameList[i]).filename();

			std::string strFileName = std::string(CT2CA(fileName.c_str()));

			std::string tmpFbxFileName = strFileName.substr(0, strFileName.size() - 4);

			_fbxModel = _fbxParser->LoadFBX(fbxPath + '/' + strFileName, tmpFbxFileName);

			_binaryExporter->SerializeDuolData(_fbxModel);

			dlg._processBar.SetPos(i + 1);
		}

		dlg.DestroyWindow();

		// Dig is EXIT
		MSG msg;
		if (::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			if (!AfxGetApp()->PumpMessage())
				break;
		}
	}

	MessageBox(L"프로세스 완료", L"FBXProcess Load", MB_OK);

}

void CFBXLoaderDlg::OneLoad()
{
	// strpath is Null return
	if (_strPath == L"")
		return;

	int pos = _strPath.Find('.');

	CString extension = _strPath.Right(_strPath.GetLength() - pos - 1);

	// extension is no fbx return
	if (extension != L"fbx")
		return;

	CString path = _strPath.Right(pos);

	bool check = true;

	while (check)
	{
		pos = path.Find('\\');
		if (pos == -1)
			check = false;
		else
			path = path.Right(path.GetLength() - pos - 1);
	}

	pos = path.Find('.');

	CString modelName = path.Left(pos);

	std::shared_ptr<FBXModel> _fbxModel = std::make_shared<FBXModel>();

	std::string strModelName = std::string(CT2CA(modelName));
	std::string strPath = std::string(CT2CA(_strPath));

	_fbxModel = _fbxParser->LoadFBX(strPath, strModelName);

	if (_binaryExporter->SerializeDuolData(_fbxModel))
		AfxMessageBox(_T("로드 완료"));
	else
		AfxMessageBox(_T("로드 실패"));

}
