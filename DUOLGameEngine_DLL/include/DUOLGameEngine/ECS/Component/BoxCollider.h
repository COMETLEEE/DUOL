/**

    @file      BoxCollider.h
    @brief     3D box-shaped primitive collider.
    @details   ~
    @author    COMETLEE
    @date      15.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLPhysics/Shapes/PhysicsBox.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 이것은 박스입니다.
	 */
	class DUOL_GAMEENGINE_API BoxCollider final : public DUOLGameEngine::ColliderBase
	{
	public:
		BoxCollider(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("BoxCollider"));

		virtual ~BoxCollider() override;

	private:
		std::weak_ptr<DUOLPhysics::PhysicsBox> _physicsBox;

		DUOLMath::Vector3 _center;

		DUOLMath::Vector3 _size;

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		inline const DUOLMath::Vector3& GetCenter() const { return _center; }

		void SetCenter(const DUOLMath::Vector3& center);

		inline const DUOLMath::Vector3& GetSize() const { return _size; }

		void SetSize(const DUOLMath::Vector3& size);

		RTTR_ENABLE(DUOLGameEngine::ColliderBase)

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}
