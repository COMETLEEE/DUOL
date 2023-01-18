#include "DUOLCommon/MetaDataType.h"

#include "DUOLEditor/Modules/Inspector.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Layout/Columns.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"
#include "DUOLEditor/UI/Widgets/Texts/TextColored.h"
#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"
#include "DUOLEditor/UI/Widgets/Layout/ContainerCollapsable.h"
#include "DUOLEditor/UI/Widgets/Selections/CheckBox.h"
#include "DUOLEditor/UI/Widgets/Drags/DragScalar.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <array>

#include "rttr/type.h"

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
		auto nameInfo = headerColumns->AddWidget<DUOLEditor::TextColored>(TEXT("Name"), TitleColor);

		auto nameText = headerColumns->AddWidget<DUOLEditor::InputText>();

		auto nameGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetName() : TEXT(""); };

		auto nameProvider = [this](const DUOLCommon::tstring* newName) { if (_selectedGameObject != nullptr) _selectedGameObject->SetName(*newName); };

		nameText->RegisterGatherer(nameGatherer);

		nameText->RegisterProvider(nameProvider);

		// Tag
		auto tagInfo = headerColumns->AddWidget<DUOLEditor::TextColored>(TEXT("Tag"), TitleColor);

		auto tagText = headerColumns->AddWidget<DUOLEditor::InputText>();

		auto tagGatherer = [this] { return _selectedGameObject != nullptr ? _selectedGameObject->GetTag() : TEXT(""); };

		auto tagProvider = [this](const DUOLCommon::tstring* newTag) { if (_selectedGameObject != nullptr) _selectedGameObject->SetTag(*newTag); };

		tagText->RegisterGatherer(nameGatherer);

		tagText->RegisterProvider(nameProvider);

		// Active
		auto activeInfo = headerColumns->AddWidget<DUOLEditor::TextColored>(TEXT("Active"), TitleColor);

		auto activeBox = headerColumns->AddWidget<DUOLEditor::CheckBox>();

		auto activeGatherer = [this]{ return _selectedGameObject != nullptr ? _selectedGameObject->GetIsActive() : false; };

		auto activeProvider = [this](const bool* value) { if (_selectedGameObject != nullptr) _selectedGameObject->SetIsActive(*value); };

		activeBox->RegisterGatherer(activeGatherer);

		activeBox->RegisterProvider(activeProvider);
#pragma endregion
		
#pragma region GAMEOBJECT_INFO
		_gameObjectInfo = AddWidget<Container>();
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
			UnsetInspectedGameObject();

		// �� ���� ������Ʈ�� ���õǾ����ϴ�.
		_selectedGameObject = gameObject;

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

	void Inspector::DrawGameObjectInformation()
	{
		// '_selectedGameObject'�� ���÷����� ���� �ν��Ͻ�ȭ�ϰ� �׳� �׷������� .. ������ �� ��

		// Transform ���� �׷��ݴϴ�.
		DUOLGameEngine::Transform* transform = _selectedGameObject->GetTransform();

		DrawComponentInformation(transform);
	}

	void Inspector::DrawTransformInformation(DUOLGameEngine::Transform* transform)
	{
		auto header = _gameObjectInfo->AddWidget<ContainerCollapsable>(TEXT("Transform"));

		auto columns = header->AddWidget<DUOLEditor::Columns<2>>();
		
		auto positionGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalPosition, transform);

		auto positionProvider = std::bind(&DUOLGameEngine::Transform::SetLocalPosition, transform, std::placeholders::_1);

		ImGuiHelper::DrawVector3(columns, TEXT("Position"), positionGatherer, positionProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		auto rotationGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalEulerAngle, transform);

		auto rotationProvider = std::bind(&DUOLGameEngine::Transform::SetLocalEulerAngle, transform, std::placeholders::_1);

		ImGuiHelper::DrawVector3(columns, TEXT("Rotation"), rotationGatherer, rotationProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		auto scaleGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalScale, transform);

		auto scaleProvider = std::bind(&DUOLGameEngine::Transform::SetLocalScale, transform, std::placeholders::_1);

		ImGuiHelper::DrawVector3(columns, TEXT("Scale"), scaleGatherer, scaleProvider, 0.05f,
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	}

	void Inspector::DrawComponentInformation(DUOLGameEngine::ComponentBase* component)
	{
		// RTTR library - Reflection�� ����մϴ�.
		using namespace rttr;

		const auto header = _gameObjectInfo->AddWidget<ContainerCollapsable>(component->GetName());

		const auto columns = header->AddWidget<DUOLEditor::Columns<2>>();

		const instance obj = *component;

		const type class_type = type::get_by_name(DUOLCommon::StringHelper::ToString(component->GetName()));

		auto properties = class_type.get_properties();

		for (auto& property : properties)
		{
			if (IsSerializable(property))
				DrawVector3_If(columns, property, obj);
		}
	}

	bool Inspector::IsSerializable(rttr::property property)
	{
		using namespace rttr;

		const variant metaData = property.get_metadata(DUOLCommon::MetaDataType::Serializable);

		return metaData.is_valid() ? metaData.get_value<bool>() : false;
	}

	bool Inspector::DrawVector3_If(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj)
	{
		using namespace rttr;

		variant metaData = property.get_metadata(DUOLCommon::MetaDataType::Draw_Vector3);

		if (metaData.is_valid())
		{
			if (metaData.get_value<bool>() == true)
			{
				variant var = property.get_value(obj);

				DUOLMath::Vector3& value = var.get_value<DUOLMath::Vector3>();

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

				DUOLEditor::ImGuiHelper::DrawVector3(rootWidget, DUOLCommon::StringHelper::ToTString(property.get_name().data()), gatherer, provider,
					0.05f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

				return true;
			}
		}

		return false;
	}
}