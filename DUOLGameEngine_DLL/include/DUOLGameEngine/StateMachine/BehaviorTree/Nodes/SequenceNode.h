/**

    @file    SequenceNode.h
    @brief   Child Node �� �ϳ��� FAILURE or RUNNING�� ��ȯ�� ������ ��ȸ
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
        SequenceNode(const DUOLCommon::tstring& name);

        /**
            @brief   SequenceNode Ŭ���� default �Ҹ���
            @details -
        **/
        virtual ~SequenceNode() override = default;

    protected:
        /**
            @brief	 �� ������ ȣ��Ǵ� �Լ�
            @details Child Node�� ��ȸ�Ͽ� �����Ű�� Child Node�� ���� ����� ���� ���¸� ��ȯ�Ѵ�.
        **/
        NodeState Tick() override;
	};
}