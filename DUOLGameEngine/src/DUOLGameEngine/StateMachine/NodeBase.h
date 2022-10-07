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
		NodeBase(const std::string& name, NodeType type);
		
		~NodeBase() = default;

		const unsigned int GetUID() const { return _UID; }

		const std::string& GetName() const { return _name; }

		NodeType GetType() const { return _type; }

		NodeState GetState() const { return _state; }

		void SetState(NodeState state);

		// Idle 상태일 때
		bool IsHalted() const;

		// Running 상태일 때
		bool IsRunning() const;

		// Success 혹은 Failure 상태일 때
		bool IsComplete() const;

		// Tick 실행전 이벤트 추가 / 제거
		unsigned int AddPreEvent(PreEvent::EventType preEvent);

		bool SubPreEvent(unsigned int preEventID);

		// Tick 실행후 이벤트 추가 / 제거
		unsigned int AddPostEvent(PostEvent::EventType postEvent);

		bool SubPostEvent(unsigned int postEventID);

		// State 변경시 이벤트 추가 / 제거
		unsigned int AddChangeEvent(ChangeEvent::EventType changeEvent);

		bool SubChangeEvent(unsigned int changeEventID);

	protected:
		virtual void Tick() abstract;
	};
}