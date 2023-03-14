#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief Transform 의 각 프로퍼티를 'Set' 하는 방법으로 컨트롤합니다.
	 * 카메라 연출, 라이팅 연출 등에 사용할 수 있습니다.
	 */
	class TransformController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		TransformController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("TransformController"));

		virtual ~TransformController();

	private:

	public:
		virtual void OnLateUpdate(float deltaTime) override;
	};
}
