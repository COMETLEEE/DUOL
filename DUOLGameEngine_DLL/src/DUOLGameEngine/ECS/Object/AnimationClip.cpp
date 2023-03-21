#include "DUOLGameEngine/ECS/Object/AnimationClip.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGameEngine/Manager/EventManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::AnimationEvent>("AnimationEvent")
	.constructor()
	(

	)
	.property("_eventName", &DUOLGameEngine::AnimationEvent::_eventName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_targetFrame", &DUOLGameEngine::AnimationEvent::_targetFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_boolParameter", &DUOLGameEngine::AnimationEvent::_boolParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_intParameter", &DUOLGameEngine::AnimationEvent::_intParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_floatParameter", &DUOLGameEngine::AnimationEvent::_floatParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_tstringParameter", &DUOLGameEngine::AnimationEvent::_tstringParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

	rttr::registration::class_<DUOLGameEngine::AnimationFrame>("AnimationFrame")
	.constructor()
	(

	)
	.property("_time", &DUOLGameEngine::AnimationFrame::_time)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_localTransform", &DUOLGameEngine::AnimationFrame::_localTransform)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_localRotation", &DUOLGameEngine::AnimationFrame::_localRotation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_localScale", &DUOLGameEngine::AnimationFrame::_localScale)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

	rttr::registration::class_<DUOLGameEngine::AnimationClip>("AnimationClip")
	.constructor()
	(

	)
	.property("_lengthInSeconds", &DUOLGameEngine::AnimationClip::_lengthInSeconds)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_maxFrame", &DUOLGameEngine::AnimationClip::_maxFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_frameRate", &DUOLGameEngine::AnimationClip::_frameRate)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_tickPerFrame", &DUOLGameEngine::AnimationClip::_tickPerFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_totalKeyFrame", &DUOLGameEngine::AnimationClip::_totalKeyFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_startKeyFrame", &DUOLGameEngine::AnimationClip::_startKeyFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_endKeyFrame", &DUOLGameEngine::AnimationClip::_endKeyFrame)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_frameList", &DUOLGameEngine::AnimationClip::_frameList)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_events", &DUOLGameEngine::AnimationClip::_events)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLGameEngine
{
	AnimationClip::AnimationClip(const DUOLCommon::tstring& name) :
		ObjectBase(name, ObjectType::Resource)
		, _lengthInSeconds(0.f)
		, _maxFrame(0.f)
		, _events({})
	{

	}

	AnimationClip::~AnimationClip()
	{

	}

	void AnimationClip::SetPrimitiveAnimationClip(DUOLGraphicsEngine::AnimationClip* animationClip)
	{
		if (animationClip == nullptr)
			return;

		__super::SetName(DUOLCommon::StringHelper::ToTString(animationClip->_animationName));

		_frameRate = animationClip->_frameRate;

		_tickPerFrame = animationClip->_tickPerFrame;

		_totalKeyFrame = animationClip->_totalKeyFrame;

		_startKeyFrame = animationClip->_startKeyFrame;

		_endKeyFrame = animationClip->_endKeyFrame;

		_lengthInSeconds = static_cast<float>(_totalKeyFrame) / _frameRate;

		_maxFrame = static_cast<float>(_totalKeyFrame);

		// �ִϸ��̼� ������ ����Ʈ �ʱ�ȭ
		_frameList.resize(animationClip->_keyFrameList.size());

		for (int i = 0 ; i < _frameList.size() ; i++)
		{
			_frameList[i].resize(animationClip->_keyFrameList[i].size());

			for (int j = 0 ; j < _frameList[i].size(); j++)
			{
				_frameList[i][j] = animationClip->_keyFrameList[i][j];
			}
		}
	}

	float AnimationClip::GetFrameRate() const
	{
		return _frameRate;
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
		auto&& frame = _frameList[targetBoneIndex][targetFrame];

		outPos = frame._localTransform;

		outRot = frame._localRotation;

		outScale = frame._localScale;
	}

	void AnimationClip::GetTargetFrameTransform(int targetFrame, int targetBoneIndex, DUOLMath::Matrix& outMatrix) const
	{
		auto&& frame = _frameList[targetBoneIndex][targetFrame];

		outMatrix = DUOLMath::Matrix::CreateScale(frame._localScale)
			* DUOLMath::Matrix::CreateFromQuaternion(frame._localRotation)
			* DUOLMath::Matrix::CreateTranslation(frame._localTransform);
	}

	void AnimationClip::GetTargetFramesTransform(int fromFrame, int toFrame, int targetBoneIndex, float tFrom,
		DUOLGameEngine::AnimationClip* toClip, DUOLMath::Matrix& outMatrix) const
	{
		auto&& fromFrameInfo = _frameList[targetBoneIndex][fromFrame];

		auto&& toFrameInfo = toClip->_frameList[targetBoneIndex][toFrame];

		DUOLMath::Vector3 outPos = DUOLMath::Vector3::Lerp(toFrameInfo._localTransform, fromFrameInfo._localTransform, tFrom);

		DUOLMath::Quaternion outRot = DUOLMath::Quaternion::Slerp(toFrameInfo._localRotation, fromFrameInfo._localRotation, tFrom);

		DUOLMath::Vector3 outScale = DUOLMath::Vector3::Lerp(toFrameInfo._localScale, fromFrameInfo._localScale, tFrom);

		outMatrix = DUOLMath::Matrix::CreateScale(outScale)
			* DUOLMath::Matrix::CreateFromQuaternion(outRot)
			* DUOLMath::Matrix::CreateTranslation(outPos);
	}

	bool AnimationClip::CheckKeyframeEventAndInvoke(float prevFrame, float currFrame)
	{
		// ���� �������� ���� �����Ӻ��� ū ������ ���´ٸ� �� ������ ���ư� �����̹Ƿ� currFrame�� MaxFrame�� �����ش�.
		if (prevFrame >= currFrame)
			currFrame += _maxFrame;

		for (auto& event : _events)
		{ 
			// �̺�Ʈ�� ������ �������� ���� �����Ӻ��� ũ��, ���� �����Ӻ��� ������ �������ٴ� ���̹Ƿ� �̺�Ʈ�� ȣ���մϴ�.
			if ((event._targetFrame >= prevFrame) && (event._targetFrame <= currFrame))
			{
				// �ϴ�, ��� �Ķ���Ϳ� ���ؼ� ȣ���մϴ�.
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._boolParameter);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._intParameter);
				DUOLGameEngine::EventManager::GetInstance()->SendEventMessage(event._eventName, event._floatParameter);
				
				// ��� ��� ���� ..
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

		// Keyframe event ��Ʈ�����ϴϱ� ..!
		_events.push_back(event);
	}
}