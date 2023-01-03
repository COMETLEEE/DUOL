/**

    @file      AnimatorController.h
    @brief     애니메이터 컴포넌트를 컨트롤합니다.
    @details   ~
    @author    COMETLEE
    @date      2.01.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

#include <map>

namespace DUOLGameEngine
{
	class Animator;
	class AnimatorController; 
	class AnimatorControllerLayer;
	class AnimatorStateMachine;
	class AnimatorState;
	class AnimatorStateTransition;

	class AnimationClip;
}

namespace DUOLGameEngine
{
	/**
	 * \brief The type of the parameter.
	 */
	enum class AnimatorControllerParameterType
	{
		Float
		, Int
		, Bool
	};

	/**
	 * \brief 애니메이터 별로 할당되는 Context. SetAnimatorController(...) 할 때 할당됩니다.
	 */
	struct AnimatorControllerContext
	{
		AnimatorControllerContext(DUOLGameEngine::Animator* animator, DUOLGameEngine::AnimatorController* controller);

		std::unordered_map<DUOLCommon::tstring, float> _floatParameters = {};

		std::unordered_map<DUOLCommon::tstring, int> _intParameters = {};

		std::unordered_map<DUOLCommon::tstring, bool> _boolParameters = {};

		// TODO - 상, 하체 등 여러 애니메이션을 섞기 위해서 Layer 별로 _currentStateMachine, _currentState가 있어야함.
		/**
		 * \brief 현재 컨텍스트에서 진행 중인 레이어 별 State machines.
		 */
		std::vector<DUOLGameEngine::AnimatorStateMachine*> _currentStateMachines = {};

		/**
		 * \brief 현재 컨텍스트에서 진행 중인 레이어 별 States.
		 */
		std::vector<DUOLGameEngine::AnimatorState*> _currentStates = {};

		/**
		 * \brief 현재 컨텍스트에서 플레이 중인 시간을 나타냅니다.
		 */
		std::vector<float> _currentFrames = {};

		/**
		 * \brief 현재 컨텍스트가 대표하는 애니메이터 컴포넌트의 주소입니다.
		 */
		DUOLGameEngine::Animator* _animator;

		/**
		 * \brief 컨텍스트의 파라미터 리스트를 AnimatorController에 맞게 세팅합니다.
		 * \param controller 해당 컨트롤러에 대응되도록 컨텍스트 밸류를 다시 세팅합니다.
		 * 게임 실행 중에는 컨트롤러를 갱신할 수 없습니다. (비효율이고 의미 없음.)
		 */
		void SetParameters(DUOLGameEngine::AnimatorController* controller);
	};

	/**
	 * \brief Controls animation through layers with state machines, controlled by parameters.
	 */
	class AnimatorController final : public DUOLGameEngine::ObjectBase
	{
	public:
		AnimatorController(const DUOLCommon::tstring& name = TEXT("AnimatorController"));

		virtual ~AnimatorController() override;

	private:
		/**
		 * \brief 해당 애니메이터 컨트롤러를 사용하는 Animator 들에 생성된 References.
		 */
		std::unordered_map<DUOLGameEngine::UUID, DUOLGameEngine::AnimatorControllerContext*> _animatorControllerContexts;

		/**
		 * \brief 해당 애니메이터 컨트롤러에서 관리하는 모든 파라미터들의 이름과 타입 리스트
		 */
		std::unordered_map<DUOLCommon::tstring, AnimatorControllerParameterType> _allParameterTypes;

		/**
		 * \brief 일단 One-layer로 갑시다. TODO - 이 녀석은 추후 Editor 상에 띄워진 Layer를 나타낼 것입니다.
		 */
		DUOLGameEngine::AnimatorControllerLayer* _currentLayer;

	public:
		/**
		 * \brief Add a parameter to the controller.
		 * \param paramName The name of the parameter.
		 * \param paramType The type of the parameter.
		 */
		void AddParameter(const DUOLCommon::tstring& paramName, AnimatorControllerParameterType paramType);

		/**
		 * \brief Remove a parameter in the controller.
		 * \param paramName The name of the parameter.
		 */
		void RemoveParameter(const DUOLCommon::tstring& paramName);

		/**
		 * \brief Add a state machine to the controller.
		 * \param stateMachineName The name of the state machine.
		 * \return state machine created.
		 */
		DUOLGameEngine::AnimatorStateMachine* AddStateMachine(const DUOLCommon::tstring& stateMachineName);

		/**
		 * \brief creates a new state with the motion in it.
		 * \param animationClip Animation of state created.
		 */
		DUOLGameEngine::AnimatorState* AddMotion(DUOLGameEngine::AnimationClip* animationClip);

		/**
		 * \brief Update animator controller state of context.
		 * \param context for update.
		 */
		void UpdateAnimatorController(DUOLGameEngine::AnimatorControllerContext* context, float deltaTime);

#pragma region FRIEND_CLASS
		friend class AnimatorControllerContext;

		friend class AnimatorStateTransition;

		friend class Animator;
#pragma endregion
	};
}