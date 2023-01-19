/**

    @file    NodeInfo.h
    @brief   Behavior Tree용 Node의 Type & State Info 헤더
    @details 
			 @li NodeType - Node의 유형 정보

				* NONE
				 - 유형이 결정되지 않음

				* ACTION
				 - 구현된 동작을 수행

				* CONDITION
				 - 주어진 조건의 성공 or 실패를 판단

				* CONTROL
				 - 노드의 흐름 제어, 1 ~ N개의 자식 노드 보유
				 - Sequence, Selector(Fallback)
					
				* DECORATOR
				 - 단 1개의 자식 노드를 보유하고, 자식 노드의 반환 값에 대한 처리
				 - Inverter, ForceSuccess, ForceFailure, Repeat, Retry

				* SUBTREE
				 - 모듈화된 트리

			 @n
			 @li NodeState - Node의 상태 정보

				* IDLE
				 - 동작 없음, IDLE은 Return 값으로 사용되지 않음

				* RUNNING
				 - 동작중

				* SUCCESS
				 - 동작이 성공으로 완료됨

				* FAILURE
				 - 동작이 실패로 완료됨
				
			@n
    @author  JKim
    @date    11.10.2022

**/
#pragma once

namespace DUOLGameEngine
{
	// Node의 유형 정보
	enum class NodeType
	{
		// 유형이 결정되지 않음
		NONE,

		// 구현된 동작을 수행
		ACTION,

		// 주어진 조건의 성공or실패를 판단
		CONDITION,

		/*
		 * 노드의 흐름 제어, 1 ~ N개의 자식 노드 보유
		 * Sequence, Selector(Fall back)
		*/
		CONTROL,

		/*
		 * 단 1개의 자식 노드를 보유하고, 자식 노드의 반환 값에 대한 처리
		 * Inverter, ForceSuccess, ForceFailure, Repeat, Retry
		*/
		DECORATOR,

		// 모듈화된 트리
		SUBTREE
	};

	// Node의 상태 정보
	enum class NodeState
	{
		IDLE,		// 동작 없음, IDLE은 Return 값으로 사용되지 않음
		RUNNING,	// 동작중
		SUCCESS,	// 동작이 성공으로 완료됨
		FAILURE		// 동작이 실패로 완료됨
	};
}