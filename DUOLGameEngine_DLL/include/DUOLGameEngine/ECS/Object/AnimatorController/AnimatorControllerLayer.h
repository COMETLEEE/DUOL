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
		 * \brief 기본생성자 !
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
		 * \brief 해당 Layer의 비중
		 */
		float _weight;

		/**
		 * \brief 해당 Layer의 이름
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief 해당 Layer의 가장 상단에 있는 State Machine.
		 */
		DUOLGameEngine::AnimatorStateMachine* _stateMachine;

		/**
		 * \brief 해당 AnimatorControllerLayer가 속한 AnimatorController
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