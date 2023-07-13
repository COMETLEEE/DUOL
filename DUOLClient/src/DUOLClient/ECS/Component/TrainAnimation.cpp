#include "DUOLClient//ECS/Component/TrainAnimation.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::TrainAnimation>("TrainAnimation")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("moveTime", &DUOLClient::TrainAnimation::_moveTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("generationTime", &DUOLClient::TrainAnimation::_generationTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}


namespace DUOLClient
{
	TrainAnimation::TrainAnimation(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
		BehaviourBase(owner, name)
		,_isGeneratedTrain(false)
		,_moveTime(10)
		,_generationTime(15)
	{
	}

	TrainAnimation::~TrainAnimation()
	{
	}

	void TrainAnimation::OnAwake()
	{
		auto children = GetTransform()->GetChildren();

		for(auto child : children)
		{
			if(child->GetGameObject()->GetName() == TEXT("TrainEndline"))
			{
				_endlineTransform = child;
			}
			else
			{
				_trainTransform = child;
			}
		}

		_startPos = _trainTransform->GetWorldPosition();
		_endPos = _endlineTransform->GetWorldPosition();
	}

	void TrainAnimation::OnUpdate(float deltaTime)
	{
		if(_isGeneratedTrain)
		{
			_currentMovementTime += deltaTime;


			if (_currentMovementTime >= _moveTime)
			{
				_isGeneratedTrain = false;
				_currentGenerationTime = 0;

				_trainTransform->SetPosition(_startPos);
			}

			float deltaT = _currentMovementTime / _moveTime; 
			auto currentPos = DUOLMath::Vector3::Lerp(_startPos, _endPos, deltaT);

			_trainTransform->SetPosition(currentPos);
		}
		else
		{
			_currentGenerationTime += deltaTime;

			if(_currentGenerationTime > _generationTime)
			{
				_isGeneratedTrain = true;
				_currentMovementTime = 0;
			}
		}
	}

}
