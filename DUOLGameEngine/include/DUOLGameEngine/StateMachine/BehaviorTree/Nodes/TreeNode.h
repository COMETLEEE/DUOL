/**

	@file    TreeNode.h
	@brief	 Child Node를 가지는 Node들의 상속 클래스
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
		@brief	 Child Node를 가지는 Node들의 상속 클래스
		@details -

	**/
	class TreeNode : public NodeBase
	{
	protected:
		std::vector<std::shared_ptr<NodeBase>> _children;

	public:
		/**
			@brief   TreeNode 클래스 생성자
			@details -
			@param   name - Node의 Name
			@param   type - Node의 Type
		**/
		TreeNode(const std::string& name, NodeType type);

		/**
			@brief   TreeNode 클래스 default 소멸자
			@details -
		**/
		virtual ~TreeNode() override = default;

		/**
			@brief   Tree Node에 Child Node 추가
			@details -
			@tparam  T    - 추가할 Child Node의 Type
			@tparam  Args - 추가할 Child Node 생성자의 인자 Type
			@param   args - 추가할 Child Node 생성자의 인수
			@retval  생성된 Child Node의 weak_ptr 반환
		**/
		template <class T, typename ...Args>
		std::weak_ptr<T> AddChild(Args ...args);

		/**
			@brief   Tree Node의 Child Node 제거
			@details -
			@param   UID - 제거할 Node의 Unique ID
			@retval  제거에 성공하면 true, 아닌 경우 false
		**/
		bool SubChild(unsigned int UID);

	protected:
		/**
			@brief   매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief   Node의 상태를 Idle로 변경하는 함수
			@details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
		**/
		virtual void Stop() abstract;
	};

	template<class T, typename ...Args>
	inline std::weak_ptr<T> TreeNode::AddChild(Args ...args)
	{
		static_assert(std::is_base_of<NodeBase, T>::value, "TreeNode's children must inherit NodeBase.");

		auto sharedPtr = std::make_shared<T>(args...);

		sharedPtr->SetParent(this);

		_children.push_back(sharedPtr);

		return sharedPtr;
	}
}