#pragma once
/**

	@class   ParticleObjectManager
	@brief   ��ƼŬ ������Ʈ���� ���� ������Ʈ �Ŵ���
	@details ~ ������Ʈ �� ������ ������Ʈ�� ���̺� �ε带 �� �� �� ��ƼŬ ������Ʈ�� ������ �� ����� ������Ʈ �Ŵ����̴�.

**/
#include <unordered_map>
#include <memory>

namespace Muscle
{
	class GameObject;
}
namespace MuscleGrapics
{
	struct RenderingData_Particle;
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

	std::unordered_map<unsigned int, std::shared_ptr<Muscle::GameObject>> _particleObjects;

public:
	const std::unordered_map<unsigned int, std::shared_ptr<Muscle::GameObject>>& GetParticleObjects();

	std::shared_ptr<Muscle::GameObject>& CreateParticleObject();

	std::shared_ptr<Muscle::GameObject>& CreateParticleObjectFromParticleData(MuscleGrapics::RenderingData_Particle& data, std::shared_ptr<Muscle::GameObject> parent = nullptr);

	void DeleteParticleObject(unsigned int index);

	void DeleteAllParticleObject();
};

