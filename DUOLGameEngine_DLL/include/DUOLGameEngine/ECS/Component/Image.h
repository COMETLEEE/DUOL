#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include <string>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "rttr/registration_friend.h"


namespace DUOLGameEngine
{
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

		void Initialize();

	private:
		DUOLGraphicsLibrary::Sprite* _sprite;

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
		DUOLGraphicsLibrary::Sprite* GetSprite() {return _sprite;}

		DUOLMath::Vector3& GetRGB() { return _rgb; }

		// �̰� ����� ���޾ƿ´�. 
		std::string GetSpriteName() { return _sprite->_texture->GetTextureName(); }

		bool GetRaycastTarget() { return _raycastTarget; }

		const DUOLCommon::tstring& GetSpritePathName() { return _spriteName; }

		/**
		 *  Setter
		 */
		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas) {_canvas = canvas;}

		void SetRGB(DUOLMath::Vector3& rgb);

		void SetSpriteName(std::string path);

		void SetRaycastTarget(bool israycast);


		void OnResize();

		void LoadTexture(const DUOLCommon::tstring& textureID);

		DUOLCommon::UUID ImageID() { return this->GetGameObject()->GetUUID(); }


	private:

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}
