#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"
#include "DUOLGraphicsEngine/ResourceManager/LightManager.h"

namespace DUOLGraphicsEngine
{
	void Light::CalculateSpotProjection(LightData& lightData)
	{
		auto projMat = DUOLMath::Matrix::CreatePerspectiveFieldOfView(lightData._angle, 1, 0.5f, lightData._attenuationRadius);

		lightData._shadowMatrix = lightData._shadowMatrix * projMat;
	}


	Light::Light(LightManager* lightManager) :
		_lightData()
		, _lightManager(lightManager)
	{
	}

	Light::~Light()
	{

	}

	LightType Light::GetLightType()
	{
		return _lightData._lightType;
	}

	float Light::GetAttenuationRadius()
	{
		return _lightData._attenuationRadius;
	}

	float Light::GetIntensity()
	{
		return _lightData._intensity;
	}

	float Light::GetAngle()
	{
		return _lightData._angle;
	}

	float Light::GetInnerAngle()
	{
		return _lightData._innerAngle;
	}

	float Light::GetFallOffExponential()
	{
		return _lightData._fallOffExponential;
	}

	float Light::GetWidth()
	{
		return _lightData._width;
	}

	float Light::GetHeight()
	{
		return _lightData._height;
	}

	void Light::SetLightType(LightType type)
	{
		//일단 셰도우맵을 있든말든 반환합니다
		_lightManager->ReturnShadowMapSpace(_shadowDynamicMapIdx, type);
		_lightManager->ReturnShadowMapSpace(_shadowStaticMapIdx, type);

		switch (_lightData._lightState)
		{
		case LightState::Static:
		{
			_shadowStaticMapIdx = _lightManager->GetShadowMapSpace(type);
			_shadowDynamicMapIdx = -1;
			break;
		}
		case LightState::Mixed:
		{
			_shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(type);
			_shadowStaticMapIdx = _lightManager->GetShadowMapSpace(type);
			break;
		}
		case LightState::Dynamic:
		{
			_shadowStaticMapIdx = -1;
			_shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(type);
			break;
		}
		case LightState::Unknown: break;
		default:;
		}

		_lightData._lightType = type;
	}

	void Light::SetDirection(const DUOLMath::Vector3& direction)
	{
		_lightData._direction = direction;
	}

	void Light::SetUp(const DUOLMath::Vector3& up)
	{
		_lightData._up = up;
	}

	void Light::SetPosition(const DUOLMath::Vector3& position)
	{
		_lightData._position = position;
	}

	void Light::SetAttenuationRadius(float attenuationRadius)
	{
		_lightData._attenuationRadius = attenuationRadius;
	}

	void Light::SetColor(const DUOLMath::Vector3& color)
	{
		_lightData._color = color;
	}

	void Light::SetIntensity(float intensity)
	{
		_lightData._intensity = intensity;
	}

	void Light::SetAngle(float angle)
	{
		_lightData._angle = std::clamp(angle, 0.01f, 1.5707f);
	}

	void Light::SetInnerAngle(float innerAngle)
	{
		_lightData._innerAngle = innerAngle;
	}

	void Light::SetFallOffExponential(float FallOffExponential)
	{
		_lightData._fallOffExponential = FallOffExponential;
	}

	void Light::SetWidth(float width)
	{
		_lightData._width = width;
	}

	void Light::SetHeight(float height)
	{
		_lightData._height = height;
	}

	void Light::SetLightWorldMatrix()
	{
		_lightViewMatrix = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + _lightData._direction, _lightData._up);
	}

	void Light::SetStaticShadowMapIndex(int shadowMapIndex)
	{
		_shadowStaticMapIdx = shadowMapIndex;
	}

	void Light::SetDynamicShadowMapIndex(int shadowMapIndex)
	{
		_shadowDynamicMapIdx = shadowMapIndex;
	}

	LightData& Light::GetLightData()
	{
		return _lightData;
	}
}
