#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLGameEngine
{
	InputManager::InputManager() :
		_hWnd(NULL)
		, _prevKeyboardBuffer{}
		, _currKeyboardBuffer{}
		, _currKeyStates{}
		, _prevMousePos(DUOLMath::Vector2::Zero)
		, _currMousePos(DUOLMath::Vector2::Zero)
		, _isLockedMode(false)
		, _isGameLockedMode(false)
		, _lockRect(DUOLMath::Vector4::Zero)
	{

	}

	InputManager::~InputManager()
	{

	}

	void InputManager::Initialize(HWND hWnd)
	{
		_hWnd = hWnd;

		ZeroMemory(_prevKeyboardBuffer, sizeof(unsigned char) * KEY_COUNT);

		ZeroMemory(_currKeyboardBuffer, sizeof(unsigned char) * KEY_COUNT);

		ZeroMemory(_currKeyStates, sizeof(KeyState) * KEY_COUNT);

		ZeroMemory(_currAxisValue, sizeof(float) * 2);

		DUOL_INFO(DUOL_FILE, "InputManager Initialize Success !");
	}

	void InputManager::UnInitialize()
	{

	}

	void InputManager::Update(float deltaTime)
	{
		_isLockedMode ? UpdateLockModeMousePosition() : UpdateMousePosition();

		UpdateAllKeyState();

		UpdateAxisValue();

		// 마우스 Set Visible
		static bool isMouseOn = true;

		if (isMouseOn && GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
		{
			GetInstance()->SetGameLockMode(true);

			isMouseOn = false;

			ShowCursor(isMouseOn);
		}
		else if (!isMouseOn && GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape))
		{
			GetInstance()->SetGameLockMode(false);

			GetInstance()->SetLockRect(DUOLMath::Vector4::Zero);

			isMouseOn = true;

			ShowCursor(isMouseOn);
		}
	}

	void InputManager::UpdateAllKeyState()
	{
		memcpy(_prevKeyboardBuffer, _currKeyboardBuffer, sizeof(unsigned char) * KEY_COUNT);

		if (GetKeyboardState(_currKeyboardBuffer) == 0)
		{
			// TODO
			// Keyboard의 상태를 불러오지 못 했습니다.
		}

		/// <summary>
		/// 0x8000 : 이전에 누른 적이 없고 호출 시점에 키가 눌린 상태
		/// </summary>
		for (int i = 0; i < KEY_COUNT; i++)
		{
			const bool isPrevKeyDown = _prevKeyboardBuffer[i] & 0b10000000;

			const bool isCurrKeyDown = _currKeyboardBuffer[i] & 0b10000000;

			if (isPrevKeyDown)
				isCurrKeyDown ? (_currKeyStates[i] = KeyState::Pressed) : (_currKeyStates[i] = KeyState::Up);
			else
				isCurrKeyDown ? (_currKeyStates[i] = KeyState::Down) : (_currKeyStates[i] = KeyState::None);
		}
	}

	void InputManager::UpdateMousePosition()
	{
		static POINT cursorPos;

		static RECT consolePos;

		// 현재 마우스 위치를 업데이트합니다.
		GetCursorPos(&cursorPos);

		// 기존 현재 마우스 위치를 이전 위치로 업데이트합니다.
		memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

		memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));

		_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

		ScreenToClient(_hWnd, &cursorPos);

		_currMousePos = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

		GetWindowRect(_hWnd, &consolePos);

		// 좌상단만 업데이트 한다. 
		_currScreenPosInScreen = DUOLMath::Vector2(static_cast<float>(consolePos.left), static_cast<float>(consolePos.top));
	}

	void InputManager::UpdateLockModeMousePosition()
	{
		if (!_isGameLockedMode)
		{
			static POINT cursorPos;

			// 현재 마우스 위치를 업데이트합니다.
			GetCursorPos(&cursorPos);

			int screenX = GetSystemMetrics(SM_CXSCREEN);

			int screenY = GetSystemMetrics(SM_CYSCREEN);

			DUOLMath::Vector2 nextCursorPos = DUOLMath::Vector2{ 0,0 };

			bool check[2] = { false, false };

			if (cursorPos.x <= 10)
			{
				nextCursorPos.x = screenX - 11;

				check[0] = true;
			}
			else if (cursorPos.x >= screenX - 10)
			{
				nextCursorPos.x = 11;

				check[0] = true;
			}
			else
				nextCursorPos.x = cursorPos.x;

			if (cursorPos.y <= 10)
			{
				nextCursorPos.y = screenY - 11;

				check[1] = true;
			}
			else if (cursorPos.y >= screenY - 10)
			{
				nextCursorPos.y = 11;

				check[1] = true;
			}
			else
				nextCursorPos.y = cursorPos.y;

			if (check[0] || check[1])
			{
				POINT newCursorPos = { static_cast<LONG>(nextCursorPos.x), static_cast<LONG>(nextCursorPos.y) };

				SetCursorPos(newCursorPos.x, newCursorPos.y);

				_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(newCursorPos.x), static_cast<float>(newCursorPos.y));

				ScreenToClient(_hWnd, &newCursorPos);

				_currMousePos = DUOLMath::Vector2(static_cast<float>(newCursorPos.x), static_cast<float>(newCursorPos.y));

				// 기존 현재 마우스 위치를 이전 위치로 업데이트합니다.
				memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

				memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));
			}
			else
			{
				// 기존 현재 마우스 위치를 이전 위치로 업데이트합니다.
				memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

				memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));

				_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

				ScreenToClient(_hWnd, &cursorPos);

				_currMousePos = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
			}
		}
		// Game Lock Mode.
		else
		{
			static POINT cursorPos;

			// 현재 마우스 위치를 업데이트합니다.
			GetCursorPos(&cursorPos);

			RECT winRect;

			if (_lockRect == DUOLMath::Vector4::Zero)
				GetWindowRect(_hWnd, &winRect);
			else
			{
				winRect.left = _lockRect.x;
				winRect.top = _lockRect.y;
				winRect.right = _lockRect.z;
				winRect.bottom = _lockRect.w;
			}

			int screenX = winRect.right - winRect.left;

			int screenY = winRect.bottom - winRect.top;

			DUOLMath::Vector2 nextCursorPos = DUOLMath::Vector2{ 0,0 };

			bool check[2] = { false, false };

			if (cursorPos.x <= winRect.left + 50)
			{
				nextCursorPos.x = winRect.right - 51;

				check[0] = true;
			}
			else if (cursorPos.x >= winRect.right - 50)
			{
				nextCursorPos.x = winRect.left + 51;

				check[0] = true;
			}
			else
				nextCursorPos.x = cursorPos.x;

			if (cursorPos.y <= winRect.top + 50)
			{
				nextCursorPos.y = winRect.bottom - 51;

				check[1] = true;
			}
			else if (cursorPos.y >= winRect.bottom - 50)
			{
				nextCursorPos.y = winRect.top + 51;

				check[1] = true;
			}
			else
				nextCursorPos.y = cursorPos.y;


			if (check[0] || check[1])
			{
				POINT newCursorPos = { static_cast<LONG>(nextCursorPos.x), static_cast<LONG>(nextCursorPos.y) };

				SetCursorPos(newCursorPos.x, newCursorPos.y);

				_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(newCursorPos.x), static_cast<float>(newCursorPos.y));

				ScreenToClient(_hWnd, &newCursorPos);

				_currMousePos = DUOLMath::Vector2(static_cast<float>(newCursorPos.x), static_cast<float>(newCursorPos.y));

				// 기존 현재 마우스 위치를 이전 위치로 업데이트합니다.
				memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

				memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));
			}
			else
			{
				// 기존 현재 마우스 위치를 이전 위치로 업데이트합니다.
				memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

				memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));

				_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

				ScreenToClient(_hWnd, &cursorPos);

				_currMousePos = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
			}
		}
	}

	void InputManager::UpdateAxisValue()
	{
		// TODO : 진짜 Axis 등록으로 인한 값으로 바꿔주어야 합니다. (BindAxis ..?)
		int right = GetKeyPressed(KeyCode::D) || GetKeyDown(KeyCode::D) ? 1 : 0;
		int left = GetKeyPressed(KeyCode::A) || GetKeyDown(KeyCode::A) ? 1 : 0;
		int up = GetKeyPressed(KeyCode::W) || GetKeyDown(KeyCode::W) ? 1 : 0;
		int down = GetKeyPressed(KeyCode::S) || GetKeyDown(KeyCode::S) ? 1 : 0;

		// 둘 다 눌렸으면 0, 라이트만만 눌렸으면 1, 레프트만 눌렸으면 -1
		int hor = static_cast<int>(right) - static_cast<int>(left);

		// 둘 다 눌렸거나 하나도 안 눌렸으면 0, 업만 눌렸으면 1, 다운만 눌렸으면 -1
		int ver = static_cast<int>(up) - static_cast<int>(down);

		if (hor == 1)
		{
			if (ver == 1)
			{
				_currAxisValue[0] = 0.5f;
				_currAxisValue[1] = 0.5f;
			}
			else if (ver == 0)
			{
				_currAxisValue[0] = 1.f;
				_currAxisValue[1] = 0.f;
			}
			else if (ver == -1)
			{
				_currAxisValue[0] = 0.5f;
				_currAxisValue[1] = -0.5f;
			}
		}
		else if (hor == 0)
		{
			if (ver == 1)
			{
				_currAxisValue[0] = 0.f;
				_currAxisValue[1] = 1.f;
			}
			else if (ver == 0)
			{
				_currAxisValue[0] = 0.f;
				_currAxisValue[1] = 0.0f;
			}
			else if (ver == -1)
			{
				_currAxisValue[0] = 0.f;
				_currAxisValue[1] = -1.0f;
			}
		}
		else if (hor == -1)
		{
			if (ver == 1)
			{
				_currAxisValue[0] = -0.5f;
				_currAxisValue[1] = 0.5f;
			}
			else if (ver == 0)
			{
				_currAxisValue[0] = -1.f;
				_currAxisValue[1] = 0.0f;
			}
			else if (ver == -1)
			{
				_currAxisValue[0] = -0.5f;
				_currAxisValue[1] = -0.5f;
			}
		}
	}

	bool InputManager::GetMouseStateRight(MouseCode mouseCode, KeyState keyState) const
	{
		return GetKeyStateRight(static_cast<KeyCode>(mouseCode), keyState);
	}

	float InputManager::GetAxis(InputAxis inputAxis)
	{
		return _currAxisValue[static_cast<unsigned int>(inputAxis)];
	}

	bool InputManager::GetKeyDown(KeyCode keyCode) const
	{
		return GetKeyStateRight(keyCode, KeyState::Down);
	}

	bool InputManager::GetKeyPressed(KeyCode keyCode) const
	{
		return GetKeyStateRight(keyCode, KeyState::Pressed);
	}

	bool InputManager::GetKeyUp(KeyCode keyCode) const
	{
		return GetKeyStateRight(keyCode, KeyState::Up);
	}

	bool InputManager::GetMouseButtonDown(MouseCode mouseCode) const
	{
		return GetMouseStateRight(mouseCode, KeyState::Down);
	}

	bool InputManager::GetMouseButtonPressed(MouseCode mouseCode) const
	{
		return GetMouseStateRight(mouseCode, KeyState::Pressed);
	}

	bool InputManager::GetMouseButtonUp(MouseCode mouseCode) const
	{
		return GetMouseStateRight(mouseCode, KeyState::Up);
	}

	const DUOLMath::Vector2& InputManager::GetMousePosition() const
	{
		return _currMousePos;
	}

	const DUOLMath::Vector2& InputManager::GetPrevMousePosition() const
	{
		return _prevMousePos;
	}

	const DUOLMath::Vector2& InputManager::GetMousePositionInScreen() const
	{
		return _currMousePosInScreen;
	}

	const DUOLMath::Vector2& InputManager::GetScreenPositionInScreen() const
	{
		return _currScreenPosInScreen;
	}

	const DUOLMath::Vector2& InputManager::GetPrevMousePositionInScreen() const
	{
		return _prevMousePosInScreen;
	}

	void InputManager::SetLockMode(bool value)
	{
		_isLockedMode = value;
	}

	void InputManager::SetGameLockMode(bool value)
	{
		_isLockedMode = value;

		_isGameLockedMode = value;
	}

	void InputManager::SetLockRect(const DUOLMath::Vector4& rect)
	{
		_lockRect = rect;
	}

	void InputManager::SetWindowSize(const DUOLMath::Vector2& resolution)
	{
		SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0, resolution.x, resolution.y, SWP_NOMOVE | SWP_NOZORDER);
		GraphicsManager::GetInstance()->SetScreenSize(resolution);

	}
}
