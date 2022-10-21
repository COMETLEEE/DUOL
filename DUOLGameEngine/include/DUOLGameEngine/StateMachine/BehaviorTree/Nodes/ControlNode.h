/**

	@file    ControlNode.h
	@brief	 Flow를 제어하는 Control Node
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
		@brief	 Flow를 제어하는 Control Node
		@details -

	**/
	class ControlNode : public TreeNode
	{
	public:
		/**
			@brief   ControlNode 클래스 생성자
			@details TreeNode 생성자를 호출할 때 NodeType::CONTROL이 지정된다.
			@param   name - Node의 Name
		**/
		ControlNode(const DUOLCommon::tstring& name);

		/**
			@brief   ControlNode 클래스 default 소멸자
			@details -
		**/
		virtual ~ControlNode() override = default;

	protected:
		int _currentIndex;

		/**
			@brief   매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief	 Node의 상태를 Idle로 변경하는 함수
			@details Child Node의 모든 동작을 정리하고 Idle 상태로 변경
		**/
		void Stop() override;
	};
}