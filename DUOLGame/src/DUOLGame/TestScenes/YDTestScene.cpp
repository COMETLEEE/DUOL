#include "DUOLGame/TestScenes/YDTestScene.h"

#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"


#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGame/TestScripts/ModelShooter.h"
#include "DUOLGame/TestScripts/PhysicsEventTest.h"
#include "DUOLGame/TestScripts/MoveController.h"
#include "DUOLGame/TestScripts/RotateAroundOrigin.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

namespace DUOLGame
{
	YDTestScene::YDTestScene() :
		Scene(TEXT("YDTestScene"))
	{

	}

	YDTestScene::~YDTestScene()
	{
		
	}

	void YDTestScene::Awake()
	{
		// ----------- Main Camera -----------
		DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

		mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 3.f, -10.f));

		mainCamObject->AddComponent<DUOLGameEngine::Camera>();

		mainCamObject->AddComponent<DUOLGameEngine::TPFController>();
		// ----------- Directional Light -----------
		DUOLGameEngine::GameObject* dirLight = CreateEmpty();

		DUOLGameEngine::Light* dirLightCom = dirLight->AddComponent<DUOLGameEngine::Light>();

		dirLightCom->SetLightType(DUOLGameEngine::LightType::Directional);

		dirLightCom->SetIntensity(10.f);

		dirLightCom->SetColor(DUOLMath::Vector3{ 1.f, 1.f, 0.5f });


		// Joy
		DUOLGameEngine::GameObject* joyObject = CreateFromFBXModel(TEXT("Capoeira"));

		joyObject->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(5.f, 3.f, 0.f));

		/*joyObject->GetComponent<DUOLGameEngine::Animator>()
			->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("Idle")));*/

		joyObject->AddComponent<DUOLGame::MoveController>();

		// Joy
	/*	DUOLGameEngine::GameObject* Button = CreateFromFBXModel(TEXT("ButtonFloor"));

		Button->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(13.f, 5.f, 0.f));*/
		//Button->GetComponent<DUOLGameEngine::Transform>()->SetLocalScale(DUOLMath::Vector3(0.05f, 0.05f, 0.05f));


//#pragma region B_Test_All_Material
//		DUOLGameEngine::GameObject* object1 = CreateFromFBXModel(TEXT("B_Test"));
//
//		object1->GetComponent<DUOLGameEngine::Transform>()->SetPosition(DUOLMath::Vector3(10.f, 5.f, 0.f));
//		
//#pragma endregion



		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}
