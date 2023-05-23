#define NOMINMAX

#include "DUOLGameEngine/ECS/Component/Text.h"
#include <rttr/registration>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLMath/DUOLMath.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGraphicsLibrary::WeightOption>("WeightOption")
	(
		value("BOLD", DUOLGraphicsLibrary::WeightOption::BOLD)
		,value("NORMAL", DUOLGraphicsLibrary::WeightOption::NORMAL)
	);
	rttr::registration::enumeration<DUOLGraphicsLibrary::StyleOption>("StyleOption")
	(
	value("NORMAL", DUOLGraphicsLibrary::StyleOption::NORMAL)
	, value("ITALIC", DUOLGraphicsLibrary::StyleOption::ITALIC)
	, value("OBLIQUE", DUOLGraphicsLibrary::StyleOption::OBLIQUE)
	);
	rttr::registration::class_<DUOLGameEngine::Text>("Text")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Text", &DUOLGameEngine::Text::_inputText)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::TextEvent)
	)
	.property("Color", &DUOLGameEngine::Text::GetColor, &DUOLGameEngine::Text::SetColor)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float4)
	)
	.property("WeightOption", &DUOLGameEngine::Text::GetWeightOption, &DUOLGameEngine::Text::SetWeightOption)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
	)
	.property("StyleOption", &DUOLGameEngine::Text::GetStyleOption, &DUOLGameEngine::Text::SetStyleOption)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
	)
	.property("Font Size", &DUOLGameEngine::Text::GetFontSize, &DUOLGameEngine::Text::SetFontSize)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("Font Type", &DUOLGameEngine::Text::_currFontName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	)
	.property("Order In Layer", &DUOLGameEngine::Text::GetLayer,&DUOLGameEngine::Text::SetLayer)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.method("TextChange", &DUOLGameEngine::Text::TextChange);

}

namespace DUOLGameEngine
{
	//Text::Text() :
	//	BehaviourBase(nullptr, TEXT("Text"))
	//	,_textBox(new DUOLGraphicsLibrary::TextBox())
	//	, _canvas(nullptr)
	//	, _orderInLayer(0)
	//{
	//	Initialize();
	//}

	Text::Text(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _textBox(new DUOLGraphicsLibrary::TextBox())
		, _canvas(nullptr)
		, _orderInLayer(0)
	{
		Initialize();
	}

	Text::~Text()
	{
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
	}

	void Text::OnUpdate(float deltaTime)
	{
		if (this->GetGameObject() != nullptr && this->GetGameObject()->GetIsActive() == false)
			return;

		if (!_canvas)
		{
			auto object = this->GetGameObject()->GetTransform()->GetParent();
			while (object->GetParent() != nullptr)
			{
				object = object->GetParent();
			}
			DUOLGameEngine::UIManager::GetInstance()->CreateCanvas(object->GetGameObject());
			SetCanvas(object->GetGameObject()->GetComponent<Canvas>()->GetCanvas());
		}

		RectTransform* rectTranform = GetGameObject()->GetComponent<RectTransform>();

		_textBox->_rect = rectTranform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());

		_textBox->_text = _inputText;

		if (rectTranform)
		{
			_textBox->_scale = DUOLMath::Vector2(rectTranform->GetScale().x, rectTranform->GetScale().y);

			_textBox->_pivot = rectTranform->GetPivot();
		}

		if(_canvas != nullptr)
			_canvas->DrawTexts(_textBox, _orderInLayer);
	}

	void Text::Initialize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				OnUpdate(1.0f);
			});

		_textBox->_fontType = DUOLGameEngine::ResourceManager::GetInstance()->GetFont(_currFontName);

		if (_textBox->_fontType == nullptr)
		{
			auto fontList = DUOLGameEngine::UIManager::GetInstance()->GetFontList();

			// 처음 만들때는 임의로 처음 폰트를 넣어준다. 
			std::string path = "Asset/Font/" + DUOLCommon::StringHelper::ToString(fontList[0]);

			_textBox->_fontType = DUOLGameEngine::ResourceManager::GetInstance()->GetFont(DUOLCommon::StringHelper::ToTString(path));
		}

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

	}

	void Text::SetLayer(int layer)
	{
		if (layer < 0)
			layer = 0;

		_orderInLayer = layer;
	}

	void Text::SetFontSize(int size)
	{
		if (size < 0)
			size = 0;

		_textBox->_fontSize = size;
	}

	void Text::SetFontType(const DUOLCommon::tstring& fontname)
	{
		_currFontName = fontname;

		std::string path = "Asset/Font/" + DUOLCommon::StringHelper::ToString(fontname);

		_textBox->_fontType = DUOLGameEngine::ResourceManager::GetInstance()->GetFont(DUOLCommon::StringHelper::ToTString(path));
	}

	void Text::SetText(const DUOLCommon::tstring& inputtext)
	{
		_inputText = inputtext;
		_textBox->_text = DUOLCommon::StringHelper::ToWString(inputtext);
	}

	void Text::OnResize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		auto canvas = object->GetComponent<Canvas>();

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		auto thisobject = this->GetGameObject();

		RectTransform* rectTranform = GetGameObject()->GetComponent<RectTransform>();

		if (rectTranform == nullptr)
			return;

		float x = rectTranform->GetPosX() * canvas->GetScreenRatio().x;
		float y = rectTranform->GetPosY() * canvas->GetScreenRatio().y;

		float width = rectTranform->GetWidth() * canvas->GetScreenRatio().x;
		float height = rectTranform->GetHeight() * canvas->GetScreenRatio().y;

		// 이것도 곱해줘야하나?
		rectTranform->SetRectX(x);
		rectTranform->SetRectY(y);

		// 이건 맞다..?
		rectTranform->SetRectZ(width);
		rectTranform->SetRectW(height);
	}

	void Text::LoadScene()
	{
		// 여기서 게임 오브젝트가 바뀐다.
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

	}

	void Text::TextChange()
	{
		//부모가 있다는 가정
		auto parent = this->GetGameObject()->GetComponent<Transform>()->GetParent();

		if(parent)
		{
			auto parentText = parent->GetGameObject()->GetComponent<Text>();
			parentText->SetText(_inputText);
		}
	}
}
