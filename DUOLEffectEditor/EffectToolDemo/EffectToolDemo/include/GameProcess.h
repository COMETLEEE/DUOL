#pragma once


#include <Windows.h>
#include <memory>

namespace Muscle
{
	class IGameEngine;
}

/// <summary>
/// �� ģ���� ���ӸŴ������ ��������!
/// ���� �Ŵ��� Ŭ������ ������� �ߴµ� ���� ���μ��� ������ �ʹ� ������ ������...
/// </summary>


class GameProcess
{
public:
	GameProcess() = default;

	~GameProcess();

private:
	std::shared_ptr<Muscle::IGameEngine> m_GameEngine;

	HWND _hwnd;

public:
	// �̱���ȭ
	static std::shared_ptr<GameProcess> Get();

	static std::shared_ptr<GameProcess> _gameProcess;

	HWND GetHWND();
public:

	void Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY);

	void Finalize();

	void Update();

	void Render();

};
