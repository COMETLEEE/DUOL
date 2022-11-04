#pragma once



namespace Muscle
{
	class IGameEngine;
}

/// <summary>
/// 이 친구를 게임매니저라고 생각하자!
/// 게임 매니저 클래스를 만드려고 했는데 게임 프로세스 역할이 너무 적은것 같으니...
/// </summary>


class GameProcess
{
public:
	GameProcess() = default;

	~GameProcess();

private:
	std::shared_ptr<Muscle::IGameEngine> m_GameEngine;

public:
	// 싱글턴화
	static std::shared_ptr<GameProcess> Get();

	static std::shared_ptr<GameProcess> _gameProcess;

public:

	void Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY);

	void Update();

	void Render();

};
