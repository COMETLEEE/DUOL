#include "DUOLClient/ECS/Component/DebugUI.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"
#include "DUOLGameEngine/ECS/Object/Sprite.h"

#include <rttr/registration>

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

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

		// UI
		{

			//auto canvascomp = canvas->AddComponent<DUOLGameEngine::Canvas>();
			//canvascomp->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, L"DebugUI", 1980, 1080);

			DUOLGameEngine::GameObject* text2 = scene->CreateEmtpyUI();

			auto textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
			auto tbox2 = textcomp2->GetTextBox();
			auto rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
			rect2->SetRect({ 100, 100, 300, 150 });

			DUOLMath::Vector2 anchormin = { 0.0f,0.7f };
			DUOLMath::Vector2 anchormax = { 0.4f,1.0f };

			rect2->SetAnchorMin(anchormin);
			rect2->SetAnchorMax(anchormax);

			tbox2->_fontType = DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(TEXT("Asset/Font/Unipix.ttf"));
			tbox2->_fontSize = 36;
			_etc = textcomp2;

			text2 = scene->CreateEmtpyUI();

			textcomp2 = text2->AddComponent<DUOLGameEngine::Text>();
			auto tbox3 = textcomp2->GetTextBox();
			tbox3->_fontType = tbox2->_fontType;
			tbox3->_fontSize = 36.f;
			rect2 = text2->GetComponent<DUOLGameEngine::RectTransform>();
			rect2->SetRect({ 100, 100, 300, 150 });
			anchormin.y = 0.4f ;
			anchormax.y = 0.8f;
			rect2->SetAnchorMin(anchormin);
			rect2->SetAnchorMax(anchormax);

			_fpstext = textcomp2;

			{
				auto image1 = scene->CreateEmtpyUI();
				auto imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
				imagecomp1->LoadTexture(L"Albedo");
				auto sprite1 = imagecomp1->GetSprite();
				sprite1->GetSprite()->_texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"SSAO");
				auto imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
				anchormin.x = 0.0f;
				anchormin.y = 0.8f;
				anchormax.x = 0.3f;
				anchormax.y = 1.0f;
				imagerect1->SetAnchorMin(anchormin);
				imagerect1->SetAnchorMax(anchormax);
				imagerect1->SetRect({});

				image1 = scene->CreateEmtpyUI();
				imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
				imagecomp1->LoadTexture(L"Normal");
				auto sprite2 = imagecomp1->GetSprite();
				sprite2->GetSprite()->_texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"PostNormal");
				imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
				anchormin.x = 0.3f;
				anchormin.y = 0.8f;
				anchormax.x = 0.6f;
				anchormax.y = 1.0f;
				imagerect1->SetAnchorMin(anchormin);
				imagerect1->SetAnchorMax(anchormax);
				imagerect1->SetRect({});

				anchormin.x = 0.6f;
				anchormin.y = 0.8f;
				anchormax.x = 0.9f;
				anchormax.y = 1.0f;
				image1 = scene->CreateEmtpyUI();
				imagecomp1 = image1->AddComponent<DUOLGameEngine::Image>();
				//imagecomp1->LoadTexture(L"World");
				auto sprite3 = imagecomp1->GetSprite();
				sprite3->GetSprite()->_texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(L"PostMetalRoughnessAO");
				imagerect1 = image1->GetComponent<DUOLGameEngine::RectTransform>();
				imagerect1->SetAnchorMin(anchormin);
				imagerect1->SetAnchorMax(anchormax);
				imagerect1->SetRect({});
			}

		}
	}

	std::wstring str = L"FPS : ";
	str += std::to_wstring(DUOLGameEngine::TimeManager::GetInstance()->GetFPS());
	_fpstext->SetText(str);

	//DUOLGraphicsLibrary::QueryInfo queryInfo;

	//if (DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsRenderingResultInfo(queryInfo))
	//{
	//	str = L"Drawed Surface : ";
	//	str += std::to_wstring(queryInfo._IAPrimitives);
	//	_etc->SetText(str);
	//}
}