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

	void Muscle::SkyBox::Initialize(tstring texturePath, std::shared_ptr<Camera>& camera)
	{
		_renderingData = std::make_shared<MuscleGrapics::RenderingData_3D>();

		// Cube랑 Sphere 에 약간 다이나믹한 차이가 있을 줄 알았는데 별로 차이가 없다.
		_renderingData->_objectInfo._meshName = "Sphere";

		_renderingData->_objectInfo._objectID = m_GameObject.lock()->GetObjectID();

		_renderingData->_shaderInfo._shaderName.push_back("SkyBoxPass");

		_renderingData->_materialInfo._albedoTexturePath = texturePath;

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
		_renderingData->_geoInfo._world = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3(100.f, 100.f, 100.f)) * DUOLMath::Matrix::CreateTranslation(_transform->GetWorldPosition());
		_renderingData->_geoInfo._worldViewProj = _renderingData->_geoInfo._world * _camera->View() * _camera->Proj();

		MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);
	}

}