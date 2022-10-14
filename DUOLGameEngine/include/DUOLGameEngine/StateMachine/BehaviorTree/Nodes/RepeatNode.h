/**

	@file    RepeatNode.h
	@brief	 Child Node�� Success�� ��ȯ�ϴ� ���� �ִ� Nȸ �ݺ��ϴ� ���
	@details -
	@author  JKim
	@date    14.10.2022

**/
#pragma once
#include "DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   RepeatNode
		@brief	 Child Node�� Success�� ��ȯ�ϴ� ���� �ִ� Nȸ �ݺ��ϴ� ���
		@details -

	**/
	class RepeatNode : public DecoratorNode
	{
		int _retryTimes;

	public:
		/**
			@brief   RepeatNode Ŭ���� ������
			@details -
			@param   name - Node�� Name
			@param   n    - �ִ� �ݺ� Ƚ��
		**/
		RepeatNode(const std::string& name, int n);

		/**
			@brief   RepeatNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~RepeatNode() override = default;

		/**
			@brief	 �ִ� �ݺ� Ƚ�� ����
			@details 1�̻� ���� ���
			@param   n - �ִ� �ݺ� Ƚ��
		**/
		void SetRetryTimes(int n);

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