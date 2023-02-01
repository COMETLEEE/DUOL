#pragma once
#include "DUOLGameEngine/ECS/Component/ColliderBase.h"
#include "DUOLPhysics/Shapes/PhysicsMesh.h"

namespace DUOLGameEngine
{
	class MeshFilter;
	using namespace DUOLGameEngine;

	struct BufferData
	{
		void* _buffer;

		unsigned _count;

		unsigned _stride;
	};

	class DUOL_GAMEENGINE_API MeshCollider : public ColliderBase
	{
	public:

		MeshCollider(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = TEXT("MeshCollider"));

		virtual ~MeshCollider() override;

	private:
		std::weak_ptr<DUOLPhysics::PhysicsMesh> _physicsMesh;

		DUOLMath::Vector3 _center;

		std::vector<DUOLMath::Vector3> vertices;

		BufferData _vertex;

		std::vector<UINT32> indices;

		BufferData _index;

	public:

		virtual void OnEnable() override;

		virtual void OnDisable() override;

		inline const DUOLMath::Vector3& GetCenter() const { return _center; }

		void SetCenter(const DUOLMath::Vector3& center);

		void SetMeshBuffer(const DUOLCommon::tstring& fbxFileName);\

		void SetMeshBuffer(const MeshFilter* mesh);

		inline const BufferData& GetVertexBuffer() const { return _vertex; }

		void SetVertexBuffer(void* buffer, unsigned size, unsigned stride);

		inline const BufferData& GetIndexBuffer() const { return _index; }

		void SetIndexBuffer(void* buffer, unsigned size, unsigned stride);

		RTTR_ENABLE(DUOLGameEngine::ColliderBase)

#pragma region FRIEND_CLASS
		friend class PhysicsManager;
#pragma endregion
	};
}