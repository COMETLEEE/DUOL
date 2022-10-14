/**

	@file    NodeBase.h
	@brief   Behavior Tree�� Node�� ��� Ŭ����
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include <string>
#include <vector>
#include <memory>

#include "NodeInfo.h"
#include "../../../Event/EventSystem.h"

namespace DUOLGameEngine
{
	/**

		@class   NodeBase
		@brief	 Behavior Tree�� Node�� ��� Ŭ����
		@details -

	**/
	class NodeBase
	{
		// <���� ��, Delta Time, PrevState, CurrentState>
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

		const unsigned int _UID;

		std::string _name;

		NodeType _type;

		NodeState _state;

	private:
		// Tick ������ �����ϴ� �̺�Ʈ
		PreEventSystem _preEventManager;

		std::vector<EventInfo<PreEvent>> _preEventList;

		// Tick ������ �����ϴ� �̺�Ʈ
		PostEventSystem _postEventManager;

		std::vector<EventInfo<PostEvent>> _postEventList;

		// State�� ����� �� �����ϴ� �̺�Ʈ
		ChangeEventSystem _changeEventManager;

		std::vector<EventInfo<ChangeEvent>> _changeEventList;

	public:
		/**
			@brief   NodeBase Ŭ������ ������
			@details ������ ID �ο� �� State default�� Idle state�� �ȴ�.
			@param   name - Node�� Name
			@param   type - Node�� Type
		**/
		NodeBase(const std::string& name, NodeType type);

		/**
			@brief   NodeBase Ŭ���� default �Ҹ���
			@details -
		**/
		~NodeBase() = default;

		/**
			@brief   Node�� Unique ID�� �޾ƿ´�.
			@details -
			@retval  Node�� UID
		**/
		const unsigned int GetUID() const { return _UID; }

		/**
			@brief   Node�� Name�� �޾ƿ´�.
			@details -
			@retval  Node�� Name
		**/
		const std::string& GetName() const { return _name; }

		/**
			@brief   Node�� Type�� �޾ƿ´�.
			@details -
			@retval  Node�� Type
		**/
		NodeType GetType() const { return _type; }

		/**
			@brief   Node�� State�� �޾ƿ´�.
			@details -
			@retval  Node�� State
		**/
		NodeState GetState() const { return _state; }

		/**
			@brief   Node�� State�� �����ϰ�, Change Event�� �����Ų��.
			@details -
			@param   state - ������ Node�� State ��
		**/
		void SetState(NodeState state);

		/**
			@brief   Node�� State�� Idle���� Ȯ���ϰ� Bool ���� ��ȯ�Ѵ�.
			@details -
			@retval  State == Idle �� ��� true, �ƴ� ��� false
		**/
		bool IsStopped() const;

		/**
			@brief   Node�� State�� Running���� Ȯ���ϰ� Bool ���� ��ȯ�Ѵ�.
			@details -
			@retval  State == Running �� ��� true, �ƴ� ��� false
		**/
		bool IsRunning() const;

		/**
			@brief   Node�� State�� Complete���� Ȯ���ϰ� Bool ���� ��ȯ�Ѵ�.
			@details -
			@retval  State == Success Ȥ�� Failure �� ��� true, �ƴ� ��� false
		**/
		bool IsCompleted() const;

		/**
			@brief	 Node�� Event �߰�
			@details -
			@tparam  T     - Event�� Type
			@param   event - Event Functor ��ü
			@retval  Event�� ID ��
		**/
		template<typename T>
		unsigned int AddEvent(T event);

		/**
			@brief	 Node�� Event ����
			@details -
			@tparam  T       - Event�� Type
			@param   eventID - Event ID
			@retval  ���ŵǴ� Event�� �ִٸ� true, ���ٸ� false
		**/
		template<typename T>
		bool SubEvent(unsigned int eventID);

		/**
			@brief	 Tick�� Event ȣ�� ���μ���
			@details -
		**/
		NodeState Execute();

	protected:
		/**
			@brief   �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		virtual NodeState Tick() abstract;

	public:
		/**
			@brief   Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
		**/
		virtual void Stop() abstract;

	private:
		/**
			@brief	 Node�� Unique ID ���� �Լ�
			@details -
			@retval  1 ~ 42������� ���� ��
		**/
		unsigned int GetNewID();

		/**
			@brief	 Event�� ID ���� �Լ�
			@details -
			@retval  1 ~ 42������� ���� ��
		**/
		unsigned int GetNewEventID();
	};

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