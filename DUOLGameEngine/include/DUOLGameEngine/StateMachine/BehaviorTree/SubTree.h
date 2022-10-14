/**

	@file    SubTree.h
	@brief	 Behavior Tree�� Node���� �׷�ȭ�Ͽ� �����ϱ� ���� Ŭ����
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "Nodes/DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   SubTree
		@brief	 Behavior Tree�� Node���� �׷�ȭ�Ͽ� �����ϱ� ���� Ŭ����
		@details -

	**/
	class SubTree : public DecoratorNode
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

	protected:
		/**
			@brief   �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		NodeState Tick() override final;

		/**
			@brief	 Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details Child Node�� ��� ������ �����ϰ� Idle ���·� ����
		**/
		void Stop() override;
	};
}