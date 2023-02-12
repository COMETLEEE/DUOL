#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API Image final : public DUOLGameEngine::BehaviourBase
	{
	public:
		Image(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Text"));

		virtual ~Image();

		void OnUpdate(float deltaTime) override;

	private:
		DUOLGraphicsLibrary::Sprite _sprite;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		unsigned int _orderInLayer;

	public:
		DUOLGraphicsLibrary::Sprite& GetTextBox()
		{
			return _sprite;
		}

		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas)
		{
			_canvas = canvas;
		}

	};
}
