/**

	@file    RetryNode.h
	@brief	 Child Node가 Success를 반환할 때까지 최대 N회 체크하는 노드
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
		@brief	 Child Node가 Success를 반환할 때까지 최대 N회 체크하는 노드
		@details -

	**/
	class RetryNode : public DecoratorNode
	{
	public:
		/**
			@brief   RetryNode 클래스 생성자
			@details -
			@param   name - Node의 Name
			@param   n    - 최대 체크 횟수
		**/
		RetryNode(const DUOLCommon::tstring& name, int n);

		/**
			@brief   RetryNode 클래스 default 소멸자
			@details -
		**/
		virtual ~RetryNode() override = default;

	private:
		int _retryTimes;

	public:
		/**
			@brief	 최대 체크 횟수 설정
			@details 1이상 값만 사용
			@param   n - 최대 체크 횟수
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