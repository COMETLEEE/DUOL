#include "DUOLCommon/MetaDataType.h"

#include "DUOLEditor/Modules/Inspector.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Layout/Columns.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"
#include "DUOLEditor/UI/Widgets/Texts/TextColored.h"
#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"
#include "DUOLEditor/UI/Widgets/Layout/ContainerCollapsable.h"
#include "DUOLEditor/UI/Widgets/Selections/CheckBox.h"
#include "DUOLEditor/UI/Widgets/Selections/ComboBox.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <array>

#include "rttr/type.h"
#include "rttr/enumeration.h"

const DUOLEditor::Color TitleColor = { 0.35f, 0.85f, 0.65f, 1.f };

namespace DUOLEditor
{
	Inspector::Inspector(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelSetting) :
		PanelWindow(title, isOpened, panelSetting)
	{
#pragma region HEADER
		_inspectorHeader = AddWidget<DUOLEditor::Container>();

		_inspectorHeader->SetIsEnable(false);

		auto headerColumns = _inspectorHeader->AddWidget<DUOLEditor::Columns<2>>();

		// Name
		auto nameGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetName() : TEXT(""); };

		auto nameProvider = [this](DUOLCommon::tstring newName) { if (_selectedGameObject != nullptr) _selectedGameObject->SetName(newName); };

		DUOLEditor::ImGuiHelper::DrawString(headerColumns, TEXT("Name"), nameGatherer, nameProvider);

		// Tag
		auto tagGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetTag() : TEXT(""); };

		auto tagProvider = [this](const DUOLCommon::tstring newTag) { if (_selectedGameObject != nullptr) _selectedGameObject->SetTag(newTag); };

		DUOLEditor::ImGuiHelper::DrawString(headerColumns, TEXT("Tag"), tagGatherer, tagProvider);

		// Active
		auto activeGatherer = [this]{ return _selectedGameObject != nullptr ? _selectedGameObject->GetIsActive() : false; };

		auto activeProvider = [this](bool value) { if (_selectedGameObject != nullptr) _selectedGameObject->SetIsActive(value); };

		DUOLEditor::ImGuiHelper::DrawBool(headerColumns, TEXT("Active"), activeGatherer, activeProvider);
#pragma endregion
		
#pragma region GAMEOBJECT_INFO
		_gameObjectInfo = AddWidget<Container>();
#pragma endregion

		// 게임 오브젝트가 선택되면 인스펙터 창에 그림을 그립니다.
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() +=
			std::bind(&Inspector::SetInspectedGameObject, this, std::placeholders::_1);

		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind(&Inspector::UnsetInspectedGameObject, this);
	}

	Inspector::~Inspector()
	{

	}

	void Inspector::SetInspectedGameObject(DUOLGameEngine::GameObject* gameObject)
	{
		// 만약 이미 게임 오브젝트가 셋팅되어 있다면 정상 작동을 위해 Inspected 상태를 꺼줍니다.
		if (_selectedGameObject != nullptr)
			UnsetInspectedGameObject();

		// 이 게임 오브젝트가 선택되었습니다.
		_selectedGameObject = gameObject;

		// 인스펙터 창의 헤더를 켜줍니다.
		_inspectorHeader->SetIsEnable(true);

		DrawGameObjectInformation();
	}

	void Inspector::UnsetInspectedGameObject()
	{
		_selectedGameObject = nullptr;

		_inspectorHeader->SetIsEnable(false);

		// 현재 있는 게임 오브젝트 인포 위젯에서 모든 위젯을 삭제합니다.
		_gameObjectInfo->RemoveAllWidgets();
	}

	void Inspector::DrawGameObjectInformation()
	{
		// 모든 컴포넌트에 대해서 인스펙트 체크 후 그립니다.
		auto&& allComponents = _selectedGameObject->GetAllComponents();

		for (auto component : allComponents)
			DrawComponentInformation(component);
	}

	void Inspector::DrawTransformInformation(DUOLGameEngine::Transform* transform)
	{
		auto header = _gameObjectInfo->AddWidget<ContainerCollapsable>(TEXT("Transform"));

		auto columns = header->AddWidget<DUOLEditor::Columns<2>>();
		
		auto positionGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalPosition, transform);

		auto positionProvider = std::bind(&DUOLGameEngine::Transform::SetLocalPosition, transform, std::placeholders::_1);

		ImGuiHelper::DrawFloat3(columns, TEXT("Position"), positionGatherer, positionProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		auto rotationGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalEulerAngle, transform);

		auto rotationProvider = std::bind(&DUOLGameEngine::Transform::SetLocalEulerAngle, transform, std::placeholders::_1);

		ImGuiHelper::DrawFloat3(columns, TEXT("Rotation"), rotationGatherer, rotationProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		auto scaleGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalScale, transform);

		auto scaleProvider = std::bind(&DUOLGameEngine::Transform::SetLocalScale, transform, std::placeholders::_1);

		ImGuiHelper::DrawFloat3(columns, TEXT("Scale"), scaleGatherer, scaleProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawComponentInformation(DUOLGameEngine::ComponentBase* component)
	{
		// RTTR library - Reflection을 사용합니다.
		using namespace rttr;

		const auto header = _gameObjectInfo->AddWidget<ContainerCollapsable>(component->GetName());

		const auto columns = header->AddWidget<DUOLEditor::Columns<2>>();

		const instance obj = *component;

		const type class_type = type::get_by_name(DUOLCommon::StringHelper::ToString(component->GetName()));

		auto properties = class_type.get_properties();

		for (auto& property : properties)
		{
			// Inspect 가능하면 유형 메타데이터에 맞게 인스펙터 창에 띄웁니다.
			if (IsInspectable(property))
			{
				DUOLCommon::InspectType inspectType = property.get_metadata(DUOLCommon::MetaDataType::InspectType).get_value<DUOLCommon::InspectType>();

				switch (inspectType)
				{
					case DUOLCommon::InspectType::Bool:
					{
						DrawBool(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Float:
					{
						DrawFloat(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Float2:
					{
						DrawFloat2(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Float3:
					{
						DrawFloat3(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Float4:
					{
						DrawFloat4(columns, property, obj);

						break;
					}
					
					case DUOLCommon::InspectType::String:
					{
						DrawString(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Color:
					{
						DrawColor3(columns, property, obj);

						break;
					}

					case DUOLCommon::InspectType::Enumeration:
					{
						DrawEnumeration(columns, property, obj);

						break;
					}
				}
			}
		}
	}

	bool Inspector::IsInspectable(rttr::property property)
	{
		using namespace rttr;

		const variant metaData = property.get_metadata(DUOLCommon::MetaDataType::Inspectable);

		return metaData.is_valid() ? metaData.get_value<bool>() : false;
	}

	void Inspector::DrawBool(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
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

	void DUOLEditor::Inspector::DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<float>();
		};

		auto provider = [obj, property](float value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawFloat(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector2>();
		};

		auto provider = [obj, property](DUOLMath::Vector2 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawFloat2(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector3>();
		};

		auto provider = [obj, property](DUOLMath::Vector3 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawFloat3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector4>();
		};

		auto provider = [obj, property](DUOLMath::Vector4 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawFloat4(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawString(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

	}

	void Inspector::DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLMath::Vector3>();
		};

		auto provider = [obj, property](DUOLMath::Vector3 vec)
		{
			if (!property.set_value(obj, vec))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawColor3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider);
	}

	void Inspector::DrawEnumeration(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
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
}