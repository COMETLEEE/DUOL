#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API Text final : public DUOLGameEngine::BehaviourBase
	{
	public:
		Text(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Text"));

		virtual ~Text();
	public:
		void OnUpdate(float deltaTime) override;

	private:
		DUOLGraphicsLibrary::TextBox _textBox;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		unsigned int _orderInLayer;

	public:
		DUOLGraphicsLibrary::TextBox& GetTextBox()
		{
			return _textBox;
		}

		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas)
		{
			_canvas = canvas;
		}

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)
	};
}
