/**

    @file      InputManager.h
    @brief     Input Management Class
    @details   ~
    @author    COMETLEE
    @date      17.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 키보드의 모든 키들과 1대 1로 대응하는 enum class Parameter입니다.
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
	 * \brief 키보드 모든 키들의 상태를 매 프레임 관리합니다.
	 * => 비트 배타적 논리합을 통해서 좀 더 효율적으로 관리할 수 있지 않을까 ..?
	 */
	enum class KeyState : int
	{
		/**
		 * \brief 아무 일도 없습니다.
		 */
		None,

		/**
		 * \brief 해당 프레임에 키가 눌렸습니다.
		 */
		Down,

		/**
		 * \brief 키가 지난 프레임부터 계속 눌려있는 상태입니다.
		 */
		Pressed,

		/**
		 * \brief 해당 프레임에 키가 올라왔습니다.
		 */
		Up
	};

	/**
	 * \brief 마우스 모든 키들의 상태를 매 프레임 관리합니다.
	 */
	enum class MouseCode : int
	{
		/**
		 * \brief 마우스 왼쪽 버튼
		 */
		Left = 0x01,

		/**
		 * \brief 마우스 중간 버튼
		 */
		Middle = 0x04,

		/**
		 * \brief 마우스 오른쪽 버튼
		 */
		Right = 0x02
	};

	enum class InputAxis : int
	{
		Horizontal = 0
		, Vertical = 1
	};

	/**
	 * \brief WinAPI에서 지원하는 가상 키코드의 갯수
	 */
	constexpr int KEY_COUNT = 256;

	/**
	 * \brief 게임 진행 중 입력계에 대한 사항들을 담당하는 매니저
	 */
	class DUOL_GAMEENGINE_API InputManager : public SingletonBase<InputManager>
	{
		friend class Engine;

		DECLARE_SINGLETON(InputManager)

		DELETE_COPY_MOVE(InputManager)

	private:
		virtual ~InputManager() override;

	public:
		/**
		 * \brief 매니저 초기화 함수
		 */
		void Initialize(HWND hWnd);

		/**
		 * \brief 매니저 초기화 해제 함수
		 */
		void UnInitialize();

	private:
		HWND _hWnd;

		unsigned char _prevKeyboardBuffer[KEY_COUNT];
		
		unsigned char _currKeyboardBuffer[KEY_COUNT];
		
		KeyState _currKeyStates[KEY_COUNT];

		DUOLMath::Vector2 _prevMousePos;

		DUOLMath::Vector2 _prevMousePosInScreen;

		DUOLMath::Vector2 _currMousePos;

		DUOLMath::Vector2 _currMousePosInScreen;

		DUOLMath::Vector2 _currScreenPosInScreen;

		// 0 : Horizontal, 1 : Vertical
		float _currAxisValue[2];

		bool _isLockedMode;

		bool _isGameLockedMode;

		// Lock Mode Rect. (left, top, right, bottom)
		DUOLMath::Vector4 _lockRect;

		// UIMode Mouse Control
		bool _isUIMode;

		bool _isCameraMode;
		
	private:
		void Update(float deltaTime);

		/**
		 * \brief 모든 가상 키에 대해서 현재 키 상태를 업데이트합니다.
		 */
		void UpdateAllKeyState();

		/**
		 * \brief 마우스의 위치를 업데이트합니다.
		 */
		void UpdateMousePosition();

		/**
		 * \brief 마우스의 위치를 업데이트합니다. (For Lock Mode)
		 */
		void UpdateLockModeMousePosition();

		/**
		 * \brief 축에 대한 입력값을 업데이트합니다.
		 */
		void UpdateAxisValue();

		inline bool GetKeyStateRight(KeyCode keyCode, KeyState keyState) const
		{
			return _currKeyStates[static_cast<int>(keyCode)] == keyState;
		}

		[[nodiscard]]
		bool GetMouseStateRight(MouseCode mouseCode, KeyState keyState) const;

	public:
		[[nodiscard]]
		float GetAxis(InputAxis inputAxis);

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

		[[nodiscard]]
		const DUOLMath::Vector2& GetPrevMousePosition() const;

		[[nodiscard]]
		const DUOLMath::Vector2& GetMousePositionInScreen() const;

		[[nodiscard]]
		const DUOLMath::Vector2& GetScreenPositionInScreen() const;

		[[nodiscard]]
		const DUOLMath::Vector2& GetPrevMousePositionInScreen() const;

		void SetLockMode(bool value);

		void SetGameLockMode(bool value);

		void SetLockRect(const DUOLMath::Vector4& rect);

		void SetWindowSize(const DUOLMath::Vector2& resolution);

		void SetUIMouseMode(bool value);

		void ShowCursorI(bool value);
	};
}