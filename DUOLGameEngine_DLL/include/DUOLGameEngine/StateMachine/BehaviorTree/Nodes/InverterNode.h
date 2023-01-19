/**

	@file    InverterNode.h
	@brief	 Child Node�� ��ȯ ���� ����� Return�ϴ� ���
	@details -
	@author  JKim
	@date    14.10.2022

**/
#pragma once
#include "DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   InverterNode
		@brief	 Child Node�� ��ȯ ���� ����� Return�ϴ� ���
		@details -

	**/
	class InverterNode : public DecoratorNode
	{
	public:
		/**
			@brief   InverterNode Ŭ���� ������
			@details -
			@param   name - Node�� Name
		**/
		InverterNode(const DUOLCommon::tstring& name);

		/**
			@brief   InverterNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~InverterNode() override = default;

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