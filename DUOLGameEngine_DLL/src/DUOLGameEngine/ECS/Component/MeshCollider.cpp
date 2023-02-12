#include "DUOLGameEngine/ECS/Component/MeshCollider.h"

#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::MeshCollider>("MeshCollider")
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	MeshCollider::MeshCollider(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		ColliderBase(owner, name)
		, _physicsMesh()
		, _center(DUOLMath::Vector3::Zero)
		, _vertex{}
		, _index{}
	{

	}

	MeshCollider::~MeshCollider()
	{
		_physicsMesh.reset();
	}

	void MeshCollider::OnEnable()
	{
		_physicsActor.lock()->AttachShape(_physicsMesh);
	}

	void MeshCollider::OnDisable()
	{
		_physicsActor.lock()->AttachShape(_physicsMesh);
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

		unsigned meshCount = model->GetMeshCount();

		for (int i = 0; i < meshCount; i++)
		{
			Mesh* engineMesh = ResourceManager::GetInstance()->GetMesh(model->GetMesh(i)->_meshName);

			ResourceManager::GetInstance()->GetMeshInfo(engineMesh, vertices, indices);

			SetVertexBuffer(vertices.data(), static_cast<unsigned>(vertices.size()), sizeof(DUOLMath::Vector3));

			SetIndexBuffer(indices.data(), static_cast<unsigned>(indices.size()), sizeof(UINT32));
		}
	}

	void MeshCollider::SetMeshBuffer(const MeshFilter* meshfilter)
	{
		auto mesh = meshfilter->GetMesh();
		auto primitveMesh = mesh->GetPrimitiveMesh();

		ResourceManager::GetInstance()->GetMeshInfo(mesh, vertices, indices);

		SetVertexBuffer(vertices.data(), static_cast<unsigned>(vertices.size()), sizeof(DUOLMath::Vector3));

		SetIndexBuffer(indices.data(), static_cast<unsigned>(indices.size()), sizeof(UINT32));
	}

	void MeshCollider::SetVertexBuffer(void* buffer, unsigned size, unsigned stride)
	{
		_vertex._buffer = buffer;
		_vertex._count = size;
		_vertex._stride = stride;
	}

	void MeshCollider::SetIndexBuffer(void* buffer, unsigned size, unsigned stride)
	{
		_index._buffer = buffer;
		_index._count = size;
		_index._stride = stride;
	}
}