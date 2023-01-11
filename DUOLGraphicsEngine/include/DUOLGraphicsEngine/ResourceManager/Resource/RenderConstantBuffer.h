#pragma once
#include "DUOLMath/DUOLMath.h"

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
			, _range(0.785398f)
			, _color(0.5f, 0.5f, 0.5f)
			, _intensity(5.f)
            , _attenuation(1.f)
			, _attenuationRadius(5.f)
        {
        }

        LightType _lightType;
        DUOLMath::Vector3 _direction;

        DUOLMath::Vector3  _position;
        float _range;

        DUOLMath::Vector3  _color;
        float _intensity;

        float _attenuation;
        float _attenuationRadius;
        DUOLMath::Vector2 _pad;

		DUOLMath::Matrix _shadowMatrix;
    };

	struct ConstantBufferPerFrame
	{
		Camera _camera;

        int _lightCount;

		int _screenSize[2];

		float _timeStep;

        Light _light[30];


	};

}
