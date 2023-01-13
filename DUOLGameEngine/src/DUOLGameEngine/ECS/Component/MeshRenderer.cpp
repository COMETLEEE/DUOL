#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"

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
		// 0. Mesh Filter�� ������ ���������� �ʽ��ϴ�.
		if (_meshFilter == nullptr)
			return;

		// 1. Static Mesh �� �°� RenderObject Update
		_renderObjectInfo._mesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		// 1 - 1. Transform Information Update
		// TODO : ���� ������Ʈ ���� �����ϸ鼭 Transform�� ������ �ִ� ��쿡�� ��ȯ�ϵ��� ����
		const Matrix& worldTM = GetTransform()->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : ���� ������Ʈ ���� �����ϸ鼭 Material�� ������ �ִ� ��쿡�� ��ȯ�ϵ��� ����
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 1 - 3. Root Object ID
		DUOLGameEngine::GameObject* rootObject = GetGameObject()->GetTransform()->GetRootTransform()->GetGameObject();

		// _meshInfo.SetObjectID(rootObject->GetUUID());
		_meshInfo.SetObjectID(DUOLMath::Vector2(1.f, 1.f));

		// 2. ���� ������Ʈ�� ������ �����ô�.
		GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);
	}
}