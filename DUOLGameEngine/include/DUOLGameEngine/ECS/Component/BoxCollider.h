#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"

namespace DUOLGameEngine
{
	class BoxCollider : public DUOLGameEngine::ColliderBase
	{
	public:
		BoxCollider(const std::weak_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name = TEXT("BoxCollider"));

		virtual ~BoxCollider() override;

	public:
		// 이벤트 방식으로 충돌 검사를 넘깁니다.
	};
}