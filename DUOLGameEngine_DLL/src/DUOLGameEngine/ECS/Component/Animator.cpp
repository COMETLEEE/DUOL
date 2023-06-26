#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorState.h"
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
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_animatorController", &DUOLGameEngine::Animator::_animatorController)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// TODO : �հ� UUID ���� �� ū ��Ʈ�� �̾Ƴ��� ID ������ �����߰ڴ� .. Ŀ�´�Ƽ�� ��û Ŀ���� ���ڰ� ��ĥ ���� �ְڴ� !
		// , metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::AnimatorController)
	)
	.property("_boneGameObjects", &DUOLGameEngine::Animator::GetBoneGameObjects, &DUOLGameEngine::Animator::SetBoneGameObjects)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// TODO : �հ� UUID ���� �� ū ��Ʈ�� �̾Ƴ��� ID ������ �����߰ڴ� .. Ŀ�´�Ƽ�� ��û Ŀ���� ���ڰ� ��ĥ ���� �ְڴ� !
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	)
	.property("_boneOffsetMatrixList", &DUOLGameEngine::Animator::GetBoneOffsetMatrices, &DUOLGameEngine::Animator::SetBoneOffsetMatrices)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_speed", &DUOLGameEngine::Animator::GetSpeed, &DUOLGameEngine::Animator::SetSpeed)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

namespace DUOLGameEngine
{
	Animator::Animator(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _animatorController(nullptr)
		, _controllerContext(nullptr)
		, _speed(1.f)
	{
		// �̺�Ʈ�� �ֽ��ϴ�.
		_onSceneEditModeUpdatingID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				for (int boneIndex = 0; boneIndex < _boneGameObjects.size(); boneIndex++)
				{
					_boneMatrixList[boneIndex] = _boneGameObjects[boneIndex]->GetTransform()->GetWorldMatrix();
				}
			});
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

