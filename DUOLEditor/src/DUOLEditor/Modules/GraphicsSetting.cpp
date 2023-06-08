#include "DUOLEditor/Modules/GraphicsSetting.h"

#include <rttr/enumeration.h>
#include <rttr/type.h>

#include "DUOLCommon/MetaDataType.h"
#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Layout/Columns.h"
#include "DUOLEditor/UI/Widgets/Layout/ContainerCollapsable.h"
#include "DUOLEditor/UI/Widgets/Selections/ComboBox.h"

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"


namespace DUOLEditor
{

	GraphicsSetting::GraphicsSetting(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelWindowSetting) :
		DUOLEditor::PanelWindow(title, isOpened, panelWindowSetting)
	{
		// Scene Changed Event
		DUOLEditor::EditorEventManager::GetInstance()->GetSceneChangedEvent() +=
			std::bind(&GraphicsSetting::SetCurrentScene, this, std::placeholders::_1);
	}

	GraphicsSetting::~GraphicsSetting()
	{

	}

	void GraphicsSetting::DrawUIProperty()
	{
		RemoveAllWidgets();

		// RTTR library - Reflection을 사용합니다.
		using namespace rttr;

		auto& graphicsSetting = _currentScene->GetGraphicsSetting();

		// 해당 객체를 rttr instance 화 합니다.
		const instance& obj = graphicsSetting;

		// 해당 인스턴스의 가장 아래에 있는 타입 (최종 자식 클래스 타입) 을 가져옵니다.
		const type class_type = obj.get_derived_type();
		std::string className = class_type.get_name().to_string();

		auto properties = class_type.get_properties();

		auto header = AddWidget<ContainerCollapsable>(DUOLCommon::StringHelper::ToTString(className), true);;
		const auto columns = header->AddWidget<Columns<1>>();

		Draw(columns, properties, obj);
		//AddWidget<>()
	}


	bool GraphicsSetting::IsInspectable(rttr::property property)
	{
		using namespace rttr;

		const variant metaData = property.get_metadata(DUOLCommon::MetaDataType::Inspectable);

		return metaData.is_valid() ? metaData.get_value<bool>() : false;
	}

	void GraphicsSetting::DrawBool(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<bool>();
		};

		auto provider = [obj, property](bool value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawBool(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider);
	}

