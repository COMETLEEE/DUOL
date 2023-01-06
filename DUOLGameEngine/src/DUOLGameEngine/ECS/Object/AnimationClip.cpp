#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/Manager/EventManager.h"

namespace DUOLGameEngine
{
	AnimationClip::AnimationClip(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animationClip(nullptr)
		, _lengthInSeconds(0.f)
		, _maxFrame(0.f)
		, _events({})
	{

	}

	AnimationClip::~AnimationClip()
	{

	}

	DUOLGraphicsEngine::AnimationClip* AnimationClip::GetPrimitiveAnimationClip() const
	{
		return _animationClip;
	}

	void AnimationClip::SetPrimitiveAnimationClip(DUOLGraphicsEngine::AnimationClip* animationClip)
	{
		if (animationClip == nullptr)
			return;

		_animationClip = animationClip;

		_lengthInSeconds = static_cast<float>(_animationClip->_totalKeyFrame) / _animationClip->_frameRate;

		_maxFrame = static_cast<float>(_animationClip->_totalKeyFrame);
	}

	float AnimationClip::GetFrameRate() const
	{
		return _animationClip->_frameRate;
	}

	float AnimationClip::GetTotalPlayTime() const
	{
		return _lengthInSeconds;
	}

	float AnimationClip::GetMaxFrame() const
	{
		return _maxFrame;
	}

	void AnimationClip::GetTargetFrameTransform(int targetFrame, int targetBoneIndex, DUOLMath::Vector3& outPos,
	                                            DUOLMath::Quaternion& outRot, DUOLMath::Vector3& outScale) const
	{
		auto&& frame = _animationClip->_keyFrameList[targetBoneIndex][targetFrame];

		outPos = frame._localTransform;

		outRot = frame._localRotation;

		outScale = frame._localScale;
	}

	void AnimationClip::GetTargetFrameTransform(int targetFrame, int targetBoneIndex, DUOLMath::Matrix& outMatrix) const
	{
		auto&& frame = _animationClip->_keyFrameList[targetBoneIndex][targetFrame];

		outMatrix = DUOLMath::Matrix::CreateScale(frame._localScale)
			* DUOLMath::Matrix::CreateFromQuaternion(frame._localRotation)
			* DUOLMath::Matrix::CreateTranslation(frame._localTransform);
	}

	void AnimationClip::GetTargetFramesTransform(int fromFrame, int toFrame, int targetBoneIndex, float tFrom,
		DUOLGameEngine::AnimationClip* toClip, DUOLMath::Matrix& outMatrix) const
	{
		auto&& fromFrameInfo = _animationClip->_keyFrameList[targetBoneIndex][fromFrame];

		auto&& toFrameInfo = toClip->_animationClip->_keyFrameList[targetBoneIndex][toFrame];

		DUOLMath::Vector3 outPos = DUOLMath::Vector3::Lerp(toFrameInfo._localTransform, fromFrameInfo._localTransform, tFrom);

		DUOLMath::Quaternion outRot = DUOLMath::Quaternion::Slerp(toFrameInfo._localRotation, fromFrameInfo._localRotation, tFrom);

		DUOLMath::Vector3 outScale = DUOLMath::Vector3::Lerp(toFrameInfo._localScale, fromFrameInfo._localScale, tFrom);

		outMatrix = DUOLMath::Matrix::CreateScale(outScale)
			* DUOLMath::Matrix::CreateFromQuaternion(outRot)
			* DUOLMath::Matrix::CreateTranslation(outPos);
	}

	void AnimationClip::AddEvent(const AnimationEvent& event)
	{
		// 애니메이션 이벤트를 등록합니다.
		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<void>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<bool>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<int>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<float>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<const DUOLCommon::tstring&>(event._eventName);

		_events.push_back(event);
	}
}