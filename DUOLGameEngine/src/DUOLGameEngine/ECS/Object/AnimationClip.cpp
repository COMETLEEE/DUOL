#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

namespace DUOLGameEngine
{
	AnimationClip::AnimationClip(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _animationClip(nullptr)
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
		_animationClip = animationClip;
	}

	float AnimationClip::GetFrameRate() const
	{
		return _animationClip->_frameRate;
	}

	float AnimationClip::GetLength() const
	{
		return static_cast<float>(_animationClip->_totalKeyFrame) / _animationClip->_frameRate;
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

	void AnimationClip::AddEvent(const AnimationEvent& event)
	{
		AnimationEvent animEvent = event;
	}
}
