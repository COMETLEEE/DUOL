#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGameEngine/ECS/Object/Avatar.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

namespace DUOLGameEngine
{
	Animator::Animator(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _currentAnimationClip(nullptr)
		, _currentAvatar(nullptr)
	{
	}

	Animator::~Animator()
	{
	}

	void Animator::ComputeAnimationTM()
	{
		// Calc Animation TM
		DUOLGraphicsEngine::AnimationClip* animClip = _currentAnimationClip->GetPrimitiveAnimationClip();

		animClip->_keyFrameList;
	}
}