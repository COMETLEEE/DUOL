#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Ű������ ��� Ű��� 1�� 1�� �����ϴ� enum class Parameter�Դϴ�.
	 */
	enum class KeyCode : int
	{
		// https://docs.unity3d.com/kr/2022.1/ScriptReference/KeyCode.html
	};

	/**
	 * \brief Ű���� ��� Ű���� ���¸� �� ������ �����մϴ�.
	 */
	enum class KeyState : int
	{
		None,
		Down,
		Pressed,
		Up
	};

	/**
	 * \brief ���콺 ��� Ű���� ���¸� �� ������ �����մϴ�.
	 */
	enum class MouseCode : int
	{
		Left,
		Middle,
		Right
	};

	/**
	 * \brief ���� ���� �� �Է°迡 ���� ���׵��� ����ϴ� �Ŵ���
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