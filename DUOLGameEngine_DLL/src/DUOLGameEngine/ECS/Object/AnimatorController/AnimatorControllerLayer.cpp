#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AnimatorControllerLayer>("AnimatorControllerLayer")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_weight", &DUOLGameEngine::AnimatorControllerLayer::_weight)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_name", &DUOLGameEngine::AnimatorControllerLayer::_name)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_stateMachine", &DUOLGameEngine::AnimatorControllerLayer::_stateMachine)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_animatorController", &DUOLGameEngine::AnimatorControllerLayer::_animatorController)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::UUIDSerializeType, DUOLCommon::UUIDSerializeType::FileUUID)
	);
}

namespace DUOLGameEngine
{
	AnimatorControllerLayer::AnimatorControllerLayer() :
		_name(TEXT("BaseLayer"))
		, _weight(1.f)
		, _stateMachine(nullptr)
		// , _currentStateMachine(nullptr)
		, _animatorController(nullptr)
	{
	}

	AnimatorControllerLayer::AnimatorControllerLayer(DUOLGameEngine::AnimatorController* animatorController, const DUOLCommon::tstring& name) :
		_name(name)
		, _weight(1.f)
		, _stateMachine(nullptr)
		// , _currentStateMachine(nullptr)
		, _animatorController(animatorController)
	{
		// _stateMachine = new AnimatorStateMachine(this,TEXT("RootStateMachine"));
		_stateMachine = new AnimatorStateMachine(_animatorController, TEXT("RootStateMachine"));

		// _currentStateMachine = _stateMachine;

		// _stateMachines.insert({ _stateMachine->GetName(), _stateMachine });
	}

	AnimatorControllerLayer::~AnimatorControllerLayer()
	{
		/*for (auto& [key, value] : _stateMachines)
			delete value;

		_stateMachines.clear();*/

		delete _stateMachine;
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::AddStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		/*if (_stateMachines.contains(stateMachineName))
		{
			return AddStateMachine(stateMachineName + TEXT("0"));
		}
		else
		{
			DUOLGameEngine::AnimatorStateMachine* newStateMachine = new AnimatorStateMachine(this, stateMachineName);

			_stateMachines.insert({ newStateMachine->GetName(), newStateMachine });

			return newStateMachine;
		}*/

		return nullptr;
	}

	void AnimatorControllerLayer::RemoveStateMachine(const DUOLCommon::tstring& stateMachineName)
	{
		if (stateMachineName == TEXT("RootStateMachine"))
			return;

		//if (_stateMachines.contains(stateMachineName))
		//{
		//	// 스테이트 머신을 해제합니다.
		//	delete _stateMachines.at(stateMachineName);

		//	// 그리고 unordered_map에서 제거합니다.
		//	_stateMachines.erase(stateMachineName);
		//}
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::GetRootStateMachine() const
	{
		return _stateMachine;
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorControllerLayer::GetCurrentStateMachine() const
	{
		return _stateMachine;
	}

	void AnimatorControllerLayer::SetCurrentStateMachine(DUOLGameEngine::AnimatorStateMachine* stateMachine)
	{
		_stateMachine = stateMachine;
	}

	void AnimatorControllerLayer::UpdateAnimatorControllerLayer(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		// Layer 별 해당 컨텍스트에서 진행 중인 State machine, State 에 대해서 업데이트를 진행한다.
		context->_currentStateMachineContexts[0]._currentStateMachine->UpdateAnimatorStateMachine(context, deltaTime);
	}
}