/**

	@file    ActionNode.h
	@brief	 동작을 구현하는 클래스
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
		@brief	 동작을 구현하는 클래스
		@details -

	**/
	class ActionNode : public NodeBase
	{

	public:
		/**
			@brief   ActionNode 클래스 생성자
			@details NodeBase 생성자를 호출할 때 NodeType::ACTION이 지정된다.
			@param   name - Node의 Name
		**/
		ActionNode(const std::string& name);

		/**
			@brief   ActionNode 클래스 default 소멸자
			@details -
		**/
		virtual ~ActionNode() override = default;

		/**
			@brief	 매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		virtual NodeState Tick() abstract;

		/**
			@brief	 Node의 상태를 Idle로 변경하는 함수
			@details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
		**/
		virtual void Stop() abstract;
	};
}