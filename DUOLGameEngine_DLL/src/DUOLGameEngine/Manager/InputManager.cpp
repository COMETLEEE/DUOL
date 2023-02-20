#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLGameEngine
{
	InputManager::InputManager() :
		_hWnd(NULL)
		, _prevKeyboardBuffer{}
		, _currKeyboardBuffer{}
		, _currKeyStates{}
		, _prevMousePos(DUOLMath::Vector2::Zero)
		, _currMousePos(DUOLMath::Vector2::Zero)
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
		UpdateMousePosition();

		UpdateAllKeyState();

		UpdateAxisValue();
	}

	void InputManager::UpdateAllKeyState()
	{
		memcpy(_prevKeyboardBuffer, _currKeyboardBuffer, sizeof(unsigned char) * KEY_COUNT);

		if (GetKeyboardState(_currKeyboardBuffer) == 0)
		{
			// TODO
			// Keyboard�� ���¸� �ҷ����� �� �߽��ϴ�.
		}

		/// <summary>
		/// 0x8000 : ������ ���� ���� ���� ȣ�� ������ Ű�� ���� ����
		/// </summary>
		for (int i = 0 ; i < KEY_COUNT ; i++)
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

		// ���� ���� ���콺 ��ġ�� ���� ��ġ�� ������Ʈ�մϴ�.
		memcpy(&_prevMousePos, &_currMousePos, sizeof(DUOLMath::Vector2));

		memcpy(&_prevMousePosInScreen, &_currMousePosInScreen, sizeof(DUOLMath::Vector2));

		// ���� ���콺 ��ġ�� ������Ʈ�մϴ�.
		GetCursorPos(&cursorPos);

		_currMousePosInScreen = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

		ScreenToClient(_hWnd, &cursorPos);

		_currMousePos = DUOLMath::Vector2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
	}

	void InputManager::UpdateAxisValue()
	{
		// TODO : ��¥ Axis ������� ���� ������ �ٲ��־�� �մϴ�.
		bool left = GetKeyPressed(KeyCode::LeftArrow);
		bool right = GetKeyPressed(KeyCode::RightArrow);
		bool up = GetKeyPressed(KeyCode::UpArrow);
		bool down = GetKeyPressed(KeyCode::DownArrow);

		// �� �� �������� 0, ����Ʈ�� �������� 1, ����Ʈ�� �������� -1
		int hor = static_cast<int>(right) - static_cast<int>(left);

		// �� �� ���Ȱų� �ϳ��� �� �������� 0, ���� �������� 1, �ٿ �������� -1
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

	const DUOLMath::Vector2& InputManager::GetPrevMousePositionInScreen() const
	{
		return _prevMousePosInScreen;
	}
}
