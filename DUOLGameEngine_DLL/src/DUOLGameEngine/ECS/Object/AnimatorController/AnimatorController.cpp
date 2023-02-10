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
		// TODO - �ϴ� �ִϸ��̼��� �� ������� ������ ��������� ���� ���� ���̾ ���� ������ ���߿� �غ��ô� ..

		// Settings all parameter map.
		SetParameters(controller);

		// ó���� Root State Machine.
		_currentStateMachineContexts.push_back({ controller->_currentLayer->GetRootStateMachine(), false });

		// ó���� Entry State.
		_currentStateContexts.push_back({ _currentStateMachineContexts[0]._currentStateMachine->GetEntryState(), 0.f });

		// Ʈ������ ���ؽ�Ʈ�� ���̾� �� �ִ� �� ���� �°��� ..
		_currentTransitionContexts.push_back({});
	}

	void AnimatorControllerContext::SetParameters(DUOLGameEngine::AnimatorController* controller)
	{
		_floatParameters.clear();

		_intParameters.clear();

		_boolParameters.clear();

		// ���� Controller�� ��ϵǾ� �ִ� ��� Parameter�� Context�� �ο����ݴϴ�.
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
		// Base Layer�� �ʱ⿡ ������ ��������ϴ�.
		_currentLayer = new AnimatorControllerLayer(this, TEXT("BaseLayer"));
	}

	AnimatorController::~AnimatorController()
	{
		// ���̾� �޸� ����
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

		// �ִϸ��̼� Ŭ���� �̸����� State�� �ϳ� ����ϴ�. ���� �ߺ��Ǿ� ������ �ٸ� �̸����� ��������ϴ�.
		DUOLGameEngine::AnimatorState* newState = currStateMachine->AddState(animationClip->GetName());

		// ������Ʈ�� ����Ǵ� �ִϸ��̼��� �������ְ� ��ȯ�մϴ�.
		newState->SetAnimationClip(animationClip);

		return newState;
	}

	void AnimatorController::UpdateAnimatorController(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		// TODO - ��� ���̾�鿡 ���ؼ� Context�� ������Ʈ�մϴ�. Layer���� Weight�� ����ؼ� �ִϸ��̼��� �����մϴ�.
		if (_currentLayer != nullptr)
			_currentLayer->UpdateAnimatorControllerLayer(context, deltaTime);
	}
}