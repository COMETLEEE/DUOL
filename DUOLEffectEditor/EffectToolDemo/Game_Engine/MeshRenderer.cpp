#include "pch.h"
#include "MeshRenderer.h"
//#include "..\GraphicsEngine\IMesh.h"


namespace Muscle
{
	MeshRenderer::MeshRenderer(std::shared_ptr<GameObject> _GameObject) :RendererBase(_GameObject)
	{
	}

	MeshRenderer::~MeshRenderer()
	{

	}


	void MeshRenderer::Start()
	{
		_renderingData->_dataType = RENDERINGDATA_TYPE::STATIC_MESH_OBJECT;

		_renderingData->_objectInfo->_objectID = m_GameObject.lock()->GetTopParent()->GetObjectID();
	}

	void MeshRenderer::Update()
	{


	}

	void MeshRenderer::Render()
	{
		// 기하 정보와 카메라 관련 정보를 업데이트해서 그래픽스 매니저에게 데이터를 송신한다.
		std::shared_ptr<Camera> mainCam = MuscleEngine::GetInstance()->GetMainCamera();

		Matrix worldTM = _transform->GetWorldTM();

		_renderingData->_geoInfo->_world = worldTM;

		_renderingData->_geoInfo->_texTransform = Matrix::Identity;

		_renderingData->_geoInfo->_worldViewProj = worldTM * mainCam->View() * mainCam->Proj();

		_renderingData->_geoInfo->_worldInvTranspose = (worldTM.Invert()).Transpose();

		// 그래픽스 매니저에게 렌더링 데이터를 보낸다.
		MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);

	}


}