#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Text.h"

namespace DUOLGame
{
	class DebugUI  final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		DebugUI(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~DebugUI() override;

	private:
		DUOLGameEngine::Text* _fpstext;

		DUOLGameEngine::Text* _etc;

	public:
		void SetFPSUIText(DUOLGameEngine::Text* uicomp){ _fpstext = uicomp; }

		void SetETC(DUOLGameEngine::Text* uicomp){ _etc = uicomp; }

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
