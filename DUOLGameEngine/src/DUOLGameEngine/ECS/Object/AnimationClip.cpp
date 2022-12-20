#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

namespace DUOLGameEngine
{
	AnimationClip::AnimationClip(const DUOLCommon::tstring& name) :
		ObjectBase(name)
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

	void AnimationClip::AddEvent(const AnimationEvent& event)
	{
		AnimationEvent animEvent = event;
	}
}
