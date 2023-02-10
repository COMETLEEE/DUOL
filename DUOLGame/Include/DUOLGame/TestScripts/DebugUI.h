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

	public:
		void SetFPSUIText(DUOLGameEngine::Text* uicomp){ _fpstext = uicomp; }

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
