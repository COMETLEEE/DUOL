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
		// �̺�Ʈ ������� �浹 �˻縦 �ѱ�ϴ�.
	};
}