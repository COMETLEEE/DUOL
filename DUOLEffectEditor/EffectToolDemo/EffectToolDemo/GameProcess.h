#pragma once



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

public:
	// �̱���ȭ
	static std::shared_ptr<GameProcess> Get();

	static std::shared_ptr<GameProcess> _gameProcess;

public:

	void Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY);

	void Finalize();

	void Update();

	void Render();

};
