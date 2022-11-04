#include "pch.h"
#include "IGameEngine.h"


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
		default:
			return DefWindowProc(hwnd, uint, wparam, lparam);
		}
	}

}
