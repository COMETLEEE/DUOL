#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/UI/OnClick.h"

#include "rttr/registration_friend.h"

namespace DUOLGameEngine
{
	class Text;
	class Sprite;
	class Image;
	class RectTransform;
	class Texture;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Button Class
	 */
	class DUOL_GAMEENGINE_API Button  final : public DUOLGameEngine::BehaviourBase
	{
	public:
		Button();

		Button(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Button"));

		virtual ~Button();

		virtual void OnAwake() override;

		void OnUpdate(float deltaTime) override;

		void Initialize();

	private:
		DUOLGraphicsLibrary::ICanvas* _canvas;

		DUOLGameEngine::Image* _image;

		DUOLGameEngine::Text* _text;

		DUOLMath::Vector3 _rgb;

		RectTransform* _canvasRectTransform;

		RectTransform* _rectTransform;

		DUOLCommon::EventListenerID _updateID;

		DUOLCommon::tstring _downSpriteName;

		DUOLCommon::tstring _spriteName;

		DUOLCommon::tstring _clickSpriteName;

		bool _isMouseClick;

		bool _isScrollButton;

		//---------------Evnet관련 변수들----------------

		DUOLCommon::tstring _loadSceneName;

		std::vector<DUOLGameEngine::OnClick*> _onClicks;

		DUOLMath::Vector2 _downMousePos;

		float _scrollGauge;

	public:
		/**
		* Getter
		 */
		 //DUOLGraphicsLibrary::Sprite* GetSprite() { return _downSprite; }
		 DUOLMath::Vector3& GetRGB() { return _rgb; }

		 RectTransform* GetImageRectTransform() const { return _rectTransform; }

		 DUOLCommon::tstring& GetLoadSceneName() { return _loadSceneName; }

		 std::vector<DUOLGameEngine::OnClick*>& GetOnClick() { return _onClicks; }

		 DUOLCommon::tstring& GetDownSpriteName() { return _downSpriteName; }

		 DUOLCommon::tstring& GetclickSpriteName() { return _clickSpriteName; }

		 /**
		  *  Setter
		 */
		 void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas) { _canvas = canvas; }

		 void SetRGB(DUOLMath::Vector3& rgb);

		 void SetDownSprite(const DUOLCommon::tstring& textureID);

		 void SetLoadSceneName(DUOLCommon::tstring& scenename);

		 void SetLoadSceneImage(DUOLGameEngine::Image* image);

		 void SetLoadSceneText(DUOLGameEngine::Text* text);

		 bool SetImage();

		 void LoadTexture(const DUOLCommon::tstring& textureID);

		 void CreateOnClick();

		 void DeleteOnClick();

		 void OnClicks();

		 bool SetText();

		 // Button Event
		 // 어디로 함수를 뺄지 고민중
		void LoadScene(std::string filename);

		 void EndGame();

		 void MainUPUI(std::string filename);

		 void MainDownUI(std::string filename);

		 void Resolution(std::string inputtext);

		 void Scrolling();

 #pragma region FRIEND_CLASS
		 friend class GameObject;

		 RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		 RTTR_REGISTRATION_FRIEND
 #pragma endregion

	};
}
