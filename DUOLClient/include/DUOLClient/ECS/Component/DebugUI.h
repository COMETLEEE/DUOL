#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Text.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API DebugUI  final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		DebugUI();

		DebugUI(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("DEBUGUI"));

		virtual ~DebugUI() override;

	private:
		DUOLGameEngine::Text* _fpstext;

		DUOLGameEngine::Text* _etc;

		bool _init = false;
	public:
		DUOLGameEngine::Image* _debugImage1;

		DUOLGameEngine::Image* __debugImage2;

		DUOLGameEngine::Image* __debugImage3;

		DUOLGameEngine::Image* __debugImage4;


	public:
		void SetFPSUIText(DUOLGameEngine::Text* uicomp) { _fpstext = uicomp; }

		void SetETC(DUOLGameEngine::Text* uicomp) { _etc = uicomp; }

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}
