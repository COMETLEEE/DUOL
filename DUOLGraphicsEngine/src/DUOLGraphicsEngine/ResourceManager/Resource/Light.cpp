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
		_lightManager->ReturnShadowMapSpace(_lightData._shadowDynamicMapIdx, _lightData._lightType);
		_lightManager->ReturnShadowMapSpace(_lightData._shadowStaticMapIdx, _lightData._lightType);
	}

	LightType Light::GetLightType()
	{
		return _lightData._lightType;
	}

	LightState Light::GetLightState()
	{
		return _lightData._lightState;
	}

	void Light::SetLightState(LightState state)
	{
		_lightManager->ReturnShadowMapSpace(_lightData._shadowDynamicMapIdx, _lightData._lightType);
		_lightManager->ReturnShadowMapSpace(_lightData._shadowStaticMapIdx, _lightData._lightType);

		if (_lightData._lightType == LightType::Direction)
		{
			_lightData._lightState = LightState::Dynamic;
			_lightData._shadowStaticMapIdx = -1;
			_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);

			return;
		}
		else
		{

			switch (state)
			{
			case LightState::Static:
			{
				_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
				_lightData._shadowDynamicMapIdx = -1;
				_needBakeStaticShadowMap = true;
				break;
			}
			case LightState::Mixed:
			{
				_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
				_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);

				_needBakeStaticShadowMap = true;
				break;
			}
			case LightState::Dynamic:
			{
				_lightData._shadowStaticMapIdx = -1;
				_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
				break;
			}
			case LightState::Unknown: break;
			default:;
			}
		}

		_lightData._lightState = state;
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

	const DUOLMath::Vector3& Light::GetPosition()
	{
		return  _lightData._position;
	}

	void Light::SetLightType(LightType type)
	{
		//일단 셰도우맵을 있든말든 기존의 타입의 공간을 반환합니다
		_lightManager->ReturnShadowMapSpace(_lightData._shadowDynamicMapIdx, _lightData._lightType);
		_lightManager->ReturnShadowMapSpace(_lightData._shadowStaticMapIdx, _lightData._lightType);

		if (type == LightType::Direction)
		{
			_lightData._lightState = LightState::Dynamic;
			_lightData._shadowStaticMapIdx = -1;
			_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(type);
		}
		else
		{

			switch (_lightData._lightState)
			{
			case LightState::Static:
			{
				_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(type);
				_lightData._shadowDynamicMapIdx = -1;
				_needBakeStaticShadowMap = true;
				break;
			}
			case LightState::Mixed:
			{
				_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(type);
				_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(type);

				_needBakeStaticShadowMap = true;
				break;
			}
			case LightState::Dynamic:
			{
				_lightData._shadowStaticMapIdx = -1;
				_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(type);
				break;
			}
			case LightState::Unknown: break;
			default:;
			}
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
		_lightData._attenuationRadius = std::max(1.f, attenuationRadius);
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
		_lightData._angle = std::clamp(angle, 0.01f, 1.56f);
	}

	void Light::SetInnerAngle(float innerAngle)
	{
		_lightData._innerAngle = innerAngle;
	}

	void Light::SetFallOffExponential(float FallOffExponential)
	{
		_lightData._fallOffExponential = std::max(1.f, FallOffExponential);
	}

	void Light::SetWidth(float width)
	{
		_lightData._width = (width < 0.1f) ? 0.1f : width;
	}

	void Light::SetHeight(float height)
	{
		_lightData._height = (height < 0.1f) ? 0.1f : height;
	}

	void Light::SetLightWorldMatrix()
	{
		_lightViewMatrix = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + _lightData._direction, _lightData._up);

		//_lightData._shadowMatrix = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + _lightData._direction, _lightData._up);
	}

	void Light::SetStaticShadowMapIndex(int shadowMapIndex)
	{
		_lightData._shadowStaticMapIdx = shadowMapIndex;
	}

	void Light::SetDynamicShadowMapIndex(int shadowMapIndex)
	{
		_lightData._shadowDynamicMapIdx = shadowMapIndex;
	}

	int Light::GetStaticShadowMapIndex()
	{
		return _lightData._shadowStaticMapIdx;
	}

	int Light::GetDynamicShadowMapIndex()
	{
		return _lightData._shadowDynamicMapIdx;
	}

	void Light::CalculateShadowMatrix()
	{
		if (_lightData._lightType == LightType::Spot)
		{
			auto projMat = DUOLMath::Matrix::CreatePerspectiveFieldOfView(_lightData._angle * 2, 1, 0.1f, _lightData._attenuationRadius);
			_lightData._shadowMatrix = _lightViewMatrix * projMat;
		}
		else if (_lightData._lightType == LightType::Area)
		{
			//스팟과 동일할 것으로 보인다.
			float dist = (_lightData._attenuationRadius) / tan(0.785398f);
			auto projMat = DUOLMath::Matrix::CreatePerspectiveFieldOfView(1.5708f, _lightData._width / _lightData._height, dist, dist + _lightData._attenuationRadius);
			_lightData._shadowMatrix = DUOLMath::Matrix::CreateLookAt(_lightData._position - (_lightData._direction * dist), _lightData._position + _lightData._direction, _lightData._up) * projMat;
		}
		else if (_lightData._lightType == LightType::Point)
		{
			//6면을 대상으로 큐브맵을 뽑는다.
			//fov값은 90도. 모든면을 각각 커버하는 angle이다.
			//auto projMat = DUOLMath::Matrix::CreatePerspectiveFieldOfView(1.5708f, 1, 0.001f, _lightData._attenuationRadius);
			auto projMat = DUOLMath::Matrix::CreatePerspectiveFieldOfView(1.5708f, 1, 0.1f, _lightData._attenuationRadius);

			//라이트의 포지션데이터를 기준으로 그림자 매트릭스를 만들어줍니다.

			//+x
			_lightData._shadowMatrix = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Right, DUOLMath::Vector3::Up);
			//-x
			_lightData._shadowMatrix1 = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Left, DUOLMath::Vector3::Up);
			//+y
			_lightData._shadowMatrix2 = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Up, DUOLMath::Vector3::Backward);
			//-y
			_lightData._shadowMatrix3 = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Down, DUOLMath::Vector3::Forward);
			//+z
			_lightData._shadowMatrix4 = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Forward, DUOLMath::Vector3::Up);
			//-z
			_lightData._shadowMatrix5 = DUOLMath::Matrix::CreateLookAt(_lightData._position, _lightData._position + DUOLMath::Vector3::Backward, DUOLMath::Vector3::Up);

			_lightData._shadowMatrix *= projMat;
			_lightData._shadowMatrix1 *= projMat;
			_lightData._shadowMatrix2 *= projMat;
			_lightData._shadowMatrix3 *= projMat;
			_lightData._shadowMatrix4 *= projMat;
			_lightData._shadowMatrix5 *= projMat;

		}
		else if (_lightData._lightType == LightType::Direction)
		{
			//direction light의 그림자는 하나만 존재할 수 있다( cascade shadow )
			//내부적으로 어짜피 계산해주고있다
		}
		else
		{

		}
	}

	void Light::ResetLightBakeFlag()
	{
		switch (_lightData._lightState)
		{
		case LightState::Static:
		{
			_needBakeDynamicShadowMap = false;
			_needBakeStaticShadowMap = false;
			break;
		}
		case LightState::Mixed:
		{
			_needBakeDynamicShadowMap = true;
			_needBakeStaticShadowMap = false;
			break;
		}
		case LightState::Dynamic:
		{
			_needBakeDynamicShadowMap = true;
			_needBakeStaticShadowMap = false;
			break;
		}
		case LightState::Unknown: break;
		default:;
		}

	}

	bool Light::GetNeedBakeStaticShadowMap() const
	{
		return _needBakeStaticShadowMap;
	}

	void Light::SetNeedBakeStaticShadowMap(bool needBakeStaticShadowMap)
	{
		_needBakeStaticShadowMap = needBakeStaticShadowMap;
	}

	bool Light::GetNeedBakeDynamicShadowMap() const
	{
		return _needBakeDynamicShadowMap;
	}

	void Light::SetNeedBakeDynamicShadowMap(bool needBakeDynamicShadowMap)
	{
		_needBakeDynamicShadowMap = needBakeDynamicShadowMap;
	}

	LightData& Light::GetLightData()
	{
		return _lightData;
	}

	void Light::ClearRenderTarget(DUOLGraphicsLibrary::Renderer* renderer)
	{
		if (_lightData._lightType != LightType::Direction)
		{
			if (_needBakeStaticShadowMap && _lightData._shadowStaticMapIdx > -1)
			{
				if (_lightData._lightType == LightType::Point)
				{
					_lightManager->ClearCubeMapRenderTarget(_lightData._shadowStaticMapIdx, renderer);
				}
				else
				{
					_lightManager->ClearTexture2DRenderTarget(_lightData._shadowStaticMapIdx, renderer);
				}

			}

			if (_needBakeDynamicShadowMap && _lightData._shadowDynamicMapIdx > -1)
			{
				if (_lightData._lightType == LightType::Point)
				{
					_lightManager->ClearCubeMapRenderTarget(_lightData._shadowDynamicMapIdx, renderer);
				}
				else
				{
					_lightManager->ClearTexture2DRenderTarget(_lightData._shadowDynamicMapIdx, renderer);
				}

			}
		}
	}

	void Light::TryGetShadowMapSpace()
	{
		if (_lightData._lightType == LightType::Direction)
		{
			_lightData._shadowStaticMapIdx = -1;

			if (_lightData._shadowDynamicMapIdx == -1)
			{
				_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
			}
		}
		else
		{

			switch (_lightData._lightState)
			{
			case LightState::Static:
			{
				if (_lightData._shadowStaticMapIdx == -1)
				{
					_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
					_needBakeStaticShadowMap = true;
				}
				_lightData._shadowDynamicMapIdx = -1;
				break;
			}
			case LightState::Mixed:
			{
				if (_lightData._shadowStaticMapIdx == -1)
				{
					_lightData._shadowStaticMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
					_needBakeStaticShadowMap = true;
				}
				if (_lightData._shadowDynamicMapIdx == -1)
				{
					_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
				}

				break;
			}
			case LightState::Dynamic:
			{
				_lightData._shadowStaticMapIdx = -1;
				if (_lightData._shadowDynamicMapIdx == -1)
				{
					_lightData._shadowDynamicMapIdx = _lightManager->GetShadowMapSpace(_lightData._lightType);
				}
				break;
			}
			case LightState::Unknown: break;
			default:;
			}
		}

	}
}
