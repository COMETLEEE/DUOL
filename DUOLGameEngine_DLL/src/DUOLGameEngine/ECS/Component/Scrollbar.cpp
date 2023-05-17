#include "DUOLGameEngine/ECS/Component/Scrollbar.h"
#include <rttr/registration>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Object/Sprite.h"
#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::DIRECTIONSTAT>("ScrollDirection")
	(
		value("RIGHT_TO_LEFT", DUOLGameEngine::DIRECTIONSTAT::RIGHT_TO_LEFT)
		, value("LEFT_TO_RIGHT", DUOLGameEngine::DIRECTIONSTAT::LEFT_TO_RIGHT)
		, value("DOWN_TO_TOP", DUOLGameEngine::DIRECTIONSTAT::DOWN_TO_TOP)
		, value("TOP_TO_DOWN", DUOLGameEngine::DIRECTIONSTAT::TOP_TO_DOWN)
	);
	rttr::registration::class_<DUOLGameEngine::Scrollbar>("Scrollbar")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("DIRECTION", &DUOLGameEngine::Scrollbar::GetDirectionState, &DUOLGameEngine::Scrollbar::SetDirectionState)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
	)
	.property("maxGauge", &DUOLGameEngine::Scrollbar::GetMaxGauge, &DUOLGameEngine::Scrollbar::SetMaxGauge)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("nowGauge", &DUOLGameEngine::Scrollbar::GetNowGauge, &DUOLGameEngine::Scrollbar::SetNowGauge)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

DUOLGameEngine::Scrollbar::Scrollbar() :
	BehaviourBase(nullptr, TEXT("Scrollbar"))
	, _scrollImage()
	, _maxGauge(100.0f)
	, _nowGauge(1.0f)
{
}

DUOLGameEngine::Scrollbar::Scrollbar(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	BehaviourBase(owner, name)
	, _scrollImage()
	, _maxGauge(100.0f)
	, _nowGauge(1.0f)
{
	Initialize(owner);
}

DUOLGameEngine::Scrollbar::~Scrollbar()
{
}

void DUOLGameEngine::Scrollbar::OnUpdate(float deltaTime)
{
}

void DUOLGameEngine::Scrollbar::Initialize(DUOLGameEngine::GameObject* owner)
{
	// 자식 객체를 만들어 줍니다.
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	DUOLGameEngine::GameObject* childObject = scene->CreateEmtpyUI();

	childObject->AddComponent<Image>();

	childObject->SetName(L"Handle");

	// 자식으로 넣어준다.
	childObject->GetComponent<Transform>()->SetParent(owner->GetComponent<Transform>());

	_scrollImage = childObject;

	owner->AddComponent<Image>();

	owner->SetName(L"Scrollbar");

	auto scrollbarImage = childObject->GetComponent<Image>();

	auto handleImage = owner->GetComponent<Image>();

	// 초기 설정시 핸들의 이미지가 더 Layer가 높게 설정해준다. 
	scrollbarImage->SetLayer(2);

	handleImage->SetLayer(1);
}

DUOLGraphicsLibrary::Rect& DUOLGameEngine::Scrollbar::CalculateRect(const DUOLGraphicsLibrary::Rect& parentRect)
{
	// Scrollbar이미지는 BackGround에 종속되있다.
	DUOLGraphicsLibrary::Rect rect;

	float ratio = 1;

	switch (_directionState)
	{
	case DIRECTIONSTAT::LEFT_TO_RIGHT:
	{
		// -> 기준
		rect.left = parentRect.left;
		rect.top = parentRect.top;
		rect.bottom = parentRect.bottom;

		if (_maxGauge != 0)
			ratio = _nowGauge / _maxGauge;

		rect.right = parentRect.left + ((parentRect.right - parentRect.left) * ratio);
	}
	break;
	case DIRECTIONSTAT::RIGHT_TO_LEFT:
	{
		// <- 기준
		rect.right = parentRect.right;
		rect.top = parentRect.top;
		rect.bottom = parentRect.bottom;

		if (_maxGauge != 0)
			ratio = _nowGauge / _maxGauge;

		rect.left = parentRect.right -((parentRect.right-parentRect.left) * ratio);

	}
	break;
	case DIRECTIONSTAT::DOWN_TO_TOP:
	{
		rect.right = parentRect.right;
		rect.left = parentRect.left;
		rect.bottom = parentRect.bottom;

		if (_maxGauge != 0)
			ratio = _nowGauge / _maxGauge;

		rect.top = parentRect.bottom - ((parentRect.bottom - parentRect.top) * ratio);
	}
	break;
	case DIRECTIONSTAT::TOP_TO_DOWN:
	{
		rect.right = parentRect.right;
		rect.top = parentRect.top;
		rect.left = parentRect.left;

		if (_maxGauge != 0)
			ratio = _nowGauge / _maxGauge;

		rect.bottom = parentRect.top + ((parentRect.bottom - parentRect.top) * ratio);
	}
	break;

	}

	return rect;
}
