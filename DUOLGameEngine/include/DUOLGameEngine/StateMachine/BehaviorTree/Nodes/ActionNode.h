/**

	@file    ActionNode.h
	@brief	 ������ �����ϴ� Ŭ����
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include "NodeBase.h"

namespace DUOLGameEngine
{
	/**

		@class   ActionNode
		@brief	 ������ �����ϴ� Ŭ����
		@details -

	**/
	class ActionNode : public NodeBase
	{

	public:
		/**
			@brief   ActionNode Ŭ���� ������
			@details NodeBase �����ڸ� ȣ���� �� NodeType::ACTION�� �����ȴ�.
			@param   name - Node�� Name
		**/
		ActionNode(const std::string& name);

		/**
			@brief   ActionNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~ActionNode() override = default;

		/**
			@brief	 �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief	 Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
		**/
		virtual void Stop() abstract;
	};
}