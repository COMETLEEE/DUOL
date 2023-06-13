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
		UIDataManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("GameManager"));

		virtual ~UIDataManager() override;

		static DUOLClient::UIDataManager* _instance;

	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		void LoadUIDataJson();

		void ChangeScene();

		void PlayerHPUI(float hp);

		static DUOLClient::UIDataManager* GetInstance();

		void InitializeMiddle();

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

	private:
		// make_pair(scene,{typevalue ,name} : List)
		//std::unordered_map<std::string, std::vector<UIDataType,std::string>> _uiDataList;

		std::unordered_map<std::string, DUOLGameEngine::Image*> _imageUIList;

		std::unordered_map<std::string, DUOLGameEngine::Text*> _textUIList;

		std::unordered_map<std::string, DUOLGameEngine::Button*> _buttonUIList;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};
	
}
