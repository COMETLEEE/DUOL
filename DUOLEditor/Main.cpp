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
#include "DUOLCommon/MetaDataType.h"
#include "DUOLEditor/Application.h"

#pragma region TEST_DIRECTX_LIB
	// ImGui 연습을 위함
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "dxgi.lib")
#pragma endregion

#include <rttr/registration>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "rttr/library.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Transform>("Transform")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&>()
	.property("Position", &DUOLGameEngine::Transform::GetLocalPosition, &DUOLGameEngine::Transform::SetLocalPosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	)
	.property("Rotation", &DUOLGameEngine::Transform::GetLocalEulerAngle, &DUOLGameEngine::Transform::SetLocalEulerAngle)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	)
	.property("Scale", &DUOLGameEngine::Transform::GetLocalScale, &DUOLGameEngine::Transform::SetLocalScale)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	);
}

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