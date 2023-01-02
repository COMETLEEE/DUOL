#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGameEngine
{
	class AnimatorState;
}

namespace DUOLGameEngine
{
	/**
	 * \brief represent interaction of states.
	 */
	class AnimatorStateMachine : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorStateMachine(const DUOLCommon::tstring& name = TEXT("AnimatorStateMachine"));

		virtual ~AnimatorStateMachine() override;

	private:
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorState*> _animatorStates;

		DUOLGameEngine::AnimatorState* _entryState;

	public:
		DUOLGameEngine::AnimatorState* AddState(const DUOLCommon::tstring& name);

		void RemoveState(const DUOLCommon::tstring& name);

		void RemoveState(DUOLGameEngine::AnimatorState* animatorState);
	};
}