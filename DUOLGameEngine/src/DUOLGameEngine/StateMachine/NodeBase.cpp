#include "NodeBase.h"

namespace DUOLGameEngine
{
	unsigned int GetNewID()
	{
		static unsigned int id = 1;

		return id++;
	}

	NodeBase::NodeBase(const std::string& name, NodeType type)
		: _UID(GetNewID())
		, _name(name)
		, _type(type)
		, _state(NodeState::IDLE)
	{

	}

	const unsigned int NodeBase::GetUID() const
	{
		return _UID;
	}

	const std::string& NodeBase::GetName() const
	{
		return _name;
	}

	NodeType NodeBase::GetType() const
	{
		return _type;
	}

	NodeState NodeBase::GetState() const
	{
		return _state;
	}

	void NodeBase::SetState(NodeState state)
	{
		_state = state;
	}

	bool NodeBase::IsRunning() const
	{
		return _state == NodeState::RUNNING;
	}

	bool NodeBase::IsComplete() const
	{
		return (_state == NodeState::SUCCESS) || (_state == NodeState::FAILURE);
	}
}