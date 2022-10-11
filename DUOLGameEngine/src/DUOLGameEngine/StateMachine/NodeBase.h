/**

	@file    NodeBase.h
	@brief   Behavior Tree용 Node의 기반 클래스
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "DUOLGameEngine/StateMachine/NodeInfo.h"
#include "DUOLGameEngine/Event/EventSystem.h"

namespace DUOLGameEngine
{
	/**

		@class   NodeBase
		@brief	 Behavior Tree용 Node의 기반 클래스
		@details -

	**/
	class NodeBase
	{
	public:
		// <리턴 값, Delta Time, PrevState, CurrentState>
		using PreEvent = EventSystem<NodeState, float, NodeBase*, NodeState, NodeState>;
		using PostEvent = EventSystem<NodeState, float, NodeBase*, NodeState, NodeState>;
		using ChangeEvent = EventSystem<NodeState, float, NodeBase*, NodeState, NodeState>;

	private:
		template <typename T>
		struct EventInfo
		{
			unsigned int _eventID;
			std::shared_ptr<T> _event;
		};

		const unsigned int _UID;

		std::string _name;

		NodeType _type;

		NodeState _state;

		// Tick 실행전 동작하는 이벤트
		PreEvent _preEventManager;

		std::vector<EventInfo<PreEvent::EventType>> _preEventList;

		// Tick 실행후 동작하는 이벤트
		PostEvent _postEventManager;

		std::vector<EventInfo<PostEvent::EventType>> _postEventList;

		// State가 변경될 때 동작하는 이벤트
		ChangeEvent _changeEventManager;

		std::vector<EventInfo<ChangeEvent::EventType>> _changeEventList;

	public:
		/**
			@brief   NodeBase 클래스의 생성자
			@details 고유한 ID 부여 및 State default로 Idle state가 된다.
			@param   name - Node의 Name
			@param   type - Node의 Type
		**/
		NodeBase(const std::string& name, NodeType type);

		/**
			@brief   NodeBase 클래스 default 소멸자
			@details -
		**/
		~NodeBase() = default;

		/**
			@brief   Node의 Unique ID를 받아온다.
			@details -
			@retval  Node의 UID
		**/
		const unsigned int GetUID() const { return _UID; }

		/**
			@brief   Node의 Name을 받아온다.
			@details -
			@retval  Node의 Name
		**/
		const std::string& GetName() const { return _name; }

		/**
			@brief   Node의 Type을 받아온다.
			@details -
			@retval  Node의 Type
		**/
		NodeType GetType() const { return _type; }

		/**
			@brief   Node의 State를 받아온다.
			@details -
			@retval  Node의 State
		**/
		NodeState GetState() const { return _state; }

		/**
			@brief   Node의 State를 변경하고, Change Event를 실행시킨다.
			@details -
			@param   state - 변경할 Node의 State 값
		**/
		void SetState(NodeState state);

		/**
			@brief   Node의 State가 Idle인지 확인하고 Bool 값을 반환한다.
			@details -
			@retval  State == Idle 일 경우 true, 아닐 경우 false
		**/
		bool isStopped() const;

		/**
			@brief   Node의 State가 Running인지 확인하고 Bool 값을 반환한다.
			@details -
			@retval  State == Running 일 경우 true, 아닐 경우 false
		**/
		bool IsRunning() const;

		/**
			@brief   Node의 State가 Complete인지 확인하고 Bool 값을 반환한다.
			@details -
			@retval  State == Success 혹은 Failure 일 경우 true, 아닐 경우 false
		**/
		bool IsCompleted() const;

		/**
			@brief   Tick 함수를 호출하기 전 실행할 Previous Event를 추가한다.
			@details -
			@param   preEvent - Functor 객체
			@retval  등록된 Previous Event의 ID 값
		**/
		unsigned int AddPreEvent(PreEvent::EventType preEvent);

		/**
			@brief   Tick 함수를 호출하기 전 실행될 Previous Event를 제거한다.
			@details -
			@param   preEventID - 제거할 Previous Event의 ID
			@retval  제거에 성공했을 경우 true, 아닐 경우 false
		**/
		bool SubPreEvent(unsigned int preEventID);

		/**
			@brief   Tick 함수를 호출한 후 실행할 Post Event를 추가한다.
			@details -
			@param   postEvent - Functor 객체
			@retval  등록된 Post Event의 ID 값
		**/
		unsigned int AddPostEvent(PostEvent::EventType postEvent);

		/**
			@brief	 Tick 함수를 호출한 후 실행될 Post Event를 제거한다.
			@details -
			@param   postEventID - 제거할 Post Event의 ID
			@retval  제거에 성공했을 경우 true, 아닐 경우 false
		**/
		bool SubPostEvent(unsigned int postEventID);

		/**
			@brief   Node의 State가 변경될 때 실행할 Change Event를 추가한다.
			@details -
			@param   changeEvent - Functor 객체
			@retval  등록된 Change Event의 ID 값
		**/
		unsigned int AddChangeEvent(ChangeEvent::EventType changeEvent);

		/**
			@brief   Node의 State가 변경될 때 실행될 Change Event를 제거한다.
			@details -
			@param   changeEventID - 제거할 Change Event의 ID
			@retval  제거에 성공했을 경우 true, 아닐 경우 false
		**/
		bool SubChangeEvent(unsigned int changeEventID);

	public:
		/**
			@brief   매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		virtual void Tick() abstract;

		/**
			@brief   Node의 상태를 Idle로 변경하는 함수
			@details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
		**/
		virtual void Stop() abstract;
	};
}