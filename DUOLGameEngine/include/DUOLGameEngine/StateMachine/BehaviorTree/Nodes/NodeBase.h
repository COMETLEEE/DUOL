/**
	@file    NodeBase.h
	@brief   Behavior Tree Node 湲곕 대
	@details -
	@author  JKim
	@date    11.10.2022
**/
#pragma once
#include <vector>
#include <memory>

#include "NodeInfo.h"
#include "../BlackBoard.h"
#include "../../../Event/EventSystem.h"
#include "DUOLCommon/StringHelper.h"

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

	/**
		@class   NodeBase
		@brief	 Behavior Tree Node 湲곕 대
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

		// <由ы 媛, Delta Time, PrevState, CurrentState>
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
			@brief   NodeBase 대ㅼ 깆
			@details 怨 ID 遺 諛 State default濡 Idle state媛 .
			@param   name - Node Name
			@param   type - Node Type
		**/
		NodeBase(const DUOLCommon::tstring& name, NodeType type);

		/**
			@brief   NodeBase 대 default 硫몄
			@details -
		**/
		virtual ~NodeBase() = default;

	private:
		const unsigned int _UID;

		DUOLCommon::tstring _name;

		NodeType _type;

		NodeState _state;

		NodeBase* _parent;

		std::weak_ptr<BlackBoard> _blackBoard;

		// Tick ㅽ�  대깽
		PreEventSystem _preEventManager;

		std::vector<EventInfo<PreEvent>> _preEventList;

		// Tick ㅽ  대깽
		PostEventSystem _postEventManager;

		std::vector<EventInfo<PostEvent>> _postEventList;

		// State媛 蹂寃쎈   대깽
		ChangeEventSystem _changeEventManager;

		std::vector<EventInfo<ChangeEvent>> _changeEventList;

	public:
		/**
			@brief   Node�� Unique ID Getter
			@details -
			@retval  Node UID
		**/
		const unsigned int GetUID() const { return _UID; }

		/**
			@brief   Node�� Name Getter
			@details -
			@retval  Node Name
		**/
		const DUOLCommon::tstring& GetName() const { return _name; }

		/**
			@brief   Node�� Type Getter
			@details -
			@retval  Node Type
		**/
		NodeType GetType() const { return _type; }

		/**
			@brief   Node�� State Getter
			@details -
			@retval  Node State
		**/
		NodeState GetState() const { return _state; }

		/**
			@brief	 Node�� Parent Getter
			@details -
			@retval  Node Parent
		**/
		NodeBase* const GetParent() const { return _parent; }

	protected:
		/**
			@brief	 BlackBoard Data瑜 �ν.
			@details -
			@tparam  T       - �ν Data Type
			@param   data    - �ν Data
			@param   keyName - �ν Data Key 媛
		**/
		template<typename T>
		void PushDataToBlackBoard(T data, const DUOLCommon::tstring& keyName);

		/**
			@brief	 BlackBoard Data瑜 諛⑤.
			@details -
			@tparam  T       - 諛 Data Type
			@param   keyName - 諛 Data
			@retval  諛 Data Key 媛
		**/
		template<typename T>
		T& GetDataFromBlackBoard(const DUOLCommon::tstring& keyName);

		/**
			@brief	 BlackBoard Data瑜 爰쇰몃.
			@details -
			@tparam  T       - 爰쇰댁 Data Type
			@param   keyName - 爰쇰댁 Data
			@retval  爰쇰댁 Data Key 媛
		**/
		template<typename T>
		T PopDataFromBlackBoard(const DUOLCommon::tstring& keyName);

		/**
			@brief   Node State瑜 蹂寃쏀怨, Change Event瑜 ㅽ⑤.
			@details -
			@param   state - 蹂寃쏀 Node State 媛
		**/
		void SetState(NodeState state);

		/**
			@brief	 Node�� Parent Setter
			@details -
			@param   parent - Target Parent
		**/
		void SetParent(NodeBase* parent);

		/**
			@brief	 BlackBoard Setter
			@details -
			@param   blackboard - Target Blackboard
		**/
		void SetBlackBoard(const std::shared_ptr<BlackBoard>& blackboard);

	public:
		/**
			@brief   Node State媛 Idle몄 명怨 Bool 媛 諛.
			@details -
			@retval  State == Idle  寃쎌 true,  寃쎌 false
		**/
		bool IsStopped() const;

		/**
			@brief   Node State媛 Running몄 명怨 Bool 媛 諛.
			@details -
			@retval  State == Running  寃쎌 true,  寃쎌 false
		**/
		bool IsRunning() const;

		/**
			@brief   Node State媛 Complete몄 명怨 Bool 媛 諛.
			@details -
			@retval  State == Success 뱀 Failure  寃쎌 true,  寃쎌 false
		**/
		bool IsCompleted() const;

		/**
			@brief	 Node Event 異媛
			@details -
			@tparam  T     - Event Type
			@param   event - Event Functor 媛泥
			@retval  Event ID 媛
		**/
		template<typename T>
		unsigned int AddEvent(T event);

		/**
			@brief	 Node Event �嫄
			@details -
			@tparam  T       - Event Type
			@param   eventID - Event ID
			@retval  �嫄곕 Event媛 ㅻ㈃ true, ㅻ㈃ false
		**/
		template<typename T>
		bool SubEvent(unsigned int eventID);

	protected:
		/**
			@brief	 Tick怨 Event 몄 濡몄
			@details -
		**/
		NodeState Execute();

		/**
			@brief   留 � 몄 ⑥
			@details 諛 Child Class Update怨 띠 ㅼ 援ы.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief   Node 瑜 Idle濡 蹂寃쏀 ⑥
			@details 諛 Child Class 硫痍  怨 띠 댁⑹ 援ы.
		**/
		virtual void Stop() abstract;

	private:
		/**
			@brief	 Node Unique ID  ⑥
			@details -
			@retval  1 ~ 42듦吏 由ы 媛
		**/
		unsigned int GetNewID();

		/**
			@brief	 Event ID  ⑥
			@details -
			@retval  1 ~ 42듦吏 由ы 媛
		**/
		unsigned int GetNewEventID();
	};

	template<typename T>
	inline void NodeBase::PushDataToBlackBoard(T data, const DUOLCommon::tstring& keyName)
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
	inline T& NodeBase::GetDataFromBlackBoard(const DUOLCommon::tstring& keyName)
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
	inline T NodeBase::PopDataFromBlackBoard(const DUOLCommon::tstring& keyName)
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