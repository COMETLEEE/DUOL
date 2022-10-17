#include "DUOLGameEngine/StateMachine/BehaviorTree/Nodes/NodeBase.h"

namespace DUOLGameEngine
{
	NodeBase::NodeBase(const std::string& name, NodeType type) :
		_UID(GetNewID())
		, _name(name)
		, _type(type)
		, _state(NodeState::IDLE)
		, _parent(nullptr)
	{

	}

	void NodeBase::SetState(NodeState state)
	{
		if (_state == state)
			return;

		_changeEventManager.Dispatch(this, _state, state);
		_state = state;

		if (IsStopped() == true)
			Stop();
	}

	void NodeBase::SetParent(NodeBase* parent)
	{
		this->_parent = parent;
	}

	void NodeBase::SetBlackBoard(const std::shared_ptr<BlackBoard>& blackboard)
	{
		this->_blackBoard = blackboard;
	}

	bool NodeBase::IsStopped() const
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

	NodeState NodeBase::Execute()
	{
		NodeState prevState = _state;

		_preEventManager.Dispatch(0.0166f/* DeltaTime */, this, prevState);

		SetState(NodeState::RUNNING);

		NodeState newState = Tick();

		_postEventManager.Dispatch(0.0166f/* DeltaTime */, this, prevState, newState);

		SetState(newState);

		return _state;
	}

	unsigned int NodeBase::GetNewID()
	{
		static unsigned int id = 1;

		return id++;
	}

	unsigned int NodeBase::GetNewEventID()
	{
		static unsigned int id = 1;

		return id++;
	}
}