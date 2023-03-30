#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"


namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API OnClickArgument final : public DUOLGameEngine::BehaviourBase
	{
	public:
		OnClickArgument();

		OnClickArgument(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClickArgument"));

		virtual ~OnClickArgument();

		void OnUpdate(float deltaTime) override;

		void Initialize();

	public:
		std::function<void(void)> _onClickVoidVoid;

	};
}
