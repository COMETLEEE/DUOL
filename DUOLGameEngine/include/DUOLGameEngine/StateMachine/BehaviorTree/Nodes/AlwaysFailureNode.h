/**

    @file    AlwaysFailureNode.h
    @brief   Child Node의 Return 값과 상관없이 항상 Failure 상태 반환
    @details -
    @author  JKim
    @date    14.10.2022

**/
#pragma once
#include "DecoratorNode.h"

namespace DUOLGameEngine
{
    /**

        @class   AlwaysFailureNode
        @brief   Child Node의 Return 값과 상관없이 항상 Failure 상태 반환
        @details -

    **/
    class AlwaysFailureNode : public DecoratorNode
    {
    public:
        /**
            @brief   AlwaysFailureNode 클래스 생성자
            @details -
            @param   name - Node의 Name
        **/
        AlwaysFailureNode(const DUOLCommon::tstring& name);

        /**
            @brief   AlwaysFailureNode 클래스 default 소멸자
            @details -
        **/
        virtual ~AlwaysFailureNode() override = default;

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