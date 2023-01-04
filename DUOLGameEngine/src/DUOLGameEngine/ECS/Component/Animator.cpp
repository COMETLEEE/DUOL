#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/Avatar.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

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

		// target frame. 
		_controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame + (animClip->_frameRate * deltaTime);

		// 현재 프레임을 모듈러 연산을 통해 프레임 사이에 위치시킵니다.
		_controllerContext->_currentStateContexts[0]._currentFrame = std::fmod(_controllerContext->_currentStateContexts[0]._currentFrame, static_cast<float>(animClip->_endKeyFrame));

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

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom + (fromAnim->_frameRate * deltaTime);

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo + (toAnim->_frameRate * deltaTime);

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = 
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom, static_cast<float>(fromAnim->_endKeyFrame));

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo, static_cast<float>(toAnim->_endKeyFrame));

		const int currentIntFrameOfFrom = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		const int currentIntFrameOfTo = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0 ; targetBoneIndex < fromAnim->_keyFrameList.size() ; targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

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
}