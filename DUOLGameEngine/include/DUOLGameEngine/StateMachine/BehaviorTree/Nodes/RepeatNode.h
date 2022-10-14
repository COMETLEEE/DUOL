/**

	@file    RepeatNode.h
	@brief	 Child Node가 Success를 반환하는 동안 최대 N회 반복하는 노드
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
		@brief	 Child Node가 Success를 반환하는 동안 최대 N회 반복하는 노드
		@details -

	**/
	class RepeatNode : public DecoratorNode
	{
		int _retryTimes;

	public:
		/**
			@brief   RepeatNode 클래스 생성자
			@details -
			@param   name - Node의 Name
			@param   n    - 최대 반복 횟수
		**/
		RepeatNode(const std::string& name, int n);

		/**
			@brief   RepeatNode 클래스 default 소멸자
			@details -
		**/
		virtual ~RepeatNode() override = default;

		/**
			@brief	 최대 반복 횟수 설정
			@details 1이상 값만 사용
			@param   n - 최대 반복 횟수
		**/
		void SetRetryTimes(int n);

	protected:
		/**
			@brief   매 프레임 호출되는 함수
			@details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
		**/
		NodeState Tick() override final;

		/**
			@brief	 Node의 상태를 Idle로 변경하는 함수
			@details Child Node의 모든 동작을 정리하고 Idle 상태로 변경
		**/
		void Stop() override;
	};
}