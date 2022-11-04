/**

    @file    AlwaysFailureNode.h
    @brief   Child Node�� Return ���� ������� �׻� Failure ���� ��ȯ
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
        @brief   Child Node�� Return ���� ������� �׻� Failure ���� ��ȯ
        @details -

    **/
    class AlwaysFailureNode : public DecoratorNode
    {
    public:
        /**
            @brief   AlwaysFailureNode Ŭ���� ������
            @details -
            @param   name - Node�� Name
        **/
        AlwaysFailureNode(const DUOLCommon::tstring& name);

        /**
            @brief   AlwaysFailureNode Ŭ���� default �Ҹ���
            @details -
        **/
        virtual ~AlwaysFailureNode() override = default;

    protected:
        /**
            @brief   �� ������ ȣ��Ǵ� �Լ�
            @details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
        **/
        NodeState Tick() override final;

        /**
            @brief	 Node�� ���¸� Idle�� �����ϴ� �Լ�
            @details Child Node�� ��� ������ �����ϰ� Idle ���·� ����
        **/
        void Stop() override;
    };
}