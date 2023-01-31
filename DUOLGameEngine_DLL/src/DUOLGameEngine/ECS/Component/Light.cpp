#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/EventManager.h"

#include <rttr/registration>
#include <rttr/policy.h>
#include "DUOLCommon/MetaDataType.h"


using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::LightType>("LightType")
	(
		value("Directional", DUOLGameEngine::LightType::Directional)
		, value("Spot", DUOLGameEngine::LightType::Spot)
		, value("Point", DUOLGameEngine::LightType::Point)
	);

	rttr::registration::class_<DUOLGameEngine::Light>("Light")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&>()
	(
		policy::ctor::as_std_shared_ptr
	)
	.property("Type", &DUOLGameEngine::Light::GetLightType, &DUOLGameEngine::Light::SetLightType)
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
	.property("Range", &DUOLGameEngine::Light::GetRange, &DUOLGameEngine::Light::SetRange)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Attenuation", &DUOLGameEngine::Light::GetAttenuation, &DUOLGameEngine::Light::SetAttenuation)
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
	);
}

namespace DUOLGameEngine
{
	Light::Light(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _lightInfo{}
		, _cbPerFrame(nullptr)
	{
		// 미리 세팅해줍니다. 유일한 존재이니까 이렇게 놔둬도 별 문제 없지 않을까 ?
		_cbPerFrame = DUOLGameEngine::GraphicsManager::GetInstance()->GetConstantBufferPerFrame();
	}

	Light::~Light()
	{

	}

	DUOLGameEngine::LightType Light::GetLightType() const
	{
		return static_cast<DUOLGameEngine::LightType>(_lightInfo._lightType);
	}

	void Light::SetLightType(DUOLGameEngine::LightType lightType)
	{
		_lightInfo._lightType = static_cast<DUOLGraphicsEngine::LightType>(lightType);
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

	float Light::GetRange() const
	{
		return _lightInfo._range;
	}

	void Light::SetRange(float range)
	{
		if (range <= 0.f)
		{
			_lightInfo._range = 0.f;

			return;
		}

		_lightInfo._range = range;
	}

	const DUOLMath::Vector3& Light::GetColor() const
	{
		return _lightInfo._color;
	}

	void Light::SetColor(const DUOLMath::Vector3& color)
	{
		_lightInfo._color = color;
	}

	float Light::GetIntensity() const
	{
		return _lightInfo._intensity;
	}

	void Light::SetIntensity(float intensity)
	{
		if (intensity <= 0.f)
		{
			_lightInfo._intensity = 0.f;

			return;
		}

		_lightInfo._intensity = intensity;
	}

	float Light::GetAttenuation() const
	{
		return _lightInfo._attenuation;
	}

	void Light::SetAttenuation(float attenuation)
	{
		if (attenuation <= 0.f)
		{
			_lightInfo._attenuation = 0.f;

			return;
		}

		_lightInfo._attenuation = attenuation;
	}

	float Light::GetAttenuationRadius() const
	{
		return _lightInfo._attenuationRadius;
	}

	void Light::SetAttenuationRadius(float radius)
	{
		_lightInfo._attenuationRadius = radius;
	}

	void Light::OnSceneLighting()
	{
		// 활성화 상태가 아니라면 보낼 필요가 없습니다.
		if (!_isEnabled)
			return;

		memcpy(&_cbPerFrame->_light[(_cbPerFrame->_lightCount)++], &_lightInfo, sizeof(DUOLGraphicsEngine::Light));
	}

	void Light::OnUpdate(float deltaTime)
	{
		_lightInfo._position = GetTransform()->GetWorldPosition();

		_lightInfo._direction = GetTransform()->GetLook();
	}

	void Light::OnEnable()
	{
		std::function<void()> functor = std::bind(&Light::OnSceneLighting, this);

		_idOfSceneLighting = EventManager::GetInstance()->AddEventFunction(TEXT("SceneLighting"), functor);
	}

	void Light::OnDisable()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneLighting"), _idOfSceneLighting);
	}
}