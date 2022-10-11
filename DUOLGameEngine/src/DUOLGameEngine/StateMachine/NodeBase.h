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

#include "DUOLGameEngine/StateMachine/NodeInfo.h"
#include "DUOLGameEngine/Event/EventSystem.h"

namespace DUOLGameEngine
{
	/**

		@class   NodeBase
		@brief	 Behavior Tree�� Node�� ��� Ŭ����
		@details -

	**/
	class NodeBase
	{
	public:
		// <���� ��, Delta Time, PrevState, CurrentState>
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

		// Tick ������ �����ϴ� �̺�Ʈ
		PreEvent _preEventManager;

		std::vector<EventInfo<PreEvent::EventType>> _preEventList;

		// Tick ������ �����ϴ� �̺�Ʈ
		PostEvent _postEventManager;

		std::vector<EventInfo<PostEvent::EventType>> _postEventList;

		// State�� ����� �� �����ϴ� �̺�Ʈ
		ChangeEvent _changeEventManager;

		std::vector<EventInfo<ChangeEvent::EventType>> _changeEventList;

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
		bool isStopped() const;

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
			@brief   Tick �Լ��� ȣ���ϱ� �� ������ Previous Event�� �߰��Ѵ�.
			@details -
			@param   preEvent - Functor ��ü
			@retval  ��ϵ� Previous Event�� ID ��
		**/
		unsigned int AddPreEvent(PreEvent::EventType preEvent);

		/**
			@brief   Tick �Լ��� ȣ���ϱ� �� ����� Previous Event�� �����Ѵ�.
			@details -
			@param   preEventID - ������ Previous Event�� ID
			@retval  ���ſ� �������� ��� true, �ƴ� ��� false
		**/
		bool SubPreEvent(unsigned int preEventID);

		/**
			@brief   Tick �Լ��� ȣ���� �� ������ Post Event�� �߰��Ѵ�.
			@details -
			@param   postEvent - Functor ��ü
			@retval  ��ϵ� Post Event�� ID ��
		**/
		unsigned int AddPostEvent(PostEvent::EventType postEvent);

		/**
			@brief	 Tick �Լ��� ȣ���� �� ����� Post Event�� �����Ѵ�.
			@details -
			@param   postEventID - ������ Post Event�� ID
			@retval  ���ſ� �������� ��� true, �ƴ� ��� false
		**/
		bool SubPostEvent(unsigned int postEventID);

		/**
			@brief   Node�� State�� ����� �� ������ Change Event�� �߰��Ѵ�.
			@details -
			@param   changeEvent - Functor ��ü
			@retval  ��ϵ� Change Event�� ID ��
		**/
		unsigned int AddChangeEvent(ChangeEvent::EventType changeEvent);

		/**
			@brief   Node�� State�� ����� �� ����� Change Event�� �����Ѵ�.
			@details -
			@param   changeEventID - ������ Change Event�� ID
			@retval  ���ſ� �������� ��� true, �ƴ� ��� false
		**/
		bool SubChangeEvent(unsigned int changeEventID);

	public:
		/**
			@brief   �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		virtual void Tick() abstract;

		/**
			@brief   Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
		**/
		virtual void Stop() abstract;
	};
}