/**

	@file    SubTree.h
	@brief	 Behavior Tree의 Node들을 그룹화하여 관리하기 위한 클래스
	@details -
	@author  JKim
	@date    12.10.2022

**/
#pragma once
#include "Nodes/DecoratorNode.h"

namespace DUOLGameEngine
{
	/**

		@class   SubTree
		@brief	 Behavior Tree의 Node들을 그룹화하여 관리하기 위한 클래스
		@details -

	**/
	class SubTree : public DecoratorNode
	{
	public:
		/**
			@brief   SubTree 클래스 생성자
			@details -
			@param   treeName - Tree의 Name
		**/
		SubTree(const std::string& treeName);

		/**
			@brief   SubTree 클래스 default 소멸자
			@details -
		**/
		~SubTree() = default;

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