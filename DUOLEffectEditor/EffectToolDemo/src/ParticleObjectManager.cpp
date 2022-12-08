#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "ParticleRenderer.h"
#include "IGameEngine.h"
#include "ObjectManager.h"

ParticleObjectManager ParticleObjectManager::_instance;

ParticleObjectManager& ParticleObjectManager::Get()
{
	return _instance;
}

const std::vector<std::shared_ptr<Muscle::GameObject>>& ParticleObjectManager::GetParticleObjects()
{
	return _particleObjects;
}

std::shared_ptr<Muscle::GameObject>& ParticleObjectManager::CreateParticleObject()
{
	auto ParticleObject = Muscle::CreateGameObject();

	ParticleObject->AddComponent<Muscle::ParticleRenderer>();

	_particleObjects.push_back(ParticleObject);

	return ParticleObject;
}

void ParticleObjectManager::DeleteParticleObject(int index)
{
	_particleObjects.erase(_particleObjects.begin() + index);
}

void ParticleObjectManager::DeleteAllParticleObject()
{
	for (auto& iter : _particleObjects)
	{
		Muscle::IGameEngine::Get()->GetObjManager()->DeleteObject(iter);
	}
	_particleObjects.clear();
}
