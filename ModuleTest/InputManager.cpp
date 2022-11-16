#include "InputManager.h"

std::shared_ptr<InputManager> InputManager::instance = nullptr;

InputManager::InputManager()
	: prevCursorXY{}
	, prevKeyState{}
{
	isMouseInWindow = false;
	isWindowActivated = true;
}

InputManager::~InputManager()
{
}

// 키 입력 확인함수.
void InputManager::Update()
{
	// 키 상태 복사
	for (auto& iter : currKeyState)
		prevKeyState[iter.first] = iter.second;

	prevCursorXY = currCursorXY;

	// 사용하는 키 목록 전체를 갱신한다.
		// 첫 프레임에는 비어있어서 동작하지 않는다.
	for (auto& iter : currKeyState)
	{
		if ((GetAsyncKeyState(iter.first) & 0x8000) != 0)
			currKeyState[iter.first] = true;
		else
			currKeyState[iter.first] = false;
	}
}

void InputManager::LateUpdate()
{
}

// 현재 프레임에서의 키 상태.
bool InputManager::GetInputState(int key, KeyState state)
{
	if (isWindowActivated != true)
		return false;

	switch (state)
	{

	case KeyState::UP:
	{
		// 키를 눌렀다가 때는 타이밍에 True를 반환한다.
		return currKeyState[key] ? false : (prevKeyState[key] ? true : false);
	}

	case KeyState::DOWN:
	{
		// 키를 누르는 타이밍에 True를 반환한다.
		return prevKeyState[key] ? false : (currKeyState[key] ? true : false);
	}

	case KeyState::STAY:
	{
		// 키를 누르고 있으면 True를 반환한다.
		return currKeyState[key];
	}

	case KeyState::TOGGLE:
	{
		// 키를 누를 때마다 On/Off 된다.
		return GetKeyState(key) & 0x0001;
	}

	}

	return false;
}

// 마우스 좌표 설정.
void InputManager::SetMousePos(float x, float y)
{
	currCursorXY.x = x;
	currCursorXY.y = y;
}

// 마우스 위치 얻어오기.
const Vector& InputManager::GetMousePos()
{
	return currCursorXY;
}

// 마우스가 움직인 벡터
Vector InputManager::GetMouseMoveVector()
{
	return Vector
	{
		currCursorXY.x - prevCursorXY.x,
		currCursorXY.y - prevCursorXY.y
	};
}

std::shared_ptr<InputManager> InputManager::GetInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<InputManager>();

	return instance;
}