	void DUOLEditor::GraphicsSetting::DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<float>();
		};

		auto provider = [obj, property, this](float value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();
		};

		DUOLEditor::ImGuiHelper::DrawFloat(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void GraphicsSetting::DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector2>();
		};

		auto provider = [obj, property, this](DUOLMath::Vector2 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();

		};

		DUOLEditor::ImGuiHelper::DrawFloat2(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void GraphicsSetting::DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector3>();
		};

		auto provider = [obj, property, this](DUOLMath::Vector3 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();

		};

		DUOLEditor::ImGuiHelper::DrawFloat3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void GraphicsSetting::DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector4>();
		};

		auto provider = [obj, property, this](DUOLMath::Vector4 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();

		};

		DUOLEditor::ImGuiHelper::DrawFloat4(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void GraphicsSetting::DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<int>();
		};

		auto provider = [obj, property, this](int value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();

		};

		DUOLEditor::ImGuiHelper::DrawInt(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			1, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
	}

	void GraphicsSetting::DrawString(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLCommon::tstring>();
		};

		auto provider = [obj, property, this](DUOLCommon::tstring value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();
		};

		DUOLEditor::ImGuiHelper::DrawString(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider);

	}

	void GraphicsSetting::DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector3>();
		};

		auto provider = [obj, property, this](DUOLMath::Vector3 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
			_currentScene->UpdateGraphicsSettings();

		};

		DUOLEditor::ImGuiHelper::DrawColor3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider);
	}

	void GraphicsSetting::DrawEnumeration(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, bool title)
	{
		using namespace rttr;

		// Enum 은 어차피 모두 int이다.
		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<int>();
		};

		auto provider = [obj, property](int enumeration)
		{
			const rttr::enumeration enumer = property.get_enumeration();

			// 왜 이렇게 한 번 바꿔주는 과정이 필요한걸까 ..?
			auto&& name = enumer.value_to_name(enumeration);

			variant t = enumer.name_to_value(name);

			if (!property.set_value(obj, t))
			{
				// ASSERT
			}

			// 'enum class' property에 이렇게 int 쌩으로 값을 넣으려고 하니 .. 동작하지 않는다 !
			// C#에서도 비슷한 문제가 있어 enum value를 parse 하여서 넣는 것 같다 !
			//if (!property.set_value(obj, enumeration))
			//{
			//	// ASSERT
			//}
		};

		enumeration enumType = property.get_enumeration();

		auto&& values = enumType.get_values();

		if (title)
			// 일단 타이틀을 그려주고
			DUOLEditor::ImGuiHelper::DrawTitle(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().to_string()));

		// Create Combo Box
		auto&& comboBox = rootWidget->AddWidget<DUOLEditor::ComboBox>(values.size());

		for (auto& value : values)
		{
			comboBox->_choices.insert({ value.get_value<int>(), DUOLCommon::StringHelper::ToTString(value.to_string()) });
		}

		comboBox->RegisterGatherer([gatherer]()
			{
				return gatherer();
			});

		comboBox->RegisterProvider([provider](int* value)
			{
				provider(*value);
			});
	}

	void GraphicsSetting::DrawStructure(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property,
		rttr::instance obj)
	{
		using namespace rttr;

		//구조체의 정보를 추가합니다.

		type class_type = property.get_type();

		bool isWrapper = class_type.is_wrapper();

		class_type = isWrapper ? class_type.get_wrapped_type() : class_type;
		std::string className = class_type.get_name().to_string();

		auto var = property.get_value(obj);
		auto tyy = var.get_type();

		//const variant& inst = var.extract_wrapped_value();
		const instance inst = var;

		auto properties = class_type.get_properties();

		auto widget = rootWidget->AddWidget<ContainerCollapsable>(DUOLCommon::StringHelper::ToTString(className), true);;
		auto columm = widget->AddWidget<Columns<2>>();

		Draw(columm, properties, inst);
	}

	void GraphicsSetting::DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
		//using namespace rttr;

		//auto gatherer = [obj, rootProperty, property]()
		//{
		//	auto rootValue = rootProperty.get_value(obj);
		//	const instance temp = rootValue;

		//	auto value = property.get_value(temp);

		//	return value.get_value<float>();
		//};

		//auto provider = [obj, rootProperty, property](float value)
		//{
		//	auto rootValue = rootProperty.get_value(obj);

		//	const instance temp = rootValue;
		//	auto res = property.set_value(temp, value);

		//	auto type = temp.get_type();

		//	variant var = temp;

		//	if (!rootProperty.set_value(obj, var))
		//	{
		//		// ASSERT ? ERROR ?
		//	}
		//};

		//DUOLEditor::ImGuiHelper::DrawFloat(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
		//	0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

	}

	void GraphicsSetting::DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
		//using namespace rttr;

		//auto gatherer = [obj, rootProperty, property]()
		//{
		//	auto rootValue = rootProperty.get_value(obj);
		//	const instance temp = rootValue;

		//	auto value = property.get_value(temp);

		//	return value.get_value<DUOLMath::Vector2>();
		//};

		//auto provider = [obj, rootProperty, property](DUOLMath::Vector2 value)
		//{
		//	auto rootValue = rootProperty.get_value(obj);
		//	const instance temp = rootValue;
		//	auto res = property.set_value(temp, value);

		//	auto type = temp.get_type();

		//	variant var = temp;

		//	if (!rootProperty.set_value(obj, var))
		//	{
		//		// ASSERT ? ERROR ?
		//	}
		//};

		//DUOLEditor::ImGuiHelper::DrawFloat2(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
		//	0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());


	}

	void GraphicsSetting::DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
		//using namespace rttr;

		//auto gatherer = [obj, rootProperty, property]()
		//{
		//	auto rootValue = rootProperty.get_value(obj);
		//	const instance temp = rootValue;

		//	auto value = property.get_value(temp);

		//	return value.get_value<DUOLMath::Vector3>();
		//};

		//auto provider = [obj, rootProperty, property](DUOLMath::Vector3 value)
		//{
		//	auto rootValue = rootProperty.get_value(obj);
		//	const instance temp = rootValue;
		//	auto res = property.set_value(temp, value);

		//	auto type = temp.get_type();

		//	variant var = temp;

		//	if (!rootProperty.set_value(obj, var))
		//	{
		//		// ASSERT ? ERROR ?
		//	}
		//};

		//DUOLEditor::ImGuiHelper::DrawFloat3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
		//	0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

	}

	void GraphicsSetting::DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
	}

	void GraphicsSetting::DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
	}

	void GraphicsSetting::DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty,
		rttr::property property, rttr::instance obj)
	{
	}

	void GraphicsSetting::Draw(DUOLEditor::WidgetGroupBase* rootWidget, const rttr::array_range<rttr::property>& properties, rttr::instance obj)
	{

		for (auto& property : properties)
		{
			// Inspect 가능하다면 ..? <=> 'property.get_metadata(DUOLCommon::MetaDataType::Inspectable) == true' 인지 체크.
			if (IsInspectable(property))
			{
				// Inspect 유형 메타데이터에 맞게 인스펙터 창에 그립니다.
				DUOLCommon::InspectType inspectType = property.get_metadata(DUOLCommon::MetaDataType::InspectType).get_value<DUOLCommon::InspectType>();

				switch (inspectType)
				{
				case DUOLCommon::InspectType::Bool:
				{
					DrawBool(rootWidget, property, obj);

					break;
				}

				case DUOLCommon::InspectType::Float:
				{
					DrawFloat(rootWidget, property, obj);

					break;
				}

				case DUOLCommon::InspectType::Float2:
				{
					DrawFloat2(rootWidget, property, obj);

					break;
				}

				case DUOLCommon::InspectType::Float3:
				{
					DrawFloat3(rootWidget, property, obj);

					break;
				}

				case DUOLCommon::InspectType::Float4:
				{
					DrawFloat4(rootWidget, property, obj);

					break;
				}
				case DUOLCommon::InspectType::Int:
				{
					DrawInt(rootWidget, property, obj);

					break;
				}
				case DUOLCommon::InspectType::String:
				{
					DrawString(rootWidget, property, obj);

					break;
				}

				case DUOLCommon::InspectType::Color:
				{
					DrawColor3(rootWidget, property, obj);

					break;
				}
				case DUOLCommon::InspectType::Enumeration:
				{
					DrawEnumeration(rootWidget, property, obj);

					break;
				}
				case DUOLCommon::InspectType::Structure:
				{

					DrawStructure(rootWidget, property, obj);

					break;
				}
				default:
					break;
				}
			}
		}
	}

	DUOLGameEngine::Scene* GraphicsSetting::GetCurrentScene() const
	{
		return _currentScene;
	}

	void GraphicsSetting::SetCurrentScene(DUOLGameEngine::Scene* currentScene)
	{
		if (currentScene == nullptr)
			return;

		_currentScene = currentScene;
	}
}
