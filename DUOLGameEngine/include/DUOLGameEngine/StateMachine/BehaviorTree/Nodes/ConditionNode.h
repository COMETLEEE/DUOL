/**

	@file    ConditionNode.h
	@brief   Success or Failure�� ��ȯ�ϴ� ���� üũ ���
	@details -
	@author  JKim
	@date    11.10.2022

**/
#pragma once
#include "NodeBase.h"

namespace DUOLGameEngine
{
	/**

		@class   ConditionNode
		@brief   Success or Failure�� ��ȯ�ϴ� ���� üũ ���
		@details -

	**/
	class ConditionNode : public NodeBase
	{
	public:
		/**
			@brief   ConditionNode Ŭ���� ������
			@details NodeBase �����ڸ� ȣ���� �� NodeType::CONDITION�� �����ȴ�.
			@param   name - Node�� Name
		**/
		ConditionNode(const std::string& name);

		/**
			@brief   ConditionNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~ConditionNode() override = default;

	private:
		/**
			@brief	 �� ������ ȣ��Ǵ� �Լ�
			@details ConditionCheck �Լ��� ȣ���ؼ� ���� ���� ���� true�� SUCCESS, false�� FAILURE ��ȯ
		**/
		NodeState Tick() override final;

		/**
			@brief	 Condition Node������ ������� ����.
			@details -
		**/
		void Stop() override final;

	protected:
		/**
			@brief	 �� ������ ȣ���ؼ� ���� üũ�ϴ� �Լ�
			@details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
			@retval  ���� Ȯ���ؼ� true, Ȥ�� false ��ȯ
		**/
		virtual bool ConditionCheck() abstract;
	};
}