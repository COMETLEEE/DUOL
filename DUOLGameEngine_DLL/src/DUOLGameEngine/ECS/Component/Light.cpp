#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/EventManager.h"

#include <rttr/registration>
#include <rttr/policy.h>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::LightType>("LightType")
	(
		value("Directional", DUOLGameEngine::LightType::Directional)
		, value("Spot", DUOLGameEngine::LightType::Spot)
		, value("Point", DUOLGameEngine::LightType::Point)
		, value("Area", DUOLGameEngine::LightType::Area)
	);

	rttr::registration::enumeration<DUOLGameEngine::LightState>("LightState")
	(
		value("Static", DUOLGameEngine::LightState::Static)
		, value("Mixed", DUOLGameEngine::LightState::Mixed)
		, value("Dynamic", DUOLGameEngine::LightState::Dynamic)
	);

	rttr::registration::class_<DUOLGameEngine::Light>("Light")
		.constructor()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
		(
			policy::ctor::as_raw_ptr
		)
		.property("Type", &DUOLGameEngine::Light::GetLightType, &DUOLGameEngine::Light::SetLightType)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
		)
		.property("ShadowState", &DUOLGameEngine::Light::GetLightState, &DUOLGameEngine::Light::SetLightState)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
			)
		.property("Color", &DUOLGameEngine::Light::GetColor, &DUOLGameEngine::Light::SetColor)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Color)
		)
		.property("Intensity", &DUOLGameEngine::Light::GetIntensity, &DUOLGameEngine::Light::SetIntensity)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
		.property("Angle", &DUOLGameEngine::Light::GetAngle, &DUOLGameEngine::Light::SetAngle)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
		.property("FallOffExponential", &DUOLGameEngine::Light::GetFallOffExponential, &DUOLGameEngine::Light::SetFallOffExponential)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
		.property("AttenuationRadius", &DUOLGameEngine::Light::GetAttenuationRadius, &DUOLGameEngine::Light::SetAttenuationRadius)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
		.property("Width", &DUOLGameEngine::Light::GetWidth, &DUOLGameEngine::Light::SetWidth)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
		.property("Height", &DUOLGameEngine::Light::GetHeight, &DUOLGameEngine::Light::SetHeight)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		);
}

namespace DUOLGameEngine
{
	Light::Light() :
		BehaviourBase(nullptr, TEXT("Light"))
		, _lightInfo(nullptr)
		, _currentSceneInfo(nullptr)
		, _idOfSceneLighting(UINT64_MAX)
	{
		//todo:: 초기화할때 uuid를 가져올 방법이 있는가?

		_lightID = id;

		_lightInfo = DUOLGameEngine::ResourceManager::GetInstance()->CreateLight(id++);
		_currentSceneInfo = DUOLGameEngine::GraphicsManager::GetInstance()->GetCurrentSceneInfo();
	}

	Light::Light(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _lightInfo(nullptr)
		, _currentSceneInfo(nullptr)
		, _idOfSceneLighting(UINT64_MAX)
	{
		
		// 미리 세팅해줍니다. 유일한 존재이니까 이렇게 놔둬도 별 문제 없지 않을까 ?
		_lightID = id;

		_lightInfo = DUOLGameEngine::ResourceManager::GetInstance()->CreateLight(id++);
		_currentSceneInfo = DUOLGameEngine::GraphicsManager::GetInstance()->GetCurrentSceneInfo();
	}

	Light::~Light()
	{
		DUOLGameEngine::ResourceManager::GetInstance()->DeleteLight(_lightID);

		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneLighting"), _idOfSceneLighting);
	}

	DUOLGameEngine::LightType Light::GetLightType() const
	{
		return static_cast<DUOLGameEngine::LightType>(_lightInfo->GetLightType());
	}

	void Light::SetLightType(DUOLGameEngine::LightType lightType)
	{
		_lightInfo->SetLightType(static_cast<DUOLGraphicsEngine::LightType>(lightType));
	}

	DUOLGameEngine::LightState Light::GetLightState() const
	{
		return static_cast<LightState>(_lightInfo->GetLightState());
	}

	void Light::SetLightState(DUOLGameEngine::LightState lightState)
	{
		_lightInfo->SetLightState(static_cast<DUOLGraphicsEngine::LightState>(lightState));
	}

	const DUOLMath::Vector3& Light::GetDirection() const
	{
		// Directional => Look vector.
		return GetTransform()->GetLook();
	}

	const DUOLMath::Vector3& Light::GetPosition() const
	{
		return GetTransform()->GetWorldPosition();
	}

	float Light::GetAngle() const
	{
		return _lightInfo->GetAngle();
	}

	void Light::SetAngle(float range)
	{
		return _lightInfo->SetAngle(range);
	}

	const DUOLMath::Vector3& Light::GetColor() const
	{
		return _lightInfo->GetLightData()._color;
	}

	void Light::SetColor(const DUOLMath::Vector3& color)
	{
		_lightInfo->SetColor(color);
	}

	float Light::GetIntensity() const
	{
		return _lightInfo->GetIntensity();
	}

	void Light::SetIntensity(float intensity)
	{
		_lightInfo->SetIntensity(intensity);
	}

	float Light::GetFallOffExponential() const
	{
		return _lightInfo->GetFallOffExponential();
	}

	void Light::SetFallOffExponential(float fallOffExponential)
	{
		_lightInfo->SetFallOffExponential(fallOffExponential);
	}

	float Light::GetAttenuationRadius() const
	{
		return _lightInfo->GetAttenuationRadius();
	}

	void Light::SetAttenuationRadius(float radius)
	{
		_lightInfo->SetAttenuationRadius(radius);
	}

	float Light::GetWidth() const
	{
		return _lightInfo->GetWidth();
	}

	void Light::SetWidth(float width)
	{
		return _lightInfo->SetWidth(width);
	}

	float Light::GetHeight() const
	{
		return _lightInfo->GetHeight();
	}

	void Light::SetHeight(float height)
	{
		_lightInfo->SetHeight(height);
	}

	void Light::OnSceneLighting()
	{
		//라이트스페이스 공간이 없을때 빈자리가 있으면 요청합니다.
		_lightInfo->TryGetShadowMapSpace();

		_lightInfo->SetPosition(GetTransform()->GetWorldPosition());
		_lightInfo->SetDirection(GetTransform()->GetLook());
		_lightInfo->SetUp(GetTransform()->GetUp());

		//set한 정보를 통해 viewmatrix를 만듭니다
		_lightInfo->SetLightWorldMatrix();

		_currentSceneInfo->_lights[(_currentSceneInfo->_lightCount)++] = _lightInfo ;
	}

	void Light::OnEnable()
	{
		if (_idOfSceneLighting == UINT64_MAX)
		{
			std::function<void()> functor = std::bind(&Light::OnSceneLighting, this);

			_idOfSceneLighting = EventManager::GetInstance()->AddEventFunction(TEXT("SceneLighting"), functor);
		}
	}

	void Light::OnDisable()
	{
		if (_idOfSceneLighting != UINT64_MAX)
		{
			EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneLighting"), _idOfSceneLighting);
			
			_idOfSceneLighting = UINT64_MAX;
		}
	}
}