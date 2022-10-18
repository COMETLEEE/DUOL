#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <Windows.h>
#include <cstdlib>

#include "DUOLGame/Application.h"

// TEST CODE
#include "DUOLGameEngine/Manager/InputManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    using namespace DUOLGameEngine;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DUOLGame::Application app;

    app.Run();

    bool result = InputManager::GetInstance()->GetKeyUp(KeyCode::A);

    return EXIT_SUCCESS;
}