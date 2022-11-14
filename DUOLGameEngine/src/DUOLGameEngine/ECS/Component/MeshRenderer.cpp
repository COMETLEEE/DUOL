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
		const std::shared_ptr<DUOLGameEngine::MeshFilter> meshFilter = GetGameObject()->GetComponent<DUOLGameEngine::MeshFilter>();

		if (meshFilter != nullptr)
			_meshFilter = meshFilter.get();

		// Transform Information to graphics.
		_renderObjectInfo.PerObjectData._transform = &_transformInfo;
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::OnEnable()
	{
		std::function<void()> functor = std::bind(&MeshRenderer::Render, this);

		_renderEventHandlerIDForGraphics = GraphicsManager::GetInstance()->AddRenderEventHandler(functor);
	}

	void MeshRenderer::OnDisable()
	{
		GraphicsManager::GetInstance()->RemoveRenderEventHandler(_renderEventHandlerIDForGraphics);
	}

	void MeshRenderer::Render()
	{
		// 0. Mesh Filter�� ������ ���������� �ʽ��ϴ�.
		if (_meshFilter == nullptr)
			return;

		// 1. Static Mesh �� �°� RenderObject Update
		_renderObjectInfo.mesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		// 1 - 1. Transform Information Update
		// TODO : ���� ������Ʈ ���� �����ϸ鼭 Transform�� ������ �ִ� ��쿡�� ��ȯ�ϵ��� ����
		const Matrix& worldTM = GetTransform()->GetWorldMatrix();

		_renderObjectInfo.PerObjectData._transform->_world = worldTM;
		_renderObjectInfo.PerObjectData._transform->_worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : ���� ������Ʈ ���� �����ϸ鼭 Material�� ������ �ִ� ��쿡�� ��ȯ�ϵ��� ����
		_renderObjectInfo.PerObjectData._material = &_primitiveMaterials;

		// 2. ���� ������Ʈ�� ������ �����ô�.
		GraphicsManager::GetInstance()->ReserveRenderObject(_renderObjectInfo);
	}
}