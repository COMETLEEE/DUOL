#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::MeshRenderer>("MeshRenderer")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("EnableScreenSpaceReflection", &DUOLGameEngine::MeshRenderer::IsEnableScreenSpaceReflection, &DUOLGameEngine::MeshRenderer::SetScreenSpaceReflection)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("IsOccluder", &DUOLGameEngine::MeshRenderer::IsOccluder, &DUOLGameEngine::MeshRenderer::SetOccluder)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	MeshRenderer::MeshRenderer() :
		RendererBase(nullptr)
		, _renderObjectInfo(DUOLGraphicsEngine::RenderObject())
		, _meshFilter(nullptr)
		, _renderEventHandlerIDForGraphics(0)
	{
		// Transform Information to graphics.
		_renderObjectInfo._renderInfo = &_meshInfo;
		_meshInfo.SetTransformPointer(&_transformInfo);
	}

	MeshRenderer::MeshRenderer(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
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

	void MeshRenderer::OnEnable()
	{
		// 렌더 이벤트 등록
		RendererBase::OnEnable();

		// 메쉬 필터 연결 지어주기.
		DUOLGameEngine::MeshFilter* meshFilter =
			reinterpret_cast<MeshFilter*>(GetGameObject()->GetComponent(TEXT("MeshFilter")));

		if (meshFilter != nullptr)
			_meshFilter = meshFilter;
	}

	void MeshRenderer::Render()
	{
		// 0. Mesh Filter가 없으면 렌더링하지 않습니다.
		if (_meshFilter == nullptr)
			return;

		if (!_meshFilter->GetMesh())
			return;

		// 1. Static Mesh 에 맞게 RenderObject Update
		_renderObjectInfo._mesh = _meshFilter->GetMesh()->GetPrimitiveMesh();

		// 1 - 1. Transform Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Transform의 변동이 있는 경우에만 변환하도록 수정
		const Matrix& worldTM = GetTransform()->GetWorldMatrix();

		_transformInfo._world = worldTM;
		_transformInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// 1 - 2. Material Information Update
		// TODO : 추후 업데이트 구조 생각하면서 Material의 변동이 있는 경우에만 변환하도록 수정
		_renderObjectInfo._materials = &_primitiveMaterials;

		// 1 - 3. Object ID
		auto gameObj = GetGameObject();

		_meshInfo.SetObjectID(gameObj->GetUUID());
		_meshInfo.SetStatic(gameObj->GetIsStatic());

#pragma region ROOT_OBJECT_VERSION
		// DUOLGameEngine::GameObject* rootObject = GetGameObject()->GetTransform()->GetRootTransform()->GetGameObject();
#pragma endregion

		// 2. 렌더 오브젝트의 참조를 보냅시다.
		GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);
	}

	void MeshRenderer::SetRimColor(DUOLMath::Vector3 albedo)
	{
		DUOLMath::Vector4 input = DUOLMath::Vector4{ albedo.x, albedo.y, albedo.z, 1.f };

		_meshInfo.SetRimColor(input);
	}

	void MeshRenderer::SetRimPower(float value)
	{
		_meshInfo.SetRimPower(value);
	}

	DUOLMath::Vector3 MeshRenderer::GetRimColor()
	{

		auto& color = _meshInfo.GetRimColor();
		DUOLMath::Vector3 output =
		{
			color.x, color.y, color.z
		};

		return output;
	}

	float MeshRenderer::GetRimPower()
	{
		return _meshInfo.GetRimPower();
	}

}