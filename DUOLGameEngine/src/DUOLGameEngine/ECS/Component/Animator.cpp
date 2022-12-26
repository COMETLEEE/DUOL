#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/Avatar.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	Animator::Animator(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _currentAnimationClip(nullptr)
		, _currentAvatar(nullptr)
		, _isPlaying(true)
	{
	}

	Animator::~Animator()
	{
	}

	void Animator::Play(float deltaTime)
	{
		// 애니메이션 클립이 없다면 종료합니다.
		if (_currentAnimationClip == nullptr)
			return;

		const DUOLGraphicsEngine::AnimationClip* animClip = _currentAnimationClip->GetPrimitiveAnimationClip();

		// target frame.
		_currentFrame = _currentFrame + (animClip->_frameRate * deltaTime);

		// 현재 프레임을 모듈러 연산을 통해 프레임 사이에 위치시킵니다.
		_currentFrame = std::fmod(_currentFrame, static_cast<float>(_maxFrame));

		const int currentIntFrame = static_cast<int>(_currentFrame);
		
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < animClip->_keyFrameList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// 해당 프레임의 Local transform을 긁어옵니다.
			_currentAnimationClip->GetTargetFrameTransform(currentIntFrame, targetBoneIndex, outMat);

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
		if (_isPlaying)
		{
			Play(deltaTime);
		}
	}

	void Animator::SetAnimationClip(DUOLGameEngine::AnimationClip* animationClip)
	{
		if (animationClip == nullptr)
			return;

		_currentAnimationClip = animationClip;

		_currentFrame = 0;

		_maxFrame = animationClip->_animationClip->_endKeyFrame;
	}

	DUOLGameEngine::AnimationClip* Animator::GetCurrentAnimationClip() const
	{
		return _currentAnimationClip;
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

	bool Animator::GetIsPlaying()
	{
		return _isPlaying;
	}

	void Animator::SetIsPlaying(bool value)
	{
		_isPlaying = value;
	}
}