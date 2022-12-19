#include "IGameEngine.h"
#include "MuscleEngine.h"

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

#include "HotKey.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace Muscle
{
	std::shared_ptr<IGameEngine> IGameEngine::_gameEngine = nullptr;

	std::shared_ptr<IGameEngine> IGameEngine::Get()
	{
		if (_gameEngine == nullptr)
			_gameEngine = Muscle::MuscleEngine::GetInstance();

		return _gameEngine;
	}

	void IGameEngine::Finalize()
	{
		MuscleEngine::GetInstance()->Finalize();
		_gameEngine.reset();
	}

	LRESULT __stdcall WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uint, wparam, lparam))
			return true;
		switch (uint)
		{

		case WM_SIZE:
			if (wparam == SIZE_MINIMIZED)
			{
				// 해결!
				// width height 0이 들어가면 터진다.
			}
			else
			{
				if (MuscleEngine::GetInstance())
					MuscleEngine::GetInstance()->OnResize();
			}

			break;
		case WM_MOUSEMOVE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_DPICHANGED:
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
			{
				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)lparam;
				::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		default:
			return DefWindowProc(hwnd, uint, wparam, lparam);
		}
	}

}
