#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "ParticleRenderer.h"
#include "IGameEngine.h"
#include "ObjectManager.h"
#include "LogSystem.h"

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
	WriteLog("[Log] CreateNewObject \n");

	auto ParticleObject = Muscle::CreateGameObject();

	ParticleObject->AddComponent<Muscle::ParticleRenderer>();

	_particleObjects.push_back(ParticleObject);

	return ParticleObject;
}

std::shared_ptr<Muscle::GameObject>& ParticleObjectManager::CreateParticleObjectFromParticleData(MuscleGrapics::RenderingData_Particle& data, std::shared_ptr<Muscle::GameObject> parent)
{
	auto ParticleObject = Muscle::CreateGameObject();

	auto particleData = ParticleObject->AddComponent<Muscle::ParticleRenderer>()->GetParticleData();

	if (parent)
		ParticleObject->SetParent(parent);

	for (auto iter : data._childrens)
	{
		CreateParticleObjectFromParticleData(iter, ParticleObject);

		auto particleData = ParticleObject->AddComponent<Muscle::ParticleRenderer>()->GetParticleData();
	}

	data._commonInfo._firstRun = true;

	data._objectID = ParticleObject->GetObjectID();

	*particleData = data;

	std::vector<MuscleGrapics::RenderingData_Particle>().swap(data._childrens);

	_particleObjects.push_back(ParticleObject);

	return ParticleObject;
}

void ParticleObjectManager::DeleteParticleObject(int index)
{
	_particleObjects.erase(_particleObjects.begin() + index);
}

void ParticleObjectManager::DeleteAllParticleObject()
{
	WriteLog("[Log] ClearAllObject \n");

	for (auto& iter : _particleObjects)
	{
		Muscle::IGameEngine::Get()->GetObjManager()->DeleteObject(iter);
	}
	_particleObjects.clear();
}
