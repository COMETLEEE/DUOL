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
	Text::Text(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _textBox(new DUOLGraphicsLibrary::TextBox())
		, _canvas(nullptr)
		, _orderInLayer(0)
	{
		Initialize(owner);
	}

	Text::~Text()
	{
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
		DUOLGameEngine::UIManager::GetInstance()->RemoveText(this);
		_textBox->_fontSize = _preFontSize;
	}

	void Text::OnUpdate(float deltaTime)
	{
		if (this->GetGameObject() != nullptr && this->GetGameObject()->GetIsActive() == false)
			return;

		if (!_rectTransform)
			_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

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

		_textBox->_rect = _rectTransform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());

		_textBox->_text = _inputText;

		if (_rectTransform)
		{
			_textBox->_scale = DUOLMath::Vector2(_rectTransform->GetScale().x, _rectTransform->GetScale().y);

			_textBox->_pivot = _rectTransform->GetPivot();
		}

		if (_canvas != nullptr)
			_canvas->DrawTexts(_textBox, _orderInLayer);
	}

	void Text::Initialize(DUOLGameEngine::GameObject* owner)
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		DUOLGameEngine::UIManager::GetInstance()->CreateText(this);

		_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				OnUpdate(1.0f);
			});

		_textBox->_fontType = DUOLGameEngine::ResourceManager::GetInstance()->GetFont(_currFontName);

		_preFontSize = _textBox->_fontSize;

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

		if (owner == nullptr)
			return;

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		_rectTransform->SetRectW(45);
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

		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		auto canvas = object->GetComponent<Canvas>();

		auto ratio = GraphicsManager::GetInstance()->GetScreenRatio();

		if (ratio.x == 1.0f && ratio.y == 1.0f)
			_preFontSize = size;
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
		std::string inputStr = DUOLCommon::StringHelper::ToString(inputtext);

	}

	void Text::OnResize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;


		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		auto thisobject = this->GetGameObject();

		if (_rectTransform == nullptr)
			_rectTransform = GetGameObject()->GetComponent<RectTransform>();

		auto ratio = GraphicsManager::GetInstance()->GetScreenRatio();

		auto rect = _rectTransform->GetRect();

		float x = rect.x * ratio.x;
		//float y = rect.y * ratio.y;

		UINT64 size = static_cast<float>(_preFontSize);

		_rectTransform->SetRectX(x);
		//_rectTransform->SetRectY(y);
	}

	void Text::LoadScene()
	{
		// 여기서 게임 오브젝트가 바뀐다.
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		auto ratio = GraphicsManager::GetInstance()->GetScreenRatio();

		auto rect = _rectTransform->GetRect();

		float x = rect.x * ratio.x;
		//float y = rect.y * ratio.y;

		_rectTransform->SetRectX(x);
		//_rectTransform->SetRectY(y);
	}

	void Text::TextChange()
	{
		//부모가 있다는 가정
		auto parent = this->GetGameObject()->GetComponent<Transform>()->GetParent();

		if (parent)
		{
			auto parentText = parent->GetGameObject()->GetComponent<Text>();
			parentText->SetText(_inputText);
		}
	}
}
