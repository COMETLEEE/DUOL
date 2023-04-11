#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "ParticleRenderer.h"
#include "IGameEngine.h"
#include "ObjectManager.h"
#include "LogSystem.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Commands.h"
ParticleObjectManager ParticleObjectManager::_instance;

ParticleObjectManager& ParticleObjectManager::Get()
{
	return _instance;
}

const std::unordered_map<unsigned int, std::shared_ptr<Muscle::GameObject>>& ParticleObjectManager::GetParticleObjects()
{
	return _particleObjects;
}

std::shared_ptr<Muscle::GameObject>& ParticleObjectManager::CreateParticleObject()
{
	WriteLog("[Log] CreateNewObject \n");

	auto ParticleObject = Muscle::CreateGameObject();

	EXCUTE(new ObjectCreateCommand(ParticleObject));

	return ParticleObject;
}

void ParticleObjectManager::CreateParticleObjectFromParticleData(std::vector<MuscleGrapics::RenderingData_Particle>& datas)
{
	std::vector<std::shared_ptr<Muscle::GameObject>> gameObjects;

	gameObjects.resize(datas.size());

	for (auto& iter : gameObjects)
	{
		iter = CreateParticleObject();
	}

	for (int i = 0; i < datas.size(); i++)
	{
		auto& particleData = gameObjects[i]->GetComponent<Muscle::ParticleRenderer>()->GetParticleData();

		*particleData = datas[i];

		for (auto childID : particleData->_childrenIDs)
		{
			gameObjects[i]->SetChild(gameObjects[childID]);
		}

		particleData->_commonInfo._firstRun = true;

		particleData->_objectID = gameObjects[i]->GetObjectID();

		particleData->_childrenIDs.clear();
	}

	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		gameObjects[i]->GetTransform()->SetXMWorldTM(datas[i]._commonInfo._transformMatrix);
	}
}

std::shared_ptr<Muscle::GameObject>& ParticleObjectManager::CreateParticleObjectFromPrevParticleData(PrevVersion::RenderingData_Particle& data, std::shared_ptr<Muscle::GameObject> parent)
{
	auto ParticleObject = CreateParticleObject();

	auto particleData = ParticleObject->GetComponent<Muscle::ParticleRenderer>()->GetParticleData();

	if (parent)
		ParticleObject->SetParent(parent);

	data._commonInfo._firstRun = true;

	data._objectID = ParticleObject->GetObjectID();

	*particleData = data;

	ParticleObject->GetTransform()->SetXMWorldTM(data._commonInfo._transformMatrix);

	for (auto iter : data._childrens)
	{
		CreateParticleObjectFromPrevParticleData(iter, ParticleObject);
	}

	_particleObjects.insert({ ParticleObject->GetObjectID(),ParticleObject });

	return ParticleObject;
}

void ParticleObjectManager::DeleteParticleObject(unsigned int index)
{
	if (_particleObjects.find(index) == _particleObjects.end())
	{
		WriteLog("DeleteObject Fail : %d\n", index);
	}
	else
	{
		EXCUTE(new ObjectDeleteCommand(_particleObjects[index]));
		WriteLog("DeleteObject Success \n");
	}
}

void ParticleObjectManager::DeleteAllParticleObject()
{
	WriteLog("[Log] ClearAllObject \n");

	for (auto& iter : _particleObjects)
	{
		Muscle::IGameEngine::Get()->GetObjManager()->DeleteObject(iter.second);
	}
	_particleObjects.clear();
}

void ParticleObjectManager::ExcuteDeleteParticleObject(unsigned int index)
{

	for (auto iter : _particleObjects[index]->GetChildrens())
	{
		ExcuteDeleteParticleObject(iter->GetObjectID());
	}
	Muscle::IGameEngine::Get()->GetObjManager()->DeleteObject(_particleObjects[index]);
	_particleObjects.erase(index);
}