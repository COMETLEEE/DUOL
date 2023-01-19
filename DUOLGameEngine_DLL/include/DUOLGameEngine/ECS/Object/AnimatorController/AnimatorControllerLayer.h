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
		 * \brief �ش� Layer�� ����
		 */
		float _weight;

		/**
		 * \brief �ش� Layer�� �̸�
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief All state machines in this layer.
		 */
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::AnimatorStateMachine*> _stateMachines;

		/**
		 * \brief �ش� Layer�� ���� ��ܿ� �ִ� State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _rootStateMachine;

		/**
		 * \brief Editor - Layer���� ���� �信 ����Ǵ� State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _currentStateMachine;

		/**
		 * \brief �ش� AnimatorControllerLayer�� ���� AnimatorController
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