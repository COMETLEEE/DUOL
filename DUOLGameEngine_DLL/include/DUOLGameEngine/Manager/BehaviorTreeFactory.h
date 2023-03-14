/**
	@file      BehaviorTreeFactory.h
	@brief     BehaviorTreeFactory
	@details   ~ BehaviorTreeFactory를 래핑한 클래스.
	@author    SinSeonghyeon
	@date      03.07.2023
	@copyright © SinSeonghyeon, 2023. All right reserved.
**/
#pragma once

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "behaviortree_cpp/bt_factory.h"
#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API BehaviorTreeFactory : public SingletonBase<BehaviorTreeFactory>
	{
		DECLARE_SINGLETON(BehaviorTreeFactory);

		DELETE_COPY_MOVE(BehaviorTreeFactory);
	private:
		BT::BehaviorTreeFactory* _factory;
	public:
		virtual ~BehaviorTreeFactory() override;

		/**
			  @brief 모든 노드를 등록한 후에 사용하여야 한다.
		 **/
		void Initialize();

		void UnInitialize();
		// --------------------------------------------- 노드 등록 함수.
		/*
		* \brief 함수 객체를 통해 액션 노드 등록.
		*/
		void RegisterSimpleAction(const std::string& ID,
			const BT::SimpleActionNode::TickFunctor& tick_functor,
			BT::PortsList ports = {});
		/**
		* \brief 함수 객체를 통해 컨디션 노드 등록.
		*/
		void RegisterSimpleCondition(const std::string& ID,
			const BT::SimpleConditionNode::TickFunctor& tick_functor,
			BT::PortsList ports = {});
		/**
		* \brief 함수 객체를 통해 데코레이터 노드 등록.
		*/
		void RegisterSimpleDecorator(const std::string& ID,
			const BT::SimpleDecoratorNode::TickFunctor& tick_functor,
			BT::PortsList ports = {});
		/**
		* \brief 노드 클래스를 상속받은 클래스를 통해 노드 등록.
		*/
		template <typename T, typename... ExtraArgs>
		void RegisterNodeType(const std::string& ID, ExtraArgs... args);

		// --------------------------------------------- 트리 등록 함수.
		/**
		* \brief 문자열을 통해 트리 등록
		*/
		void RegisterTreeFromText(const std::string& xml_str);
		/**
		* \brief Xml 파일을 통해 트리 등록
		*/
		void RegisterTreeFromFile(const std::string& file_path);
		/**
		* \brief Xml 파일을 통해 트리 등록
		*/
		void RegisterTreeFromFileInDirectory(const std::string& Directory_path);

		// --------------------------------------------- 트리 생성 함수.
		/**
		* \brief 문자열을 통해 트리를 생성하는 함수.
		*/
		BT::Tree CreateTreeFromText(const std::string& file_path, BT::Blackboard::Ptr blackboard = BT::Blackboard::create());
		/**
		* \brief Xml 파일을 통해 트리를 생성하는 함수.
		*/
		BT::Tree CreateTreeFromFile(const std::string& file_path, BT::Blackboard::Ptr blackboard = BT::Blackboard::create());
		/**
		* \brief ID 통해 미리 등록되어 있는 트리를 생성하는 함수.
		*/
		BT::Tree CreateTree(const std::string& ID, BT::Blackboard::Ptr blackboard = BT::Blackboard::create());

	};

	template <typename T, typename... ExtraArgs>
	void BehaviorTreeFactory::RegisterNodeType(const std::string& ID, ExtraArgs... args)
	{
		DUOL_TRACE(DUOL_CONSOLE, "Register Node Type : {0}", ID);
		_factory->registerNodeType<T>(ID, args...);
	}
}
