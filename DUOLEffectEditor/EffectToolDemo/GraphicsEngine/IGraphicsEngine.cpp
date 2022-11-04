#include "pch.h"
#include "IGraphicsEngine.h"
#include "DXEngine.h"

GRPHICHS_API IGraphicsEngine* CreateDXEngine()
{
	return new DXEngine();
}

GRPHICHS_API void DeleteDXEngine(IGraphicsEngine* _pEngine)
{
	delete _pEngine;
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

GRPHICHS_API LRESULT ImGui_WndProcHandler(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, uint, wparam, lparam);
}
