/**

	@file      Behaviortree.h
	@brief     비헤이비어 트리의 컨트롤러.
	@details   ~ 초기화, 실행, 정지, 현재 상태 확인 등의 작업을 수행한다.
	비헤이비어 트리의 노드를 생성하고 트리를 생성하는 것은 비헤이비어 팩토리에서 수행합니다.
	@author    SinSeongHyeon
	@date      6.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "behaviortree_cpp/bt_factory.h"

namespace DUOLGameEngine
{
#pragma once
	class DUOL_GAMEENGINE_API BehaviortreeController : public DUOLGameEngine::BehaviourBase
	{
	public:
		BehaviortreeController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("BehaviortreeController"));

		virtual ~BehaviortreeController() override;

	private:
		BT::Tree _behaviorTree; // 컨트롤러가 가지고 있는 트리입니다.

		bool _isInit; // Tree를 초기화 했는지 검사합니다.

		bool _isPlay; // 비헤이비어 트리가 실행중인지 검사합니다.

	public:
		/**
		 * \brief Behaviortree를 등록합니다.
		 */
		void Initialize(const std::string& ID, BT::Blackboard::Ptr blackboard = BT::Blackboard::create());
		/**
		 * \brief Behaviortree를 정지합니다.
		 */
		void StopBehaviorTree();
		/**
		 * \brief Behaviortree를 재생합니다.
		 */
		void PlayBehaviorTree();
		/**
		 * \brief 매프레임마다 틱을 실행합니다.
		 */
		template<class T>
		void SetBlackBoard(std::string key, T value);
		/**
		* \brief 블랙보드에 값을 입력합니다.
		*/

		virtual void OnUpdate(float deltaTime) override;



		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};

	template <class T>
	void BehaviortreeController::SetBlackBoard(std::string key, T value)
	{
		_behaviorTree.rootBlackboard()->set<T>(key, value);
	}
}
