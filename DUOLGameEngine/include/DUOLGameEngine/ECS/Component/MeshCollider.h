#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLPhysics/Shapes/PhysicsMesh.h"

namespace DUOLGameEngine
{
	using namespace DUOLGameEngine;

	struct BufferData
	{
		void* _buffer;

		unsigned _count;

		unsigned _stride;
	};

	class MeshCollider : public ColliderBase
	{
	public:

		MeshCollider(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = TEXT("MeshCollider"));

		virtual ~MeshCollider() override;

	private:
		std::weak_ptr<DUOLPhysics::PhysicsMesh> _physicsMesh;

		DUOLMath::Vector3 _center;

		BufferData _vertex;

		BufferData _index;

	public:

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		inline const DUOLMath::Vector3& GetCenter() const { return _center; }

		void SetCenter(const DUOLMath::Vector3& center);

		void SetMeshBuffer(const DUOLCommon::tstring& fbxFileName);

		inline const BufferData& GetVertexBuffer() const { return _vertex; }

		void SetVertexBuffer(void* buffer, unsigned size, unsigned stride);

		inline const BufferData& GetIndexBuffer() const { return _index; }

		void SetIndexBuffer(void* buffer, unsigned size, unsigned stride);

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}