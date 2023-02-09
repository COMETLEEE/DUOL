#include "DUOLGame/TestScenes/TestScene.h"

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


#include "DUOLGame/TestScripts/EnableTest.h"
#include "DUOLGame/TestScripts/CoroutineLogTest.h"
#include "DUOLGame/TestScripts/ModelShooter.h"
#include "DUOLGame/TestScripts/PhysicsEventTest.h"
#include "DUOLGame/TestScripts/MoveController.h"
#include "DUOLGame/TestScripts/RotateAroundOrigin.h"
#include "DUOLGame/TestScripts/ThirdPersonCamera.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/Text.h"

namespace DUOLGame
{
	class ThirdPersonCamera;
}

DUOLGame::TestScene::TestScene() :
	Scene(TEXT("TestScene"))
{
}

DUOLGame::TestScene::~TestScene()
{
}

void DUOLGame::TestScene::Awake()
{
	// ----------- Main Camera -----------
	DUOLGameEngine::GameObject* mainCamObject = CreateEmpty();

	mainCamObject->GetComponent<DUOLGameEngine::Transform>()->Translate(DUOLMath::Vector3(0.f, 0.f, 0.f));

	mainCamObject->AddComponent<DUOLGameEngine::Camera>();

	auto cameraComp = mainCamObject->AddComponent<DUOLGameEngine::TPFController>();

	// ----------- Directional Light -----------
	DUOLGameEngine::GameObject* dirLight = CreateEmpty();

	DUOLGameEngine::Light* dirLightCom = dirLight->AddComponent<DUOLGameEngine::Light>();

	dirLightCom->SetLightType(DUOLGameEngine::LightType::Directional);
	dirLightCom->SetIntensity(5.f);
	dirLight->GetTransform()->LookAt({ -0.4f, -1.f, -0.4f });

	dirLightCom->SetColor(DUOLMath::Vector3{ 1.f, 1.f, 1.0f });

	{
		DUOLGameEngine::GameObject* b = CreateFromFBXModel(TEXT("B_Test"));

		//for(auto& child : b->GetTransform()->GetChildren())
		//{
		//	auto meshfilter = child->GetGameObject()->GetComponent<DUOLGameEngine::MeshFilter>();
		//	if(meshfilter != nullptr)
		//	{
		//		child->GetGameObject()->AddComponent<DUOLGameEngine::MeshCollider>()->SetMeshBuffer(meshfilter);
		//	}
		//}
	}

	{
		DUOLGameEngine::GameObject* charac = CreateFromFBXModel(TEXT("stomtrupper"));
		charac->AddComponent<MoveController>();
		//sphere->GetComponent<DUOLGameEngine::Animator>()
		//	->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("TestAnimCon")));
		//auto collider = sphere->AddComponent<DUOLGameEngine::CapsuleCollider>();
		////sphere->AddComponent<DUOLGameEngine::Rigidbody>();

		charac->GetTransform()->SetPosition({ 2.f, 5.f, -10.f });
		charac->GetTransform()->SetLocalScale({ 10.f, 10.f, 10.f });
		////sphere->GetTransform()->SetRotation({ -0.7071068, 0, 0, 0.7071068 });
		//auto cameraComp = mainCamObject->AddComponent<ThirdPersonCamera>();
		//cameraComp->SetTargetObject(sphere);

		DUOLGameEngine::GameObject* sphere = CreateFromFBXModel(TEXT("Sphere"));
		sphere->GetTransform()->SetPosition({ 2.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere2"));
		sphere->GetTransform()->SetPosition({ 0.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere3"));
		sphere->GetTransform()->SetPosition({ -2.f, 0.f, -5.f });

		sphere = CreateFromFBXModel(TEXT("Sphere4"));
		sphere->GetTransform()->SetPosition({ -4.f, 0.f, -5.f });

	}

	{
		DUOLGameEngine::GameObject* canvas = CreateEmpty();

		auto canvascomp = canvas->AddComponent<DUOLGameEngine::Canvas>();
		canvascomp->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::BackBuffer);

		DUOLGameEngine::GameObject* text = CreateEmtpyUI();

		auto textcomp = text->AddComponent<DUOLGameEngine::Text>();
		textcomp->SetCanvas(canvascomp->GetCanvas());
		auto& tbox = textcomp->GetTextBox();

		tbox._fontType = DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(TEXT("Asset/Font/Unipix.ttf"));
		tbox._text = L"yes";

		DUOLGameEngine::GameObject* text2 = CreateEmtpyUI();

		auto textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
		textcomp2->SetCanvas(canvascomp->GetCanvas());
		auto& tbox2 = textcomp2->GetTextBox();
		auto rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
		rect2->SetRect({100, 100, 300, 150});

		tbox2._fontType = DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(TEXT("Asset/Font/PyeongChangPeace-Light.ttf"));
		tbox2._text = L"평창폰트에용 ABCDEFG";

		auto imageComp2 = text2->AddComponent<DUOLGameEngine::Image>();
		imageComp2->SetCanvas(canvascomp->GetCanvas());
		auto& sprite = imageComp2->GetTextBox();
		
		sprite._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(TEXT("GameView"));
	}

	{
		DUOLGameEngine::GameObject* plain = CreateEmpty();

		plain->AddComponent<DUOLGameEngine::BoxCollider>();

	}

	auto testfx = CreateFromParticleData(_T("test.dfx"));

	__super::Awake();
}

void DUOLGame::TestScene::Update(float deltaTime)
{
	__super::Update(deltaTime);
}
