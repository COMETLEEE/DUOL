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

#include "NodeInfo.h"
#include "../BlackBoard.h"
#include "../../../Event/EventSystem.h"

namespace DUOLGameEngine
{
	class RetryNode;
	class RepeatNode;
	class AlwaysFailureNode;
	class AlwaysSuccessNode;
	class InverterNode;
	class SubTree;
	class SelectorNode;
	class SequenceNode;
	class ControlNode;
	class DecoratorNode;
	class TreeNode;


	int test(float f, char c)
	{

	}

	/**

		@class   NodeBase
		@brief	 Behavior Tree용 Node의 기반 클래스
		@details -

	**/
	class NodeBase
	{
		friend RetryNode;
		friend RepeatNode;
		friend AlwaysFailureNode;
		friend AlwaysSuccessNode;
		friend InverterNode;
		friend SubTree;
		friend SelectorNode;
		friend SequenceNode;
		friend DecoratorNode;
		friend ControlNode;
		friend TreeNode;

		// <리턴 값, Delta Time, PrevState, CurrentState>
		using PreEventSystem = EventSystem<void, float, NodeBase*, NodeState>;
		using PostEventSystem = EventSystem<void, float, NodeBase*, NodeState, NodeState>;
		using ChangeEventSystem = EventSystem<void, NodeBase*, NodeState, NodeState>;

	public:
		using PreEvent = PreEventSystem::EventType;
		using PostEvent = PostEventSystem::EventType;
		using ChangeEvent = ChangeEventSystem::EventType;

	private:
		template <typename T>
		struct EventInfo
		{
			unsigned int _eventID;
			std::shared_ptr<T> _event;
		};

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
		virtual ~NodeBase() = default;

	private:
		const unsigned int _UID;

		std::string _name;

		NodeType _type;

		NodeState _state;

		NodeBase* _parent;

		std::weak_ptr<BlackBoard> _blackBoard;

		// Tick 실행전 동작하는 이벤트
		PreEventSystem _preEventManager;

		std::vector<EventInfo<PreEvent>> _preEventList;

		// Tick 실행후 동작하는 이벤트
		PostEventSystem _postEventManager;

		std::vector<EventInfo<PostEvent>> _postEventList;

		// State가 변경될 때 동작하는 이벤트
		ChangeEventSystem _changeEventManager;

		std::vector<EventInfo<ChangeEvent>> _changeEventList;

	public:
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
			@brief	 Node의 Parent를 받아온다.
			@details -
			@retval  Node의 Parent
		**/
		NodeBase* const GetParent() const { return _parent; }

	protected:
		/**
			@brief	 BlackBoard에 Data를 저장한다.
			@details -
			@tparam  T       - 저장할 Data Type
			@param   data    - 저장할 Data
			@param   keyName - 저장할 Data의 Key 값
		**/
		template<typename T>
		void PushDataToBlackBoard(T data, const std::string& keyName);

		/**
			@brief	 BlackBoard에서 Data를 받아온다.
			@details -
			@tparam  T       - 받아올 Data Type
			@param   keyName - 받아올 Data
			@retval  받아올 Data의 Key 값
		**/
		template<typename T>
		T& GetDataFromBlackBoard(const std::string& keyName);

		/**
			@brief	 BlackBoard에서 Data를 꺼낸다.
			@details -
			@tparam  T       - 꺼내올 Data Type
			@param   keyName - 꺼내올 Data
			@retval  꺼내올 Data의 Key 값
		**/
		template<typename T>
		T PopDataFromBlackBoard(const std::string& keyName);

		/**
			@brief   Node의 State를 변경하고, Change Event를 실행시킨다.
			@details -
			@param   state - 변경할 Node의 State 값
		**/
		void SetState(NodeState state);

		/**
			@brief	 Node의 부모를 설정한다.
			@details -
			@param   parent - Target Parent
		**/
		void SetParent(NodeBase* parent);

		/**
			@brief	 BlackBoard 설정
			@details -
			@param   blackboard - Target Blackboard
		**/
		void SetBlackBoard(const std::shared_ptr<BlackBoard>& blackboard);

	public:
		/**
			@brief   Node의 State가 Idle인지 확인하고 Bool 값을 반환한다.
			@details -
			@retval  State == Idle 일 경우 true, 아닐 경우 false
		**/
		bool IsStopped() const;

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
			@brief	 Node에 Event 추가
			@details -
			@tparam  T     - Event의 Type
			@param   event - Event Functor 객체
			@retval  Event의 ID 값
		**/
		template<typename T>
		unsigned int AddEvent(T event);

