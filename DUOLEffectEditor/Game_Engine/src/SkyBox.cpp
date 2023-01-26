#include "SkyBox.h"
#include "Export\RenderingData.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"

namespace Muscle
{

	Muscle::SkyBox::SkyBox(std::shared_ptr<GameObject> _GameObj) :IComponents(_GameObj)
	{
	}

	void Muscle::SkyBox::Initialize(const uint64& cubeMapID, std::shared_ptr<Camera> camera)
	{
		_renderingData = std::make_shared<MuscleGrapics::RenderingData_3D>();

		// Cube�� Sphere �� �ణ ���̳����� ���̰� ���� �� �˾Ҵµ� ���� ���̰� ����.
		//_renderingData->_objectInfo->_meshID = ResourceManager::Get()->GetMeshID(TEXT("WhiteCube"));

		// _renderingData->_objectInfo->_meshID = ResourceManager::Get()->GetMeshID(TEXT("GreenSphere"));

		_renderingData->_objectInfo._objectID = m_GameObject.lock()->GetObjectID();

		_renderingData->_shaderInfo._shaderName.push_back(TEXT("VS_SkyBox"));

		//_renderingData->_materialInfo->_diffuseMapID = cubeMapID;

		_renderingData->_objectInfo._usingLighting = false;

		_camera = camera;

	}

	void SkyBox::Start()
	{
		_transform = m_GameObject.lock()->GetTransform();
	}

	void Muscle::SkyBox::LateUpdate()
	{


	}

	void SkyBox::Render()
	{
		//if (_renderingData->_materialInfo->_diffuseMapID == ULLONG_MAX)
		//	return;

		_renderingData->_geoInfo._world = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3(100.f, 100.f, 100.f)) * DUOLMath::Matrix::CreateTranslation(_transform->GetWorldPosition());
		_renderingData->_geoInfo._worldViewProj = DUOLMath::Matrix::CreateTranslation(_transform->GetWorldPosition()) * _camera->View() * _camera->Proj();


		MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);
	}

}