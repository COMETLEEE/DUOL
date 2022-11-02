#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <Windows.h>
#include <cstdlib>

#include "DUOLCommon/LogHelper.h"

#include "DUOLGame/Application.h"
#include "DUOLGameEngine/Manager/InputManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    using namespace DUOLGameEngine;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const DUOLGame::Application app;

    app.Run();

    return EXIT_SUCCESS;
}