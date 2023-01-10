#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#define NOMINMAX
	#ifdef max
	#undef max
#endif
	#ifdef min
	#undef min
#endif

#include <Windows.h>
#include <cstdlib>

#include "DUOLCommon/LogHelper.h"
#include "DUOLEditor/Application.h"

#pragma region TEST_DIRECTX_LIB
	// ImGui 연습을 위함
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "dxgi.lib")
#pragma endregion

#pragma region TEST_RTTR
	// RTTR 연습을 위함
	#include <iostream>
	#include <rttr/registration>
	using namespace rttr;

	struct RTTRTest
	{
	    RTTRTest() {};

	    void TestFunction(double f)
	    {
	        DUOL_ENGINE_TRACE("{0} number.");
	    }

	    int data;
	};

	RTTR_REGISTRATION
	{
	    rttr::registration::class_<RTTRTest>("RTTRTest")
	    .constructor<>()
	    .method("TestFunction", &RTTRTest::TestFunction)
	    .property("data", &RTTRTest::data);
	}
#pragma endregion

DUOLEditor::Application g_App;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	g_App.Run();

    return EXIT_SUCCESS;
}