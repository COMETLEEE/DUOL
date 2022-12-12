#pragma once
#include "DUOLMath/DUOLMath.h"
#include <vector>

namespace DUOLGraphicsEngine
{

	struct Camera
	{
		DUOLMath::Matrix _viewMatrix;

		DUOLMath::Matrix _projectionMatrix;

		DUOLMath::Matrix _viewProjectionMatrix;

		DUOLMath::Matrix _viewProjectionInverseTransposeMatrix;

        DUOLMath::Vector4 _cameraPosition;
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
			, _range(0.f)
			, _color(0.5f, 0.5f, 0.5f)
			, _intensity(15.f)
            , _attenuation(1.f)
			, _attenuationRadius(1.f)
			, _viewProjectionMatrix()
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

        DUOLMath::Matrix _viewProjectionMatrix;
    };

	struct ConstantBufferPerFrame
	{
		Camera _camera;

        int _lightCount;
        DUOLMath::Vector3 _pad;
        Light _light[30];
	};

    struct Transform
    {
        DUOLMath::Matrix _world;

        DUOLMath::Matrix _worldInvTranspose;
    };

    struct Material;

    struct ConstantBufferPerObject
    {
        std::vector<Material*>* _material;

        Transform* _transform;
    };

    struct ConstantBufferPerSkinnedObject
    {
        std::vector<Material*>* _material;

        Transform* _transform;
        //+ bone data
        //+ bone
    };
}