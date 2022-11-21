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
	};

    enum class LightType
    {
		Direction = 1,
		Point = 2,
		Spot = 3,
		Unknown = -1,
    };

    struct Light
    {
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
        Light _light[20];
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
