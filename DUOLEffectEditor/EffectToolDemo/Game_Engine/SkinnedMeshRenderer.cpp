#include "pch.h"
#include "SkinnedMeshRenderer.h"

namespace Muscle
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(std::shared_ptr<GameObject> _GameObject) :RendererBase(_GameObject), _boneCount(0)
	{
		_skinNodeTM = Matrix::Identity;
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
		for (auto bone : _boneList)
			bone.reset();
	}
	

	void SkinnedMeshRenderer::Start()
	{
		_renderingData->_dataType = RENDERINGDATA_TYPE::SKINNED_MESH_OBJECT;
		_renderingData->_objectInfo->_objectID = m_GameObject.lock()->GetTopParent()->GetObjectID();

		//_renderingData->_shaderInfo->_vsName = TEXT("VS_A_Skinning");
		//_renderingData->_shaderInfo->_psName = TEXT("PS_A_Skinning");

		_transform = m_GameObject.lock()->GetComponent<Transform>();

		for (int i = 0; i < _boneList.size(); i++)
		{
			std::shared_ptr<GameObject> boneGO = _boneList[i]->_boneObject;

			//boneGO->GetComponent<MeshRenderer>()->SetIsEnable(false);
		}
	}

	void SkinnedMeshRenderer::Update()
	{

	}

	void SkinnedMeshRenderer::Render()
	{

		// 본 매트릭스 팔레트 던짐
		memcpy(_renderingData->_animInfo->_boneMatrixList, _boneMatrixList.data() , sizeof(Matrix) * _boneCount);

		// 기하 정보와 카메라 관련 정보를 업데이트해서 그래픽스 매니저에게 데이터를 송신한다.
		std::shared_ptr<Camera> mainCam = MuscleEngine::GetInstance()->GetMainCamera();

		Matrix worldTM = _transform->GetWorldTM();

		_renderingData->_geoInfo->_world = worldTM;

		_renderingData->_geoInfo->_texTransform = Matrix::Identity;

		_renderingData->_geoInfo->_worldViewProj = worldTM * mainCam->View() * mainCam->Proj();

		_renderingData->_geoInfo->_worldInvTranspose = (worldTM.Invert()).Transpose();

		// 그래픽스 매니저에게 렌더링 데이터를 보낸다.
		MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);
	}

	void SkinnedMeshRenderer::Initialize(const Matrix& skinNodeTM, const std::vector<std::shared_ptr<Bone>>& boneList)
	{
		_skinNodeTM = skinNodeTM;

		_boneList = boneList;

		_boneMatrixList.resize(_boneList.size());

		_boneCount = static_cast<uint32>(_boneList.size());

		for (int i = 0; i < _boneList.size(); i++)
		{
			std::shared_ptr<GameObject> boneGO = _boneList[i]->_boneObject;

			//boneGO->GetComponent<MeshRenderer>()->SetIsEnable(false);
		}
	}

	void SkinnedMeshRenderer::LateUpdate()
	{
		Matrix boneWorldTM;

		Matrix boneOffsetTM;

		Matrix boneNodeTM;

		Matrix finalBoneTM;

		// 본 매트릭스 업뎃
		for (int i = 0; i < _boneList.size(); i++)
		{
			std::shared_ptr<Transform> boneTransform = _boneList[i]->_boneObject->GetComponent<Transform>();

			boneWorldTM = boneTransform->GetWorldTM();

			boneNodeTM = _boneList[i]->_boneNodeTM;

			boneOffsetTM = boneNodeTM * _skinNodeTM.Invert();

			// 스킨은 본이 이동한 만큼 + 가중치에 따라서 이동한다. + 쉐이더에서 월드 행렬 곱해지는 것 막기 위해서 그냥 인버스 곱함
			// 행렬 한 번 더 곱하는 것 정도는 딱히 그래픽 카드에게 무례한 일 시키는 것 아니다.
			_boneMatrixList[i] = boneOffsetTM.Invert() * boneWorldTM * _transform->GetWorldTM().Invert();
		}
	}
}
