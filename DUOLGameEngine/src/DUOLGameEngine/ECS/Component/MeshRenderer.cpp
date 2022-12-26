#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLGameEngine
{
	MeshRenderer::MeshRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
		, _renderObjectInfo(DUOLGraphicsEngine::RenderObject())
		, _meshFilter(nullptr)
		, _renderEventHandlerIDForGraphics(0)
	{
		DUOLGameEngine::MeshFilter* meshFilter =
			GetGameObject()->GetComponent<DUOLGameEngine::MeshFilter>();

		if (meshFilter != nullptr)
			_meshFilter = meshFilter;

		// Transform Information to graphics.
		_renderObjectInfo._renderInfo = &_meshInfo;
		_meshInfo.SetTransformPointer(&_transformInfo);
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::Render()
	{
		// 0. Mesh Filter가 없으면 렌더링하지 않습니다.
		if (_meshFilter == nullptr)
			return;

		// 1. Static Mesh 에 맞게 RenderObject Update
		_renderObjectInfo._mesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		std::vector<DUOLMath::Vector3> vertices;
		std::vector<UINT32> indices;

		ResourceManager::GetInstance()->GetMeshInfo(_meshFilter->GetMesh(), vertices, indices);

		// 1 - 1. Transform Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Transform의 변동이 있는 경우에만 변환하도록 수정
		const Matrix& worldTM = GetTransform()->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Material의 변동이 있는 경우에만 변환하도록 수정
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 2. 렌더 오브젝트의 참조를 보냅시다.
		GraphicsManager::GetInstance()->ReserveRenderObject(_renderObjectInfo);
	}
}
