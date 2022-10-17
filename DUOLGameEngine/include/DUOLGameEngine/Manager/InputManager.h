#pragma once
#include "DUOLGameEngine/Util/Defines.h"


namespace DUOLGameEngine
{
	/**
	 * \brief Ű������ ��� Ű��� 1�� 1�� �����ϴ� enum class Parameter�Դϴ�.
	 */
	enum class KeyCode : int
	{
		None = 0x00,
		Backspace = 0x08,
		Delete = 0x2E,
		Tab = 0x09,
		Clear = 0x0C,
		Return = 0x0D,
		Pause = 0x13,
		Escape = 0x1B,
		Space = 0x20,

		/**
		 * \brief Numeric Keypad number.
		 */
		Keypad0 = 0x60,
		Keypad1 = 0x61,
		Keypad2 = 0x62,
		Keypad3 = 0x63,
		Keypad4 = 0x64,
		Keypad5 = 0x65,
		Keypad6 = 0x66,
		Keypad7 = 0x67,
		Keypad8 = 0x68,
		Keypad9 = 0x69,
		KeypadPeriod = 0xBE,
		KeypadDivide = 0x6F,
		KeypadMultiply = 0x6A,
		KeypadMinus = 0x6D,
		KeypadPlus = 0x6B,
		// KeypadEnter,
		// KeypadEquals,

		UpArrow = 0x26,
		DownArrow = 0x28,
		RightArrow = 0x27,
		LeftArrow = 0x25,
		Insert = 0x2D,
		Home = 0x24,
		End = 0x23,
		PageUp = 0x21,
		PageDown = 0x22,

		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,

		/**
		 * \brief Alpha Numeric Keyboard number.
		 */
		Alpha0 = 0x30,
		Alpha1 = 0x31,
		Alpha2 = 0x32,
		Alpha3 = 0x33,
		Alpha4 = 0x34,
		Alpha5 = 0x35,
		Alpha6 = 0x36,
		Alpha7 = 0x37,
		Alpha8 = 0x38,
		Alpha9 = 0x39,

		/**
		 * \brief Alphabet.
		 */
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		Numlock = 0x90,
		CapsLock = 0x14,
		ScrollLock = 0x91,
		RightShift = 0xA1,
		LeftShift = 0xA0,
		RightControl = 0xA3,
		LeftControl = 0xA2,
		RightAlt = 0xA5,
		LeftAlt = 0xA4
	};

	/**
	 * \brief Ű���� ��� Ű���� ���¸� �� ������ �����մϴ�.
	 */
	enum class KeyState : int
	{
		/**
		 * \brief �ƹ� �ϵ� �����ϴ�.
		 */
		None,

		/**
		 * \brief �ش� �����ӿ� Ű�� ���Ƚ��ϴ�.
		 */
		Down,

		/**
		 * \brief Ű�� ���� �����Ӻ��� ��� �����ִ� �����Դϴ�.
		 */
		Pressed,

		/**
		 * \brief �ش� �����ӿ� Ű�� �ö�Խ��ϴ�.
		 */
		Up
	};

	/**
	 * \brief ���콺 ��� Ű���� ���¸� �� ������ �����մϴ�.
	 */
	enum class MouseCode : int
	{
		/**
		 * \brief ���콺 ���� ��ư
		 */
		Left = 0x01,

		/**
		 * \brief ���콺 �߰� ��ư
		 */
		Middle = 0x04,

		/**
		 * \brief ���콺 ������ ��ư
		 */
		Right = 0x02
	};

	/**
	 * \brief WinAPI���� �����ϴ� ���� Ű�ڵ��� ����
	 */
	constexpr int KEY_COUNT = 256;

	/**
	 * \brief ���� ���� �� �Է°迡 ���� ���׵��� ����ϴ� �Ŵ���
	 */
	class InputManager
	{
	public:
		InputManager();

		~InputManager();

		DELETE_COPY_MOVE(InputManager)

		/**
		 * \brief �Ŵ��� �ʱ�ȭ �Լ�
		 */
		void Initialize(HWND hWnd);

		/**
		 * \brief �Ŵ��� �ʱ�ȭ ���� �Լ�
		 */
		void UnInitialize();

	private:
		HWND _hWnd;

		unsigned char _prevKeyboardBuffer[KEY_COUNT];
		
		unsigned char _currKeyboardBuffer[KEY_COUNT];
		
		KeyState _currKeyStates[KEY_COUNT];

		DUOLMath::Vector2 _prevMousePos;

		DUOLMath::Vector2 _currMousePos;

	private:
		void Update(float deltaTime);

		/**
		 * \brief ��� ���� Ű�� ���ؼ� ���� Ű ���¸� ������Ʈ�մϴ�.
		 */
		void UpdateAllKeyState();

		/**
		 * \brief ���콺�� ��ġ�� ������Ʈ�մϴ�.
		 */
		void UpdateMousePosition();

		inline bool GetKeyStateRight(KeyCode keyCode, KeyState keyState) const
		{
			return _currKeyStates[static_cast<int>(keyCode)] == keyState;
		}

		[[nodiscard]]
		bool GetMouseStateRight(MouseCode mouseCode, KeyState keyState) const;

	public:
		[[nodiscard]]
		bool GetKeyDown(KeyCode keyCode) const;

		[[nodiscard]]
		bool GetKeyPressed(KeyCode keyCode) const;

		[[nodiscard]]
		bool GetKeyUp(KeyCode keyCode) const;

		[[nodiscard]]
		bool GetMouseButtonDown(MouseCode mouseCode) const;

		[[nodiscard]]
		bool GetMouseButtonPressed(MouseCode mouseCode) const;

		[[nodiscard]]
		bool GetMouseButtonUp(MouseCode mouseCode) const;

		[[nodiscard]]
		const DUOLMath::Vector2& GetMousePosition() const;
	};
}