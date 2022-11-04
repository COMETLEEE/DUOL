#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <Windows.h>
#include <cstdlib>

#include "DUOLCommon/LogHelper.h"
#include "DUOLEditor/Application.h"

// ImGui ������ ����
#pragma region TEST_DIRECTX_LIB
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "dxgi.lib")
#pragma endregion

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const DUOLEditor::Application app;

    app.Run();

    return EXIT_SUCCESS;
}