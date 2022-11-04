#include "pch.h"
#include "XPad.h"

using namespace Muscle;

std::shared_ptr<Muscle::XPad> XPad::_Instance = nullptr;

XPad::XPad()
{	
	_userMaxCount = 1;	// �ϴ� 1�ο��� �ٲٰ������ ������ �Ű������� ���ô��� .. �Ͻʽÿ� ..
	_oldKeyStates.resize(_userMaxCount);
	_curKeyStates.resize(_userMaxCount);
	_commandStates.resize(_userMaxCount);
	_curStickXPoint.resize(_userMaxCount);
	_curStickYPoint.resize(_userMaxCount);
	_curnormalMagnitude.resize(_userMaxCount);

	// XInput ��Ʈ�ѷ� Ű �Է� ���� �߰����� �ʱ�ȭ
	std::vector<unsigned char> inputkey_ZeroContainer;

	int inputkeyNumber = (int)XPadInput::XInputKeyMax;

	inputkey_ZeroContainer.resize(inputkeyNumber);

	for (int key = 0; key < inputkeyNumber; key++)
	{
		inputkey_ZeroContainer[key] = 0;
	}

	for (int pl = 0; pl < _userMaxCount; pl++)
	{
		_oldKeyStates[pl] = inputkey_ZeroContainer;
		_curKeyStates[pl] = inputkey_ZeroContainer;
		_commandStates[pl] = inputkey_ZeroContainer;
		_curStickXPoint[pl] = 0;
		_curStickYPoint[pl] = 0;
		_curnormalMagnitude[pl] = 0.0f;
	}

	_tickChecker = 0.0f;

	_updateTime = 0.0f;
}

XPad::~XPad()
{
	for (auto& keyStates : _curKeyStates)
	{
		keyStates.clear();
	}

	for (auto& keyStates : _oldKeyStates)
	{
		keyStates.clear();
	}

	for (auto& keyStates : _commandStates)
	{
		keyStates.clear();
	}
	
	_curKeyStates.clear();
	_oldKeyStates.clear();
	_commandStates.clear();
}

void XPad::UpdateXPadStates(float tick)
{
	//m_tickChecker += tick;
	//if (m_tickChecker < m_updateTime)
	//	return;

	for (int pl = 0; pl < _userMaxCount; pl++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(pl, &state) != ERROR_SUCCESS)
			return;

		CopyPadState(_curKeyStates[pl], _oldKeyStates[pl]);
		SetZeroPadState(_curKeyStates[pl]);
		SetZeroPadState(_commandStates[pl]);

		_curnormalMagnitude[pl] = CreateNormalizedMagnitude(state);

		// �е� �Է� üũ
		CheckMovement(state, _curnormalMagnitude[pl], _curKeyStates[pl], _curStickXPoint[pl], _curStickYPoint[pl]);
		CheckButton(state, _curKeyStates[pl]);
		CheckTrigger(state, _curKeyStates[pl]);
		CheckDPAD(state, _curKeyStates[pl]);

		// ���ſ� ���縦 ���ؼ� Pressed���� �ƴ��� �� üũ
		CheckCommand(state, _curKeyStates[pl], _oldKeyStates[pl], _commandStates[pl]);
	}

	//m_tickChecker = 0.0f;
	//m_updateTime = tick;
}

// padstate�� ����� �������� �ʰ�, ������ �� �������� 0���� �ʱ�ȭ���ش�.
void XPad::SetZeroPadState(std::vector<unsigned char>& state)
{
	fill(state.begin(), state.end(), 0);
}

// padstate�� ����� �������� �ʰ�, �ҽ� ���� �� �������� target�� �������ش�.
void XPad::CopyPadState(std::vector<unsigned char>& source, std::vector<unsigned char>& target)
{
	SetZeroPadState(target);

	copy(source.begin(), source.end(), target.begin());
}

// �Ƴ��α� ��ƽ �������� ���� normalized magnitude ��ȯ
float XPad::CreateNormalizedMagnitude(XINPUT_STATE& state)
{
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX * LX + LY * LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	float inputDeadZone = 25000;

	//check if the controller is outside a circular dead zone
	if (magnitude > inputDeadZone)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= inputDeadZone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - inputDeadZone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return normalizedMagnitude;
}

// �̵�üũ, �̵��� �Է¿� ������ �߰������� üũ����.
void XPad::CheckMovement(XINPUT_STATE& state, float& normalMagnitude, std::vector<unsigned char>& curkeyState, SHORT& stickX, SHORT& stickY)
{
	stickX = state.Gamepad.sThumbLX;
	stickY = state.Gamepad.sThumbLY;

	double radian = atan2((double)stickY,(double)stickX);
	float stickdegree = (float)radian * 57.32f;

	// 0 ~ 360
	if (stickdegree > 360.0f)
		stickdegree -= 360.0f;
	else if (stickdegree < 0.f)
		stickdegree += 360.f;

	// Right
	if ((state.Gamepad.sThumbLY > 0 && normalMagnitude > 0.99f && (stickdegree < 67.5f) || (stickdegree > 292.5f)))
		curkeyState[(int)XPadInput::Right] = 1;
	else
		curkeyState[(int)XPadInput::Right] = 0;

	// Up
	if ((state.Gamepad.sThumbLY > 0 && normalMagnitude > 0.99f && 22.5f < stickdegree && stickdegree < 157.5f))
		curkeyState[(int)XPadInput::Up] = 1;
	else
		curkeyState[(int)XPadInput::Up] = 0;

	// Down
	if ((state.Gamepad.sThumbLY < 0 && normalMagnitude > 0.99f && 202.5f < stickdegree && 337.5f > stickdegree))
		curkeyState[(int)XPadInput::Down] = 1;
	else
		curkeyState[(int)XPadInput::Down] = 0;

	// Left
	if ((state.Gamepad.sThumbLX < 0 && normalMagnitude > 0.99f && 112.5f < stickdegree && 247.5f > stickdegree))
		curkeyState[(int)XPadInput::Left] = 1;
	else
		curkeyState[(int)XPadInput::Left] = 0;
}

