#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/Avatar.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Animator>("Animator")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_animatorController", &DUOLGameEngine::Animator::_animatorController)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// TODO : �հ� UUID ���� �� ū ��Ʈ�� �̾Ƴ��� ID ������ �����߰ڴ� .. Ŀ�´�Ƽ�� ��û Ŀ���� ���ڰ� ��ĥ ���� �ְڴ� !
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::UUIDSerializeType, DUOLCommon::UUIDSerializeType::Resource)
	);
}

namespace DUOLGameEngine
{
	Animator::Animator(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _animatorController(nullptr)
		, _controllerContext(nullptr)
	{
	}

	Animator::~Animator()
	{
		// Animator Controller �ʿ��� �����ϰ� �����սô�.
		if (_controllerContext != nullptr)
		{
			if (_animatorController != nullptr)
			{
				_animatorController->_animatorControllerContexts.erase(this->GetUUID());

				delete _controllerContext;
			}
		}
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* animationClip)
	{
		const DUOLGraphicsEngine::AnimationClip* animClip = animationClip->GetPrimitiveAnimationClip();

		// prevFrame = currentFrame ���� ������Ʈ���ֱ�.
		_controllerContext->_currentStateContexts[0]._prevFrame = _controllerContext->_currentStateContexts[0]._currentFrame;

		// target (== current)frame. 
		_controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame + (animClip->_frameRate * deltaTime);

		// ���� �������� ��ⷯ ������ ���� ������ ���̿� ��ġ��ŵ�ϴ�.
		_controllerContext->_currentStateContexts[0]._currentFrame = std::fmod(_controllerContext->_currentStateContexts[0]._currentFrame, static_cast<float>(animClip->_endKeyFrame));

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		animationClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentStateContexts[0]._prevFrame, _controllerContext->_currentStateContexts[0]._currentFrame);

		const int currentIntFrame = static_cast<int>(_controllerContext->_currentStateContexts[0]._currentFrame);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < animClip->_keyFrameList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// �ش� �������� Local transform�� �ܾ�ɴϴ�.
			animationClip->GetTargetFrameTransform(currentIntFrame, targetBoneIndex, outMat);

			// bone's local transform update.
			_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTM(outMat);

			// bone matrices pallet update
			_boneMatrixList[targetBoneIndex] =
				_boneOffsetMatrixList[targetBoneIndex]
				* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
		}
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip,
		float tFrom)
	{
		const DUOLGraphicsEngine::AnimationClip* fromAnim = fromClip->GetPrimitiveAnimationClip();
		const DUOLGraphicsEngine::AnimationClip* toAnim = toClip->GetPrimitiveAnimationClip();

		// ���� �� ���� �������� ��� ��ġ�ϴ��� �����մϴ�.
		_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom;

		_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo;

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom + (fromAnim->_frameRate * deltaTime);

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo + (toAnim->_frameRate * deltaTime);

		// �������� �� ������ ���� �Ѿ�ٸ�, ��ⷯ ������ ���� �������� ���� ��ġ��ŵ�ϴ�..
		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom, static_cast<float>(fromAnim->_endKeyFrame));

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo, static_cast<float>(toAnim->_endKeyFrame));

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		fromClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		toClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		// �� ������ ���� �̸� ���� ����� �������� ���� Int ������ ����ϴ�.
		const int currentIntFrameOfFrom = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		const int currentIntFrameOfTo = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < fromAnim->_keyFrameList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// outMat �� ������ ����� ����մϴ�.
			fromClip->GetTargetFramesTransform(currentIntFrameOfFrom, currentIntFrameOfTo, targetBoneIndex, tFrom, toClip, outMat);

			// bone's local transform update.
			_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTM(outMat);

			// bone matrices pallet update
			_boneMatrixList[targetBoneIndex] =
				_boneOffsetMatrixList[targetBoneIndex]
				* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
		}
	}

	void Animator::OnLateUpdate(float deltaTime)
	{
		// Controller�� ������ Animator Component�� �ǵ���� �������� �ʽ��ϴ�.
		if (_animatorController == nullptr)
			return;

		_animatorController->UpdateAnimatorController(_controllerContext, deltaTime);
	}

	void Animator::SetAnimatorController(DUOLGameEngine::AnimatorController* animatorController)
	{
		// ���� AnimatorController�� �ִٸ� �����մϴ�.
		if ((_controllerContext != nullptr) && (_animatorController != nullptr))
		{
			// AnimatorController�� Context�� �� ��ϵǾ� �ִٸ� �޸𸮿��� �����ϰ� AnimatorController ���� ������ �������ݴϴ�.(������ ����)
			if (_animatorController->_animatorControllerContexts.contains(this->GetUUID()))
			{
				_animatorController->_animatorControllerContexts.erase(this->GetUUID());

				delete _controllerContext;
			}
		}

		// ��Ʈ�ѷ��� ���ְ�
		_animatorController = animatorController;

		// ��Ʈ�ѷ��� �����Ǵ� ���ؽ�Ʈ�� ������ְ�
		_controllerContext = new AnimatorControllerContext(this, _animatorController);

		// ��Ʈ�ѷ� �ȿ� �� �ִϸ����͸� ��ǥ�ϴ� ���ؽ�Ʈ�� ������ �����ݴϴ�.
		_animatorController->_animatorControllerContexts.insert({ this->GetUUID(), _controllerContext });
	}

	DUOLGameEngine::AnimatorController* Animator::GetAnimatorController() const
	{
		return _animatorController;
	}

	void Animator::SetBoneGameObjects(const std::vector<DUOLGameEngine::GameObject*>& boneObjects)
	{
		_boneGameObjects = boneObjects;

		_boneMatrixList.resize(_boneGameObjects.size());
	}

	const std::vector<DUOLGameEngine::GameObject*>& Animator::GetBoneGameObjects()
	{
		return _boneGameObjects;
	}

	void Animator::SetBoneOffsetMatrices(const std::vector<DUOLMath::Matrix>& matrices)
	{
		_boneOffsetMatrixList = matrices;
	}

	const std::vector<DUOLMath::Matrix>& Animator::GetBoneOffsetMatrices()
	{
		return _boneOffsetMatrixList;
	}

	std::vector<DUOLMath::Matrix>* Animator::GetBoneMatrices()
	{
		return &_boneMatrixList;
	}

	void Animator::SetBool(const DUOLCommon::tstring& paramName, bool value) const
	{
		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_boolParameters.contains(paramName))
			_controllerContext->_boolParameters[paramName] = value;
	}

	void Animator::SetFloat(const DUOLCommon::tstring& paramName, float value) const
	{
		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_floatParameters.contains(paramName))
			_controllerContext->_floatParameters[paramName] = value;
	}

	void Animator::SetInt(const DUOLCommon::tstring& paramName, int value) const
	{
		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_intParameters.contains(paramName))
			_controllerContext->_intParameters[paramName] = value;
	}

	bool Animator::GetBool(const DUOLCommon::tstring& paramName) const
	{
		return _controllerContext->_boolParameters.contains(paramName) ? _controllerContext->_boolParameters.at(paramName) : false;
	}

	float Animator::GetFloat(const DUOLCommon::tstring& paramName) const
	{
		return _controllerContext->_floatParameters.contains(paramName) ? _controllerContext->_floatParameters.at(paramName) : 0.f;
	}

	int Animator::GetInt(const DUOLCommon::tstring& paramName) const
	{
		return _controllerContext->_intParameters.contains(paramName) ? _controllerContext->_intParameters.at(paramName) : 0;
	}
}