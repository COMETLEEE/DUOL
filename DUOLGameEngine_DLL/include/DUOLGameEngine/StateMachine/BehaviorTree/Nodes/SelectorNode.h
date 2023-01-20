/**

    @file    SelectorNode.h
    @brief   Child Node �� �ϳ��� SUCCESS or RUNNING�� ��ȯ�� ������ ��ȸ
    @details -
    @author  JKim
    @date    12.10.2022

**/
#pragma once
#include "ControlNode.h"

namespace DUOLGameEngine
{
 /**

     @class   SelectorNode
     @brief   Child Node �� �ϳ��� SUCCESS or RUNNING�� ��ȯ�� ������ ��ȸ
     @details -

 **/
	class SelectorNode : public ControlNode
	{
    public:
        /**
            @brief   SelectorNode Ŭ���� ������
            @details -
            @param   name - Node�� Name 
        **/
        SelectorNode(const DUOLCommon::tstring& name);

        /**
            @brief   SelectorNode Ŭ���� default �Ҹ���
            @details -
        **/
        virtual ~SelectorNode() = default;

    protected:
        /**
            @brief	 �� ������ ȣ��Ǵ� �Լ�
            @details Child Node�� ��ȸ�Ͽ� �����Ű�� Child Node�� ���� ����� ���� ���¸� ��ȯ�Ѵ�.
        **/
        NodeState Tick() override;
	};
}