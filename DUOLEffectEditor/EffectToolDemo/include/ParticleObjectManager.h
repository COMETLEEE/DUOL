#pragma once
/**

	@class   ParticleObjectManager
	@brief   ��ƼŬ ������Ʈ���� ���� ������Ʈ �Ŵ���
	@details ~ ������Ʈ �� ������ ������Ʈ�� ���̺� �ε带 �� �� �� ��ƼŬ ������Ʈ�� ������ �� ����� ������Ʈ �Ŵ����̴�.

**/
#include <vector>
#include <memory>
namespace Muscle
{
	class GameObject;
}

class ParticleObjectManager
{
public:
	ParticleObjectManager(const ParticleObjectManager& other) = delete;

	ParticleObjectManager& operator =(const ParticleObjectManager& other) = delete;

	ParticleObjectManager(ParticleObjectManager&& other) = delete;

protected:
	ParticleObjectManager() = default;

	~ParticleObjectManager() = default;

public:
	static ParticleObjectManager& Get();

private:
	static ParticleObjectManager _instance;

	std::vector<std::shared_ptr<Muscle::GameObject>> _particleObjects;

public:
	const std::vector<std::shared_ptr<Muscle::GameObject>>& GetParticleObjects();

	std::shared_ptr<Muscle::GameObject>& CreateParticleObject();

	void DeleteParticleObject(int index);
};

