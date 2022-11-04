/**

    @file    SequenceNode.h
    @brief   Child Node 중 하나라도 FAILURE or RUNNING을 반환할 때까지 순회
    @details -
    @author  JKim
    @date    12.10.2022

**/
#pragma once
#include "ControlNode.h"

namespace DUOLGameEngine
{
 /**

     @class   SequenceNode
     @brief   Child Node 중 하나라도 FAILURE or RUNNING을 반환할 때까지 순회
     @details -

 **/
	class SequenceNode : public ControlNode
	{
    public:
        /**
            @brief   SequenceNode 클래스 생성자
            @details -
            @param   name - Node의 Name
        **/
        SequenceNode(const DUOLCommon::tstring& name);

        /**
            @brief   SequenceNode 클래스 default 소멸자
            @details -
        **/
        virtual ~SequenceNode() override = default;

    protected:
        /**
            @brief	 매 프레임 호출되는 함수
            @details Child Node를 순회하여 실행시키고 Child Node의 리턴 결과에 따라 상태를 반환한다.
        **/
        NodeState Tick() override;
	};
}