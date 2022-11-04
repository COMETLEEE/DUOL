/**

	@file    ControlNode.h
	@brief	 Flow�� �����ϴ� Control Node
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "TreeNode.h"

namespace DUOLGameEngine
{
	/**

		@class   ControlNode
		@brief	 Flow�� �����ϴ� Control Node
		@details -

	**/
	class ControlNode : public TreeNode
	{
	public:
		/**
			@brief   ControlNode Ŭ���� ������
			@details TreeNode �����ڸ� ȣ���� �� NodeType::CONTROL�� �����ȴ�.
			@param   name - Node�� Name
		**/
		ControlNode(const DUOLCommon::tstring& name);

		/**
			@brief   ControlNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~ControlNode() override = default;

	protected:
		int _currentIndex;

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