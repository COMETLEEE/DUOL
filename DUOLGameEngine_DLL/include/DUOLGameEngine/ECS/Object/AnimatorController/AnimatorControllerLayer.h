#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

#include <rttr/registration_friend>

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
		/**
		 * \brief �⺻������ !
		 */
		AnimatorControllerLayer();

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
		 * \brief �ش� Layer�� ���� ��ܿ� �ִ� State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _stateMachine;

		/**
		 * \brief �ش� AnimatorControllerLayer�� ���� AnimatorController
		 */
		DUOLGameEngine::AnimatorController* _animatorController;

		/**
		 * \brief Update animator controller layer state of context.
		 * \param context for update.
		 */
		void UpdateAnimatorControllerLayer(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime);

		RTTR_REGISTRATION_FRIEND		

#pragma region FRIEND_CLASS
		friend class AnimatorStateTransition;

		friend class AnimatorController;
#pragma endregion
	};
}