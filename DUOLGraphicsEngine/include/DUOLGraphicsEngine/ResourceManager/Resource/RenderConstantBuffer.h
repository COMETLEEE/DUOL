#pragma once
#include "DUOLMath/DUOLMath.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/ShadowHelper.h"

namespace DUOLGraphicsEngine
{
	struct Camera
	{
		DUOLMath::Matrix _viewMatrix;

		DUOLMath::Matrix _projectionMatrix;

		DUOLMath::Matrix _viewProjectionMatrix;

		DUOLMath::Matrix _viewProjectionInverseTransposeMatrix;

        DUOLMath::Vector3 _cameraPosition;

		float _pad;

		float _cameraFar;

		float _cameraNear;

		float _cameraVerticalFOV;

		float _aspectRatio;
	};

    enum class LightType
    {
		Direction = 1u,
		Point = 2u,
		Spot = 3u,
		Unknown = 0,
    };

    struct Light
    {
		Light():
			_lightType(LightType::Direction)
			, _direction(0.f, -1.f, 0.f)
			, _position(0.f, 10.f, 0.f)
			, _color(0.5f, 0.5f, 0.5f)
			, _intensity(5.f)
			, _angle(0.785398f)
            , _innerAngle(0.01f)
			, _attenuationRadius(1.f)
        {
        }

        LightType _lightType;
        DUOLMath::Vector3 _direction;

        DUOLMath::Vector3  _position;
        float _attenuationRadius;

        DUOLMath::Vector3  _color;
        float _intensity;

        float _angle;
        float _innerAngle;
        float _fallOffExponential;
        float _pad;

		DUOLMath::Matrix _shadowMatrix;
    };

	struct ConstantBufferPerFrame
	{
		float _gamePlayTime; // 게임 시작부터 흐른 시간.

		int _screenSize[2];

		float _timeStep;

		Light _light[30];

		int _lightCount;

		int pad[3];
	};

	struct ConstantBufferPerCamera
	{
		Camera _camera;

		CascadeShadowInfo _cascadeShadowInfo;
	};

	//ConstantBuffer Per Object Info는 RenderObject의 Struct입니다.

}
