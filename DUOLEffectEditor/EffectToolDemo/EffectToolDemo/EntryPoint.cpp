#include "pch.h"

//#include <vld.h>

const int width = 1080;

const int Height = 768;

std::shared_ptr<GameProcess> _GameProcess = nullptr;


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	LPCWSTR szAppName = L"EffectTool";
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Muscle::WndProc;//
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	// ���� Ŭ���� ���
	RegisterClass(&wndclass);
	

	//���� ����
	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, width, Height, NULL, NULL, hInstance, NULL);
	//������ ������ ȭ�鿡 ǥ��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	_GameProcess = GameProcess::Get();

	_GameProcess->Initialize(hWnd, width, Height);

	/// �޼��� ����
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			::TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_GameProcess->Update();

			_GameProcess->Render();
		}
	}
	
	_GameProcess->Finalize();

	return (int)msg.wParam;


	return 0;
}
