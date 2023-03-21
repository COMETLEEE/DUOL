#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLMath/DUOLMath.h"
#include "DUOLCommon/Event/Event.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLGameEngine/FiniteStateMachine/FiniteStateMachine.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 가벼운 FSM 의 State를 나타내는 클래스.
	 */
	class DUOL_GAMEENGINE_API StateBase
	{
	public:
		StateBase(const DUOLCommon::tstring& stateName);

		virtual ~StateBase();

	private:
		DUOLCommon::tstring _stateName;

	public:
		DUOLCommon::Event<void> _stateEnterEvent;

		DUOLCommon::Event<void> _stateStayEvent;

		DUOLCommon::Event<void> _stateExitEvent;

		DUOLCommon::Event<void> _stateStayFixedEvent;

		DUOLGameEngine::FiniteStateMachine* _stateMachine;

		bool _isOnStay;

	public:
		virtual void OnStateEnter(float deltaTime);

		virtual void OnStateStay(float deltaTime);

		virtual void OnStateStayFixed(float fixedTimeStep);

		virtual void OnStateExit(float deltaTime);

		friend class FiniteStateMachine;
	};
}
