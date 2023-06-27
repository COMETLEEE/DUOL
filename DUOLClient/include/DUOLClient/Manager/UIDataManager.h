#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include <vector>

namespace DUOLGameEngine
{
	class Button;
	class Text;
	class Image;
}

namespace DUOLClient
{
	class UIDataManager : public DUOLGameEngine::MonoBehaviourBase
	{
		DELETE_COPY_MOVE(DUOLClient::UIDataManager)

	public:
		UIDataManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("UIDataManager"));

		virtual ~UIDataManager() override;

	private:
		static DUOLClient::UIDataManager* _instance;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void ChangeScene();

		void SetPlayerHPUI(float hp);

		void SetPlayerHPMaxUI(float hp);

		void SetPlayerOverDriveUI(float overdrive);

		void SetPlayerOverDriveMaxUI(float overdrive);

		void SetUltimateUI(float ultimate);

		void SetUltimateMaxUI(float ultimate);

		void KOUIActive(bool value);

		void PlusKOCount();

		static DUOLClient::UIDataManager* GetInstance();
		
		void InitializeMiddle(DUOLGameEngine::GameObject* player);

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

	private:
		static int _koCount;

		float _comboTime;

	private:
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
	
}
