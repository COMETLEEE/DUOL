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
		Image();

		Image(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Image"));

		virtual ~Image();

		void OnUpdate(float deltaTime) override;

		void Initialize(DUOLGameEngine::GameObject* owner);

	private:
		DUOLGameEngine::Sprite* _sprite;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		unsigned int _orderInLayer;

		DUOLMath::Vector3 _rgb;

		RectTransform* _canvasRectTransform;

		RectTransform* _rectTransform;

		bool _raycastTarget;

		DUOLCommon::EventListenerID _updateID;

		DUOLCommon::tstring _spriteName;
	public:
		/**
		 * Getter
		 */
		DUOLGameEngine::Sprite* GetSprite() const {return _sprite;}

		void SetSprite(DUOLGameEngine::Sprite* sprite) { _sprite = sprite; }

		DUOLMath::Vector3& GetRGB() { return _rgb; }

		// 이거 제대로 못받아온다. 
		std::string GetSpriteName() { return _sprite->GetSprite()->_texture->GetTextureName(); }

		bool GetRaycastTarget() { return _raycastTarget; }

		const DUOLCommon::tstring& GetSpritePathName() { return _spriteName; }

		DUOLGraphicsLibrary::ICanvas* GetCanvas() { return _canvas; }

		int GetLayer() { return _orderInLayer; }

		RectTransform* GetImageRectTransform() { return _rectTransform; }

		/**
		 *  Setter
		 */
		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas) {_canvas = canvas;}

		void SetRGB(DUOLMath::Vector3& rgb);

		void SetSpriteName(std::string path);

		void SetRaycastTarget(bool israycast);

		void SetLayer(int layer);

		void OnResize();

		void LoadTexture(const DUOLCommon::tstring& textureID);

		DUOLCommon::UUID ImageID() { return this->GetGameObject()->GetUUID(); }

		void LoadScene();

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}
