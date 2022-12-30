#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/ECS/Component/Transform.h"

namespace DUOLGameEngine
{
	Light::Light(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _lightInfo{}
		, _cbPerFrame(nullptr)
	{
		_cbPerFrame = DUOLGameEngine::GraphicsManager::GetInstance()->GetConstantBufferPerFrame();
	}

	Light::~Light()
	{

	}

	const DUOLGraphicsEngine::LightType& Light::GetRightType() const
	{
		return _lightInfo._lightType;
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

	void Light::CopyLightInformation()
	{
		memcpy(&_cbPerFrame->_light[(_cbPerFrame->_lightCount)++], &_lightInfo, sizeof(DUOLGraphicsEngine::Light));
	}

	void Light::OnUpdate(float deltaTime)
	{
		_lightInfo._position = GetTransform()->GetWorldPosition();

		_lightInfo._direction = GetTransform()->GetLook();

		CopyLightInformation();
	}
}