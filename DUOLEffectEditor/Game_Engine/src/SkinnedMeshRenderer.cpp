#include "SkinnedMeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"
#include "AnimationClip.h"


namespace Muscle
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(std::shared_ptr<GameObject> _GameObject) :RendererBase(_GameObject), _boneCount(0)
	{
		_skinNodeTM = DUOLMath::Matrix::Identity;
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
		for (auto bone : _boneList)
			bone.reset();
	}
	

	void SkinnedMeshRenderer::Start()
	{
		_renderingData->_objectInfo._objectID = m_GameObject.lock()->GetTopParent()->GetObjectID();

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

		// �� ��Ʈ���� �ȷ�Ʈ ����
		memcpy(_renderingData->_animInfo._boneMatrixList, _boneMatrixList.data() , sizeof(DUOLMath::Matrix) * _boneCount);

		// ���� ������ ī�޶� ���� ������ ������Ʈ�ؼ� �׷��Ƚ� �Ŵ������� �����͸� �۽��Ѵ�.
		std::shared_ptr<Camera> mainCam = MuscleEngine::GetInstance()->GetMainCamera();

		DUOLMath::Matrix worldTM = _transform->GetWorldTM();

		_renderingData->_geoInfo._world = worldTM;

		_renderingData->_geoInfo._texTransform = DUOLMath::Matrix::Identity;

		_renderingData->_geoInfo._worldViewProj = worldTM * mainCam->View() * mainCam->Proj();

		_renderingData->_geoInfo._worldInvTranspose = (worldTM.Invert()).Transpose();

		// �׷��Ƚ� �Ŵ������� ������ �����͸� ������.
		MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_3D(_renderingData);
	}

	void SkinnedMeshRenderer::Initialize(const DUOLMath::Matrix& skinNodeTM, const std::vector<std::shared_ptr<Bone>>& boneList)
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
		DUOLMath::Matrix boneWorldTM;

		DUOLMath::Matrix boneOffsetTM;

		DUOLMath::Matrix boneNodeTM;

		DUOLMath::Matrix finalBoneTM;

		// �� ��Ʈ���� ����
		for (int i = 0; i < _boneList.size(); i++)
		{
			std::shared_ptr<Transform> boneTransform = _boneList[i]->_boneObject->GetComponent<Transform>();

			boneWorldTM = boneTransform->GetWorldTM();

			boneNodeTM = _boneList[i]->_boneNodeTM;

			boneOffsetTM = boneNodeTM * _skinNodeTM.Invert();

			// ��Ų�� ���� �̵��� ��ŭ + ����ġ�� ���� �̵��Ѵ�. + ���̴����� ���� ��� �������� �� ���� ���ؼ� �׳� �ι��� ����
			// ��� �� �� �� ���ϴ� �� ������ ���� �׷��� ī�忡�� ������ �� ��Ű�� �� �ƴϴ�.
			_boneMatrixList[i] = boneOffsetTM.Invert() * boneWorldTM * _transform->GetWorldTM().Invert();
		}
	}
}
