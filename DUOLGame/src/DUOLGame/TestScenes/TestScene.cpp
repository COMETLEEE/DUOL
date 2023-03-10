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


#include "DUOLClient/ECS/Component/EnableTest.h"
#include "DUOLClient/ECS/Component/CoroutineLogTest.h"
#include "DUOLClient/ECS/Component/DebugUI.h"
#include "DUOLClient/ECS/Component/MateraiController.h"
#include "DUOLClient/ECS/Component/ModelShooter.h"
#include "DUOLClient/ECS/Component/PhysicsEventTest.h"
#include "DUOLClient/ECS/Component/MoveController.h"
#include "DUOLClient/ECS/Component/RotateAroundOrigin.h"
#include "DUOLClient/ECS/Component/ThirdPersonCamera.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/MeshCollider.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
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
	dirLightCom->SetIntensity(10.f);
	dirLight->GetTransform()->LookAt({ -0.4f, -1.f, -0.4f });
	dirLightCom->SetColor(DUOLMath::Vector3{ 1.f, 1.f, 1.f });

	// ----------- Point Light -----------xxz
	//DUOLGameEngine::GameObject* pointLight = CreateFromFBXModel(TEXT("Sphere"));
	//DUOLGameEngine::Light* pLightCom = pointLight->AddComponent<DUOLGameEngine::Light>();

	//pointLight->AddComponent<MoveController>();

	//pLightCom->SetLightType(DUOLGameEngine::LightType::Point);
	//pLightCom->SetIntensity(75.f);
	//pLightCom->SetFallOffExponential(1.f);
	//pointLight->GetTransform()->SetLocalScale({ 0.1f,  0.1f, 0.1f });
	//pLightCom->SetAttenuationRadius(10.f);
	//pLightCom->SetColor(DUOLMath::Vector3{ 1.f, 0.f, 0.0f });

	// ----------- Spot Light -----------
	DUOLGameEngine::GameObject* spotLight = CreateFromFBXModel(TEXT("Sphere"));
	DUOLGameEngine::Light* sLightCom = spotLight->AddComponent<DUOLGameEngine::Light>();

	spotLight->AddComponent<DUOLClient::MoveController>();
	spotLight->AddComponent<DUOLClient::MaterialController>();

	sLightCom->SetLightType(DUOLGameEngine::LightType::Spot);
	sLightCom->SetIntensity(10);
	spotLight->GetTransform()->LookAt({ 0.0f, -1.f, 0.0f });
	spotLight->GetTransform()->SetPosition({ 0.0f, 10.f, 0.0f });
	sLightCom->SetFallOffExponential(1.f);
	sLightCom->SetColor(DUOLMath::Vector3{ 0.f, 1.f, 0.0f });

	{
		DUOLGameEngine::GameObject* b = CreateFromFBXModel(TEXT("Model_Test_C_Pivot_INDE"));
	}

	//MATTest
	{
		//DUOLGameEngine::GameObject* mat = CreateFromFBXModel(TEXT("Sphere"));
		//DUOLGameEngine::Light* sLightCom = spotLight->AddComponent<DUOLGameEngine::Light>();
		//mat->AddComponent<MoveController>();
		//mat->AddComponent<MaterialController>();
	}

	//UI
	{

		DUOLGameEngine::GameObject* canvas = CreateEmpty();

		auto canvascomp = canvas->AddComponent<DUOLGameEngine::Canvas>();
		canvascomp->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::BackBuffer);

		DUOLGameEngine::GameObject* text2 = CreateEmtpyUI();

		auto textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
		textcomp2->SetCanvas(canvascomp->GetCanvas());
		auto& tbox2 = textcomp2->GetTextBox();
		auto rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
		rect2->SetRect({ 100, 100, 300, 150 });
		rect2->SetAnchorMin({ 0.f, 0.7f });
		rect2->SetAnchorMax({ 0.f, 0.7f });

		tbox2._fontType = DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(TEXT("Asset/Font/Unipix.ttf"));
		tbox2._fontSize = 22;
		auto debug = text2->AddComponent<DUOLClient::DebugUI>();
		debug->SetFPSUIText(textcomp2);

		text2 = CreateEmtpyUI();

		textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
		textcomp2->SetCanvas(canvascomp->GetCanvas());
		auto& tbox3 = textcomp2->GetTextBox();
		tbox3._fontType = tbox2._fontType;
		tbox3._fontSize = 16.f;
		rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
		rect2->SetRect({ 100, 100, 300, 150 });
		rect2->SetAnchorMin({ 0.f, 0.3f });
		rect2->SetAnchorMax({ 0.f, 0.3f });
		debug->SetETC(textcomp2);


		auto image1 = CreateEmtpyUI();
		auto imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
		imagecomp1->SetCanvas(canvascomp->GetCanvas());
		auto& sprite1 = imagecomp1->GetSprite();
		sprite1._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"Albedo");
		auto imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
		imagerect1->SetAnchorMin({ 0.f, 1.f });
		imagerect1->SetAnchorMax({ 0.f, 1.f });
		imagerect1->SetPivot({ 0.f, -1.f });
		imagerect1->SetRect({ 0.f, 0.f, 300.f, 150.f });

		image1 = CreateEmtpyUI();
		imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
		imagecomp1->SetCanvas(canvascomp->GetCanvas());
		auto& sprite2 = imagecomp1->GetSprite();
		sprite2._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"Normal");
		imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
		imagerect1->SetAnchorMin({ 0.f, 1.f });
		imagerect1->SetAnchorMax({ 0.f, 1.f });
		imagerect1->SetPivot({ 0.f, -1.f });
		imagerect1->SetRect({ 300.f, 0.f, 300.f, 150.f });


		image1 = CreateEmtpyUI();
		imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
		imagecomp1->SetCanvas(canvascomp->GetCanvas());
		auto& sprite3 = imagecomp1->GetSprite();
		sprite3._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"World");
		imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
		imagerect1->SetAnchorMin({ 0.f, 1.f });
		imagerect1->SetAnchorMax({ 0.f, 1.f });
		imagerect1->SetPivot({ 0.f, -1.f });
		imagerect1->SetRect({ 600.f, 0.f, 300.f, 150.f });


		image1 = CreateEmtpyUI();
		imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
		imagecomp1->SetCanvas(canvascomp->GetCanvas());
		auto& sprite4 = imagecomp1->GetSprite();
		sprite4._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"MetalRoughnessAO");
		imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
		imagerect1->SetAnchorMin({ 0.f, 1.f });
		imagerect1->SetAnchorMax({ 0.f, 1.f });
		imagerect1->SetPivot({ 0.f, -1.f });
		imagerect1->SetRect({ 900.f, 0.f, 300.f, 150.f });

	}

	__super::Awake();
}

void DUOLGame::TestScene::Update(float deltaTime)
{
	__super::Update(deltaTime);
}
