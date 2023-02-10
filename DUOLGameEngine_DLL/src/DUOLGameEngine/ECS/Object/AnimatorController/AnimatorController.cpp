#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::AnimatorControllerParameterType>("AnimatorControllerParameterType")
	(
		value("0", DUOLGameEngine::AnimatorControllerParameterType::Float)
		, value("1", DUOLGameEngine::AnimatorControllerParameterType::Int)
		, value("2", DUOLGameEngine::AnimatorControllerParameterType::Bool)
	);

	rttr::registration::class_<DUOLGameEngine::AnimatorController>("AnimatorController")
	.constructor<>()
	(

	)
	.property("_allParameterTypes", &DUOLGameEngine::AnimatorController::_allParameterTypes)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_currentLayer", &DUOLGameEngine::AnimatorController::_currentLayer)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLGameEngine
{
#pragma region ANIMATOR_CONTROLLER_CONTEXT
	AnimatorControllerContext::AnimatorControllerContext(DUOLGameEngine::Animator* animator, DUOLGameEngine::AnimatorController* controller) :
		_animator(animator)
	{
		// TODO - 일단 애니메이션을 내 마음대로 완전히 섞어버리기 위한 여러 레이어에 대한 생각은 나중에 해봅시다 ..

		// Settings all parameter map.
		SetParameters(controller);

		// 처음은 Root State Machine.
		_currentStateMachineContexts.push_back({ controller->_currentLayer->GetRootStateMachine(), false });

		// 처음은 Entry State.
		_currentStateContexts.push_back({ _currentStateMachineContexts[0]._currentStateMachine->GetEntryState(), 0.f });

		// 트랜지션 컨텍스트는 레이어 별 최대 한 개가 맞겠지 ..
		_currentTransitionContexts.push_back({});
	}

	void AnimatorControllerContext::SetParameters(DUOLGameEngine::AnimatorController* controller)
	{
		_floatParameters.clear();

		_intParameters.clear();

		_boolParameters.clear();

		// 현재 Controller에 등록되어 있는 모든 Parameter를 Context에 부여해줍니다.
		for (auto& [key, value] : controller->_allParameterTypes)
		{
			switch (value)
			{
				case AnimatorControllerParameterType::Float:
				{
					_floatParameters.insert({ key, 0.f });

					break;
				}

				case AnimatorControllerParameterType::Int:
				{
					_intParameters.insert({ key, 0 });

					break;
				}

				case AnimatorControllerParameterType::Bool:
				{
					_boolParameters.insert({ key, true });

					break;
				}
			}
		}
	}

#pragma endregion
	AnimatorController::AnimatorController() :
		DUOLGameEngine::ObjectBase(TEXT("AnimatorController"), ObjectType::Resource)
		, _allParameterTypes({})
	{

	}

	AnimatorController::AnimatorController(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
		, _allParameterTypes({})
	{
		// Base Layer는 초기에 무조건 만들어집니다.
		_currentLayer = new AnimatorControllerLayer(this, TEXT("BaseLayer"));
	}

	AnimatorController::~AnimatorController()
	{
		// 레이어 메모리 해제
		delete _currentLayer;
	}

	void AnimatorController::AddParameter(const DUOLCommon::tstring& paramName,
		AnimatorControllerParameterType paramType)
	{
		if (_allParameterTypes.contains(paramName))
			AddParameter(paramName + TEXT("0"), paramType);
		else
			_allParameterTypes.insert({ paramName, paramType });
	}

	void AnimatorController::RemoveParameter(const DUOLCommon::tstring& paramName)
	{
		if (_allParameterTypes.contains(paramName))
			_allParameterTypes.erase(paramName);
	}

	DUOLGameEngine::AnimatorStateMachine* AnimatorController::AddStateMachine(
		const DUOLCommon::tstring& stateMachineName)
	{
		DUOLGameEngine::AnimatorStateMachine* stateMachine = _currentLayer->AddStateMachine(stateMachineName);

		return stateMachine;
	}

	DUOLGameEngine::AnimatorState* AnimatorController::AddMotion(DUOLGameEngine::AnimationClip* animationClip)
	{
		DUOLGameEngine::AnimatorStateMachine* currStateMachine = _currentLayer->GetCurrentStateMachine();

		// 애니메이션 클립의 이름으로 State를 하나 만듭니다. 물론 중복되어 있으면 다른 이름으로 만들어집니다.
		DUOLGameEngine::AnimatorState* newState = currStateMachine->AddState(animationClip->GetName());

		// 스테이트에 적용되는 애니메이션을 지정해주고 반환합니다.
		newState->SetAnimationClip(animationClip);

		return newState;
	}

	void AnimatorController::UpdateAnimatorController(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		// TODO - 모든 레이어들에 대해서 Context를 업데이트합니다. Layer들의 Weight에 비례해서 애니메이션이 루프합니다.
		if (_currentLayer != nullptr)
			_currentLayer->UpdateAnimatorControllerLayer(context, deltaTime);
	}
}