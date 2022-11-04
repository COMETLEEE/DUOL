#include "pch.h"
#include "IGraphicsEngine.h"
#include "DXEngine.h"

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

GRPHICHS_API IGraphicsEngine* CreateDXEngine()
{
	return new DXEngine();
}

GRPHICHS_API void DeleteDXEngine(IGraphicsEngine* _pEngine)
{
	delete _pEngine;

}

GRPHICHS_API ImGuiContext* GetImguiContext()
{
	return ImGui::GetCurrentContext();
}

