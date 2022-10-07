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

		// Running ������ ��
		bool IsRunning() const;

		// Success Ȥ�� Failure ������ ��
		bool IsComplete() const;

	protected:
		virtual void Tick() abstract;
	};
}