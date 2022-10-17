/**

    @file    AlwaysSuccessNode.h
    @brief   Child Node�� Return ���� ������� �׻� Success ���� ��ȯ
    @details -
    @author  JKim
    @date    14.10.2022

**/
#pragma once
#include "DecoratorNode.h"

namespace DUOLGameEngine
{
    /**

        @class   AlwaysSuccessNode
        @brief   Child Node�� Return ���� ������� �׻� Success ���� ��ȯ
        @details -

    **/
    class AlwaysSuccessNode : public DecoratorNode
    {
    public:
        /**
            @brief   AlwaysSuccessNode Ŭ���� ������
            @details -
            @param   name - Node�� Name
        **/
        AlwaysSuccessNode(const std::string& name);

        /**
            @brief   AlwaysSuccessNode Ŭ���� default �Ҹ���
            @details -
        **/
        virtual ~AlwaysSuccessNode() override = default;

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