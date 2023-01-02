#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorControllerLayer.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

namespace DUOLGameEngine
{
	AnimatorController::AnimatorController(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
		, _allParameterTypes({})
	{
		_currentLayer = new AnimatorControllerLayer(TEXT("BaseLayer"));
	}

	AnimatorController::~AnimatorController()
	{

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

		DUOLGameEngine::AnimatorState* newState = currStateMachine->AddState(animationClip->GetName());

		newState->SetAnimationClip(animationClip);

		return newState;
	}
}