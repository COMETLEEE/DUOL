#include "BossConditionalWaveHandler.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"


using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::BossConditionalWaveHandler>("BossConditionalWaveHandler")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_waveTrigger", &DUOLClient::BossConditionalWaveHandler::_waveTrigger)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	)
	.property("_bossTrigger", &DUOLClient::BossConditionalWaveHandler::_bossTrigger)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
		)
	.property("_isCleared", &DUOLClient::BossConditionalWaveHandler::_isCleared)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLClient
{
	DUOLClient::BossConditionalWaveHandler::BossConditionalWaveHandler(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _bossEnemyTrigger(nullptr)
		, _waveEnemyTrigger(nullptr)
	{

	}

	DUOLClient::BossConditionalWaveHandler::~BossConditionalWaveHandler()
	{

	}

	void BossConditionalWaveHandler::OnAwake()
	{
		auto& objects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		//Ä³½Ì
		for (auto& go : objects)
		{
			if (_bossTrigger == go->GetName())
			{
				_bossEnemyTrigger = go->GetComponent<EnemyGroupController>();
			}
			if (_waveTrigger == go->GetName())
			{
				_waveEnemyTrigger = go->GetComponent<EnemyGroupController>();
			}
		}

		if (_bossEnemyTrigger == nullptr || _waveEnemyTrigger == nullptr)
		{
			Destroy(this);
		}
	}

	void BossConditionalWaveHandler::OnUpdate(float deltaTime)
	{
		if ((_bossEnemyTrigger != nullptr &&_waveEnemyTrigger != nullptr) && _bossEnemyTrigger->GetIsClearGroup())
		{
			_waveEnemyTrigger->SetIsWaveClear(true);
			_waveEnemyTrigger->SetTrueWaveCondition(true);
			_waveEnemyTrigger->AllDie();
			//Destroy(_waveEnemyTrigger);
			Destroy(this);
		}
	}
}