		/**
			@brief	 Node에 Event 제거
			@details -
			@tparam  T       - Event의 Type
			@param   eventID - Event ID
			@retval  제거되는 Event가 있다면 true, 없다면 false
		**/
		template<typename T>
		bool SubEvent(unsigned int eventID);

	protected:
		/**
			@brief	 Tick과 Event 호출 프로세스
			@details -
		**/
		NodeState Execute();

		/**
			@brief   매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief   Node의 상태를 Idle로 변경하는 함수
			@details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
		**/
		virtual void Stop() abstract;

	private:
		/**
			@brief	 Node의 Unique ID 생성 함수
			@details -
			@retval  1 ~ 42억까지의 리턴 값
		**/
		unsigned int GetNewID();

		/**
			@brief	 Event의 ID 생성 함수
			@details -
			@retval  1 ~ 42억까지의 리턴 값
		**/
		unsigned int GetNewEventID();
	};

	template<typename T>
	inline void NodeBase::PushDataToBlackBoard(T data, const std::string& keyName)
	{
		try
		{
			if (this->_parent != nullptr)
				return _parent->PushDataToBlackBoard(data, keyName);

			auto bb = _blackBoard.lock();

			if (bb == nullptr)
				throw "No blackboard";

			bb->Push(data, keyName);
		}
		catch (const char* errStr)
		{

		}
		catch (...)
		{

		}
	}

	template<typename T>
	inline T& NodeBase::GetDataFromBlackBoard(const std::string& keyName)
	{
		try
		{
			if (this->_parent != nullptr)
				return _parent->GetDataFromBlackBoard<T>(keyName);

			auto bb = _blackBoard.lock();

			if (bb == nullptr)
				throw "No blackboard";

			return bb->Get<T>(keyName);
		}
		catch (const char* errStr)
		{

		}
		catch (...)
		{

		}
	}

	template<typename T>
	inline T NodeBase::PopDataFromBlackBoard(const std::string& keyName)
	{
		try
		{
			if (this->_parent != nullptr)
				return _parent->PopDataFromBlackBoard<T>(keyName);

			auto bb = _blackBoard.lock();

			if (bb == nullptr)
				throw "No blackboard";

			return bb->Pop<T>(keyName);
		}
		catch (const char* errStr)
		{

		}
		catch (...)
		{

		}
	}

	template<typename T>
	inline unsigned int NodeBase::AddEvent(T event)
	{
		static_assert(
			std::is_same<T, PreEvent>::value
			|| std::is_same<T, PostEvent>::value
			|| std::is_same<T, ChangeEvent>::value
			,
			"Node Event mush be one of PreEvent, PostEvent or ChangeEvent");

		return 0;
	}

	template<>
	inline unsigned int NodeBase::AddEvent(PreEvent event)
	{
		unsigned int preEventID = GetNewEventID();

		auto result = _preEventManager.AddEvent(event);

		_preEventList.push_back({ preEventID, result });

		return preEventID;
	}

	template<>
	inline unsigned int NodeBase::AddEvent(PostEvent event)
	{
		unsigned int postEventID = GetNewEventID();

		auto result = _postEventManager.AddEvent(event);

		_postEventList.push_back({ postEventID, result });

		return postEventID;
	}

	template<>
	inline unsigned int NodeBase::AddEvent(ChangeEvent event)
	{
		unsigned int changeEventID = GetNewEventID();

		auto result = _changeEventManager.AddEvent(event);

		_changeEventList.push_back({ changeEventID, result });

		return changeEventID;
	}

	template<typename T>
	inline bool NodeBase::SubEvent(unsigned int eventID)
	{
		static_assert(true, "Node Event mush be one of PreEvent, PostEvent or ChangeEvent");

		return false;
	}

	template<>
	inline bool NodeBase::SubEvent<NodeBase::PreEvent>(unsigned int eventID)
	{
		for (int i = 0; i < _preEventList.size(); i++)
		{
			if (_preEventList[i]._eventID == eventID)
			{
				_preEventList.erase(_preEventList.begin() + i);

				return true;
			}
		}

		return false;
	}

	template<>
	inline bool NodeBase::SubEvent<NodeBase::PostEvent>(unsigned int eventID)
	{
		for (int i = 0; i < _postEventList.size(); i++)
		{
			if (_postEventList[i]._eventID == eventID)
			{
				_postEventList.erase(_postEventList.begin() + i);

				return true;
			}
		}

		return false;
	}

	template<>
	inline bool NodeBase::SubEvent<NodeBase::ChangeEvent>(unsigned int eventID)
	{
		for (int i = 0; i < _changeEventList.size(); i++)
		{
			if (_changeEventList[i]._eventID == eventID)
			{
				_changeEventList.erase(_changeEventList.begin() + i);

				return true;
			}
		}

		return false;
	}
}