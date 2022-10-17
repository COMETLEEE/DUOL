/**

	@file    ConditionNode.h
	@brief   Success or Failure만 반환하는 조건 체크 노드
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
		@brief   Success or Failure만 반환하는 조건 체크 노드
		@details -

	**/
	class ConditionNode : public NodeBase
	{
	public:
		/**
			@brief   ConditionNode 클래스 생성자
			@details NodeBase 생성자를 호출할 때 NodeType::CONDITION이 지정된다.
			@param   name - Node의 Name
		**/
		ConditionNode(const std::string& name);

		/**
			@brief   ConditionNode 클래스 default 소멸자
			@details -
		**/
		virtual ~ConditionNode() override = default;

	private:
		/**
			@brief	 매 프레임 호출되는 함수
			@details ConditionCheck 함수를 호출해서 리턴 값에 따라 true면 SUCCESS, false면 FAILURE 반환
		**/
		NodeState Tick() override final;

		/**
			@brief	 Condition Node에서는 사용하지 않음.
			@details -
		**/
		void Stop() override final;

	protected:
		/**
			@brief	 매 프레임 호출해서 조건 체크하는 함수
			@details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
			@retval  조건 확인해서 true, 혹은 false 반환
		**/
		virtual bool ConditionCheck() abstract;
	};
}