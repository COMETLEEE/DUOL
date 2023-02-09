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
		// TODO : 먼가 UUID 보다 더 큰 비트로 뽑아내는 ID 개념을 만들어야겠다 .. 커뮤니티가 엄청 커지면 숫자가 겹칠 수도 있겠다 !
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
		// Animator Controller 맵에서 제거하고 해제합시다.
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

		// prevFrame = currentFrame 으로 업데이트해주기.
		_controllerContext->_currentStateContexts[0]._prevFrame = _controllerContext->_currentStateContexts[0]._currentFrame;

		// target (== current)frame. 
		_controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame + (animClip->_frameRate * deltaTime);

		// 현재 프레임을 모듈러 연산을 통해 프레임 사이에 위치시킵니다.
		_controllerContext->_currentStateContexts[0]._currentFrame = std::fmod(_controllerContext->_currentStateContexts[0]._currentFrame, static_cast<float>(animClip->_endKeyFrame));

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		animationClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentStateContexts[0]._prevFrame, _controllerContext->_currentStateContexts[0]._currentFrame);

		const int currentIntFrame = static_cast<int>(_controllerContext->_currentStateContexts[0]._currentFrame);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < animClip->_keyFrameList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// 해당 프레임의 Local transform을 긁어옵니다.
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

		// 이전 및 현재 프레임이 어디에 위치하는지 갱신합니다.
		_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom;

		_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo;

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom + (fromAnim->_frameRate * deltaTime);

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo + (toAnim->_frameRate * deltaTime);

		// 프레임이 총 프레임 수를 넘어갔다면, 모듈러 연산을 통해 프레임을 정상 위치시킵니다..
		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom, static_cast<float>(fromAnim->_endKeyFrame));

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo, static_cast<float>(toAnim->_endKeyFrame));

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		fromClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		toClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		// 각 프레임 별로 미리 계산된 행렬을 가져오기 위해 Int 변수를 만듭니다.
		const int currentIntFrameOfFrom = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		const int currentIntFrameOfTo = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < fromAnim->_keyFrameList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// outMat 에 보간된 행렬을 계산합니다.
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
		// Controller가 없으면 Animator Component는 의도대로 동작하지 않습니다.
		if (_animatorController == nullptr)
			return;

		_animatorController->UpdateAnimatorController(_controllerContext, deltaTime);
	}

	void Animator::SetAnimatorController(DUOLGameEngine::AnimatorController* animatorController)
	{
		// 기존 AnimatorController가 있다면 제거합니다.
		if ((_controllerContext != nullptr) && (_animatorController != nullptr))
		{
			// AnimatorController에 Context가 잘 등록되어 있다면 메모리에서 해제하고 AnimatorController 에서 참조를 제거해줍니다.(정상적 동작)
			if (_animatorController->_animatorControllerContexts.contains(this->GetUUID()))
			{
				_animatorController->_animatorControllerContexts.erase(this->GetUUID());

				delete _controllerContext;
			}
		}

		// 컨트롤러를 껴주고
		_animatorController = animatorController;

		// 컨트롤러와 대응되는 컨텍스트를 만들어주고
		_controllerContext = new AnimatorControllerContext(this, _animatorController);

		// 컨트롤러 안에 이 애니메이터를 대표하는 컨텍스트의 참조를 남겨줍니다.
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
		// 컨텍스트가 없으면 == 애니메이터 컨트롤러가 없으면 동작하지 않습니다.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_boolParameters.contains(paramName))
			_controllerContext->_boolParameters[paramName] = value;
	}

	void Animator::SetFloat(const DUOLCommon::tstring& paramName, float value) const
	{
		// 컨텍스트가 없으면 == 애니메이터 컨트롤러가 없으면 동작하지 않습니다.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_floatParameters.contains(paramName))
			_controllerContext->_floatParameters[paramName] = value;
	}

	void Animator::SetInt(const DUOLCommon::tstring& paramName, int value) const
	{
		// 컨텍스트가 없으면 == 애니메이터 컨트롤러가 없으면 동작하지 않습니다.
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