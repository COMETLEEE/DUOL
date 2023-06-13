#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "rttr/registration_friend.h"

namespace DUOLGraphicsLibrary
{
	struct TextBox;
	class ICanvas;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API Text final : public DUOLGameEngine::BehaviourBase
	{
	public:
	//	Text();

		Text(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Text"));

		virtual ~Text();

		void OnUpdate(float deltaTime) override;

	private:
		void Initialize(DUOLGameEngine::GameObject* owner);

	private:
		DUOLGraphicsLibrary::TextBox* _textBox;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		unsigned int _orderInLayer;

		DUOLCommon::tstring _inputText;

		DUOLCommon::tstring _currFontName;

		DUOLCommon::EventListenerID _updateID;

		RectTransform* _rectTransform;

		UINT64 _preFontSize;

	public:
		// Getter
		DUOLGraphicsLibrary::TextBox* GetTextBox() { return _textBox; }

		int GetLayer() { return _orderInLayer; }

		DUOLMath::Vector4& GetColor() { return _textBox->_color; }

		int GetFontSize() { return _textBox->_fontSize; }

		DUOLGraphicsLibrary::WeightOption& GetWeightOption() { return _textBox->_weightOption; }

		DUOLGraphicsLibrary::StyleOption& GetStyleOption() { return _textBox->_styleOption; }

		DUOLCommon::tstring& GetFontType() { return _currFontName; }

		DUOLCommon::tstring& GetText()
		{
			return _inputText;
		}

		DUOLGraphicsLibrary::Rect GetTextRectTransform() { return _textBox->_rect; }

		// Setter
		void SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas) { _canvas = canvas; }

		void SetLayer(int layer);

		void SetColor(DUOLMath::Vector4& color) { _textBox->_color = color; }

		void SetFontSize(int size);

		void SetWeightOption(DUOLGraphicsLibrary::WeightOption& weightoption) { _textBox->_weightOption = weightoption; }

		void SetStyleOption(DUOLGraphicsLibrary::StyleOption& styleoption) { _textBox->_styleOption = styleoption; }

		void SetFontType(const DUOLCommon::tstring& fontname);

		void SetText(const DUOLCommon::tstring& inputtext);

		void OnResize();

		void LoadScene();

		// Event
		void TextChange();

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};
}
