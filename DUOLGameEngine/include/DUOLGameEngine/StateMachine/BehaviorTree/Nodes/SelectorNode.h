/**

    @file    SelectorNode.h
    @brief   Child Node �� �ϳ��� SUCCESS or RUNNING�� ��ȯ�� ������ ��ȸ
    @details -
    @author  JKim
    @date    12.10.2022

**/
#pragma once
#include "../src/DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ControlNode.h"

namespace DUOLGameEngine
{
 /**

     @class   SelectorNode
     @brief   Child Node �� �ϳ��� SUCCESS or RUNNING�� ��ȯ�� ������ ��ȸ
     @details -

 **/
	class SelectorNode final : public ControlNode
	{
    public:
        /**
            @brief   SelectorNode Ŭ���� ������
            @details -
            @param   name - Node�� Name 
        **/
        SelectorNode(const std::string& name);

        /**
            @brief   SelectorNode Ŭ���� default �Ҹ���
            @details -
        **/
        ~SelectorNode() = default;

        /**
            @brief	 �� ������ ȣ��Ǵ� �Լ�
            @details Child Node�� ��ȸ�Ͽ� �����Ű�� Child Node�� ���� ����� ���� ���¸� ��ȯ�Ѵ�.
        **/
        NodeState Tick() override;
	};
}