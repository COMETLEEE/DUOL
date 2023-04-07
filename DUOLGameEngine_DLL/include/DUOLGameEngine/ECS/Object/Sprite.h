#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

namespace DUOLGraphicsLibrary
{
	class sprite;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API Sprite final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<Sprite>
	{
	public:
		Sprite(const DUOLCommon::tstring & name = TEXT("Sprite"));

		virtual ~Sprite() override;

	private:
		DUOLGraphicsLibrary::Sprite* _sprite;

	public:
		inline 	DUOLGraphicsLibrary::Sprite* GetSprite() const { return _sprite; }

		inline void SetSprite(DUOLGraphicsLibrary::Sprite* sprite) { _sprite = sprite; }

		inline void SetSpriteName(const DUOLCommon::tstring& name);

#pragma region FRIEND_CLASS
		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};

}
