#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	struct AnimatorControllerContext;
	class AnimatorController;

	class AnimatorStateMachine;
}

namespace DUOLGameEngine
{
	class AnimatorStateMachine;

	class DUOL_GAMEENGINE_API AnimatorControllerLayer
	{
	public:
		AnimatorControllerLayer(DUOLGameEngine::AnimatorController* animatorController, const DUOLCommon::tstring& name);

		~AnimatorControllerLayer();

	public:
		DUOLGameEngine::AnimatorStateMachine* AddStateMachine(const DUOLCommon::tstring& stateMachineName);

		void RemoveStateMachine(const DUOLCommon::tstring& stateMachineName);

		DUOLGameEngine::AnimatorStateMachine* GetRootStateMachine() const;

		DUOLGameEngine::AnimatorStateMachine* GetCurrentStateMachine() const;

		void SetCurrentStateMachine(DUOLGameEngine::AnimatorStateMachine* stateMachine);

	private:
		/**
		 * \brief 해당 Layer의 비중
		 */
		float _weight;

		/**
		 * \brief 해당 Layer의 이름
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief All state machines in this layer.
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorStateMachine*> _stateMachines;

		/**
		 * \brief 해당 Layer의 가장 상단에 있는 State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _rootStateMachine;

		/**
		 * \brief Editor - Layer에서 가장 뷰에 노출되는 State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _currentStateMachine;

		/**
		 * \brief 해당 AnimatorControllerLayer가 속한 AnimatorController
		 */
		DUOLGameEngine::AnimatorController* _animatorController;

		/**
		 * \brief Update animator controller layer state of context.
		 * \param context for update.
		 */
		void UpdateAnimatorControllerLayer(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime);

#pragma region FRIEND_CLASS
		friend class AnimatorStateTransition;

		friend class AnimatorController;
#pragma endregion
	};
}