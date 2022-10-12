#include "../src/DUOLGameEngine/StateMachine/BehaviorTree/Nodes/NodeBase.h"

namespace DUOLGameEngine
{
	unsigned int GetNewID()
	{
		static unsigned int id = 1;

		return id++;
	}

	NodeBase::NodeBase(const std::string& name, NodeType type) :
		_UID(GetNewID())
		, _name(name)
		, _type(type)
		, _state(NodeState::IDLE)
	{

	}

	void NodeBase::SetState(NodeState state)
	{
		_state = state;
	}

	bool NodeBase::isStopped() const
	{
		return _state == NodeState::IDLE;
	}

	bool NodeBase::IsRunning() const
	{
		return _state == NodeState::RUNNING;
	}

	bool NodeBase::IsCompleted() const
	{
		return (_state == NodeState::SUCCESS) || (_state == NodeState::FAILURE);
	}

	unsigned int NodeBase::AddPreEvent(PreEvent::EventType preEvent)
	{
		static unsigned int preEventID = 1;

		auto result = _preEventManager.AddEvent(preEvent);

		_preEventList.push_back({ preEventID, result });

		return preEventID++;
	}

	bool NodeBase::SubPreEvent(unsigned int preEventID)
	{
		for (int i = 0; i < _preEventList.size(); i++)
		{
			if (_preEventList[i]._eventID == preEventID)
			{
				_preEventList.erase(_preEventList.begin() + i);

				return true;
			}
		}

		return false;
	}

	unsigned int NodeBase::AddPostEvent(PostEvent::EventType postEvent)
	{
		static unsigned int postEventID = 1;

		auto result = _postEventManager.AddEvent(postEvent);

		_postEventList.push_back({ postEventID, result });

		return postEventID++;
	}

	bool NodeBase::SubPostEvent(unsigned int postEventID)
	{
		for (int i = 0; i < _postEventList.size(); i++)
		{
			if (_postEventList[i]._eventID == postEventID)
			{
				_postEventList.erase(_postEventList.begin() + i);

				return true;
			}
		}

		return false;
	}

	unsigned int NodeBase::AddChangeEvent(ChangeEvent::EventType changeEvent)
	{
		static unsigned int changeEventID = 1;

		auto result = _changeEventManager.AddEvent(changeEvent);

		_changeEventList.push_back({ changeEventID, result });

		return changeEventID++;
	}

	bool NodeBase::SubChangeEvent(unsigned int changeEventID)
	{
		for (int i = 0; i < _changeEventList.size(); i++)
		{
			if (_changeEventList[i]._eventID == changeEventID)
			{
				_changeEventList.erase(_changeEventList.begin() + i);

				return true;
			}
		}

		return false;
	}
}