Vector2 XPad::GetStickDirection(int player)
{
	if ((_curStickXPoint[player] == 0) && (_curStickYPoint[player] == 0))
	{
		return Vector2::Zero;
	}
	else
	{
		Vector2 temp = Vector2(static_cast<float>(_curStickXPoint[player]), static_cast<float>(_curStickYPoint[player]));

		temp.Normalize();

		return temp;
	}
}

// ��ưüũ, ��ư�� �Է��� ������ �߰������� üũ����. LR�� ������. �ʿ��ϸ� �߰�.
void XPad::CheckButton(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState)
{
	// A��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		curkeyState[(int)XPadInput::ButtonA] = 1;
	else
		curkeyState[(int)XPadInput::ButtonA] = 0;

	// B��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		curkeyState[(int)XPadInput::ButtonB] = 1;
	else
		curkeyState[(int)XPadInput::ButtonB] = 0;

	// X��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		curkeyState[(int)XPadInput::ButtonX] = 1;
	else
		curkeyState[(int)XPadInput::ButtonX] = 0;

	// Y��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		curkeyState[(int)XPadInput::ButtonY] = 1;
	else
		curkeyState[(int)XPadInput::ButtonY] = 0;

	// Start��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		curkeyState[(int)XPadInput::Start] = 1;
	else
		curkeyState[(int)XPadInput::Start] = 0;

	// Back��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		curkeyState[(int)XPadInput::Back] = 1;
	else
		curkeyState[(int)XPadInput::Back] = 0;
}

void XPad::CheckCommand(XINPUT_STATE& state,
	std::vector<unsigned char>& curkeyState, std::vector<unsigned char>& oldkeyState, std::vector<unsigned char>& commandState)
{
	for (int key = 0; key < (int)XPadInput::XInputKeyMax; key++)
	{
		if (curkeyState[key] && oldkeyState[key])			// ���� �����̳� Ű�� ��������.
			commandState[key] = (int)XPadCommand::KeepPress;
		else if (curkeyState[key] && !(oldkeyState[key]))	// ������ �ȴ��Ⱦ��µ�, ���� ��������.
			commandState[key] = (int)XPadCommand::Push;
		else if (!(curkeyState[key]) && oldkeyState[key])	// ������ �����µ�, ���� �ȴ�������.
			commandState[key] = (int)XPadCommand::Release;
		else
			commandState[key] = 0;
	}
}

// �е忡 �Է��� �ִ��� üũ.
// XPadInput = ��ƽ, �е�, ��ư�� ������� ����
// XpadCommand = � �Է��ൿ����.. Push�� ������, KeepPress�� ������ �ִ°�, Release�� ����
bool XPad::IsPadCommand(XPadInput input, XPadCommand inputState, int player)
{
	return (_commandStates[player][(int)input] == (int)inputState) ? true : false;
}

bool XPad::IsPadStickMove(XPadInput input, int player)
{
	return (bool)_curKeyStates[player][(int)input];
}

// ��ü��ư�� �Է� üũ. � ��ư�̶�, XpadCommand�� �ൿ�� ������ true ����
bool XPad::IsAnyButtonCommand(XPadCommand inputState, int player)
{
	for (int button = (int)XPadInput::ButtonA; button < (int)XPadInput::XInputKeyMax; button++)
	{
		if (_commandStates[player][button] == (int)inputState)
			return true;
	}
	return false;
}

// ���� ����
void XPad::StartVibration(WORD leftMotorSpeed, WORD rightMotorSpeed, int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorSpeed; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = rightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}

// ���� ��
void XPad::StopVibration(int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 0; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}

std::shared_ptr<Muscle::XPad> XPad::Get()
{
	if (_Instance == nullptr)
		_Instance = std::make_shared<XPad>();

	return _Instance;
}

void XPad::CheckTrigger(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState)
{
	// LTRIGGER
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		curkeyState[(int)XPadInput::LTRIGGER] = 1;
	else
		curkeyState[(int)XPadInput::LTRIGGER] = 0;

	// RTRIGGER
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		curkeyState[(int)XPadInput::RTRIGGER] = 1;
	else
		curkeyState[(int)XPadInput::RTRIGGER] = 0;
}

void XPad::CheckDPAD(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState)
{
	// D_LEFT
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		curkeyState[(int)XPadInput::D_Left] = 1;
	else
		curkeyState[(int)XPadInput::D_Left] = 0;

	// D_RIGHT
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		curkeyState[(int)XPadInput::D_Right] = 1;
	else
		curkeyState[(int)XPadInput::D_Right] = 0;

	// D_UP
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		curkeyState[(int)XPadInput::D_Up] = 1;
	else
		curkeyState[(int)XPadInput::D_Up] = 0;

	// D_DOWN
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		curkeyState[(int)XPadInput::D_Down] = 1;
	else
		curkeyState[(int)XPadInput::D_Down] = 0;
}

void XPad::CheckMenuButton(XINPUT_STATE& state, std::vector<unsigned char>& curkeyState)
{
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		curkeyState[(int)XPadInput::Start] = 1;
	else
		curkeyState[(int)XPadInput::Start] = 0;
}