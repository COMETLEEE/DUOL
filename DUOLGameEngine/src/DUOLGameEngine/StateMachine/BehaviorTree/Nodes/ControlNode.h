/**

	@file    ControlNode.h
	@brief	 Flow�� �����ϴ� Control Node
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "../src/DUOLGameEngine/StateMachine/BehaviorTree/Nodes/TreeNode.h"

namespace DUOLGameEngine
{
	/**

		@class   ControlNode
		@brief	 Flow�� �����ϴ� Control Node
		@details -

	**/
	class ControlNode : public TreeNode
	{
	protected:
		int _currentIndex;

	public:
		/**
			@brief   ControlNode Ŭ���� ������
			@details TreeNode �����ڸ� ȣ���� �� NodeType::CONTROL�� �����ȴ�.
			@param   name - Node�� Name
		**/
		ControlNode(const std::string& name);

		/**
			@brief   ControlNode Ŭ���� default �Ҹ���
			@details -
		**/
		~ControlNode() = default;

		/**
			@brief   �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief	 Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details Child Node�� ��� ������ �����ϰ� Idle ���·� ����
		**/
		void Stop() override;
	};
}