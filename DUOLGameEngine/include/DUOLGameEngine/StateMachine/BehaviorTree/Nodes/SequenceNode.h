/**

    @file    SequenceNode.h
    @brief   Child Node �� �ϳ��� FAILURE or RUNNING�� ��ȯ�� ������ ��ȸ
    @details -
    @author  JKim
    @date    12.10.2022

**/
#pragma once
#include "../src/DUOLGameEngine/StateMachine/BehaviorTree/Nodes/ControlNode.h"

namespace DUOLGameEngine
{
 /**

     @class   SequenceNode
     @brief   Child Node �� �ϳ��� FAILURE or RUNNING�� ��ȯ�� ������ ��ȸ
     @details -

 **/
	class SequenceNode : public ControlNode
	{
    public:
        /**
            @brief   SequenceNode Ŭ���� ������
            @details -
            @param   name - Node�� Name
        **/
        SequenceNode(const std::string& name);

        /**
            @brief   SequenceNode Ŭ���� default �Ҹ���
            @details -
        **/
        ~SequenceNode() = default;

        /**
            @brief	 �� ������ ȣ��Ǵ� �Լ�
            @details Child Node�� ��ȸ�Ͽ� �����Ű�� Child Node�� ���� ����� ���� ���¸� ��ȯ�Ѵ�.
        **/
        NodeState Tick() override;
	};
}