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

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/Util/enabled_shared_from_base.h"
#include "behaviortree_cpp/bt_factory.h"

namespace DUOLGameEngine
{
#pragma once
	class DUOL_GAMEENGINE_API BehaviortreeController : public DUOLGameEngine::ComponentBase
	{
	public:
		BehaviortreeController(DUOLGameEngine::GameObject* owner);

		virtual ~BehaviortreeController() override;

	private:
		BT::Tree _behaviorTree; // 컨트롤러가 가지고 있는 트리입니다.

		bool _isInit; // Tree를 초기화 했는지 검사합니다.

		bool _isPlay; // 비헤이비어 트리가 실행중인지 검사합니다.

	public:
		/**
		 * \brief Behaviortree를 등록합니다.
		 */
		void Initialize(BT::Tree&& tree);
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
		virtual void OnUpdate(float deltaTime) override;
	};
}
