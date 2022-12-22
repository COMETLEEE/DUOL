#include "DUOLGameEngine/ECS/Component/MeshCollider.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"

namespace DUOLGameEngine
{
	MeshCollider::MeshCollider(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _center(DUOLMath::Vector3::Zero)
		, _vertex{}
		, _index{}
	{

	}

	MeshCollider::~MeshCollider()
	{

	}

	void MeshCollider::OnEnable()
	{

	}

	void MeshCollider::OnDisable()
	{

	}

	void MeshCollider::SetCenter(const DUOLMath::Vector3& center)
	{
		_center = center;

		if (auto physicsShape = _physicsMesh.lock())
		{
			physicsShape->SetLocalPose(center);
		}
	}

	void MeshCollider::SetMeshBuffer(const DUOLCommon::tstring& fbxFileName)
	{
		auto model = ResourceManager::GetInstance()->GetModel(fbxFileName);

		if (model->GetIsSkinningModel() == true)
			return;

		unsigned meshCount = model->GetMeshCount();

		for (int i = 0; i < meshCount; i++)
		{
			Mesh* engineMesh = ResourceManager::GetInstance()->GetMesh(model->GetMesh(i)->_meshName);

			auto bufferDesc = engineMesh->GetPrimitiveMesh()->_vertexBuffer->GetBufferDesc();
		}

	}

	void MeshCollider::SetVertexBuffer(void* buffer, unsigned size, unsigned stride)
	{
		_vertex._buffer = buffer;
		_vertex._count = size;
		_vertex._stride= stride;
	}

	void MeshCollider::SetIndexBuffer(void* buffer, unsigned size, unsigned stride)
	{
		_index._buffer = buffer;
		_index._count = size;
		_index._stride = stride;
	}
}