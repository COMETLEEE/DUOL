#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/Manager/EventManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	// DUOLGameEngine::AnimationClip 의 시리얼라이즈는 UUID만 있으면 충분하다 ..!
	rttr::registration::class_<DUOLGameEngine::AnimationClip>("AnimationClip")
	.constructor<>()
	(

	);
}

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

	bool AnimationClip::CheckKeyframeEventAndInvoke(float prevFrame, float currFrame)
	{
		// 이전 프레임이 기존 프레임보다 큰 값으로 들어온다면 한 루프가 돌아간 상태이므로 currFrame에 MaxFrame을 더해준다.
		if (prevFrame >= currFrame)
			currFrame += _maxFrame;

		for (auto& event : _events)
		{
			// 이벤트에 설정된 프레임이 이전 프레임보다 크고, 현재 프레임보다 작으면 지나갔다는 뜻이므로 이벤트를 호출합니다.
			if ((event._targetFrame >= prevFrame) && (event._targetFrame <= currFrame))
			{
				// 일단, 모든 파라미터에 대해서 호출합니다.
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._boolParameter);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._intParameter);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._floatParameter);
				
				// 어떻게 방법 없나 ..
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage<const DUOLCommon::tstring&>(event._eventName, event._tstringParameter);
			}
		}

		return false;
	}

	void AnimationClip::AddEvent(const AnimationEvent& event)
	{
		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<void>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<bool>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<int>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<float>(event._eventName);

		DUOLGameEngine::EventManager::GetInstance()->RegisterEvent<const DUOLCommon::tstring&>(event._eventName);

		// Keyframe event 터트려야하니까 ..!
		_events.push_back(event);
	}
}