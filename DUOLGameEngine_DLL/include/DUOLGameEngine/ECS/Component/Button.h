#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"


#include "DUOLGameEngine/ECS/GameObject.h"
#include "rttr/registration_friend.h"

namespace DUOLGameEngine
{

	enum class OnClickEventFunctionMode
	{
		VoidVoid
		, VoidBool
		, VoidInt
		,
	};
}

namespace DUOLGameEngine
{
	class Sprite;
	class Image;
	class RectTransform;
	class Texture;
	//class OnClick;
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

		void OnUpdate(float deltaTime) override;

		void Initialize();

	private:
		DUOLGameEngine::Sprite* _downSprite;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		DUOLGameEngine::Image* _image;

		DUOLMath::Vector3 _rgb;

		RectTransform* _canvasRectTransform;

		RectTransform* _rectTransform;

		DUOLCommon::EventListenerID _updateID;

		DUOLCommon::tstring _downSpriteName;

		DUOLCommon::tstring _spriteName;

		DUOLCommon::tstring _clickSpriteName;

		bool _isMouseClick;

		//---------------Evnet관련 변수들----------------

		DUOLCommon::tstring _loadSceneName;

		//GameObject* _onClickGameObject;

		//std::function<void(void)> _onClickVoidVoid;


		//OnClickEventFunctionMode _currentMode;

	//	DUOLGameEngine::OnClick _onClick;


	public:
		/**
		* Getter
		 */
		 //DUOLGraphicsLibrary::Sprite* GetSprite() { return _downSprite; }
		 DUOLMath::Vector3& GetRGB() { return _rgb; }

		 DUOLGameEngine::Sprite* GetSprite() const { return _downSprite; }

		 RectTransform* GetImageRectTransform() const { return _rectTransform; }

		 DUOLCommon::tstring& GetLoadSceneName() { return _loadSceneName; }

		 /**
		  *  Setter
		 */
		 void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas) { _canvas = canvas; }

		 void SetRGB(DUOLMath::Vector3& rgb);

		 void SetDownSprite(const DUOLCommon::tstring& textureID);

		 void SetLoadSceneName(DUOLCommon::tstring& scenename);

		 void LoadTexture(const DUOLCommon::tstring& textureID);

		 void LoadScene(DUOLGameEngine::Image* image);


 #pragma region FRIEND_CLASS
		 friend class GameObject;

		 RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		 RTTR_REGISTRATION_FRIEND
 #pragma endregion

	};
}
