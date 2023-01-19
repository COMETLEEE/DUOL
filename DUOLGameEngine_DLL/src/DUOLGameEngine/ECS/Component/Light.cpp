#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/EventManager.h"

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

	const DUOLGraphicsEngine::LightType& Light::GetLightType() const
	{
		return _lightInfo._lightType;
	}

	void Light::SetLightType(const DUOLGameEngine::LightType& lightType)
	{
		_lightInfo._lightType = static_cast<DUOLGraphicsEngine::LightType>(lightType);
	}

	const DUOLMath::Vector3& Light::GetDirection() const
	{
		// Direction => Look vector.
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
		_lightInfo._intensity = intensity;
	}

	float Light::GetAttenuation() const
	{
		return _lightInfo._attenuation;
	}

	void Light::SetAttenuation(float attenuation)
	{
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