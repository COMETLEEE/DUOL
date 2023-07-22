#include "DUOLClient/ECS/Component/Contents/DominationArea.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/Scrollbar.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLGameEngine
{
	class Image;
}

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::DominationArea>("DominationArea")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("destroyIncreasePerMonster", &DUOLClient::DominationArea::_destroyIncreasePerMonster)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("_destroyIncreasePerFrame", &DUOLClient::DominationArea::_destroyIncreasePerFrame)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("maxClearPercent", &DUOLClient::DominationArea::_maxClearPercent)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("dominationTime", &DUOLClient::DominationArea::_dominationTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("currentClearPercent", &DUOLClient::DominationArea::_currentClearPercent)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
				.property("_defaultDestroyIncrease", &DUOLClient::DominationArea::_defaultDestroyIncrease)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
					.property("_clearPercent", &DUOLClient::DominationArea::_clearPercent)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("isCleared", &DUOLClient::DominationArea::_isCleared)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
				.property("_monsterSpawnBelowCurrentClear", &DUOLClient::DominationArea::_monsterSpawnBelowCurrentClear)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("isTotallyCleared", &DUOLClient::DominationArea::_isTotallyCleared)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLClient
{
	class EnemySpawnTrigger;

	DominationArea::DominationArea(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _destroyIncreasePerMonster(1)
		, _destroyIncreasePerFrame(1.f)
		, _maxClearPercent(100)
		, _currentClearPercent(0)
		, _dominationTime(10)
		, _isCleared(false)
		, _currentEnemyCountInZone(0)
		, _playerInZone(0)
		, _defaultDestroyIncrease(0.5f)
		, _clearPercent(20.f)
		, _destroyColor(255.0f, 0.f, 0.f)
		, _clearColor(0.0f, 255.f, 0.f)
		, _isTotallyCleared(false)
		, _monsterSpawnBelowCurrentClear(80.f)
	{
	}

	DominationArea::~DominationArea()
	{
	}

	bool DominationArea::IsCleared() const
	{
		return _isCleared;
	}

	void DominationArea::SetTotallyClear(bool value)
	{
		if (value == true)
		{
			_enemyGroupControl->SetIsWaveClear(true);
			_enemyGroupControl->SetTrueWaveCondition(true);
		}
		else
		{
			_enemyGroupControl->SetIsWaveClear(false);
			_enemyGroupControl->CreateEnemy();
		}


		_currentClearPercent = _maxClearPercent;
		_isTotallyCleared = value;
	}

	void DominationArea::OnEnable()
	{
	}

	void DominationArea::OnDisable()
	{
	}

	void DominationArea::OnAwake()
	{
		_areaCollider = GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>();
		_areaCollider->SetIsTrigger(true);

		auto children = GetTransform()->GetChildren();

		for (auto& child : children)
		{
			auto go = child->GetGameObject();

			if (go->GetName() == TEXT("DominationBorder"))
			{
				_dominationBorder = go->GetComponent<DUOLGameEngine::MeshRenderer>();
				_dominationBorder->SetEmissivePower(DOMINATION_BORDER_EMISSIVE_POWER);
			}

			if (go->GetName() == TEXT("ConditionalWave"))
			{
				_enemyGroupControl = go->GetComponent<EnemyGroupController>();
			}
		}

		auto anchorpos = DUOLMath::Vector2{ 0.5f, 0.82f };

		auto ui = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmtpyUI();
		_dominationGauge = ui->AddComponent<DUOLGameEngine::Scrollbar>();
		auto gaugeimage = ui->GetComponent<DUOLGameEngine::Image>();
		gaugeimage->SetSprite(DUOLGameEngine::ResourceManager::GetInstance()->GetSprite(TEXT("05_ingame_health_enemy_base.png")));

		auto uibackgroundRect = ui->GetComponent<DUOLGameEngine::RectTransform>();
		uibackgroundRect->SetAnchorMin(anchorpos);
		uibackgroundRect->SetAnchorMax(anchorpos);
		uibackgroundRect->SetRectZ(800);
		uibackgroundRect->SetRectW(30);

		for(auto& child : ui->GetTransform()->GetChildGameObjects())
		{
				if(child->GetName() == TEXT("HandleButton"))
				{
					Destroy(child);
				}
				else if(child->GetName() == TEXT("HandleImage"))
				{
					child->GetComponent<DUOLGameEngine::Image>()->SetSprite(DUOLGameEngine::ResourceManager::GetInstance()->GetSprite(TEXT("OverDrive.png")));
					auto rectimage = child->GetComponent<DUOLGameEngine::RectTransform>();

					rectimage->SetAnchorMin(anchorpos);
					rectimage->SetAnchorMax(anchorpos);
					rectimage->SetRectZ(800);
					rectimage->SetRectW(30);
				}
		}

		_uiGameObject = ui;
		ui->SetIsActiveSelf(false);
	}

	void DominationArea::OnStart()
	{
		//GetTGetComponent<DUOLGameEngine::BoxCollider>();
	}

	void DominationArea::OnDestroy()
	{
	}

	void DominationArea::OnUpdate(float deltaTime)
	{
		if (!_isTotallyCleared)
		{
			if (_playerInZone && _currentEnemyCountInZone == 0)
			{
				float addClear = (_maxClearPercent / _dominationTime) * deltaTime;
				_currentClearPercent += addClear;
			}
			else if (_playerInZone)
			{
				//플레이어가 존에 있고 적이 있으면 치열하게 싸우는중
			}
			else
			{
				float upDestroyPercent = std::min(_defaultDestroyIncrease + _destroyIncreasePerMonster * _currentEnemyCountInZone, _destroyIncreasePerFrame);
				_currentClearPercent -= upDestroyPercent * deltaTime;
			}

			if (_currentClearPercent >= _clearPercent)
			{
				_isCleared = true;
			}
			else
			{
				_isCleared = false;
			}


			if (_currentClearPercent <= _monsterSpawnBelowCurrentClear)
			{
				_enemyGroupControl->SetTrueWaveCondition(false);
			}
			else
			{
				_enemyGroupControl->SetTrueWaveCondition(true);
			}

			_currentClearPercent = std::clamp(_currentClearPercent, 0.f, _maxClearPercent);
			auto nowGauge = _currentClearPercent / _maxClearPercent * 100.f;
			_dominationGauge->SetNowGauge(nowGauge);
			DUOLMath::Vector3 currentColor = DUOLMath::Vector3::Lerp(_destroyColor, _clearColor, _currentClearPercent / _maxClearPercent);

			_dominationBorder->SetEmissive(currentColor);
		}
		else
		{
			_isCleared = true;
			auto nowGauge = _currentClearPercent / _maxClearPercent * 100.f;
			_dominationGauge->SetNowGauge(nowGauge);
			_dominationBorder->SetEmissive(_clearColor);
		}
	}

	void DominationArea::OnFixedUpdate(float deltaTime)
	{
		_currentEnemyCountInZone = 0;
		_playerInZone = false;
	}

	void DominationArea::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto gameObject = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (gameObject->GetLayer() == TEXT("EnemyRigidbody"))
		{
			_currentEnemyCountInZone++;
		}
		else if (gameObject->GetLayer() == TEXT("Player"))
		{
			_playerInZone = true;

			if (!_uiGameObject->GetIsActive())
			{
				_uiGameObject->SetIsActiveSelf(true);
			}
		}
	}

	void DominationArea::OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		if (_uiGameObject->GetIsActive())
		{
			_uiGameObject->SetIsActiveSelf(false);
		}
	}
}
