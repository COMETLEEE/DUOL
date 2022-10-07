#pragma once
#include "DUOLGameEngine/StateMachine/NodeInfo.h"

#include <string>

namespace DUOLGameEngine
{
	class NodeBase
	{
		const unsigned int _UID;

		std::string _name;

		NodeType _type;

		NodeState _state;

	public:
		NodeBase(const std::string& name, NodeType type);
		
		~NodeBase() = default;

		const unsigned int GetUID() const;

		const std::string& GetName() const;

		NodeType GetType() const;

		NodeState GetState() const;

		void SetState(NodeState state);

		// Running 상태일 때
		bool IsRunning() const;

		// Success 혹은 Failure 상태일 때
		bool IsComplete() const;

	protected:
		virtual void Tick() abstract;
	};
}