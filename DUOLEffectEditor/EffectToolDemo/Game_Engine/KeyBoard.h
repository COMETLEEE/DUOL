#pragma once

namespace Muscle
{
#define UP 1
#define PRESS 2
#define DOWN 3

class KeyBoard
{
public:
	KeyBoard() = default;

	~KeyBoard() = default;

private:

	HWND m_hWnd;

	static std::shared_ptr<KeyBoard> Instance;

	unsigned char cur[256];//현재 키

	unsigned char old[256];//이전의 키

	unsigned char map[256];//현재 키의 상태. Press, Down, Up

	void KeyCheck();

public:

	static std::shared_ptr<KeyBoard> Get();

	//마우스 위치 반환
	const Vector2 GetPos();

	void Update();

	bool KeyDown(unsigned char _Key) { return map[_Key] == DOWN ? true : false; }

	bool KeyUp(unsigned char _Key) { return map[_Key] == UP ? true : false; }

	bool KeyPress(unsigned char _Key) { return map[_Key] == PRESS ? true : false; }

};
}

