/**

    @file    DecoratorNode.h
    @brief   �ϳ��� Child Node���� ������ Ư�� Node Ŭ����
    @details -
    @author  JKim
    @date    12.10.2022

**/
#pragma once
#include "NodeBase.h"

namespace DUOLGameEngine
{
 /**

     @class   DecoratorNode
     @brief   �ϳ��� Child Node���� ������ Ư�� Node Ŭ����
     @details -

 **/
	class DecoratorNode : public NodeBase
	{
    public:
        /**
            @brief   DecoratorNode Ŭ���� ������
            @details -
            @param   name - Node�� Name
            @param   type - Node�� Type
        **/
        DecoratorNode(const DUOLCommon::tstring& name, NodeType type);

        /**
            @brief   DecoratorNode Ŭ���� default �Ҹ���
            @details -
        **/
        virtual ~DecoratorNode() override = default;

    protected:
        std::shared_ptr<NodeBase> _node;

    public:
        /**
            @brief   Decorator Node�� Node Setter
            @details -
            @tparam  T    - Node�� Type
            @tparam  Args - Node �������� ���� Type
            @param   args - Node �������� �μ�
            @retval  ������ Node�� weak_ptr ��ȯ
        **/
        template <class T, typename ...Args>
        std::weak_ptr<T> SetNode(Args ...args);

    protected:
        /**
            @brief   �� ������ ȣ��Ǵ� �Լ�
            @details ��ӹ��� Child Class���� Update�ϰ� ���� ���۵��� �����Ѵ�.
        **/
        virtual NodeState Tick() abstract;

        /**
            @brief   Node�� ���¸� Idle�� �����ϴ� �Լ�
            @details ��ӹ��� Child Class���� ������ �� �۾��ϰ� ���� ������ �����Ѵ�.
        **/
        virtual void Stop() abstract;
	};

    template<class T, typename ...Args>
    inline std::weak_ptr<T> DecoratorNode::SetNode(Args ...args)
    {
        static_assert(std::is_base_of<NodeBase, T>::value, "DecoratorNode must inherit NodeBase.");

        if (_node != nullptr)
            _node.reset();

        _node = std::make_shared<T>(args...);

        _node->SetParent(this);

        return std::dynamic_pointer_cast<T>(_node);
    }
}