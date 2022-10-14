#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 키보드의 모든 키들과 1대 1로 대응하는 enum class Parameter입니다.
	 */
	enum class KeyCode : int
	{
		// https://docs.unity3d.com/kr/2022.1/ScriptReference/KeyCode.html
	};

	/**
	 * \brief 키보드 모든 키들의 상태를 매 프레임 관리합니다.
	 */
	enum class KeyState : int
	{
		None,
		Down,
		Pressed,
		Up
	};

	/**
	 * \brief 마우스 모든 키들의 상태를 매 프레임 관리합니다.
	 */
	enum class MouseCode : int
	{
		Left,
		Middle,
		Right
	};

	/**
	 * \brief 게임 진행 중 입력계에 대한 사항들을 담당하는 매니저
	 */
	class InputManager
	{
	public:
		InputManager() {};

		~InputManager() {};

		DELETE_COPY_MOVE(InputManager)

	private:
	
	public:
		bool GetKeyDown(KeyCode keyCode) const;

		bool GetKeyPressed(KeyCode keyCode) const;

		bool GetKeyUp(KeyCode keyCode) const;

		bool GetMouseButtonDown(MouseCode mouseCode) const;

		bool GetMouseButtonPressed(MouseCode mouseCode) const;

		bool GetMouseButtonUp(MouseCode mouseCode) const;
	};
}