		// �̺�Ʈ���� ���ϴ�.
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _onSceneEditModeUpdatingID);

		//// TODO : TEST
		//DUOLGameEngine::PhysicsManager::GetInstance()->RemoveFixedUpdateEventHandler(_fixedUpdateEventHandlerID);
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* animationClip)
	{
		//IsLoop�̰� 
		if (!animationClip->IsLoop() && _controllerContext->_currentStateContexts[0]._loopCount > 0)
		{
			_controllerContext->_currentStateContexts[0]._prevFrame = 0;
			_controllerContext->_currentStateContexts[0]._currentFrame = 0;

			const int currentIntFrame = static_cast<int>(_controllerContext->_currentStateContexts[0]._currentFrame);

			animationClip->GetIsRootMotion()
				? CalcRootMotion(_controllerContext->_currentStateContexts[0]._prevFrame, currentIntFrame, animationClip)
				: CalcNormalMotion(currentIntFrame, animationClip);

			return;
		}
		// prevFrame = currentFrame ���� ������Ʈ���ֱ�.
		_controllerContext->_currentStateContexts[0]._prevFrame = _controllerContext->_currentStateContexts[0]._currentFrame;

		// Speed Parameter ����Ѵٸ� ..?
		_controllerContext->_currentStateContexts[0]._currentState->GetSpeedParameterActive()
			? _controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame
			+ (animationClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentStateContexts[0]._currentState->GetSpeedParameter()))
			: _controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame
			+ (animationClip->_frameRate * deltaTime * _speed);

		// ���� �������� ��ⷯ ������ ���� ������ ���̿� ��ġ��ŵ�ϴ�.
		_controllerContext->_currentStateContexts[0]._currentFrame = std::fmod(_controllerContext->_currentStateContexts[0]._currentFrame, static_cast<float>(animationClip->_endKeyFrame));

		const int currentIntFrame = static_cast<int>(_controllerContext->_currentStateContexts[0]._currentFrame);

		DUOLMath::Matrix outMat;

		animationClip->GetIsRootMotion()
			? CalcRootMotion(_controllerContext->_currentStateContexts[0]._prevFrame, currentIntFrame, animationClip)
			: CalcNormalMotion(currentIntFrame, animationClip);

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		animationClip->CheckKeyframeEventAndInvoke(
			_controllerContext->_currentStateContexts[0]._prevFrame,
			_controllerContext->_currentStateContexts[0]._currentFrame,
			_controllerContext->_currentStateContexts[0]._currentEventIndex,
			GetGameObject());

		// ���� �����Ӻ��� ���� �������� �� �۽��ϴ�. ==> 1ȸ ������ �Ǿ����ϴ� ..!
		if (_controllerContext->_currentStateContexts[0]._currentFrame < _controllerContext->_currentStateContexts[0]._prevFrame)
		{
			_controllerContext->_currentStateContexts[0]._loopCount++;
			_controllerContext->_currentStateContexts[0]._currentEventIndex = 0;
		}
	}

	void Animator::CalcRootMotion(int prevFrame, int currFrame, DUOLGameEngine::AnimationClip* animationClip)
	{
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// ��Ʈ ���� �̵��Ѹ�ŭ ���� ������Ʈ�� �̵���ŵ�ϴ�.
			if (targetBoneIndex == animationClip->GetRootMotionTargetIndex())
			{
				// ��Ÿ ��Ʈ������� ����
				DUOLMath::Vector3 prevPos;

				DUOLMath::Quaternion prevRotation;

				DUOLMath::Vector3 prevScale;

				DUOLMath::Vector3 currentPos;

				DUOLMath::Quaternion currentRotation;

				DUOLMath::Vector3 currentScale;

				DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

				DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

				// (���� ������) ���� (���� ������)������ �������� �����Ѵ�.
				for (int targetFrame = prevFrame + 1; targetFrame <= currFrame; targetFrame++)
				{
					animationClip->GetTargetFrameTransform(static_cast<int>(prevFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

					animationClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

					DUOLMath::Vector3 pos = currentPos - prevPos;

					prevRotation.Inverse(prevRotation);

					DUOLMath::Quaternion rot = prevRotation * currentRotation;

					// y�� �̵� X
					pos.y = 0.f;

					// X��, Z�� ȸ�� X
					DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

					deltaEuler.x = 0.f;

					deltaEuler.z = 0.f;

					deltaPos += pos;

					deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
				}

				// TODO : ������ for loop �� ������ ���� �����ϱ� �����ش�.
				animationClip->GetTargetFrameTransform(currFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

				// ���� ������Ʈ�� ��Ʈ�� �������� �ٷ� �����Ѵ�.
				GetTransform()->Translate(deltaPos, Space::Self);

				// TODO : ȸ���� ���� �ʾƺ���.
				// GetTransform()->Rotate(deltaRot, Space::World);

				// bone's local transform update.
				DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

				DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

				currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
				// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

				DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

				// ���� ������Ʈ�� ������ ���� ���� �κ��� ��Ʈ ���� �����Ѵ�.
				// TODO : XZ ȸ���� ������, �����ϸ� ..?
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(DUOLMath::Matrix::CreateScale(currentScale) * DUOLMath::Matrix::CreateFromQuaternion(xzRotForRoot) * DUOLMath::Matrix::CreateTranslation(yPosForRoot));

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex] *
					_boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
			else
			{
				// �ش� �������� Local transform�� �ܾ�ɴϴ�.
				animationClip->GetTargetFrameTransform(currFrame, targetBoneIndex, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex] *
					_boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
		}
	}

	void Animator::CalcNormalMotion(int currFrame, DUOLGameEngine::AnimationClip* animationClip)
	{
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// �ش� �������� Local transform�� �ܾ�ɴϴ�.
			animationClip->GetTargetFrameTransform(currFrame, targetBoneIndex, outMat);

			// bone's local transform update.
			_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

			// bone matrices pallet update
			_boneMatrixList[targetBoneIndex] =
				_boneOffsetMatrixList[targetBoneIndex]
				* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
		}
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip,
		float tFrom)
	{

		// ���� �� ���� �������� ��� ��ġ�ϴ��� �����մϴ�.
		_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom;

		_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo;

		// Speed Parameter ����Ѵٸ� ..?
		_controllerContext->_currentTransitionContexts[0]._currentTransition->GetFromState()->GetSpeedParameterActive()
			? _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom
			+ (fromClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentTransitionContexts[0]._currentTransition->GetFromState()->GetSpeedParameter()))
			: _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom
			+ (fromClip->_frameRate * deltaTime * _speed);

		_controllerContext->_currentTransitionContexts[0]._currentTransition->GetToState()->GetSpeedParameterActive()
			? _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo
			+ (toClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentTransitionContexts[0]._currentTransition->GetToState()->GetSpeedParameter()))
			: _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo
			+ (toClip->_frameRate * deltaTime * _speed);

		// �������� �� ������ ���� �Ѿ�ٸ�, ��ⷯ ������ ���� �������� ���� ��ġ��ŵ�ϴ�..
		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom, static_cast<float>(fromClip->_endKeyFrame));

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo, static_cast<float>(toClip->_endKeyFrame));

		// �� ������ ���� �̸� ���� ����� �������� ���� Int ������ ����ϴ�.
		const int currentIntFrameOfFrom = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		const int currentIntFrameOfTo = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		// �� �� ��Ʈ ����� �ƴմϴ�.
		if (!fromClip->GetIsRootMotion() && !toClip->GetIsRootMotion())
		{
			CalcNormalMotion(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, currentIntFrameOfFrom
				, _controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, currentIntFrameOfTo
				, fromClip, toClip, tFrom);
		}
		// From Animation Clip �� Root Motion�Դϴ�.
		else if (fromClip->GetIsRootMotion() && !toClip->GetIsRootMotion())
		{
			CalcRootMotionFrom(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, currentIntFrameOfFrom
				, _controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, currentIntFrameOfTo
				, fromClip, toClip, tFrom);
		}
		// to Animation Clip�� Root Motion �Դϴ�.
		else if (!fromClip->GetIsRootMotion() && toClip->GetIsRootMotion())
		{
			CalcRootMotionTo(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, currentIntFrameOfFrom
				, _controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, currentIntFrameOfTo
				, fromClip, toClip, tFrom);
		}
		// �� �� Root Motion �Դϴ�.
		else
		{
			CalcRootMotionFromTo(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, currentIntFrameOfFrom
				, _controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, currentIntFrameOfTo
				, fromClip, toClip, tFrom);
		}

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		if (fromClip->_useEventInTransition)
			fromClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom,
				_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom,
				_controllerContext->_currentTransitionContexts[0]._currentFromEventIndex,
				GetGameObject());

		// �ش� �ִϸ��̼� Ŭ���� ���Ͽ� ��ϵ� Ű ������ �̺�Ʈ�� �ִٸ� ȣ���մϴ�.
		if (toClip->_useEventInTransition)
			toClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo,
				_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo,
				_controllerContext->_currentTransitionContexts[0]._currentToEventIndex,
				GetGameObject());
	}

	void Animator::CalcRootMotionFrom(int prevFromFrame, int currFromFrame, int prevToFrame, int currToFrame,
		DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom)
	{
		DUOLGameEngine::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			if (targetBoneIndex == fromClip->GetRootMotionTargetIndex())
			{
				// ��Ÿ ��Ʈ������� ����
				DUOLMath::Vector3 prevPos;

				DUOLMath::Quaternion prevRotation;

				DUOLMath::Vector3 prevScale;

				DUOLMath::Vector3 currentPos;

				DUOLMath::Quaternion currentRotation;

				DUOLMath::Vector3 currentScale;

				DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

				DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

				// ���� �����Ӻ��� ���� �����ӱ����� �������� �����Ѵ�.
				for (int targetFrame = prevFromFrame + 1; targetFrame <= currFromFrame; targetFrame++)
				{
					fromClip->GetTargetFrameTransform(static_cast<int>(prevFromFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

					fromClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

					DUOLMath::Vector3 pos = currentPos - prevPos;

					prevRotation.Inverse(prevRotation);

					DUOLMath::Quaternion rot = prevRotation * currentRotation;

					// y�� �̵� X
					pos.y = 0.f;

					// X��, Z�� ȸ�� X
					DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

					deltaEuler.x = 0.f;

					deltaEuler.z = 0.f;

					deltaPos += pos;

					deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
				}

				// ���� ������Ʈ�� ��Ʈ�� �������� �ٷ� �����Ѵ�.
				GetTransform()->Translate(deltaPos, Space::Self);

				// TODO : ȸ���� ���� �ʾƺ���.
				// GetTransform()->Rotate(deltaRot, Space::World);

				// TODO : ������ for loop �� ������ ���� �����ϱ� �����ش�.
				fromClip->GetTargetFrameTransform(currFromFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

				// bone's local transform update.
				DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

				DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

				currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
				// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

				DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

				DUOLMath::Vector3 currentToPos;

				DUOLMath::Quaternion currentToRotation;

				DUOLMath::Vector3 currentToScale;

				toClip->GetTargetFrameTransform(currToFrame, targetBoneIndex, currentToPos, currentToRotation, currentToScale);

				DUOLMath::Vector3 outPos = DUOLMath::Vector3::Lerp(currentToPos, yPosForRoot, tFrom);

				DUOLMath::Quaternion outRot = DUOLMath::Quaternion::Slerp(currentToRotation, xzRotForRoot, tFrom);

				DUOLMath::Vector3 outScale = DUOLMath::Vector3::Lerp(currentToScale, currentScale, tFrom);

				outMat = DUOLMath::Matrix::CreateScale(outScale)
					* DUOLMath::Matrix::CreateFromQuaternion(outRot)
					* DUOLMath::Matrix::CreateTranslation(outPos);

				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				_boneMatrixList[targetBoneIndex] = _boneOffsetMatrixList[targetBoneIndex] * _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
			else
			{
				// �ش� �������� Local transform�� �ܾ�ɴϴ�.
				fromClip->GetTargetFramesTransform(currFromFrame, currToFrame, targetBoneIndex, tFrom, toClip, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex]
					* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
		}
	}

	void Animator::CalcRootMotionTo(int prevFrameFrom, int currFrameFrom, int prevFrameTo, int currFrameTo,
		DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom)
	{
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			if (targetBoneIndex == toClip->GetRootMotionTargetIndex())
			{
				// �������� ����ϴµ� .. Root ��ŭ ..
				// ��Ÿ ��Ʈ������� ����
				DUOLMath::Vector3 prevPos;

				DUOLMath::Quaternion prevRotation;

				DUOLMath::Vector3 prevScale;

				DUOLMath::Vector3 currentPos;

				DUOLMath::Quaternion currentRotation;

				DUOLMath::Vector3 currentScale;

				DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

				DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

				// ���� �����Ӻ��� ���� �����ӱ����� �������� �����Ѵ�.
				for (int targetFrame = prevFrameTo + 1; targetFrame <= currFrameTo; targetFrame++)
				{
					toClip->GetTargetFrameTransform(static_cast<int>(prevFrameTo++), targetBoneIndex, prevPos, prevRotation, prevScale);

					toClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

					DUOLMath::Vector3 pos = currentPos - prevPos;

					prevRotation.Inverse(prevRotation);

					DUOLMath::Quaternion rot = prevRotation * currentRotation;

					// y�� �̵� X
					pos.y = 0.f;

					// X��, Z�� ȸ�� X
					DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

					deltaEuler.x = 0.f;

					deltaEuler.z = 0.f;

					deltaPos += pos;

					deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
				}

				// ���� ������Ʈ�� ��Ʈ�� �������� �ٷ� �����Ѵ�.
				GetTransform()->Translate(deltaPos, Space::Self);

				// TODO : ȸ���� ���� �ʾƺ���.
				// GetTransform()->Rotate(deltaRot, Space::World);

				// TODO : ������ for loop �� ������ ���� �����ϱ� �����ش�.
				toClip->GetTargetFrameTransform(currFrameTo, targetBoneIndex, currentPos, currentRotation, currentScale);

				// bone's local transform update.
				DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

				DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

				currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
				// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

				DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

				DUOLMath::Vector3 currentFromPos;

				DUOLMath::Quaternion currentFromRotation;

				DUOLMath::Vector3 currentFromScale;

				fromClip->GetTargetFrameTransform(currFrameFrom, targetBoneIndex, currentFromPos, currentFromRotation, currentFromScale);

				DUOLMath::Vector3 outPos = DUOLMath::Vector3::Lerp(currentFromPos, yPosForRoot, tFrom);

				DUOLMath::Quaternion outRot = DUOLMath::Quaternion::Slerp(currentFromRotation, xzRotForRoot, tFrom);

				DUOLMath::Vector3 outScale = DUOLMath::Vector3::Lerp(currentFromScale, currentScale, tFrom);

				outMat = DUOLMath::Matrix::CreateScale(outScale)
					* DUOLMath::Matrix::CreateFromQuaternion(outRot)
					* DUOLMath::Matrix::CreateTranslation(outPos);

				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				_boneMatrixList[targetBoneIndex] = _boneOffsetMatrixList[targetBoneIndex] * _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
			else
			{
				// �ش� �������� Local transform�� �ܾ�ɴϴ�.
				fromClip->GetTargetFramesTransform(currFrameFrom, currFrameTo, targetBoneIndex, tFrom, toClip, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex]
					* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
		}
	}

	void Animator::CalcRootMotionFromTo(int prevFrameFrom, int currFrameFrom, int prevFrameTo, int currFrameTo,
		DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom)
	{
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// �� ���� Ŭ���� Ÿ�� �� => ��Ʈ ���� ���ٰ� ��������. (��κ��� ��쿡�� �´� ������ ���̴� ...)
			if (targetBoneIndex == toClip->GetRootMotionTargetIndex())
			{
				// �������� ����ϴµ� .. Root ��ŭ ..
				// ��Ÿ ��Ʈ������� ����
				DUOLMath::Vector3 prevPos;

				DUOLMath::Quaternion prevRotation;

				DUOLMath::Vector3 prevScale;

				DUOLMath::Vector3 currentPos;

				DUOLMath::Quaternion currentRotation;

				DUOLMath::Vector3 currentScale;

				DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

				DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

				// To �� ��Ʈ ��� ��
				// ���� �����Ӻ��� ���� �����ӱ����� �������� �����Ѵ�.
				for (int targetFrame = prevFrameTo + 1; targetFrame <= currFrameTo; targetFrame++)
				{
					toClip->GetTargetFrameTransform(static_cast<int>(prevFrameTo++), targetBoneIndex, prevPos, prevRotation, prevScale);

					toClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

					DUOLMath::Vector3 pos = currentPos - prevPos;

					prevRotation.Inverse(prevRotation);

					DUOLMath::Quaternion rot = prevRotation * currentRotation;

					// y�� �̵� X
					pos.y = 0.f;

					// X��, Z�� ȸ�� X
					DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

					deltaEuler.x = 0.f;

					deltaEuler.z = 0.f;

					deltaPos += pos;

					deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
				}

				// ���� ������Ʈ�� ��Ʈ�� �������� �ٷ� �����Ѵ�.
				GetTransform()->Translate(deltaPos, Space::Self);

				// TODO : ȸ���� ���� �ʾƺ���.
				// GetTransform()->Rotate(deltaRot, Space::World);

				deltaPos = DUOLMath::Vector3::Zero;

				deltaRot = DUOLMath::Quaternion::Identity;

				// From �� ��Ʈ ��� ��
				// ���� �����Ӻ��� ���� �����ӱ����� �������� �����Ѵ�.
				for (int targetFrame = prevFrameFrom + 1; targetFrame <= currFrameFrom; targetFrame++)
				{
					fromClip->GetTargetFrameTransform(static_cast<int>(prevFrameFrom++), targetBoneIndex, prevPos, prevRotation, prevScale);

					fromClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

					DUOLMath::Vector3 pos = currentPos - prevPos;

					prevRotation.Inverse(prevRotation);

					DUOLMath::Quaternion rot = prevRotation * currentRotation;

					// y�� �̵� X
					pos.y = 0.f;

					// X��, Z�� ȸ�� X
					DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

					deltaEuler.x = 0.f;

					deltaEuler.z = 0.f;

					deltaPos += pos;

					deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
				}

				// ���� ������Ʈ�� ��Ʈ�� �������� �ٷ� �����Ѵ�.
				GetTransform()->Translate(deltaPos, Space::Self);

				// TODO : ȸ���� ���� �ʾƺ���.
				// GetTransform()->Rotate(deltaRot, Space::World);



				toClip->GetTargetFrameTransform(currFrameTo, targetBoneIndex, currentPos, currentRotation, currentScale);

				// bone's local transform update.
				DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

				DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

				currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
				// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

				DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);





				DUOLMath::Vector3 currentScaleFrom;

				fromClip->GetTargetFrameTransform(currFrameFrom, targetBoneIndex, currentPos, currentRotation, currentScaleFrom);

				DUOLMath::Vector3 yPosForRootFrom = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

				DUOLMath::Vector3 currentEulerFrom = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

				currentEulerFrom = DUOLMath::Vector3(currentEulerFrom.x, currentEulerFrom.y, currentEulerFrom.z);
				// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

				DUOLMath::Quaternion xzRotForRootFrom = DUOLMath::Quaternion::CreateFromEulerAngle(currentEulerFrom);

				DUOLMath::Vector3 outPos = DUOLMath::Vector3::Lerp(yPosForRootFrom, yPosForRoot, tFrom);

				DUOLMath::Quaternion outRot = DUOLMath::Quaternion::Slerp(xzRotForRootFrom, xzRotForRoot, tFrom);

				DUOLMath::Vector3 outScale = DUOLMath::Vector3::Lerp(currentScaleFrom, currentScale, tFrom);

				outMat = DUOLMath::Matrix::CreateScale(outScale)
					* DUOLMath::Matrix::CreateFromQuaternion(outRot)
					* DUOLMath::Matrix::CreateTranslation(outPos);

				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				_boneMatrixList[targetBoneIndex] = _boneOffsetMatrixList[targetBoneIndex] * _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
			else
			{
				// �ش� �������� Local transform�� �ܾ�ɴϴ�.
				fromClip->GetTargetFramesTransform(currFrameFrom, currFrameTo, targetBoneIndex, tFrom, toClip, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex]
					* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
		}
	}

	void Animator::CalcNormalMotion(int prevFrameFrom, int currFrameFrom, int prevFrameTo, int currFrameTo,
		DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip, float tFrom)
	{
		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// �ش� �������� Local transform�� �ܾ�ɴϴ�.
			fromClip->GetTargetFramesTransform(currFrameFrom, currFrameTo, targetBoneIndex, tFrom, toClip, outMat);

			// bone's local transform update.
			_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

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

		if (_controllerContext == nullptr)
			SetAnimatorController(_animatorController);

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

		DUOL_TRACE(DUOL_CONSOLE, "Animator | Change Animator Context");

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

	float Animator::GetSpeed() const
	{
		return _speed;
	}

	int Animator::GetCurrentLoopCount()
	{
		if (_controllerContext != nullptr)
			return _controllerContext->_currentStateContexts[0]._loopCount;

		return 0;
	}

	void Animator::SetSpeed(float value)
	{
		_speed = value;
	}

	void Animator::InitializeCurrentLoopCount()
	{
		if (_controllerContext != nullptr)
		{
			_controllerContext->_currentStateContexts[0]._loopCount = 0;
			_controllerContext->_currentStateContexts[0]._currentEventIndex = 0;
		}
	}

	bool Animator::IsOnTransition()
	{
		if (_controllerContext != nullptr)
			return _controllerContext->_currentStateMachineContexts[0]._isOnTransition;

		return false;
	}

	void Animator::SetBool(const DUOLCommon::tstring& paramName, bool value) const
	{
		std::string str(paramName.begin(), paramName.end());
		DUOL_TRACE(DUOL_CONSOLE, "Animatior Transition SetBool | Play from {0} -> to {1} ", str, value);


		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_boolParameters.contains(paramName))
			_controllerContext->_boolParameters[paramName] = value;
	}

	void Animator::SetFloat(const DUOLCommon::tstring& paramName, float value) const
	{
		std::string str(paramName.begin(), paramName.end());
		DUOL_TRACE(DUOL_CONSOLE, "Animatior Transition SetFloat | Play from {0} -> to {1} ", str, value);

		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		if (_controllerContext->_floatParameters.contains(paramName))
			_controllerContext->_floatParameters[paramName] = value;
	}

	void Animator::SetInt(const DUOLCommon::tstring& paramName, int value) const
	{
		std::string str(paramName.begin(), paramName.end());
		DUOL_TRACE(DUOL_CONSOLE, "Animatior Transition SetInt | Play from {0} -> to {1} ", str, value);

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
		if (_controllerContext->_floatParameters.contains(L"AnimationSqeed"))
		{
			int a =  0;
		}

		return _controllerContext->_floatParameters.contains(paramName) ? _controllerContext->_floatParameters.at(paramName) : 0.f;
	}

	int Animator::GetInt(const DUOLCommon::tstring& paramName) const
	{
		return _controllerContext->_intParameters.contains(paramName) ? _controllerContext->_intParameters.at(paramName) : 0;
	}

	void Animator::AllParamReset() const
	{
		// ���ؽ�Ʈ�� ������ == �ִϸ����� ��Ʈ�ѷ��� ������ �������� �ʽ��ϴ�.
		if (_controllerContext == nullptr)
			return;

		for (auto& iter : _controllerContext->_intParameters)
			iter.second = 0;

		for (auto& iter : _controllerContext->_floatParameters)
			iter.second = 0;

		for (auto& iter : _controllerContext->_boolParameters)
			iter.second = false;
	}

	const DUOLCommon::tstring& Animator::GetCurrentStateName() const
	{
		return _controllerContext->_currentStateMachineContexts[0]._isOnTransition
			? _controllerContext->_currentTransitionContexts[0]._currentTransition->GetToState()->GetName()
			: _controllerContext->_currentStateContexts[0]._currentState->GetName();
	}
}