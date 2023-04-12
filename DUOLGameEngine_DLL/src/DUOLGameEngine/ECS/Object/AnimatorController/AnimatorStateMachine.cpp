#include <algorithm>

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorStateMachine.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Object/AnimationClip.h"

#include "DUOLCommon/Log/LogHelper.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AnimatorStateMachine>("AnimatorStateMachine")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_animatorStates", &DUOLGameEngine::AnimatorStateMachine::_animatorStates)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_entryState", &DUOLGameEngine::AnimatorStateMachine::_entryState)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	)
	.property("_animatorController", &DUOLGameEngine::AnimatorStateMachine::_animatorController)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	);
}

namespace DUOLGameEngine
{
	AnimatorStateMachine::AnimatorStateMachine(DUOLGameEngine::AnimatorController* controller, const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animatorStates({})
		, _entryState(nullptr)
		, _animatorController(controller)
	{
		// _entryState가 기본적으로 제공이 되면 좋을 것 같은데 .. 그래야 깔끔하게 스테이트 머신에 진입하는 느낌이 들 것 같음 ..
	}

	AnimatorStateMachine::~AnimatorStateMachine()
	{
		// 모든 스테이트의 메모리를 해제합니다.
		for (auto& pair : _animatorStates)
			delete pair.second;

		_animatorStates.clear();
	}

	DUOLGameEngine::AnimatorState* AnimatorStateMachine::AddState(const DUOLCommon::tstring& name)
	{
		DUOLGameEngine::AnimatorState* newState;

		// 중복 이름을 허용하지 않습니다.
		if (_animatorStates.contains(name))
			return AddState(name + TEXT("0"));
		else
			newState = new AnimatorState(this, name);

		// 가장 처음 등록된 State 입니다. => _entryState로 적용해줍니다.
		if (_animatorStates.empty())
			_entryState = newState;

		_animatorStates.insert({ newState->GetName(), newState });

		return newState;
	}

	void AnimatorStateMachine::RemoveState(const DUOLCommon::tstring& name)
	{
		// 만약 해당 이름을 가진 상태가 존재한다면
		if (_animatorStates.contains(name))
		{
			// 메모리에서 해제하고
			delete _animatorStates.at(name);

			// 리스트에서 제거합니다.
			_animatorStates.erase(name);
		}
	}

	void AnimatorStateMachine::RemoveState(DUOLGameEngine::AnimatorState* animatorState)
	{
		for (auto& pair : _animatorStates)
		{
			// 만약 똑같은 AnimatorState가 존재한다면 ..
			if (pair.second == animatorState)
			{
				// 메모리에서 해제하고
				delete animatorState;

				// 리스트에서 제거합니다.
				_animatorStates.erase(pair.first);

				return;
			}
		}
	}

	DUOLGameEngine::AnimatorState* AnimatorStateMachine::GetEntryState() const
	{
		return _entryState;
	}

	void AnimatorStateMachine::UpdateAnimatorStateMachine(DUOLGameEngine::AnimatorControllerContext* context,
		float deltaTime)
	{
		// 트랜지션 중이라면 트랜지션을 계속 진행합니다.
		if (context->_currentStateMachineContexts[0]._isOnTransition)
		{
			// 컨텍스트가 해당 스테이트 머신에서 트랜지션을 수행 중이라면 이 함수를 호출합니다.
			OnTransitionStay(context, deltaTime);

			return;
		}

		// 트랜지션이 진행 중이지 않다면 컨텍스트의 현재 스테이트에서 트랜지션 가능한지 여부를 확인합니다.
		// Context의 현재 스테이트를 가져옵니다.
		DUOLGameEngine::AnimatorState* currentState = context->_currentStateContexts[0]._currentState;

		// 현재 스테이트의 모든 트랜지션을 체크해서 가능한 Transition을 받습니다.
		// 한 번에 여러 개의 트랜지션이 가능하다고 체크될 수도 있으니까 Order 같은 것 정해놓으면 좋을지도 ..?
		DUOLGameEngine::AnimatorStateTransition* transition = currentState->CheckAllTransition(context);

		// 트랜지션 할 수 없는 상황입니다. 현재 스테이트에 대해서 Context를 갱신합니다. 
		if (transition == nullptr)
			NotTransition(context, deltaTime);
		// 트랜지션 할 수 있는 상황입니다. AnimatorControllerContext에 Transition 준비합니다.
		else
		{
			OnTransitionEnter(context, transition);

			OnTransitionStay(context, deltaTime);
		}
	}

	void AnimatorStateMachine::NotTransition(DUOLGameEngine::AnimatorControllerContext* targetContext, float deltaTime)
	{
		// 현재 상태의 애니메이션을 계속 플레이합니다.
		targetContext->_animator->Play(deltaTime, targetContext->_currentStateContexts[0]._currentState->GetAnimationClip());
	}

	void AnimatorStateMachine::OnTransitionEnter(DUOLGameEngine::AnimatorControllerContext* context, DUOLGameEngine::AnimatorStateTransition* targetTransition)
	{
		// TODO - 만약, 현재 스테이트의 동작을 모두 마무리하고 트랜지션을 진행할지의 여부 체크도 있으면 좋은 기능일듯.
		context->_currentStateMachineContexts[0]._isOnTransition = true;

		context->_currentTransitionContexts[0]._currentTransition = targetTransition;

		const float transitionOffset = targetTransition->GetTransitionOffset();

		const float transitionDuration = targetTransition->GetTransitionDuration();

		float startFrameOfTo = targetTransition->_to->GetAnimationClip()->GetMaxFrame() * std::clamp<float>(transitionOffset, 0.f, 1.f);

		float remainTime = 0.f;

		// Fixed Duration이면 초 단위 시간으로 Transition을 수행합니다.
		if (!targetTransition->_fixedDuration)
			remainTime = transitionDuration * targetTransition->_from->GetAnimationClip()->GetTotalPlayTime();
		else
			remainTime = transitionDuration;

		// Transition에 들어가기 전에 필요한 정보들을 세팅합니다.
		// 1. From State의 현재 프레임
		// 2. To State의 시작 프레임 (transitionOffset을 통해서 구합니다.)
		// 3. Transition의 총 시간 (초 단위)
		// 4. Transition의 남은 시간 (여기서는 3과 같지만 점점 줄이면서 보간 계수를 바꿔 나갑니다.)
		context->_currentTransitionContexts[0]._currentFrameOfFrom = context->_currentStateContexts[0]._currentFrame;

		context->_currentTransitionContexts[0]._prevFrameOfFrom = context->_currentStateContexts[0]._prevFrame; 

		context->_currentTransitionContexts[0]._currentFrameOfTo = startFrameOfTo;

		// To는 이제 시작하는 녀석이니까 이전 프레임은 1보다 작은 프레임입니다.
		context->_currentTransitionContexts[0]._prevFrameOfTo = (startFrameOfTo == 0) ? 0 : std::abs(startFrameOfTo - 1.f);

		context->_currentTransitionContexts[0]._totalTransitionTime = remainTime;

		context->_currentTransitionContexts[0]._remainTransitionTime = remainTime;
	}

	void AnimatorStateMachine::OnTransitionStay(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime)
	{
		DUOLGameEngine::AnimatorStateTransition* transition = context->_currentTransitionContexts[0]._currentTransition;

		DUOLGameEngine::AnimationClip* fromClip = transition->_from->_animationClip;

		DUOLGameEngine::AnimationClip* toClip = transition->_to->_animationClip;

		// 트랜지션에 남은 시간이 없으니 종료합니다.
		if (context->_currentTransitionContexts[0]._remainTransitionTime <= 0.f)
		{
			OnTransitionExit(context, transition, deltaTime);

			return;
		}

		// fromClip에 적용될 보간 계수 (== 잔여 시간 / 총 시간)
		const float tFrom = std::clamp<float>(context->_currentTransitionContexts[0]._remainTransitionTime / 
			context->_currentTransitionContexts[0]._totalTransitionTime, 0.f, 1.f);

		// Remain Time 갱신
		context->_currentTransitionContexts[0]._remainTransitionTime -= deltaTime;

		// Transition에 사용된 모든 시간이 끝나면 계산
		// tFrom은 현재 트랜지션 컨텍스트가 얼마나 진행됬는가에 따라 보정됩니다.
		context->_animator->Play(deltaTime, fromClip, toClip, tFrom);
	}

	void AnimatorStateMachine::OnTransitionExit(DUOLGameEngine::AnimatorControllerContext* context, DUOLGameEngine::AnimatorStateTransition* transition, float deltaTime)
	{
		// 컨텍스트에서 현재 상태를 바꿔줍니다.
		context->_currentStateContexts[0]._currentState = transition->_to;

		// 현재 프레임도 지금 ToAnimation에서 진행 중인 것으로 바꿔줍니다.
		context->_currentStateContexts[0]._currentFrame = context->_currentTransitionContexts[0]._currentFrameOfTo;

		// 이전 프레임도 지금 ToAnimation에서 지난 프레임이였던 것으로 바꿔줍니다.
		context->_currentStateContexts[0]._prevFrame = context->_currentTransitionContexts[0]._prevFrameOfTo;

		// Loop Count 도 0입니다 ..!
		context->_currentStateContexts[0]._loopCount = 0;

		// 트랜지션 진행 중이라는 플래그를 꺼주고
		context->_currentStateMachineContexts[0]._isOnTransition = false;

		// 단일 애니메이션에 대한 갱신
		context->_animator->Play(deltaTime, context->_currentStateContexts[0]._currentState->GetAnimationClip());

		// 컨텍스트를 정리합니다.
		context->_currentTransitionContexts[0]._currentTransition = nullptr;
		context->_currentTransitionContexts[0]._currentFrameOfFrom = 0.f;
		context->_currentTransitionContexts[0]._prevFrameOfFrom = 0.f;
		context->_currentTransitionContexts[0]._currentFrameOfTo = 0.f;
		context->_currentTransitionContexts[0]._prevFrameOfTo = 0.f;
	}
}