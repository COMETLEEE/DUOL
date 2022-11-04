#pragma once
#include <XInput.h>

namespace Muscle
{
	enum class XPadInput
	{  
		Up,
		D_Up,

		Down,
		D_Down,

		Left,
		D_Left,

		Right,
		D_Right,

		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,

		Start,
		Back,

		LTRIGGER,
		RTRIGGER,

		XInputKeyMax
	};

	enum class XPadCommand
	{
		Release = 1,
		KeepPress = 2,
		Push = 3
	};

	/// <summary>
	/// XInput �е� �Է� �ڵ� Ŭ����
	/// 2022. 08. 23. ���Ŭ���� �� Almond�� �ҽ� �ڵ带 ������.
	/// XBox Pad Controller�� �޾ƿ��� ..!
	/// </summary>
	class XPad
	{
	private:
		// ��Ʈ�ѷ� üũ�� �����迭����. index 0, 1, 2.. = 1p, 2p, 3p...
		std::vector<std::vector<unsigned char>> _curKeyStates;
		std::vector<std::vector<unsigned char>> _oldKeyStates;
		std::vector<std::vector<unsigned char>> _commandStates;

		// ��Ʈ�ѷ� ��ƽ �̵� ��ǥ
		std::vector<SHORT> _curStickXPoint;
		std::vector<SHORT> _curStickYPoint;
		std::vector<float> _curnormalMagnitude;

		// ���
		int							  _userMaxCount;
		float						  _tickChecker;
		float						  _updateTime;

	private:
		static std::shared_ptr<XPad> _Instance;

	public:
		static std::shared_ptr<XPad> Get();
		
		XPad();

		~XPad();

		void UpdateXPadStates(float tick);

		bool IsPadStickMove(XPadInput input, int player = 0);

		bool IsPadCommand(XPadInput input, XPadCommand inputState, int player = 0);

		bool IsAnyButtonCommand(XPadCommand inputState, int player = 0);

		void StartVibration(WORD leftMotorSpeed = 32000, WORD rightMotorSpeed = 32000, int player = 0);

		void StopVibration(int player = 0);

		Vector2 GetStickDirection(int player = 0);

		float GetStickMagnitude(int player = 0) { return _curnormalMagnitude[player]; }

	private:
		static float CreateNormalizedMagnitude(XINPUT_STATE& state);

		static void CheckMovement(XINPUT_STATE& state, float& normalMagnitude, std::vector<unsigned char>& curkeyState, SHORT& stickX, SHORT& stickY);

		static void CheckButton(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState);

		static void CheckTrigger(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState);

		static void CheckDPAD(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState);

		static void CheckMenuButton(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState);

		static void CheckCommand(XINPUT_STATE& state,
			std::vector<unsigned char>& curkeyState, std::vector<unsigned char>& oldkeyState, std::vector<unsigned char>& commandState);

		static void SetZeroPadState(std::vector<unsigned char>& state);

		static void CopyPadState(std::vector<unsigned char>& source, std::vector<unsigned char>& target);
	};
}