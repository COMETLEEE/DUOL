/**

	@file    RetryNode.h
	@brief	 Child Node�� Success�� ��ȯ�� ������ �ִ� Nȸ üũ�ϴ� ���
	@details -
	@author  JKim
	@date    14.10.2022

**/
#pragma once
#include "DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   RetryNode
		@brief	 Child Node�� Success�� ��ȯ�� ������ �ִ� Nȸ üũ�ϴ� ���
		@details -

	**/
	class RetryNode : public DecoratorNode
	{
	public:
		/**
			@brief   RetryNode Ŭ���� ������
			@details -
			@param   name - Node�� Name
			@param   n    - �ִ� üũ Ƚ��
		**/
		RetryNode(const DUOLCommon::tstring& name, int n);

		/**
			@brief   RetryNode Ŭ���� default �Ҹ���
			@details -
		**/
		virtual ~RetryNode() override = default;

	private:
		int _retryTimes;

	public:
		/**
			@brief	 �ִ� üũ Ƚ�� ����
			@details 1�̻� ���� ���
			@param   n - �ִ� üũ Ƚ��
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