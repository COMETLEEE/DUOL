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

#include "DUOLEditor/UI/Widgets/Buttons/Button.h"
#include "DUOLEditor/UI/Widgets/Display/Separator.h"
#include "DUOLEditor/UI/Widgets/Drags/DragScalar.h"
#include "DUOLEditor/UI/Widgets/Layout/NewLine.h"
#include "DUOLEditor/UI/Widgets/Layout/SameLine.h"
#include "DUOLEditor/UI/Widgets/Selections/ListBox.h"
#include "DUOLEditor/UI/Widgets/Texts/TextSelectable.h"
#include "rttr/type.h"
#include "rttr/enumeration.h"

const DUOLEditor::Color TitleColor = { 0.35f, 0.85f, 0.65f, 1.f };

namespace DUOLEditor
{
	Inspector::Inspector(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& panelSetting) :
		PanelWindow(title, isOpened, panelSetting)
		, _selectedGameObject(nullptr)
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

		// ���� ���� ������ �� �� �Ѵ�.
		_inspectorHeader->AddWidget<DUOLEditor::NewLine>();
#pragma endregion
		
#pragma region GAMEOBJECT_INFO
		_gameObjectInfo = AddWidget<DUOLEditor::Container>();
#pragma endregion

		// ���� ������Ʈ�� ���õǸ� �ν����� â�� �׸��� �׸��ϴ�.
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
		// ���� �̹� ���� ������Ʈ�� ���õǾ� �ִٸ� ���� �۵��� ���� Inspected ���¸� ���ݴϴ�.
		if (_selectedGameObject != nullptr)
		{
			// ������Ʈ ���� ���� �̺�Ʈ ���ش� !
			_selectedGameObject->_componentCountChangedEvent -= _selectedGameObjectListenerID;

			UnsetInspectedGameObject();
		}

		// �� ���� ������Ʈ�� ���õǾ����ϴ�.
		_selectedGameObject = gameObject;

		// ������Ʈ ������ ���ϸ� �ٽ� �׸� �� �ֵ��� ���ݽô�.
		_selectedGameObjectListenerID = _selectedGameObject->_componentCountChangedEvent += [this]()
		{
			this->UnsetInspectedSameGameObject();

			this->SetInspectedSameGameObject();
		};

		// �ν����� â�� ����� ���ݴϴ�.
		_inspectorHeader->SetIsEnable(true);

