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

		// 멋을 위해 개행을 한 번 한다.
		_inspectorHeader->AddWidget<DUOLEditor::NewLine>();
#pragma endregion
		
#pragma region GAMEOBJECT_INFO
		_gameObjectInfo = AddWidget<DUOLEditor::Container>();
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
		{
			// 컴포넌트 갯수 변경 이벤트 빼준다 !
			_selectedGameObject->_componentCountChangedEvent -= _selectedGameObjectListenerID;

			UnsetInspectedGameObject();
		}

		// 이 게임 오브젝트가 선택되었습니다.
		_selectedGameObject = gameObject;

		// 컴포넌트 갯수가 변하면 다시 그릴 수 있도록 해줍시다.
		_selectedGameObjectListenerID = _selectedGameObject->_componentCountChangedEvent += [this]()
		{
			this->UnsetInspectedSameGameObject();

			this->SetInspectedSameGameObject();
		};

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

	void Inspector::SetInspectedSameGameObject()
	{
		_inspectorHeader->SetIsEnable(true);

		DrawGameObjectInformation();
	}

	void Inspector::UnsetInspectedSameGameObject()
	{
		_inspectorHeader->SetIsEnable(false);

		// 현재 있는 게임 오브젝트 인포 위젯에서 모든 위젯을 삭제합니다.
		_gameObjectInfo->RemoveAllWidgets();
	}

	void Inspector::DrawGameObjectInformation()
	{
#pragma region ALL_COMPONENT
		auto&& allComponents = _selectedGameObject->GetAllComponents();

		for (auto component : allComponents)
			DrawComponentInformation(component);
#pragma endregion

		// UI 레이아웃을 예쁘게 정리하자.
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
		// RTTR library - Reflection을 사용합니다.
		using namespace rttr;

		ContainerCollapsable* header = nullptr;

		// 트랜스폼만 아니면 삭제할 수 있습니다.
		if (component->GetName() != TEXT("Transform"))
		{
			// 트랜스폼은 처음에 열어놓습니다.
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

	void Inspector::DrawAddComponentInformation()
	{
		using namespace rttr;

		// AddComponent Button을 만든다.
		auto addComponent = _gameObjectInfo->AddWidget<DUOLEditor::Button>(TEXT("          Add Component          "));

		auto componentBar = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		componentBar->SetIsEnable(false);

		auto column = componentBar->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Component"));

		auto componentSearch = column->AddWidget<DUOLEditor::InputText>();

		auto componentList = componentBar->AddWidget<DUOLEditor::ListBox>();

		// 모든 타입을 돌면서 컴포넌트로부터 상속된 녀석이면 Inspecting 합니다.
		auto&& allTypes = type::get_types();

		// 컴포넌트이면 .. 어쩌구 한다.
		for (auto& type : allTypes)
		{
			auto typeName = type.get_name().to_string();

			// Base가 들어가 있으면 추가하지 않습니다.
			if (typeName.find("Base") != std::string::npos)
				continue;

			// Transform 또한 추가하지 않습니다.
			if (typeName == "Transform")
				continue;

			auto&& baseClasses = type.get_base_classes();

			// 부모 클래스 중 ComponentBase가 있다면 리스트 박스에 추가합니다.
			for (auto& base : baseClasses)
			{
				if (base.get_name().to_string() == "ComponentBase")
				{
					componentList->AddChoice(DUOLCommon::StringHelper::ToTString(type.get_name().to_string()));
				}
			}
		}

		// 컴포넌트 검색 기능에서 이름이 바뀌었을 때 Component List에서 해당 이름을 가진 녀석들만 보이게 합니다.
		componentSearch->_textChangedEvent += [this, componentList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = componentList->_choices;

			auto& viewChoices = componentList->_viewChoices;

			// 일단 보이는 Choice List를 비워
			viewChoices.clear();

			// 아무 내용도 없다.
			if (name.empty())
			{
				// 전부 다 넣어
				for (auto [key, value] : allChoices)
					viewChoices.insert({ key, value });

				return;
			}

			// 모든 선택에서 이름이 속한 녀석이 있으면 viewChoices에 넣는다
			for (auto [key, value] : allChoices)
			{
				auto choiceValue = value;

				std::transform(choiceValue.begin(), choiceValue.end(), choiceValue.begin(), ::tolower);

				if (choiceValue.find(text) != DUOLCommon::tstring::npos)
				{
					viewChoices.insert({ key, value });
				}
			}

			// 검색한 내용이 없으면 이거라도 넣어주자
			if (viewChoices.empty())
			{
				viewChoices.insert({ 1000000, TEXT("There's No Component with that name") });
			}
		};

		// 눌리면 해당 이름의 컴포넌트를 리플렉션하여 _selectedGameObject 애 붙여줍시다.
		componentList->_choiceChangedEvent += [this](const DUOLCommon::tstring& componentName)
		{
			// 해당 게임 오브젝트에게 컴포넌트를 추가하자 ! 이 안에서 Component Count Changed Event On !
			_selectedGameObject->AddComponent(componentName);
		};

		// Add Component 버튼 끄고 키기
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