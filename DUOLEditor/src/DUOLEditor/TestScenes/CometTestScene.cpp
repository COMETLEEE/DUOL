#include "DUOLEditor/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"


#include "DUOLEditor/TestScripts/EnableTest.h"
#include "DUOLEditor/TestScripts/CoroutineLogTest.h"
#include "DUOLEditor/TestScripts/ModelShooter.h"
#include "DUOLEditor/TestScripts/PhysicsEventTest.h"
#include "DUOLEditor/TestScripts/MoveController.h"

namespace DUOLEditor
{
	CometTestScene::CometTestScene() :
		Scene(TEXT("CometTestScene"))
	{

	}

	CometTestScene::~CometTestScene()
	{

	}

	void CometTestScene::Awake()
	{
		// ----------- Main Camera -----------
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

		//// ----------- Spot Light -----------
		//DUOLGameEngine::GameObject* spotLight = CreateEmpty();

		//spotLight->GetTransform()->SetParent(mainCamObject->GetTransform());

		//DUOLGameEngine::Light* spotLightCom = spotLight->AddComponent<DUOLGameEngine::Light>();

		//spotLightCom->SetLightType(DUOLGameEngine::LightType::Spot);

		//spotLightCom->SetIntensity(8.f);

		//spotLightCom->SetColor(DUOLMath::Vector3{0.f, 0.2f, 1.f});

		// ----------- Coroutine Logger -----------
		DUOLGameEngine::GameObject* testObject = CreateEmpty();

		testObject->AddComponent<DUOLEditor::CoroutineLogTest>();

		// ----------- Directional Light -----------
		DUOLGameEngine::GameObject* dirLight = CreateEmpty();

		DUOLGameEngine::Light* dirLightCom = dirLight->AddComponent<DUOLGameEngine::Light>();

		dirLightCom->SetLightType(DUOLGameEngine::LightType::Direction);

		dirLightCom->SetIntensity(10.f);

		dirLightCom->SetColor(DUOLMath::Vector3{ 1.f, 0.f, 0.5f });

		// dirLight->AddComponent<DUOLGame::RotateAroundOrigin>()->SetRotateSpeed(60.f);

		// ----------- Point Light -----------
		DUOLGameEngine::GameObject* pointLight = CreateFromFBXModel(TEXT("ButtonFloor"));

		pointLight->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3{ 5.f, 3.f, 0.f });

		DUOLGameEngine::Light* pointLightCom = pointLight->AddComponent<DUOLGameEngine::Light>();

		pointLightCom->SetLightType(DUOLGameEngine::LightType::Point);

		pointLightCom->SetIntensity(20.f);

		pointLightCom->SetColor(DUOLMath::Vector3{ 0.2f, 1.f, 0.2f });

		pointLightCom->SetRange(15.f);

		// pointLight->AddComponent<DUOLGame::RotateAroundOrigin>()->SetRotateSpeed(90.f);

#pragma region JOY
		// Joy
		{
			DUOLGameEngine::GameObject* joyObject = CreateFromFBXModel(TEXT("Drunk Idle"));

			// 테스트용 캐싱
			_joyObject = joyObject;

			joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(5.f, 3.f, 0.f));

			joyObject->GetComponent<DUOLGameEngine::Animator>()
				->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("TestAnimCon")));

			_moveCon = joyObject->AddComponent<DUOLEditor::MoveController>();

			auto children = joyObject->GetTransform()->GetChildren();
		}
#pragma endregion

		__super::Awake();
	}

	void CometTestScene::Update(float deltaTime)
	{
		static bool _isEnable = true;

		static float _ableTime = 0.f;

		_ableTime += deltaTime;

		if (_ableTime >= 3.f && _isEnable == true)
		{
			// _joyObject->SetIsActive(false);

			// _moveCon->SetIsEnabled(false);

			_isEnable = false;

			_ableTime = 0.f;
		}
		else if (_ableTime >= 3.f && _isEnable == false)
		{
			// _joyObject->SetIsActive(true);

			// _moveCon->SetIsEnabled(true);

			_isEnable = true;

			_ableTime = 0.f;
		}

		__super::Update(deltaTime);
	}
}