		DrawGameObjectInformation();
	}

	void Inspector::UnsetInspectedGameObject()
	{
		_selectedGameObject = nullptr;

		_inspectorHeader->SetIsEnable(false);

		// ���� �ִ� ���� ������Ʈ ���� �������� ��� ������ �����մϴ�.
		_gameObjectInfo->RemoveAllWidgets();
	}

	void Inspector::SetInspectedSameGameObject()
	{
		_inspectorHeader->SetIsEnable(true);

		DrawGameObjectInformation();
	}

	void Inspector::UnsetInspectedSameGameObject()
	{
		_inspectorHeader->SetIsEnable(false);

		// ���� �ִ� ���� ������Ʈ ���� �������� ��� ������ �����մϴ�.
		_gameObjectInfo->RemoveAllWidgets();
	}

	void Inspector::DrawGameObjectInformation()
	{
#pragma region ALL_COMPONENT
		auto&& allComponents = _selectedGameObject->GetAllComponents();

		for (auto component : allComponents)
			DrawComponentInformation(component);
#pragma endregion

		// UI ���̾ƿ��� ���ڰ� ��������.
		_gameObjectInfo->AddWidget<DUOLEditor::NewLine>();

		_gameObjectInfo->AddWidget<DUOLEditor::Separator>();

		_gameObjectInfo->AddWidget<DUOLEditor::Separator>();

		_gameObjectInfo->AddWidget<DUOLEditor::NewLine>();

#pragma region ADD_COMPONENT
		DrawAddComponentInformation();
#pragma endregion
	}

	void Inspector::DrawComponentInformation(DUOLGameEngine::ComponentBase* component)
	{
		// RTTR library - Reflection�� ����մϴ�.
		using namespace rttr;

		ContainerCollapsable* header = nullptr;

		// Ʈ�������� �ƴϸ� ������ �� �ֽ��ϴ�.
		if (component->GetName() != TEXT("Transform"))
		{
			// Ʈ�������� ó���� ��������ϴ�.
			header = _gameObjectInfo->AddWidget<ContainerCollapsable>(component->GetName(), false);
			
			header->_closable = true;

			// RemoveComponent
			header->_closeEvent += [this, component]()
			{
				DUOLGameEngine::ObjectBase::Destroy(component);
			};
		}
		else
			header = _gameObjectInfo->AddWidget<ContainerCollapsable>(component->GetName(), true);

		const auto columns = header->AddWidget<DUOLEditor::Columns<2>>();

		const instance obj = *component;

		const type class_type = type::get_by_name(DUOLCommon::StringHelper::ToString(component->GetName()));

		auto properties = class_type.get_properties();

		for (auto& property : properties)
		{
			// Inspect �����ϸ� ���� ��Ÿ�����Ϳ� �°� �ν����� â�� ���ϴ�.
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

	void Inspector::DrawAddComponentInformation()
	{
		using namespace rttr;

		// AddComponent Button�� �����.
		auto addComponent = _gameObjectInfo->AddWidget<DUOLEditor::Button>(TEXT("          Add Component          "));

		auto componentBar = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		componentBar->SetIsEnable(false);

		auto column = componentBar->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Component"));

		auto componentSearch = column->AddWidget<DUOLEditor::InputText>();

		auto componentList = componentBar->AddWidget<DUOLEditor::ListBox>();

		// ��� Ÿ���� ���鼭 ������Ʈ�κ��� ��ӵ� �༮�̸� Inspecting �մϴ�.
		auto&& allTypes = type::get_types();

		// ������Ʈ�̸� .. ��¼�� �Ѵ�.
		for (auto& type : allTypes)
		{
			auto typeName = type.get_name().to_string();

			// Base�� �� ������ �߰����� �ʽ��ϴ�.
			if (typeName.find("Base") != std::string::npos)
				continue;

			// Transform ���� �߰����� �ʽ��ϴ�.
			if (typeName == "Transform")
				continue;

			auto&& baseClasses = type.get_base_classes();

			// �θ� Ŭ���� �� ComponentBase�� �ִٸ� ����Ʈ �ڽ��� �߰��մϴ�.
			for (auto& base : baseClasses)
			{
				if (base.get_name().to_string() == "ComponentBase")
				{
					componentList->AddChoice(DUOLCommon::StringHelper::ToTString(type.get_name().to_string()));
				}
			}
		}

		// ������Ʈ �˻� ��ɿ��� �̸��� �ٲ���� �� Component List���� �ش� �̸��� ���� �༮�鸸 ���̰� �մϴ�.
		componentSearch->_textChangedEvent += [this, componentList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = componentList->_choices;

			auto& viewChoices = componentList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			// �ƹ� ���뵵 ����.
			if (name.empty())
			{
				// ���� �� �־�
				for (auto [key, value] : allChoices)
					viewChoices.insert({ key, value });

				return;
			}

			// ��� ���ÿ��� �̸��� ���� �༮�� ������ viewChoices�� �ִ´�
			for (auto [key, value] : allChoices)
			{
				auto choiceValue = value;

				std::transform(choiceValue.begin(), choiceValue.end(), choiceValue.begin(), ::tolower);

				if (choiceValue.find(text) != DUOLCommon::tstring::npos)
				{
					viewChoices.insert({ key, value });
				}
			}

			// �˻��� ������ ������ �̰Ŷ� �־�����
			if (viewChoices.empty())
			{
				viewChoices.insert({ 1000000, TEXT("There's No Component with that name") });
			}
		};

		// ������ �ش� �̸��� ������Ʈ�� ���÷����Ͽ� _selectedGameObject �� �ٿ��ݽô�.
		componentList->_choiceChangedEvent += [this](const DUOLCommon::tstring& componentName)
		{
			// �ش� ���� ������Ʈ���� ������Ʈ�� �߰����� ! �� �ȿ��� Component Count Changed Event On !
			_selectedGameObject->AddComponent(componentName);
		};

		// Add Component ��ư ���� Ű��
		addComponent->_clickedEvent += [this, componentBar]()
		{
			bool enable = componentBar->GetIsEnable();

			componentBar->SetIsEnable(!enable);
		};
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

		// Enum �� ������ ��� int�̴�.
		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<int>();
		};

		auto provider = [obj, property](int enumeration)
		{
			const rttr::enumeration enumer = property.get_enumeration();

			// �� �̷��� �� �� �ٲ��ִ� ������ �ʿ��Ѱɱ� ..?
			auto&& name = enumer.value_to_name(enumeration);

			variant t = enumer.name_to_value(name);

			if (!property.set_value(obj, t))
			{
				// ASSERT
			}

			// 'enum class' property�� �̷��� int ������ ���� �������� �ϴ� .. �������� �ʴ´� !
			// C#������ ����� ������ �־� enum value�� parse �Ͽ��� �ִ� �� ���� !
			//if (!property.set_value(obj, enumeration))
			//{
			//	// ASSERT
			//}
		};

		enumeration enumType = property.get_enumeration();

		auto&& values = enumType.get_values();

		// �ϴ� Ÿ��Ʋ�� �׷��ְ�
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