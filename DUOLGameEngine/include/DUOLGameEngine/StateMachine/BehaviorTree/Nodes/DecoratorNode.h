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
    protected:
        std::shared_ptr<NodeBase> _node;

    public:
        /**
            @brief   DecoratorNode Ŭ���� ������
            @details -
            @param   name - Node�� Name
            @param   type - Node�� Type
        **/
        DecoratorNode(const std::string& name, NodeType type);

        /**
            @brief   DecoratorNode Ŭ���� default �Ҹ���
            @details -
        **/
        ~DecoratorNode() = default;

        /**
            @brief   Decorator Node�� Node ����
            @details -
            @tparam  T    - Node�� Type
            @tparam  Args - Node �������� ���� Type
            @param   args - Node �������� �μ�
            @retval  ������ Node�� weak_ptr ��ȯ
        **/
        template <class T, typename ...Args>
        std::weak_ptr<T> SetRoot(Args ...args);

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
    inline std::weak_ptr<T> DecoratorNode::SetRoot(Args ...args)
    {
        static_assert(std::is_base_of<NodeBase, T>::value, "RootNode must inherit NodeBase.");

        if (_node != nullptr)
            _node.reset();

        _node = std::make_shared<T>(args...);

        return _node;
    }
}