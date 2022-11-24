#include "Export/IGraphicsEngine.h"
#include "Core/DirectX11/DXEngine.h"
#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"
#include "Export/dllExport.h"

namespace MuscleGrapics
{
	GRPHICHS_API IGraphicsEngine* CreateDXEngine()
	{
		return new DXEngine();
	}

	GRPHICHS_API void DeleteDXEngine(IGraphicsEngine* _pEngine)
	{
		delete _pEngine;

	}
	GRPHICHS_API ImGuiContext* MuscleGrapics::GetImguiContext()
	{
		return ImGui::GetCurrentContext();
	}
}
