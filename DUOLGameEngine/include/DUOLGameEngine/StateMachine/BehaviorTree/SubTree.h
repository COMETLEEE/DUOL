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
	class ControlNode;
	class BehaviorTree;

	/**

		@class   SubTree
		@brief	 Behavior Tree�� Node���� �׷�ȭ�Ͽ� �����ϱ� ���� Ŭ����
		@details -

	**/
	class SubTree : public DecoratorNode
	{
		friend BehaviorTree;

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
		virtual ~SubTree() override = default;

		/**
			@brief   ControlNode�� Root�� �� �� �ֵ��� ����
			@details DecoratorNode�� SetNode �������̵�
			@tparam  T    - Node�� Type
			@tparam  Args - Node �������� ���� Type
			@param   args - Node �������� �μ�
			@retval  ������ Node�� weak_ptr ��ȯ
		**/
		template <class T, typename ...Args>
		std::weak_ptr<T> SetNode(Args ...args);

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

	template<class T, typename ...Args>
	inline std::weak_ptr<T> SubTree::SetNode(Args ...args)
	{
		static_assert(std::is_base_of<ControlNode, T>::value, "RootNode must inherit ControlNode.");

		return DecoratorNode::SetNode<T, Args...>(args...);
	}
}