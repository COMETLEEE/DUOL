/**

	@file    TreeNode.h
	@brief	 Child Node�� ������ Node���� ��� Ŭ����
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "NodeBase.h"

namespace DUOLGameEngine
{
	/**

		@class   TreeNode
		@brief	 Child Node�� ������ Node���� ��� Ŭ����
		@details -

	**/
	class TreeNode : public NodeBase
	{
	protected:
		std::vector<std::shared_ptr<NodeBase>> _children;

	public:
		/**
			@brief   TreeNode Ŭ���� ������
			@details -
			@param   name - Node�� Name
			@param   type - Node�� Type
		**/
		TreeNode(const std::string& name, NodeType type);

		/**
			@brief   TreeNode Ŭ���� default �Ҹ���
			@details -
		**/
		~TreeNode() = default;

		/**
			@brief   Tree Node�� Child Node �߰�
			@details -
			@tparam  T    - �߰��� Child Node�� Type
			@tparam  Args - �߰��� Child Node �������� ���� Type
			@param   args - �߰��� Child Node �������� �μ�
			@retval  ������ Child Node�� weak_ptr ��ȯ
		**/
		template <class T, typename ...Args>
		std::weak_ptr<T> AddChild(Args ...args);

		/**
			@brief   Tree Node�� Child Node ����
			@details -
			@param   UID - ������ Node�� Unique ID
			@retval  ���ſ� �����ϸ� true, �ƴ� ��� false
		**/
		bool SubChild(unsigned int UID);

		/**
			@brief   �� ������ ȣ��Ǵ� �Լ�
			@details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief   Node�� ���¸� Idle�� �����ϴ� �Լ�
			@details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
		**/
		virtual void Stop() abstract;
	};

	template<class T, typename ...Args>
	inline std::weak_ptr<T> TreeNode::AddChild(Args ...args)
	{
		static_assert(std::is_base_of<NodeBase, T>::value, "TreeNode's children must inherit NodeBase.");

		auto sharedPtr = std::make_shared<T>(args...);

		_children.insert(sharedPtr);

		return sharedPtr;
	}
}