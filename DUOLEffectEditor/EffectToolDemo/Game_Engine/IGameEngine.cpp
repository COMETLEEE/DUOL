#include "pch.h"
#include "IGameEngine.h"


namespace Muscle
{
	std::shared_ptr<IGameEngine> IGameEngine::_gameEngine = nullptr;

	std::shared_ptr<IGameEngine> IGameEngine::Get()
	{
		if (_gameEngine == nullptr)
			_gameEngine = Muscle::MuscleEngine::GetInstance();

		return _gameEngine;
	}

	LRESULT __stdcall WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_WndProcHandler(hwnd, uint, wparam, lparam))
			return true;
		switch (uint)
		{
		case WM_SIZE:
			if (wparam == SIZE_MINIMIZED)
			{
				// �ذ�!
				// width height 0�� ���� ������.
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
