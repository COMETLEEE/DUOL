#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일

#include <map>
#include <unordered_map>
#include <windows.h>
#include <vector>
#include <memory>
#include <queue>

#include <DirectXMath.h>
#include "SimpleMath.h"





#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace Muscle
{
constexpr float PI = 3.141592f;
}

#include "../GraphicsEngine/include/Export/IGraphicsEngine.h"
//#include "..\Loader\Loader_Interface.h"
#include "CTime.h"
#include "KeyBoard.h"
#include "XPad.h"

#include "ComponentsHead.h"

//#include "ResourceManager.h"
#include "ObjectManager.h"
#include "GraphicsManager.h"


#include "IGameEngine.h"
#include "MuscleEngine.h"
#include "SceneManager.h"

#pragma comment(lib, "Xinput9_1_0.lib")