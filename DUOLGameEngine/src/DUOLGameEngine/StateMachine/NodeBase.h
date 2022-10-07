#pragma once
#include <string>
#include <vector>
#include <memory>

#include "DUOLGameEngine/StateMachine/NodeInfo.h"
#include "DUOLGameEngine/Event/EventManager.h"

namespace DUOLGameEngine
{
	class NodeBase
	{
	public:
		using PreEvent = EventManager<NodeState, float, NodeBase*, NodeState, NodeState>;
		using PostEvent = EventManager<NodeState, float, NodeBase*, NodeState, NodeState>;
		using ChangeEvent = EventManager<NodeState, float, NodeBase*, NodeState, NodeState>;

	private:
		template <typename T>
		struct EventInfo
		{
			unsigned int _eventID;
			std::shader_ptr<T> _event;
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
		NodeBase(const std::string& name, NodeType type);
		
		~NodeBase() = default;

		const unsigned int GetUID() const { return _UID; }

		const std::string& GetName() const { return _name; }

		NodeType GetType() const { return _type; }

		NodeState GetState() const { return _state; }

		void SetState(NodeState state);

		// Idle ������ ��
		bool IsHalted() const;

		// Running ������ ��
		bool IsRunning() const;

		// Success Ȥ�� Failure ������ ��
		bool IsComplete() const;

		// Tick ������ �̺�Ʈ �߰� / ����
		unsigned int AddPreEvent(PreEvent::EventType preEvent);

		bool SubPreEvent(unsigned int preEventID);

		// Tick ������ �̺�Ʈ �߰� / ����
		unsigned int AddPostEvent(PostEvent::EventType postEvent);

		bool SubPostEvent(unsigned int postEventID);

		// State ����� �̺�Ʈ �߰� / ����
		unsigned int AddChangeEvent(ChangeEvent::EventType changeEvent);

		bool SubChangeEvent(unsigned int changeEventID);

	protected:
		virtual void Tick() abstract;
	};
}