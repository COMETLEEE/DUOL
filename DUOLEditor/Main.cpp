#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <Windows.h>
#include <cstdlib>

#include "DUOLCommon/LogHelper.h"
#include "DUOLEditor/Application.h"

DUOLEditor::Application g_App;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	g_App.Run();

    g_App.UnInitialize();

    return EXIT_SUCCESS;
}