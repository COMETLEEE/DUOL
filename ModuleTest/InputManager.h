#pragma once
#include <map>
#include <memory>
#include <windows.h>

enum class KeyState
{
	UP,
	DOWN,
	STAY,
	TOGGLE
};

struct Vector
{
	int x;
	int y;
};

/**
 * \brief �̱������� ��� ��ǲ�� �����ϴ� ��ǲ �Ŵ���.
 *
 * 2022.08.01 Dev.BlackFoot
 */
class InputManager final
{
private:
	// Key
	std::map<int, bool> prevKeyState;
	std::map<int, bool> currKeyState;

	// Cursor Coordinate
	Vector prevCursorXY;
	Vector currCursorXY;

	// inputManger ����ƽ ����
	static std::shared_ptr<InputManager> instance;

public:
	// ������ Ȱ��ȭ ���� Input �Ÿ���
	bool isMouseInWindow;
	bool isWindowActivated;

public:
	InputManager();
	~InputManager();
	// Key Input
	void Update();
	void LateUpdate();
	virtual bool GetInputState(int key, KeyState state);

	// Mouse Pos
	void SetMousePos(float x, float y);
	const Vector& GetMousePos();
	Vector GetMouseMoveVector();

	static std::shared_ptr<InputManager> GetInstance();
};