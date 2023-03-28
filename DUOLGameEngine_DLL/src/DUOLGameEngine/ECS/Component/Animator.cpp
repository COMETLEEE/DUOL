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
		// TODO : 먼가 UUID 보다 더 큰 비트로 뽑아내는 ID 개념을 만들어야겠다 .. 커뮤니티가 엄청 커지면 숫자가 겹칠 수도 있겠다 !
		// , metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::AnimatorController)
	)
	.property("_boneGameObjects", &DUOLGameEngine::Animator::GetBoneGameObjects, &DUOLGameEngine::Animator::SetBoneGameObjects)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// TODO : 먼가 UUID 보다 더 큰 비트로 뽑아내는 ID 개념을 만들어야겠다 .. 커뮤니티가 엄청 커지면 숫자가 겹칠 수도 있겠다 !
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
		// 이벤트에 넣습니다.
		_onSceneEditModeUpdatingID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
		{
			for (int boneIndex = 0; boneIndex < _boneGameObjects.size() ; boneIndex++)
			{
				_boneMatrixList[boneIndex] = _boneGameObjects[boneIndex]->GetTransform()->GetWorldMatrix();
			}
		});
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

		// 이벤트에서 뺍니다.
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _onSceneEditModeUpdatingID);
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* animationClip)
	{
		float prevFrame = _controllerContext->_currentStateContexts[0]._prevFrame;

		// prevFrame = currentFrame 으로 업데이트해주기.
		_controllerContext->_currentStateContexts[0]._prevFrame = _controllerContext->_currentStateContexts[0]._currentFrame;

		// target (== current)frame.

		// Speed Parameter 사용한다면 ..?
		if (_controllerContext->_currentStateContexts[0]._currentState->GetSpeedParameterActive())
		{
			_controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame +
				(animationClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentStateContexts[0]._currentState->GetSpeedParameter()));
		}
		else
			_controllerContext->_currentStateContexts[0]._currentFrame = _controllerContext->_currentStateContexts[0]._currentFrame + (animationClip->_frameRate * deltaTime * _speed);

		// 현재 프레임을 모듈러 연산을 통해 프레임 사이에 위치시킵니다.
		_controllerContext->_currentStateContexts[0]._currentFrame = std::fmod(_controllerContext->_currentStateContexts[0]._currentFrame, static_cast<float>(animationClip->_endKeyFrame));

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		animationClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentStateContexts[0]._prevFrame, _controllerContext->_currentStateContexts[0]._currentFrame);

		const int currentIntFrame = static_cast<int>(_controllerContext->_currentStateContexts[0]._currentFrame);

		// 이전 프레임보다 현재 프레임이 더 작습니다. ==> 1회 루프가 되었습니다 ..!
		if (_controllerContext->_currentStateContexts[0]._currentFrame < _controllerContext->_currentStateContexts[0]._prevFrame)
			_controllerContext->_currentStateContexts[0]._loopCount++;

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// 루트 모션을 사용하지 않는 애니메이션 클립입니다.
			if (!animationClip->GetIsRootMotion())
			{
				// 해당 프레임의 Local transform을 긁어옵니다.
				animationClip->GetTargetFrameTransform(currentIntFrame, targetBoneIndex, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex]
					* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
			// 루트 모션을 사용하는 애니메이션 클립입니다.
			else
			{
				// 루트 본이 이동한만큼 게임 오브젝트를 이동시킵니다.
				if (targetBoneIndex == animationClip->GetRootMotionTargetIndex())
				{
					// 델타 매트릭스라고 보자
					DUOLMath::Vector3 prevPos;

					DUOLMath::Quaternion prevRotation;

					DUOLMath::Vector3 prevScale;

					DUOLMath::Vector3 currentPos;

					DUOLMath::Quaternion currentRotation;

					DUOLMath::Vector3 currentScale;

					DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

					DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;
					
					// 지난 프레임부터 현재 프레임까지의 변동량을 누적한다.
					for (int targetFrame = prevFrame + 1; targetFrame <= currentIntFrame ; targetFrame++)
					{
						animationClip->GetTargetFrameTransform(static_cast<int>(prevFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

						animationClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

						DUOLMath::Vector3 pos = currentPos - prevPos;

						prevRotation.Inverse(prevRotation);

						DUOLMath::Quaternion rot = prevRotation * currentRotation;

						// y축 이동 X
						pos.y = 0.f;

						// X축, Z축 회전 X
						DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

						deltaEuler.x = 0.f;

						deltaEuler.z = 0.f;

						deltaPos += pos;

						deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
					}

					// 게임 오브젝트에 루트의 변동량을 바로 적용한다.
					GetTransform()->Translate(deltaPos, Space::Self);

					// TODO : 회전은 하지 않아본다.
					// GetTransform()->Rotate(deltaRot, Space::World);

					// bone's local transform update.
					DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

					DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

					currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
					// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

					DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

					// 게임 오브젝트에 적용이 되지 않은 부분을 루트 본에 적용한다.
					// TODO : XZ 회전과 포지션, 스케일만 ..?
					_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(DUOLMath::Matrix::CreateScale(currentScale) * DUOLMath::Matrix::CreateFromQuaternion(xzRotForRoot) * DUOLMath::Matrix::CreateTranslation(yPosForRoot));

					// bone matrices pallet update
					_boneMatrixList[targetBoneIndex] =
						_boneOffsetMatrixList[targetBoneIndex] *
						_boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
				}
				else
				{
					// 해당 프레임의 Local transform을 긁어옵니다.
					animationClip->GetTargetFrameTransform(currentIntFrame, targetBoneIndex, outMat);

					// bone's local transform update.
					_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

					// bone matrices pallet update
					_boneMatrixList[targetBoneIndex] =
						_boneOffsetMatrixList[targetBoneIndex] *
						_boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
				}
			}
		}
	}

	void Animator::Play(float deltaTime, DUOLGameEngine::AnimationClip* fromClip, DUOLGameEngine::AnimationClip* toClip,
		float tFrom)
	{
		float prevFromFrame = _controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom;

		float prevToFrame = _controllerContext->_currentTransitionContexts[0]._prevFrameOfTo;

		// 이전 및 현재 프레임이 어디에 위치하는지 갱신합니다.
		_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom;

		_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo;

		// Speed Parameter 사용한다면 ..?
		if (_controllerContext->_currentTransitionContexts[0]._currentTransition->GetFromState()->GetSpeedParameterActive())
		{
			_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom +
				(fromClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentTransitionContexts[0]._currentTransition->GetFromState()->GetSpeedParameter()));
		}
		else
			_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom = _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom + (fromClip->_frameRate * deltaTime * _speed);

		if (_controllerContext->_currentTransitionContexts[0]._currentTransition->GetToState()->GetSpeedParameterActive())
		{
			_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo +
				(toClip->_frameRate * deltaTime * GetFloat(_controllerContext->_currentTransitionContexts[0]._currentTransition->GetToState()->GetSpeedParameter()));
		}
		else
			_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo = _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo + (toClip->_frameRate * deltaTime * _speed);

		// 프레임이 총 프레임 수를 넘어갔다면, 모듈러 연산을 통해 프레임을 정상 위치시킵니다..
		_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom, static_cast<float>(fromClip->_endKeyFrame));

		_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo =
			std::fmod(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo, static_cast<float>(toClip->_endKeyFrame));

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		if (fromClip->_useEventInTransition)
			fromClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfFrom, _controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		// 해당 애니메이션 클립에 대하여 등록된 키 프레임 이벤트가 있다면 호출합니다.
		if (toClip->_useEventInTransition)
			toClip->CheckKeyframeEventAndInvoke(_controllerContext->_currentTransitionContexts[0]._prevFrameOfTo, _controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		// 각 프레임 별로 미리 계산된 행렬을 가져오기 위해 Int 변수를 만듭니다.
		const int currentIntFrameOfFrom = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfFrom);

		const int currentIntFrameOfTo = static_cast<int>(_controllerContext->_currentTransitionContexts[0]._currentFrameOfTo);

		DUOLMath::Matrix outMat;

		for (int targetBoneIndex = 0; targetBoneIndex < _boneMatrixList.size(); targetBoneIndex++)
		{
			if ((_boneGameObjects.size() < targetBoneIndex) || (_boneOffsetMatrixList.size() < targetBoneIndex))
				break;

			// From 만 루트 모션
			if (fromClip->GetIsRootMotion() && !toClip->GetIsRootMotion())
			{
				if (targetBoneIndex == fromClip->GetRootMotionTargetIndex())
				{
					// 델타 매트릭스라고 보자
					DUOLMath::Vector3 prevPos;

					DUOLMath::Quaternion prevRotation;

					DUOLMath::Vector3 prevScale;

					DUOLMath::Vector3 currentPos;

					DUOLMath::Quaternion currentRotation;

					DUOLMath::Vector3 currentScale;

					DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

					DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

					// 지난 프레임부터 현재 프레임까지의 변동량을 누적한다.
					for (int targetFrame = prevFromFrame + 1; targetFrame <= currentIntFrameOfFrom; targetFrame++)
					{
						fromClip->GetTargetFrameTransform(static_cast<int>(prevFromFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

						fromClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

						DUOLMath::Vector3 pos = currentPos - prevPos;

						prevRotation.Inverse(prevRotation);

						DUOLMath::Quaternion rot = prevRotation * currentRotation;

						// y축 이동 X
						pos.y = 0.f;

						// X축, Z축 회전 X
						DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

						deltaEuler.x = 0.f;

						deltaEuler.z = 0.f;

						deltaPos += pos;

						deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
					}

					// 게임 오브젝트에 루트의 변동량을 바로 적용한다.
					GetTransform()->Translate(deltaPos, Space::Self);

					// TODO : 회전은 하지 않아본다.
					// GetTransform()->Rotate(deltaRot, Space::World);
					
					fromClip->GetTargetFrameTransform(currentIntFrameOfFrom, targetBoneIndex, currentPos, currentRotation, currentScale);

					// bone's local transform update.
					DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

					DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

					currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
					// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

					DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

					DUOLMath::Vector3 currentToPos;

					DUOLMath::Quaternion currentToRotation;

					DUOLMath::Vector3 currentToScale;

					toClip->GetTargetFrameTransform(currentIntFrameOfTo, targetBoneIndex, currentToPos, currentToRotation, currentToScale);

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
					// 해당 프레임의 Local transform을 긁어옵니다.
					fromClip->GetTargetFramesTransform(currentIntFrameOfFrom, currentIntFrameOfTo, targetBoneIndex, tFrom, toClip, outMat);

					// bone's local transform update.
					_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

					// bone matrices pallet update
					_boneMatrixList[targetBoneIndex] =
						_boneOffsetMatrixList[targetBoneIndex]
						* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
				}
			}
			// To 만 루트 모션
			else if (!fromClip->GetIsRootMotion() && toClip->GetIsRootMotion())
			{
				if (targetBoneIndex == toClip->GetRootMotionTargetIndex())
				{
					// 움직임을 줘야하는데 .. Root 만큼 ..
					// 델타 매트릭스라고 보자
					DUOLMath::Vector3 prevPos;

					DUOLMath::Quaternion prevRotation;

					DUOLMath::Vector3 prevScale;

					DUOLMath::Vector3 currentPos;

					DUOLMath::Quaternion currentRotation;

					DUOLMath::Vector3 currentScale;

					DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

					DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

					// 지난 프레임부터 현재 프레임까지의 변동량을 누적한다.
					for (int targetFrame = prevToFrame + 1; targetFrame <= currentIntFrameOfTo; targetFrame++)
					{
						toClip->GetTargetFrameTransform(static_cast<int>(prevToFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

						toClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

						DUOLMath::Vector3 pos = currentPos - prevPos;

						prevRotation.Inverse(prevRotation);

						DUOLMath::Quaternion rot = prevRotation * currentRotation;

						// y축 이동 X
						pos.y = 0.f;

						// X축, Z축 회전 X
						DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

						deltaEuler.x = 0.f;

						deltaEuler.z = 0.f;

						deltaPos += pos;

						deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
					}

					// 게임 오브젝트에 루트의 변동량을 바로 적용한다.
					GetTransform()->Translate(deltaPos, Space::Self);

					// TODO : 회전은 하지 않아본다.
					// GetTransform()->Rotate(deltaRot, Space::World);

					toClip->GetTargetFrameTransform(currentIntFrameOfTo, targetBoneIndex, currentPos, currentRotation, currentScale);

					// bone's local transform update.
					DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

					DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

					currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
					// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

					DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

					DUOLMath::Vector3 currentFromPos;

					DUOLMath::Quaternion currentFromRotation;

					DUOLMath::Vector3 currentFromScale;

					fromClip->GetTargetFrameTransform(currentIntFrameOfFrom, targetBoneIndex, currentFromPos, currentFromRotation, currentFromScale);

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
					// 해당 프레임의 Local transform을 긁어옵니다.
					fromClip->GetTargetFramesTransform(currentIntFrameOfFrom, currentIntFrameOfTo, targetBoneIndex, tFrom, toClip, outMat);

					// bone's local transform update.
					_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

					// bone matrices pallet update
					_boneMatrixList[targetBoneIndex] =
						_boneOffsetMatrixList[targetBoneIndex]
						* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
				}
			}
			// From 과 To 루트 모션
			else if (fromClip->GetIsRootMotion() && toClip->GetIsRootMotion())
			{
				// 두 개의 클립이 타겟 본 => 루트 본이 같다고 가정하자. (대부분의 경우에서 맞는 가정일 것이다 ...)
				if (targetBoneIndex == toClip->GetRootMotionTargetIndex())
				{
					// 움직임을 줘야하는데 .. Root 만큼 ..
					// 델타 매트릭스라고 보자
					DUOLMath::Vector3 prevPos;

					DUOLMath::Quaternion prevRotation;

					DUOLMath::Vector3 prevScale;

					DUOLMath::Vector3 currentPos;

					DUOLMath::Quaternion currentRotation;

					DUOLMath::Vector3 currentScale;

					DUOLMath::Vector3 deltaPos = DUOLMath::Vector3::Zero;

					DUOLMath::Quaternion deltaRot = DUOLMath::Quaternion::Identity;

					// To 의 루트 모션 값
					// 지난 프레임부터 현재 프레임까지의 변동량을 누적한다.
					for (int targetFrame = prevToFrame + 1; targetFrame <= currentIntFrameOfTo; targetFrame++)
					{
						toClip->GetTargetFrameTransform(static_cast<int>(prevToFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

						toClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

						DUOLMath::Vector3 pos = currentPos - prevPos;

						prevRotation.Inverse(prevRotation);

						DUOLMath::Quaternion rot = prevRotation * currentRotation;

						// y축 이동 X
						pos.y = 0.f;

						// X축, Z축 회전 X
						DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

						deltaEuler.x = 0.f;

						deltaEuler.z = 0.f;

						deltaPos += pos;

						deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
					}

					// 게임 오브젝트에 루트의 변동량을 바로 적용한다.
					GetTransform()->Translate(deltaPos, Space::Self);

					// TODO : 회전은 하지 않아본다.
					// GetTransform()->Rotate(deltaRot, Space::World);

					deltaPos = DUOLMath::Vector3::Zero;

					deltaRot = DUOLMath::Quaternion::Identity;

					// From 의 루트 모션 값
					// 지난 프레임부터 현재 프레임까지의 변동량을 누적한다.
					for (int targetFrame = prevFromFrame + 1; targetFrame <= currentIntFrameOfFrom; targetFrame++)
					{
						fromClip->GetTargetFrameTransform(static_cast<int>(prevToFrame++), targetBoneIndex, prevPos, prevRotation, prevScale);

						fromClip->GetTargetFrameTransform(targetFrame, targetBoneIndex, currentPos, currentRotation, currentScale);

						DUOLMath::Vector3 pos = currentPos - prevPos;

						prevRotation.Inverse(prevRotation);

						DUOLMath::Quaternion rot = prevRotation * currentRotation;

						// y축 이동 X
						pos.y = 0.f;

						// X축, Z축 회전 X
						DUOLMath::Vector3 deltaEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(rot);

						deltaEuler.x = 0.f;

						deltaEuler.z = 0.f;

						deltaPos += pos;

						deltaRot *= DUOLMath::Quaternion::CreateFromEulerAngle(deltaEuler);
					}


					// 게임 오브젝트에 루트의 변동량을 바로 적용한다.
					GetTransform()->Translate(deltaPos, Space::Self);

					// TODO : 회전은 하지 않아본다.
					// GetTransform()->Rotate(deltaRot, Space::World);

					toClip->GetTargetFrameTransform(currentIntFrameOfTo, targetBoneIndex, currentPos, currentRotation, currentScale);

					// bone's local transform update.
					DUOLMath::Vector3 yPosForRoot = DUOLMath::Vector3(0.f, currentPos.y, 0.f);

					DUOLMath::Vector3 currentEuler = DUOLMath::Quaternion::ConvertQuaternionToEuler(currentRotation);

					currentEuler = DUOLMath::Vector3(currentEuler.x, currentEuler.y, currentEuler.z);
					// currentEuler = DUOLMath::Vector3(currentEuler.x, 0.f, currentEuler.z);

					DUOLMath::Quaternion xzRotForRoot = DUOLMath::Quaternion::CreateFromEulerAngle(currentEuler);

					DUOLMath::Vector3 currentScaleFrom;

					fromClip->GetTargetFrameTransform(currentIntFrameOfFrom, targetBoneIndex, currentPos, currentRotation, currentScaleFrom);

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
					// 해당 프레임의 Local transform을 긁어옵니다.
					fromClip->GetTargetFramesTransform(currentIntFrameOfFrom, currentIntFrameOfTo, targetBoneIndex, tFrom, toClip, outMat);

					// bone's local transform update.
					_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

					// bone matrices pallet update
					_boneMatrixList[targetBoneIndex] =
						_boneOffsetMatrixList[targetBoneIndex]
						* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
				}
			}
			// 둘 다 루트 모션 X
			else
			{
				// 해당 프레임의 Local transform을 긁어옵니다.
				fromClip->GetTargetFramesTransform(currentIntFrameOfFrom, currentIntFrameOfTo, targetBoneIndex, tFrom, toClip, outMat);

				// bone's local transform update.
				_boneGameObjects[targetBoneIndex]->GetTransform()->SetLocalTMWithoutDirt(outMat);

				// bone matrices pallet update
				_boneMatrixList[targetBoneIndex] =
					_boneOffsetMatrixList[targetBoneIndex]
					* _boneGameObjects[targetBoneIndex]->GetTransform()->GetWorldMatrix();
			}
		}
	}

	void Animator::OnLateUpdate(float deltaTime)
	{
		// Controller가 없으면 Animator Component는 의도대로 동작하지 않습니다.
		if (_animatorController == nullptr)
			return;

		if (_controllerContext == nullptr)
			SetAnimatorController(_animatorController);

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

	float Animator::GetSpeed() const
	{
		return _speed;
	}

	void Animator::SetSpeed(float value)
	{
		_speed = value;
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

	const DUOLCommon::tstring& Animator::GetCurrentStateName() const
	{
		return _controllerContext->_currentStateContexts[0]._currentState->GetName();
	}
}