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
		std::unordered_map<DUOLCommon::tstring, float> _floatParameters;

		std::unordered_map<DUOLCommon::tstring, int> _intParameters;

		std::unordered_map<DUOLCommon::tstring, bool> _boolParameters;

		DUOLGameEngine::AnimatorStateMachine* _currentStateMachine;

		DUOLGameEngine::AnimatorState* _currentState;
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
		 * \brief 해당 애니메이터 컨트롤러에서 관리하는 모든 파라미터들의 이름과 타입 리스트
		 */
		std::unordered_map<DUOLCommon::tstring, AnimatorControllerParameterType> _allParameterTypes;

		/**
		 * \brief 일단 One-layer로 갑시다.
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
	};
}