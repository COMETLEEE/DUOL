#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

namespace DUOLGameEngine
{
#pragma region ANIMATOR_CONTROLLER_CONTEXT
	AnimatorControllerContext::AnimatorControllerContext(DUOLGameEngine::Animator* animator, DUOLGameEngine::AnimatorController* controller) :
		_animator(animator)
	{
		// Settings all parameter map.
		SetParameters(controller);

		// TODO - 일단 애니메이션을 내 마음대로 완전히 섞어버리기 위한 여러 레이어에 대한 생각은 나중에 해봅시다 ..
		// Root State Machine.
		_currentStateMachines.push_back(controller->_currentLayer->GetRootStateMachine());

		// Entry State.
		_currentStates.push_back(_currentStateMachines[0]->GetEntryState());

		// 근데 이제 생각난건데 여러 애니메이션을 한 레이어에서 Blending Tree 하려면 .. 이렇게 하면 안될 것 같은데 ..
		_currentFrames.push_back(0.f);
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

	AnimatorController::AnimatorController(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
		, _allParameterTypes({})
	{
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
		_currentLayer->UpdateAnimatorControllerLayer(context, deltaTime);
	}
}