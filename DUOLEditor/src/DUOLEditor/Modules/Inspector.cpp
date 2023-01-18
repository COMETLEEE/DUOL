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
	}

	void Inspector::DrawGameObjectInformation()
	{
		// 현재 있는 위젯들은 모두 삭제합니다 !
		_gameObjectInfo->RemoveAllWidgets();

		// Transform 먼저 그려줍니다.
		DUOLGameEngine::Transform* transform = _selectedGameObject->GetTransform();

		DrawTransformInformation(transform);
	}

	void Inspector::DrawTransformInformation(DUOLGameEngine::Transform* transform)
	{
		auto header = _gameObjectInfo->AddWidget<ContainerCollapsable>(TEXT("Transform"));

		auto columns = header->AddWidget<DUOLEditor::Columns<2>>();

		columns->_widths[0] = 200;

		auto positionInfo = columns->AddWidget<TextColored>(TEXT("Position"), TitleColor);

		auto positionWidget = columns->AddWidget<DragScalar<float, 3>>
			(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f, 0.05f, TEXT(""), TEXT("%.3f"));

		auto positionGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalPosition, transform);

		auto positionProvider = std::bind(&DUOLGameEngine::Transform::SetLocalPosition, transform, std::placeholders::_1);

		positionWidget->RegisterGatherer([positionGatherer]()
			{
				DUOLMath::Vector3 position = positionGatherer();

				return reinterpret_cast<const std::array<float, 3>&>(position);
			});

		positionWidget->RegisterProvider([positionProvider](std::array<float, 3>* value)
			{
				positionProvider(reinterpret_cast<DUOLMath::Vector3&>(*value));
			});

		auto rotationInfo = columns->AddWidget<TextColored>(TEXT("Rotation"), TitleColor);

		auto rotationWidget = columns->AddWidget<DragScalar<float, 3>>
			(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f, 0.05f, TEXT(""), TEXT("%.3f"));

		auto rotationGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalEulerAngle, transform);

		auto rotationProvider = std::bind(&DUOLGameEngine::Transform::SetLocalEulerAngle, transform, std::placeholders::_1);

		rotationWidget->RegisterGatherer([rotationGatherer]()
			{
				DUOLMath::Vector3 euler = rotationGatherer();

				return reinterpret_cast<const std::array<float, 3>&>(euler);
			});

		rotationWidget->RegisterProvider([rotationProvider](std::array<float, 3>* value)
			{
				rotationProvider(reinterpret_cast<DUOLMath::Vector3&>(*value));
			});

		auto scaleInfo = columns->AddWidget<TextColored>(TEXT("Scale"), TitleColor);

		auto scaleWidget = columns->AddWidget<DragScalar<float, 3>>
			(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f, 0.05f, TEXT(""), TEXT("%.3f"));

		auto scaleGatherer = std::bind(&DUOLGameEngine::Transform::GetLocalScale, transform);

		auto scaleProvider = std::bind(&DUOLGameEngine::Transform::SetLocalScale, transform, std::placeholders::_1);

		scaleWidget->RegisterGatherer([scaleGatherer]()
			{
				DUOLMath::Vector3 scale = scaleGatherer();

				return reinterpret_cast<const std::array<float, 3>&>(scale);
			});

		scaleWidget->RegisterProvider([scaleProvider](std::array<float, 3>* value)
			{
				scaleProvider(reinterpret_cast<DUOLMath::Vector3&>(*value));
			});
	}
}