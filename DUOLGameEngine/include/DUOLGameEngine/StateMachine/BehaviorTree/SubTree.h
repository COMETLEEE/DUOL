/**

	@file    SubTree.h
	@brief	 Behavior Tree�� Node���� �׷�ȭ�Ͽ� �����ϱ� ���� Ŭ����
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "../src/DUOLGameEngine/StateMachine/BehaviorTree/Nodes/DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   SubTree
		@brief	 Behavior Tree�� Node���� �׷�ȭ�Ͽ� �����ϱ� ���� Ŭ����
		@details -

	**/
	class SubTree : private DecoratorNode
	{
	public:
		/**
			@brief   SubTree Ŭ���� ������
			@details -
			@param   treeName - Tree�� Name
		**/
		SubTree(const std::string& treeName);

		/**
			@brief   SubTree Ŭ���� default �Ҹ���
			@details -
		**/
		~SubTree() = default;

		void Execute();

	protected:
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