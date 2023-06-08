#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include <string>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Object/Sprite.h"
#include "rttr/registration_friend.h"


namespace DUOLGameEngine
{
	class Sprite;
	class Canvas;
	class RectTransform;
	class Texture;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API Image final : public DUOLGameEngine::BehaviourBase
	{
	public:
		Image(DUOLGameEngine::GameObject* owner=nullptr, const DUOLCommon::tstring& name = TEXT("Image"));

		virtual ~Image();

		void OnUpdate(float deltaTime) override;

	private:
		void Initialize(DUOLGameEngine::GameObject* owner);

	private:
		DUOLGameEngine::Sprite* _sprite;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		unsigned int _orderInLayer;

		RectTransform* _canvasRectTransform;

		RectTransform* _rectTransform;

		bool _raycastTarget;

		DUOLCommon::EventListenerID _updateID;

		DUOLCommon::tstring _spriteName;

		bool _isFadeInit;

	public:
		/**
		 * Getter
		 */
		DUOLGameEngine::Sprite* GetSprite() const { return _sprite; }

		void SetSprite(DUOLGameEngine::Sprite* sprite) { _sprite = sprite; }

		// 이거 제대로 못받아온다.
		std::string GetSpriteName() { return _sprite->GetSprite()->_texture->GetTextureName(); }

		bool GetRaycastTarget() { return _raycastTarget; }

		DUOLCommon::tstring& GetSpritePathName() { return _spriteName; }

		DUOLGraphicsLibrary::ICanvas* GetCanvas() { return _canvas; }

		int GetLayer() { return _orderInLayer; }

		RectTransform* GetImageRectTransform() { return _rectTransform; }

		DUOLMath::Vector4 GetColor();

		void SetColor(DUOLMath::Vector4 vector);

		/**
		 *  Setter
		 */
		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas);

		void SetSpriteName(std::string path);

		// rttr이 오버로딩을 인식하는지 몰라서 일단 이렇게 짬
		void SetSpritePathName(DUOLCommon::tstring& path) { _spriteName = path; }

		void SetRaycastTarget(bool israycast);

		void SetLayer(int layer);

		void SetSourceLeftTop(DUOLMath::Vector2 pivot);

		void SetSourceRightBottom(DUOLMath::Vector2 pivot);

		DUOLMath::Vector2 GetSourceLeftTop();

		DUOLMath::Vector2 GetSourceRightBottom();

		void OnResize();

		void LoadTexture(const DUOLCommon::tstring& textureID);

		DUOLCommon::UUID ImageID() { return this->GetGameObject()->GetUUID(); }

		void LoadScene();

		void ImageRender();

		bool _isFadeImage;

		float _fadeTime;
#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}
