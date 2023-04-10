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
#include <filesystem>

#include "DUOLEditor/UI/Widgets/Buttons/Button.h"
#include "DUOLEditor/UI/Widgets/Display/Separator.h"
#include "DUOLEditor/UI/Widgets/Drags/DragScalar.h"
#include "DUOLEditor/UI/Widgets/Layout/Dummy.h"
#include "DUOLEditor/UI/Widgets/Layout/NewLine.h"
#include "DUOLEditor/UI/Widgets/Layout/SameLine.h"
#include "DUOLEditor/UI/Widgets/Selections/ListBox.h"
#include "DUOLEditor/UI/Widgets/Texts/TextClickable.h"
#include "DUOLEditor/UI/Widgets/Texts/TextSelectable.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"
#include "DUOLGameEngine/ECS/Object/AudioClip.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/RendererBase.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

#include "DUOLGameEngine/Manager/UIManager.h"

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

		// Layer
		auto layerGatherer = [this] { return _selectedGameObject != nullptr ? DUOLPhysics::ObjectLayerControl::_layers.at(_selectedGameObject->GetLayer()) : 0; };

		/*DUOLEditor::ImGuiHelper::DrawTitle(headerColumns, TEXT("Layer"));

		auto allLayers = headerColumns->AddWidget<ComboBox>(DUOLPhysics::ObjectLayerControl::_layers.size());

		for (auto& [key, value] : DUOLPhysics::ObjectLayerControl::_layers)
		{
			allLayers->_choices.insert({ value, key });
		}

		allLayers->RegisterGatherer([layerGatherer]()
			{
				return layerGatherer();
			});

		allLayers->_valueChangedEvent += [this](int count)
		{
			if (_selectedGameObject != nullptr)
			{
				for (auto [key, value] : DUOLPhysics::ObjectLayerControl::_layers)
				{
					if (value == count)
					{
						_selectedGameObject->SetLayer(key);

						return;
					}
				}
			}
		};*/

		// Active
		auto activeGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetIsActive() : false; };

		auto activeProvider = [this](bool value) { if (_selectedGameObject != nullptr) _selectedGameObject->SetIsActiveSelf(value); };

		DUOLEditor::ImGuiHelper::DrawBool(headerColumns, TEXT("Active"), activeGatherer, activeProvider);

		// Static
		auto staticGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetIsStatic() : false; };

		auto staticProvider = [this](bool value) { if (_selectedGameObject != nullptr) _selectedGameObject->SetIsStatic(value); };

		DUOLEditor::ImGuiHelper::DrawBool(headerColumns, TEXT("Static"), staticGatherer, staticProvider);




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

		DUOLEditor::EditorEventManager::GetInstance()->GetSceneChangedEvent() +=
			[this](DUOLGameEngine::Scene* scene)
		{
			UnsetInspectedGameObject();
		};
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

		// ���õ� ���� ������Ʈ�� UI�� UIManager�� �Ѱ��ݴϴ�.
		if (_selectedGameObject->GetName() == L"UI")
		{
			DUOLGameEngine::UIManager::GetInstance()->SetPickGameObject(_selectedGameObject);
		}

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

		// �ش� ��ü�� rttr instance ȭ �մϴ�.
		const instance obj = *component;

		// �ش� �ν��Ͻ��� ���� �Ʒ��� �ִ� Ÿ�� (���� �ڽ� Ŭ���� Ÿ��) �� �����ɴϴ�.
		const type class_type = obj.get_derived_type();

		std::string className = class_type.get_name().to_string();

		// Ʈ�������� �ƴϸ� ������Ʈ�� ������ �� �ֽ��ϴ�.
		if (className != std::string("Transform") && className != std::string("RectTransform"))
		{
			// header = _gameObjectInfo->AddWidget<ContainerCollapsable>(DUOLCommon::StringHelper::ToTString(className), false);
			header = _gameObjectInfo->AddWidget<ContainerCollapsable>(DUOLCommon::StringHelper::ToTString(className), true);

			header->_closable = true;

			// Remove Component ���
			header->_closeEvent += [this, component]()
			{
				DUOLGameEngine::ObjectBase::Destroy(component);
			};
		}
		else
			// Ʈ�������� ó������ ��������ϴ�.
			header = _gameObjectInfo->AddWidget<ContainerCollapsable>(DUOLCommon::StringHelper::ToTString(className), true);

		// '������Ƽ�� �̸� : ������Ƽ ���� �׸���' �� UI �� �����մϴ�.
		const auto columns = header->AddWidget<DUOLEditor::Columns<2>>();

		// �ش� ������Ʈ�� ��� ������Ƽ�� �����ɴϴ�.
		auto properties = class_type.get_properties();


		if (className == "RectTransform")
		{
			for (auto& property : properties)
			{
				auto name = property.get_name().to_string();

				if (name == "Position" || name == "Rotation" || name == "Scale")
					continue;
				else
				{
					// Inspect �����ϴٸ� ..? <=> 'property.get_metadata(DUOLCommon::MetaDataType::Inspectable) == true' ���� üũ.
					if (IsInspectable(property))
					{
						// Inspect ���� ��Ÿ�����Ϳ� �°� �ν����� â�� �׸��ϴ�.
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
						case DUOLCommon::InspectType::Int:
						{
							DrawInt(columns, property, obj);

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

						case DUOLCommon::InspectType::SkinnedMesh:
						{
							DrawMesh(columns, property, obj, reinterpret_cast<DUOLGameEngine::SkinnedMeshRenderer*>(component));

							break;
						}

						case DUOLCommon::InspectType::Mesh:
						{
							DrawMesh(columns, property, obj, reinterpret_cast<DUOLGameEngine::MeshFilter*>(component));

							break;
						}

						case DUOLCommon::InspectType::AnimatorController:
						{
							DrawAnimatorController(columns, property, obj, reinterpret_cast<DUOLGameEngine::Animator*>(component));

							break;
						}
						case DUOLCommon::InspectType::UIFileName:
						{
							if (reinterpret_cast<DUOLGameEngine::Image*>(component)->GetName() == L"Image")
								DrawUIFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Image*>(component));
							else if (reinterpret_cast<DUOLGameEngine::Button*>(component)->GetName() == L"Button")
								DrawUIFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Button*>(component));

							break;
						}
						case DUOLCommon::InspectType::ButtonEvent:
						{
							DrawButtonFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Button*>(component));

							break;
						}

						}
					}
				}
			}

			return;
		}


		for (auto& property : properties)
		{
			// Inspect �����ϴٸ� ..? <=> 'property.get_metadata(DUOLCommon::MetaDataType::Inspectable) == true' ���� üũ.
			if (IsInspectable(property))
			{
				// Inspect ���� ��Ÿ�����Ϳ� �°� �ν����� â�� �׸��ϴ�.
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
				case DUOLCommon::InspectType::Int:
				{
					DrawInt(columns, property, obj);

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

				case DUOLCommon::InspectType::SkinnedMesh:
				{
					DrawMesh(columns, property, obj, reinterpret_cast<DUOLGameEngine::SkinnedMeshRenderer*>(component));

					break;
				}

				case DUOLCommon::InspectType::Mesh:
				{
					DrawMesh(columns, property, obj, reinterpret_cast<DUOLGameEngine::MeshFilter*>(component));

					break;
				}

				case DUOLCommon::InspectType::AnimatorController:
				{
					DrawAnimatorController(columns, property, obj, reinterpret_cast<DUOLGameEngine::Animator*>(component));

					break;
				}
				case DUOLCommon::InspectType::UIFileName:
				{
					if (reinterpret_cast<DUOLGameEngine::Image*>(component)->GetName() == L"Image")
						DrawUIFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Image*>(component));
					else if (reinterpret_cast<DUOLGameEngine::Button*>(component)->GetName() == L"Button")
						DrawUIFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Button*>(component));

					break;
				}
				case DUOLCommon::InspectType::ButtonEvent:
				{
					DrawButtonFileName(columns, property, obj, reinterpret_cast<DUOLGameEngine::Button*>(component));

					break;
				}


				case DUOLCommon::InspectType::AudioClip:
				{
					DrawAudioClip(columns, property, obj, reinterpret_cast<DUOLGameEngine::AudioSource*>(component));

					break;
				}
				case DUOLCommon::InspectType::Material:
				{
					DrawMaterial(columns, property, obj, reinterpret_cast<DUOLGameEngine::RendererBase*>(component));
					break;
				}
				}
			}
		}
	}

	void Inspector::DrawUIProperty(DUOLEditor::WidgetGroupBase* rootWidget, const rttr::instance obj, DUOLGameEngine::ObjectBase* objectbase)
	{
		// RTTR library - Reflection�� ����մϴ�.
		using namespace rttr;

		// �ش� �ν��Ͻ��� ���� �Ʒ��� �ִ� Ÿ�� (���� �ڽ� Ŭ���� Ÿ��) �� �����ɴϴ�.
		const type class_type = obj.get_derived_type();

		std::string className = class_type.get_name().to_string();

		auto properties = class_type.get_properties();

		for (auto& property : properties)
		{
			// Inspect �����ϴٸ� ..? <=> 'property.get_metadata(DUOLCommon::MetaDataType::Inspectable) == true' ���� üũ.
			if (IsInspectable(property))
			{
				// Inspect ���� ��Ÿ�����Ϳ� �°� �ν����� â�� �׸��ϴ�.
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
				case DUOLCommon::InspectType::UIFileName:
				{
						// ���⿡ �巡���� ������Ʈ�� �־��ش�. 
					DrawOnClickCallFileName(rootWidget, property, obj, reinterpret_cast<DUOLGameEngine::OnClickCall*>(objectbase));

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

		type componentType = type::get_by_name("ComponentBase");

		auto&& allTypes = componentType.get_derived_classes();

		// 'ComponentBase' �� ����� ��� Ÿ�Կ� ���� �����մϴ�.
		for (auto& type : allTypes)
		{
			auto typeName = type.get_name().to_string();

			// Base�� �� ������ �߰����� �ʽ��ϴ�.
			if (typeName.find("Base") != std::string::npos)
				continue;

			// Transform ���� �߰����� �ʽ��ϴ�.
			if (typeName == "Transform")
				continue;

			// Transform ���� �߰����� �ʽ��ϴ�.
			if (typeName == "RectTransform")
				continue;

			// AddComponent �ڽ��� �߰��մϴ�.
			componentList->AddChoice(DUOLCommon::StringHelper::ToTString(type.get_name().to_string()));
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

			viewChoices.insert({ 0, TEXT("None") });

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

		// ������ �ش� ���� �̸��� ���� ������Ʈ�� Ÿ�� �̸��� ���� ���÷������� _selectedGameObject �� �ٿ��ݴϴ�.
		componentList->_choiceChangedEvent += [this](const DUOLCommon::tstring& componentName)
		{
			// �� �ȿ��� Component Count Changed Event On ..
			if (DUOLEditor::EditorEventManager::GetInstance()->GetEditorMode() == DUOLEditor::EditorMode::Edit)
				_selectedGameObject->AddComponentEditor(componentName);
			else
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

	void Inspector::DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<int>();
		};

		auto provider = [obj, property](int value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawInt(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
			1, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
	}

	void Inspector::DrawString(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		auto gatherer = [obj, property]()
		{
			variant var = property.get_value(obj);

			return var.get_value<DUOLCommon::tstring>();
		};

		auto provider = [obj, property](DUOLCommon::tstring value)
		{
			if (!property.set_value(obj, value))
			{
				// ASSERT ? ERROR ?
			}
		};

		DUOLEditor::ImGuiHelper::DrawString(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider);

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


	void Inspector::DrawAnimatorController(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property,
		rttr::instance obj, DUOLGameEngine::Animator* animator)
	{
		using namespace rttr;

		variant var = property.get_value(obj);

		auto gatherer = [animator]()
		{
			auto animatorController = animator->GetAnimatorController();

			return animatorController == nullptr ? DUOLCommon::tstring(TEXT("Empty")) : animatorController->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring enumeration)
		{
			// ���� ���޹��� �ʾƿ� ..
		};

		auto callbackAfter = [animator]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					// .dcontroller file
					if (rePathExtension == ".dcontroller")
					{
						// �̹� �ֳ��� ..?
						DUOLGameEngine::AnimatorController* animatorController = DUOLGameEngine::ResourceManager::GetInstance()->LoadAnimatorController(rePath);

						animator->SetAnimatorController(animatorController);
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllAnimatorControllerInformation(textClickable, animator);
	}

	void Inspector::DrawAllAnimatorControllerInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::Animator* animator)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search AnimatorController"));

		auto acSearch = column->AddWidget<DUOLEditor::InputText>();

		auto acList = meshUI->AddWidget<DUOLEditor::ListBox>();

		auto allAnimatorControllers = DUOLGameEngine::ResourceManager::GetInstance()->GetAllAnimatorControllers();

		for (auto [name, animatorController] : allAnimatorControllers)
		{
			acList->AddChoice(name);
		}

		acSearch->_textChangedEvent += [this, acList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = acList->_choices;

			auto& viewChoices = acList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No mesh with that name") });
			}
		};

		// �ִϸ����� ��Ʈ�ѷ��� �ٲ��ݴϴ�.
		acList->_choiceChangedEvent += [this, animator](const DUOLCommon::tstring& animatorControllerName)
		{
			// Set Mesh
			DUOLGameEngine::AnimatorController* animCon = DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(animatorControllerName);

			if (animCon != nullptr)
				animator->SetAnimatorController(animCon);
		};

		// ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawMesh(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj,
		DUOLGameEngine::MeshFilter* meshFilter)
	{
		using namespace rttr;

		auto gatherer = [meshFilter]()
		{
			auto mesh = meshFilter->GetMesh();

			return mesh == nullptr ? DUOLCommon::tstring(TEXT("Empty")) : mesh->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [meshFilter]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					// TODO : .DUOL �� �ƴ� ,dmesh �� �ٲ��� �� �� ..
					if (rePathExtension == ".DUOL")
					{
						// �̹� �ֳ��� ..?
						DUOLGameEngine::Mesh* mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(rePath.stem());

						meshFilter->SetMesh(mesh);
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllStaticMeshInformation(textClickable, meshFilter);
	}

	void Inspector::DrawAllStaticMeshInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::MeshFilter* meshFilter)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Static Mesh"));

		auto meshSearch = column->AddWidget<DUOLEditor::InputText>();

		auto meshList = meshUI->AddWidget<DUOLEditor::ListBox>();

		auto allMeshes = DUOLGameEngine::ResourceManager::GetInstance()->GetAllMeshes();

		for (auto [name, mesh] : allMeshes)
		{
			if ((mesh->GetPrimitiveMesh() != nullptr) && (!mesh->IsSkinnedMesh()))
				meshList->AddChoice(name);
		}

		// ������Ʈ �˻� ��ɿ��� �̸��� �ٲ���� �� Component List���� �ش� �̸��� ���� �༮�鸸 ���̰� �մϴ�.
		meshSearch->_textChangedEvent += [this, meshList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = meshList->_choices;

			auto& viewChoices = meshList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No mesh with that name") });
			}
		};

		// �޽��� �ٲ��ݽô� ..!
		meshList->_choiceChangedEvent += [this, meshFilter](const DUOLCommon::tstring& meshName)
		{
			// Set Mesh
			DUOLGameEngine::Mesh* mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

			if (mesh != nullptr)
				meshFilter->SetMesh(mesh);
		};

		// Add Component ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawMesh(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj,
		DUOLGameEngine::SkinnedMeshRenderer* skinnedMeshRenderer)
	{
		using namespace rttr;

		auto gatherer = [skinnedMeshRenderer]()
		{
			auto skinnedMesh = skinnedMeshRenderer->GetSkinnedMesh();

			return skinnedMesh == nullptr ? DUOLCommon::tstring(TEXT("Empty")) : skinnedMesh->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [skinnedMeshRenderer]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					// TODO : .DUOL �� �ƴ� ,dmesh �� �ٲ��� �� �� ..
					if (rePathExtension == ".DUOL")
					{
						// �̹� �ֳ��� ..?
						DUOLGameEngine::Mesh* mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(rePath.stem());

						skinnedMeshRenderer->SetSkinnedMesh(mesh);
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllSkinnedMeshInformation(textClickable, skinnedMeshRenderer);
	}

	void Inspector::DrawAllSkinnedMeshInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::SkinnedMeshRenderer* skinnedMeshRenderer)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Skinned Mesh"));

		auto meshSearch = column->AddWidget<DUOLEditor::InputText>();

		auto meshList = meshUI->AddWidget<DUOLEditor::ListBox>();

		auto allMeshes = DUOLGameEngine::ResourceManager::GetInstance()->GetAllMeshes();

		for (auto [name, mesh] : allMeshes)
		{
			if ((mesh->GetPrimitiveMesh() != nullptr) && (mesh->IsSkinnedMesh()))
				meshList->AddChoice(name);
		}

		// ������Ʈ �˻� ��ɿ��� �̸��� �ٲ���� �� Component List���� �ش� �̸��� ���� �༮�鸸 ���̰� �մϴ�.
		meshSearch->_textChangedEvent += [this, meshList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = meshList->_choices;

			auto& viewChoices = meshList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No mesh with that name") });
			}
		};

		// �޽��� �ٲ��ݽô� ..!
		meshList->_choiceChangedEvent += [this, skinnedMeshRenderer](const DUOLCommon::tstring& meshName)
		{
			// Set Mesh
			DUOLGameEngine::Mesh* mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

			skinnedMeshRenderer->SetSkinnedMesh(mesh);
		};

		// Add Component ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawAudioClip(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj,
		DUOLGameEngine::AudioSource* audioSource)
	{
		using namespace rttr;

		auto gatherer = [audioSource]()
		{
			auto clip = audioSource->GetAudioClip();

			return clip == nullptr ? DUOLCommon::tstring(TEXT("Empty")) : clip->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [audioSource]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					// TODO : .DUOL �� �ƴ� ,dmesh �� �ٲ��� �� �� ..
					if (rePathExtension == ".wav")
					{
						// Create Sound ... ���̺� �߰� ��� 
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllAudioClipInformation(textClickable, audioSource);
	}

	void Inspector::DrawAllAudioClipInformation(DUOLEditor::TextClickable* textClickable,
		DUOLGameEngine::AudioSource* audioSource)
	{
		using namespace rttr;

		auto audioUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		audioUI->SetIsEnable(false);

		auto column = audioUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Audio Clip"));

		auto audioClipSearch = column->AddWidget<DUOLEditor::InputText>();

		auto audioClipList = audioUI->AddWidget<DUOLEditor::ListBox>();

		auto&& allAudioClips = DUOLGameEngine::ResourceManager::GetInstance()->GetAllAudioClips();

		for (auto& [name, audioClip] : allAudioClips)
		{
			if (audioClip != nullptr)
				audioClipList->AddChoice(name);
		}

		// ������Ʈ �˻� ��ɿ��� �̸��� �ٲ���� �� Component List���� �ش� �̸��� ���� �༮�鸸 ���̰� �մϴ�.
		audioClipSearch->_textChangedEvent += [this, audioClipList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = audioClipList->_choices;

			auto& viewChoices = audioClipList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No audio clips with that name") });
			}
		};

		// �޽��� �ٲ��ݽô� ..!
		audioClipList->_choiceChangedEvent += [this, audioSource](const DUOLCommon::tstring& clipName)
		{
			// Set Mesh
			DUOLGameEngine::AudioClip* audioClip = DUOLGameEngine::ResourceManager::GetInstance()->GetAudioClip(clipName);

			audioSource->SetAudioClip(audioClip);
		};

		// Add Component ��ư ���� Ű��
		textClickable->_clickedEvent += [this, audioUI]()
		{
			bool enable = audioUI->GetIsEnable();

			audioUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawUIFileName(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::Image* image)
	{
		using namespace rttr;

		variant var = property.get_value(obj);

		auto gatherer = [image]()
		{
			auto imageSprite = image->GetSprite();

			return imageSprite == nullptr ? DUOLCommon::tstring(TEXT("None (Sprite)")) : image->GetSpritePathName();
		};


		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [image]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					if (rePathExtension == ".png")
					{
						// �̹� �ֳ��� ..?
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllUIInformation(textClickable, image);

	}

	void Inspector::DrawAllUIInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::Image* image)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search UI Sprite"));

		auto acSearch = column->AddWidget<DUOLEditor::InputText>();

		auto acList = meshUI->AddWidget<DUOLEditor::ListBox>();

		auto allUiImageList = DUOLGameEngine::UIManager::GetInstance()->GetSpriteFileList();

		for (auto uiFilename : allUiImageList)
		{
			acList->AddChoice(uiFilename);
		}

		acSearch->_textChangedEvent += [this, acList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = acList->_choices;

			auto& viewChoices = acList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No sprite with that name") });
			}
		};

		// UI�� �ٲ��ݴϴ�.
		acList->_choiceChangedEvent += [this, image](const DUOLCommon::tstring& uiName)
		{
			if (image != nullptr)
				image->LoadTexture(uiName);
		};

		// ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawUIFileName(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::Button* button)
	{
		using namespace rttr;

		variant var = property.get_value(obj);

		auto gatherer = [button]()
		{
			auto buttonSprite = button->GetSprite();

			return buttonSprite == nullptr ? DUOLCommon::tstring(TEXT("None (Sprite)")) : button->GetSprite()->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [button]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Button ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					if (rePathExtension == ".png")
					{
						// �̹� �ֳ��� ..?
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllUIInformation(textClickable, button);

	}

	void Inspector::DrawAllUIInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::Button* button)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search UI Sprite"));

		auto acSearch = column->AddWidget<DUOLEditor::InputText>();

		auto acList = meshUI->AddWidget<DUOLEditor::ListBox>();

		auto allUiImageList = DUOLGameEngine::UIManager::GetInstance()->GetSpriteFileList();

		for (auto uiFilename : allUiImageList)
		{
			acList->AddChoice(uiFilename);
		}

		acSearch->_textChangedEvent += [this, acList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = acList->_choices;

			auto& viewChoices = acList->_viewChoices;

			// �ϴ� ���̴� Choice List�� ���
			viewChoices.clear();

			viewChoices.insert({ 0, TEXT("None") });

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
				viewChoices.insert({ 1000000, TEXT("There's No sprite with that name") });
			}
		};

		// UI�� �ٲ��ݴϴ�.
		acList->_choiceChangedEvent += [this, button](const DUOLCommon::tstring& uiName)
		{
			if (button != nullptr)
			{
				button->SetDownSprite(uiName);
			}
			//button->LoadTexture(uiName);
		};

		// ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	/**
	 * \brief
	 * \param rootWidget
	 * \param property
	 * \param obj
	 * \param button
	 */
	void Inspector::DrawButtonFileName(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property,
		rttr::instance obj, DUOLGameEngine::Button* button)
	{
		using namespace rttr;

		// ������ �����Ѵ�.
		rootWidget->AddWidget<DUOLEditor::Separator>();

		rootWidget->AddWidget<DUOLEditor::NewLine>();

		// On Click Tile
		DUOLEditor::ImGuiHelper::DrawTitle(rootWidget, TEXT("On Click()"));

		rootWidget->AddWidget<DUOLEditor::Dummy>();

		variant var = property.get_value(obj);

		// Button�� OnClick�� �ҷ��´�. 
		auto onclick = button->GetOnClick();

		// ���� OnClick vector�� ��������� �׳� ����ִٰ� ���
		if (onclick.empty())
		{
			rootWidget->AddWidget<DUOLEditor::Dummy>();

			DUOLEditor::ImGuiHelper::DrawTitle(rootWidget, TEXT("List is Empty"));
		}
		// ���� �Լ����� �־��ݴϴ�. 
		else
		{
			// click �����ŭ ����ؾ��Ѵ�. 
			for (auto clickevent : onclick)
			{
				// click�� �޾ƿ´�.
				const instance obj = clickevent;

				DrawUIProperty(rootWidget, obj, clickevent);

				// click�ȿ� �ִ� onclick call �Լ��� �޾ƿ´�.
				auto clickcallevent = clickevent->GetCalls();
				const instance objsecond = *clickcallevent;

				DrawUIProperty(rootWidget, objsecond, clickcallevent);

			}

		}

		auto plusButton = rootWidget->AddWidget< DUOLEditor::Button>(TEXT("+"));
		auto minusButton = rootWidget->AddWidget< DUOLEditor::Button>(TEXT("-"));

		// �̺�Ʈ�� �־��ݴϴ�. �׸��� â�� �ٽ� �׷�����մϴ�. 
		plusButton->_clickedEvent += [this, button]()
		{
			button->CreateOnClick();

			SetInspectedGameObject(_selectedGameObject);
		};

		minusButton->_clickedEvent += [this, button]()
		{
			button->DeleteOnClick();

			SetInspectedGameObject(_selectedGameObject);
		};

		auto callbackAfter = [button]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("CONTENTS_BROWSER_ITEM", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Button ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLCommon::tstring relativePath = DUOLCommon::StringHelper::ToTString(reinterpret_cast<const wchar_t*>(payload->Data));

					std::filesystem::path rePath = relativePath;

					std::filesystem::path rePathExtension = rePath.extension();

					if (rePathExtension == ".png")
					{
						// �̹� �ֳ��� ..?
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		rootWidget->AddWidget<DUOLEditor::Separator>();

		rootWidget->AddWidget<DUOLEditor::NewLine>();
	}

	void Inspector::DrawOnClickCallFileName(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::OnClickCall* onclickcall)
	{
		using namespace rttr;

		variant var = property.get_value(obj);

		auto gatherer = [onclickcall]()
		{
			auto clickGameObject = onclickcall->GetTargetObject();

			return clickGameObject == nullptr ? DUOLCommon::tstring(TEXT("None (Object)")) : onclickcall->GetName();
		};

		auto provider = [obj, property](DUOLCommon::tstring name)
		{
			// ���� �ش� UI�κ��� ���޹��� �ʽ��ϴ�.
		};

		auto callbackAfter = [onclickcall]()
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("GameObject", ImGuiDragDropFlags_AcceptBeforeDelivery);

				// Content_Browser_Item ����.
				if (payload != nullptr && payload->IsDelivery())
				{
					DUOLGameEngine::GameObject* gameobject = reinterpret_cast<DUOLGameEngine::GameObject*>(payload->Data);

					if (gameobject != nullptr)
					{
						onclickcall->SetTargetGameObject(gameobject);
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawOnClickCallInformation(textClickable, onclickcall);

	}

	void Inspector::DrawOnClickCallInformation(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::OnClickCall* onclickcall)
	{
		using namespace rttr;

		auto meshUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		meshUI->SetIsEnable(false);

		auto column = meshUI->AddWidget<DUOLEditor::Columns<2>>();


		/*for (auto uiFilename : allUiImageList)
		{
			acList->AddChoice(uiFilename);
		}*/

		//auto acSearch = column->AddWidget<DUOLEditor::InputText>();

		//auto acList = meshUI->AddWidget<DUOLEditor::ListBox>();


		//acSearch->_textChangedEvent += [this, acList](const DUOLCommon::tstring& name)
		//{
		//	auto text = name;

		//	std::transform(text.begin(), text.end(), text.begin(), ::tolower);

		//	auto& allChoices = acList->_choices;

		//	auto& viewChoices = acList->_viewChoices;

		//	// �ϴ� ���̴� Choice List�� ���
		//	viewChoices.clear();

		//	viewChoices.insert({ 0, TEXT("None") });

		//	// �ƹ� ���뵵 ����.
		//	if (name.empty())
		//	{
		//		// ���� �� �־�
		//		for (auto [key, value] : allChoices)
		//			viewChoices.insert({ key, value });

		//		return;
		//	}

		//	// ��� ���ÿ��� �̸��� ���� �༮�� ������ viewChoices�� �ִ´�
		//	for (auto [key, value] : allChoices)
		//	{
		//		auto choiceValue = value;

		//		std::transform(choiceValue.begin(), choiceValue.end(), choiceValue.begin(), ::tolower);

		//		if (choiceValue.find(text) != DUOLCommon::tstring::npos)
		//		{
		//			viewChoices.insert({ key, value });
		//		}
		//	}

		//	// �˻��� ������ ������ �̰Ŷ� �־�����
		//	if (viewChoices.empty())
		//	{
		//		viewChoices.insert({ 1000000, TEXT("There's No sprite with that name") });
		//	}
		//};

		//// UI�� �ٲ��ݴϴ�.
		//acList->_choiceChangedEvent += [this, button](const DUOLCommon::tstring& uiName)
		//{
		//	if (button != nullptr)
		//	{
		//		button->SetDownSprite(uiName);
		//	}
		//	//button->LoadTexture(uiName);
		//};

		// ��ư ���� Ű��
		textClickable->_clickedEvent += [this, meshUI]()
		{
			bool enable = meshUI->GetIsEnable();

			meshUI->SetIsEnable(!enable);
		};
	}

	void Inspector::DrawMaterial(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj,
		DUOLGameEngine::RendererBase* rendererBase)
	{
		using namespace rttr;

		variant var = property.get_value(obj);

		auto gatherer = [rendererBase]()
		{
			auto materials = rendererBase->GetMaterials();

			return materials.empty() ? DUOLCommon::tstring(TEXT("Empty")) : rendererBase->GetMaterialName();
		};

		auto provider = [obj, property](DUOLCommon::tstring enumeration)
		{
		};

		auto callbackAfter = [rendererBase]()
		{

		};

		auto textClickable = DUOLEditor::ImGuiHelper::DrawStringNoInput(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider, callbackAfter);

		DrawAllHaveMaterial(textClickable, rendererBase);

		DrawAllMaterial(textClickable, rendererBase);
	}

	void Inspector::DrawAllMaterial(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::RendererBase* rendererBase)
	{
		using namespace rttr;

		auto materialUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		materialUI->SetIsEnable(false);

		auto column = materialUI->AddWidget<DUOLEditor::Columns<2>>();

		DUOLEditor::ImGuiHelper::DrawTitle(column, TEXT("Search Material"));

		auto materialSearch = column->AddWidget<DUOLEditor::InputText>();

		auto materialList = materialUI->AddWidget<DUOLEditor::ListBox>();

		auto&& allMaterials = DUOLGameEngine::ResourceManager::GetInstance()->GetAllMaterialMap();

		for (auto& [name, audioClip] : allMaterials)
		{
			if (audioClip != nullptr)
				materialList->AddChoice(name);
		}

		// ������Ʈ �˻� ��ɿ��� �̸��� �ٲ���� �� Component List���� �ش� �̸��� ���� �༮�鸸 ���̰� �մϴ�.
		materialSearch->_textChangedEvent += [this, materialList](const DUOLCommon::tstring& name)
		{
			auto text = name;

			std::transform(text.begin(), text.end(), text.begin(), ::tolower);

			auto& allChoices = materialList->_choices;

			auto& viewChoices = materialList->_viewChoices;

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
				viewChoices.insert({ 1000000, TEXT("There's No material with that name") });
			}
		};

		// ���͸����� �ٲ��ݴϴ�.
		materialList->_choiceChangedEvent += [this, rendererBase](const DUOLCommon::tstring& materialName)
		{
			auto material = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(materialName);

			rendererBase->AddMaterial(material);

			SetInspectedGameObject(_selectedGameObject);
		};

		materialUI->SetIsEnable(true);
	}

	void Inspector::DrawAllHaveMaterial(DUOLEditor::TextClickable* textClickable, DUOLGameEngine::RendererBase* rendererBase)
	{
		using namespace rttr;

		auto materials = rendererBase->GetMaterials();

		auto materialUI = _gameObjectInfo->AddWidget<DUOLEditor::Container>();

		materialUI->SetIsEnable(false);

		auto materialList = materialUI->AddWidget<DUOLEditor::ListBox>();

		auto&& allMaterials = materials;

		bool eraseNone = false;

		for (auto& iter : allMaterials)
		{
			if (iter != nullptr)
			{
				if(!eraseNone)
				{
					materialList->DeleteChoice();
					eraseNone = true;
				}

				materialList->AddChoice(iter->GetName());
			}
		}

		auto allChoices = materialList->_choices;

		for (auto& [key, name] : allChoices)
		{
			materialList->_viewChoices.insert({ key, name });
		}

		materialUI->SetIsEnable(true);

		auto deleteButton = _gameObjectInfo->AddWidget<DUOLEditor::Button>(TEXT("DeleteBack"));

		materialList->_valueChangedEvent += [rendererBase](int value)
		{
			rendererBase->SetCurrentSelectedMaterial(value);
		};

		deleteButton->_clickedEvent += [this, rendererBase]()
		{
			rendererBase->DeleteBackMaterial();

			SetInspectedGameObject(_selectedGameObject);
		};
	}
}