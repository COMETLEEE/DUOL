#include "DUOLClient/ECS/Component/DebugUI.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::DebugUI>("DebugUI")
		.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}


DUOLClient::DebugUI::DebugUI() :
	MonoBehaviourBase(nullptr, L"")
{
}

DUOLClient::DebugUI::DebugUI(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name)
{
}

DUOLClient::DebugUI::~DebugUI()
{
}

void DUOLClient::DebugUI::OnStart()
{
}

void DUOLClient::DebugUI::OnUpdate(float deltaTime)
{
	if (!_init)
	{
		_init = true;

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		//UI
		{
			DUOLGameEngine::GameObject* canvas = scene->CreateEmpty();

			auto canvascomp = canvas->AddComponent<DUOLGameEngine::Canvas>();
			canvascomp->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, L"DebugUI", 1980, 1080);

			DUOLGameEngine::GameObject* text2 = scene->CreateEmtpyUI();

			auto textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
			textcomp2->SetCanvas(canvascomp->GetCanvas());
			auto& tbox2 = textcomp2->GetTextBox();
			auto rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
			rect2->SetRect({ 100, 100, 300, 150 });
			rect2->SetAnchorMin({ 0.f, 0.7f });
			rect2->SetAnchorMax({ 0.f, 0.7f });

			tbox2._fontType = DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(TEXT("Asset/Font/Unipix.ttf"));
			tbox2._fontSize = 22;
			_etc = textcomp2;

			text2 = scene->CreateEmtpyUI();

			textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
			textcomp2->SetCanvas(canvascomp->GetCanvas());
			auto& tbox3 = textcomp2->GetTextBox();
			tbox3._fontType = tbox2._fontType;
			tbox3._fontSize = 16.f;
			rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
			rect2->SetRect({ 100, 100, 300, 150 });
			rect2->SetAnchorMin({ 0.f, 0.3f });
			rect2->SetAnchorMax({ 0.f, 0.3f });

			_fpstext = textcomp2;

			auto image1 = scene->CreateEmtpyUI();
			auto imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
			imagecomp1->SetCanvas(canvascomp->GetCanvas());
			auto& sprite1 = imagecomp1->GetSprite();
			sprite1._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"Albedo");
			auto imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
			imagerect1->SetAnchorMin({ 0.f, 1.f });
			imagerect1->SetAnchorMax({ 0.f, 1.f });
			imagerect1->SetPivot({ 0.f, -1.f });
			imagerect1->SetRect({ 0.f, 0.f, 300.f, 150.f });

			image1 = scene->CreateEmtpyUI();
			imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
			imagecomp1->SetCanvas(canvascomp->GetCanvas());
			auto& sprite2 = imagecomp1->GetSprite();
			sprite2._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"Normal");
			imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
			imagerect1->SetAnchorMin({ 0.f, 1.f });
			imagerect1->SetAnchorMax({ 0.f, 1.f });
			imagerect1->SetPivot({ 0.f, -1.f });
			imagerect1->SetRect({ 300.f, 0.f, 300.f, 150.f });


			image1 = scene->CreateEmtpyUI();
			imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
			imagecomp1->SetCanvas(canvascomp->GetCanvas());
			auto& sprite3 = imagecomp1->GetSprite();
			sprite3._texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"World");
			imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
			imagerect1->SetAnchorMin({ 0.f, 1.f });
			imagerect1->SetAnchorMax({ 0.f, 1.f });
			imagerect1->SetPivot({ 0.f, -1.f });
			imagerect1->SetRect({ 600.f, 0.f, 300.f, 150.f });


			image1 = scene->CreateEmtpyUI();
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
	}


	std::wstring str = L"FPS : ";
	str += std::to_wstring(DUOLGameEngine::TimeManager::GetInstance()->GetFPS());
	_fpstext->GetTextBox()._text = str;

	DUOLGraphicsLibrary::QueryInfo queryInfo;

	if (DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsRenderingResultInfo(queryInfo))
	{
		str = L"Drawed Surface : ";
		str += std::to_wstring(queryInfo._IAPrimitives);
		_etc->GetTextBox()._text = str;
	}
}
