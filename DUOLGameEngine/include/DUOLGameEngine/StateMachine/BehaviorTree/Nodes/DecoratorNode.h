/**

    @file    DecoratorNode.h
    @brief   하나의 Child Node만을 가지는 특수 Node 클래스
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
     @brief   하나의 Child Node만을 가지는 특수 Node 클래스
     @details -

 **/
	class DecoratorNode : public NodeBase
	{
    protected:
        std::shared_ptr<NodeBase> _node;

    public:
        /**
            @brief   DecoratorNode 클래스 생성자
            @details -
            @param   name - Node의 Name
            @param   type - Node의 Type
        **/
        DecoratorNode(const std::string& name, NodeType type);

        /**
            @brief   DecoratorNode 클래스 default 소멸자
            @details -
        **/
        ~DecoratorNode() = default;

        /**
            @brief   Decorator Node에 Node 세팅
            @details -
            @tparam  T    - Node의 Type
            @tparam  Args - Node 생성자의 인자 Type
            @param   args - Node 생성자의 인수
            @retval  생성된 Node의 weak_ptr 반환
        **/
        template <class T, typename ...Args>
        std::weak_ptr<T> SetNode(Args ...args);

        /**
            @brief   매 프레임 호출되는 함수
            @details 상속받은 Child Class에서 Update하고 싶은 동작들을 구현한다.
        **/
        virtual NodeState Tick() abstract;

        /**
            @brief   Node의 상태를 Idle로 변경하는 함수
            @details 상속받은 Child Class에서 멈췄을 때 작업하고 싶은 내용을 구현한다.
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

        return std::dynamic_pointer_cast<T>(_node);
    }
}