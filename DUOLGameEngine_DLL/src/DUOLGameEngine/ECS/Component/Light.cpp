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
		, value("Area", DUOLGameEngine::LightType::Area)
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
		, _lightInfo({})
		, _cbPerFrame(nullptr)
	{
		_cbPerFrame = DUOLGameEngine::GraphicsManager::GetInstance()->GetConstantBufferPerFrame();
	}

	Light::Light(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _lightInfo{}
		, _cbPerFrame(nullptr)
	{
		// 미리 세팅해줍니다. 유일한 존재이니까 이렇게 놔둬도 별 문제 없지 않을까 ?
		_cbPerFrame = DUOLGameEngine::GraphicsManager::GetInstance()->GetConstantBufferPerFrame();
	}

	Light::~Light()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneLighting"), _idOfSceneLighting);
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

	float Light::GetAngle() const
	{
		return _lightInfo._angle;
	}

	void Light::SetAngle(float range)
	{
		if (range <= 0.f)
		{
			_lightInfo._angle = 0.f;

			return;
		}

		_lightInfo._angle = range;
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

	float Light::GetFallOffExponential() const
	{
		return _lightInfo._innerAngle;
	}

	void Light::SetFallOffExponential(float fallOffExponential)
	{
		if (fallOffExponential <= 0.f)
		{
			_lightInfo._fallOffExponential = 0.f;

			return;
		}

		_lightInfo._fallOffExponential = fallOffExponential;
	}

	float Light::GetAttenuationRadius() const
	{
		return _lightInfo._attenuationRadius;
	}

	void Light::SetAttenuationRadius(float radius)
	{
		_lightInfo._attenuationRadius = radius;
	}

	float Light::GetWidth() const
	{
		return _lightInfo._width;
	}

	void Light::SetWidth(float width)
	{
		_lightInfo._width = width;
	}

	float Light::GetHeight() const
	{
		return _lightInfo._height;
	}

	void Light::SetHeight(float height)
	{
		_lightInfo._height = height;
	}

	void Light::OnSceneLighting()
	{
		_lightInfo._position = GetTransform()->GetWorldPosition();

		_lightInfo._direction = GetTransform()->GetLook();

		_lightInfo._up = GetTransform()->GetUp();

		_lightInfo._shadowMatrix = DUOLMath::Matrix::CreateLookAt(_lightInfo._position, _lightInfo._position + _lightInfo._direction, DUOLMath::Vector3(0.f, 1.f, 0.f));

		memcpy(&_cbPerFrame->_light[(_cbPerFrame->_lightCount)++], &_lightInfo, sizeof(DUOLGraphicsEngine::Light));
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