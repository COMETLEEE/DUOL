#include "DUOLClient/ECS/Component/ThirdPersonCamera.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/InputManager.h"

DUOLClient::ThirdPersonCamera::ThirdPersonCamera(DUOLGameEngine::GameObject* owner):
	MonoBehaviourBase(owner),
	_distance(-4)
	,_moveSpeed(10)
	,_axisXLimit(80)
{
}

DUOLClient::ThirdPersonCamera::~ThirdPersonCamera()
{
}

void DUOLClient::ThirdPersonCamera::SetTargetObject(DUOLGameEngine::GameObject* targetObject)
{
	_targetObject = targetObject;
}

DUOLMath::Vector3 DUOLClient::ThirdPersonCamera::CalcRotation(const DUOLMath::Vector3& lookVec)
{
	DUOLMath::Vector3 look{ lookVec.x, 0, lookVec.z};

	auto right = DUOLMath::Vector3::Up.Cross(look);
	right.Normalize();
	auto up = lookVec.Cross(right);
	up.Normalize();

	float pitch = acos(right.Dot(DUOLMath::Vector3::Right));
	float yaw = acos(up.Dot(DUOLMath::Vector3::Up));
	float roll = acos(lookVec.Dot(DUOLMath::Vector3::Forward));

	DUOLMath::Matrix rotmat;



	return { pitch, yaw, roll };
}

void DUOLClient::ThirdPersonCamera::OnAwake()
{
	DUOLGameEngine::Transform* transform = GetTransform();
	//transform->SetParent(_targetObject->GetTransform());
}

void DUOLClient::ThirdPersonCamera::OnStart()
{
}

void DUOLClient::ThirdPersonCamera::OnDestroy()
{
}

void DUOLClient::ThirdPersonCamera::OnUpdate(float deltaTime)
{
	DUOLGameEngine::Transform* targetTransform = _targetObject->GetTransform();
	DUOLGameEngine::Transform* transform = GetTransform();

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftShift))
		_moveSpeed = 20.f;
	else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyUp(DUOLGameEngine::KeyCode::LeftShift))
		_moveSpeed = 10.f;

	const DUOLMath::Vector2& currMousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

	if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Right))
	{
		// 여기서 각각 X : World Up / Y : World Right 만큼 회전시킵니다.
		const DUOLMath::Vector2 deltaMouseMove = 0.15f * (_prevMousePosition - currMousePosition);

		auto rot = transform->GetLocalRotation();

		DUOLMath::Vector3 eulerlocalRot = transform->GetLocalEulerAngle();;
		DUOLMath::Vector3 eulerRot = eulerlocalRot + DUOLMath::Vector3{ -deltaMouseMove.y, -deltaMouseMove.x , 0.f };

		eulerRot.x = std::clamp(eulerRot.x, -_axisXLimit, _axisXLimit);

		//todo : Rotation 누적이 이상하게 들어감. 각각의 성분으로 쪼개서 더해주니까 잘된다.... 왠진 모름
		//가정 1: x축의 회전이 기존 rot축의 성분들에게 모두 영향을 미쳐서.
		//auto xQuat = DUOLMath::Quaternion::CreateFromAxisAngle(DUOLMath::Vector3::Up, -deltaMouseMove.x *0.1);
		//auto yQuat = DUOLMath::Quaternion::CreateFromAxisAngle(DUOLMath::Vector3::Right, 5 * 0.01);

		//auto resultQuat = rot;

		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Z))
		//{
		//	resultQuat *= xQuat;
		//}
		//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::X))
		//{
		//	resultQuat *= yQuat;
		//}

		//DUOLMath::Vector3 eulerRot = DUOLMath::Quaternion::ConvertQuaternionToEuler(resultQuat);
		eulerRot.z = 0.f;
		//transform->SetRotation(DUOLMath::Quaternion::ConvertEulerToQuaternion(eulerRot), DUOLGameEngine::Space::Self);
		//transform->SetRotation(DUOLMath::Quaternion::ConvertEulerToQuaternion(eulerRot), DUOLGameEngine::Space::Self);
		transform->SetLocalEulerAngle(eulerRot);

		//z축 로테이션은 하지않는다.
		//x축은 로테이션 범위를 지정한다 -90 < rotx < 90
		//y축은 자유롭게.
		//로테이션값은 누적으로 계산해야할까? lookat으로 갱신해줄까? -> 누적이 맞다.

		//transform->Rotate(DUOLMath::Vector3::Up, -deltaMouseMove.x, DUOLGameEngine::Space::Self);
		////// X local Right rotate.
		//transform->Rotate(DUOLMath::Vector3::Right, -deltaMouseMove.y, DUOLGameEngine::Space::Self);

