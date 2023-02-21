#pragma once
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Text.h"

namespace DUOLGame
{
	class DebugUI  final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		DebugUI(DUOLGameEngine::GameObject* owner);

		virtual ~DebugUI() override;

	private:
		DUOLGameEngine::Text* _fpstext;

		DUOLGameEngine::Text* _etc;

	public:
		DUOLGameEngine::Image* _debugImage1;

		DUOLGameEngine::Image* __debugImage2;

		DUOLGameEngine::Image* __debugImage3;

		DUOLGameEngine::Image* __debugImage4;


	public:
		void SetFPSUIText(DUOLGameEngine::Text* uicomp){ _fpstext = uicomp; }

		void SetETC(DUOLGameEngine::Text* uicomp){ _etc = uicomp; }

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
