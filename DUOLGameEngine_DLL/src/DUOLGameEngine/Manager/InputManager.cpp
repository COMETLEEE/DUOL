#include "DUOLGameEngine/Manager/InputManager.h"

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
	}

	void InputManager::UnInitialize()
	{

	}

	void InputManager::Update(float deltaTime)
	{
		UpdateMousePosition();

		UpdateAllKeyState();
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

	bool InputManager::GetMouseStateRight(MouseCode mouseCode, KeyState keyState) const
	{
		return GetKeyStateRight(static_cast<KeyCode>(mouseCode), keyState);
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