#pragma region AXIS_ANGLE_VER
		//// Y world up rotate.
		//targetTransform->Rotate(DUOLMath::Vector3::Up, -deltaMouseMove.x, DUOLGameEngine::Space::World);
		//// X local Right rotate.
		//targetTransform->Rotate(DUOLMath::Vector3::Right, -deltaMouseMove.y);
#pragma endregion
	}


	DUOLMath::Vector3 Look = transform->GetLook();
	DUOLMath::Vector3 moveLook = Look;
	DUOLMath::Vector3 up = transform->GetUp();
	DUOLMath::Vector3 right = transform->GetRight();

	moveLook.y = 0.f;
	up.y = 0.f;
	right.y = 0.f;

	//움직였을때, 카메라는 로테이션이 {기존, 0, 기존} euler. , lookvec * -distance;
	//자신의 로테이션은 그대로 오브젝트에게 적용(y축만);


	static bool _isMove = false;

	DUOLMath::Vector3 move;

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::W))
	{
		targetTransform->Translate(moveLook * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		move += moveLook;

		_targetObject->GetComponent<DUOLGameEngine::Animator>()->SetBool(TEXT("TrueIsIdle"), false);
		_isMove = true;
	}
	
	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::A))
	{
		targetTransform->Translate(-right * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		move += -right;

		_targetObject->GetComponent<DUOLGameEngine::Animator>()->SetBool(TEXT("TrueIsIdle"), false);
		_isMove = true;

	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::S))
	{
		targetTransform->Translate(-moveLook * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		move += -moveLook;

		_targetObject->GetComponent<DUOLGameEngine::Animator>()->SetBool(TEXT("TrueIsIdle"), false);
		_isMove = true;

	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::D))
	{
		targetTransform->Translate(right * deltaTime * _moveSpeed, DUOLGameEngine::Space::World);
		move += right;

		_targetObject->GetComponent<DUOLGameEngine::Animator>()->SetBool(TEXT("TrueIsIdle"), false);
		_isMove = true;
	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::Q))
	{
		targetTransform->Translate(DUOLMath::Vector3::Down * deltaTime * _moveSpeed);

		////로테이션 교체
		//DUOLMath::Vector3 camEulerlocalRot = transform->GetLocalEulerAngle();
		//DUOLMath::Vector3 objEulerlocalRot = targetTransform->GetWorldEulerAngle();;
		//objEulerlocalRot.y = DUOLMath::MathHelper::RadianToDegree(camEulerlocalRot.y);
		//targetTransform->SetLocalEulerAngle(DUOLMath::Vector3::RadianToDegree(objEulerlocalRot));
	}
	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::E))
	{
		targetTransform->Translate(DUOLMath::Vector3::Up * deltaTime * _moveSpeed);

		//로테이션 교체
		//DUOLMath::Vector3 camEulerlocalRot = transform->GetLocalEulerAngle();
		//DUOLMath::Vector3 objEulerlocalRot = targetTransform->GetWorldEulerAngle();;
		//objEulerlocalRot.y = DUOLMath::MathHelper::DegreeToRadian(camEulerlocalRot.y);
		//targetTransform->SetLocalEulerAngle(DUOLMath::Vector3::RadianToDegree(objEulerlocalRot));
	}

	if (!_isMove)
	{
		_targetObject->GetComponent<DUOLGameEngine::Animator>()->SetBool(TEXT("TrueIsIdle"), true);
	}
	else
	{
		move.Normalize();
		if(move.Length() > 0)
			targetTransform->LookAt(targetTransform->GetWorldPosition() + move);
	}

	_isMove = false;

	transform->SetPosition(targetTransform->GetTransform()->GetWorldPosition() + DUOLMath::Vector3{0.f, 2.f, 0.f} + Look * _distance, DUOLGameEngine::Space::Self);

	_prevMousePosition = currMousePosition;
}

void DUOLClient::ThirdPersonCamera::OnLateUpdate(float deltaTime)
{

}
