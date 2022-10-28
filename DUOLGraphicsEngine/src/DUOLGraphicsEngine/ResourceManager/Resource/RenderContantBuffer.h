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
	};

    struct DirectionalLight //5
    {
        DUOLMath::Vector4 _ambient;

        DUOLMath::Vector4 _diffuse;

        DUOLMath::Vector4 _specular;

        DUOLMath::Vector3 _direction;
        //패딩을 하는 이유. hlsl은 4차원 벡터에 채워넣는 구조를 띈다.
        //채워 넣되, 두개의 4차원 벡터 사이에 걸쳐 나누어지면 안된다.
        float _pad;

        DUOLMath::Matrix _viewProjectionMatrix;
    };

    struct PointLight //5
    {
        DUOLMath::Vector4 _ambient;

        DUOLMath::Vector4 _diffuse;

        DUOLMath::Vector4 _specular;

        DUOLMath::Vector3 _position;
        float _range;

        //거리에 따른 감쇠를 위한
        DUOLMath::Vector3 _att;
        float pad;

        DUOLMath::Matrix _viewProjectionMatrix;
    };

    struct SpotLight //6
    {
        DUOLMath::Vector4 _ambient;

        DUOLMath::Vector4 _diffuse;

        DUOLMath::Vector4 _specular;

        DUOLMath::Vector3 _position;
        float _range;

        //스팟라이트의 점적광 원뿔을 제어하는데 쓰이는 지수.
        DUOLMath::Vector3 Direction;

        float _spot;

        //거리에 따른 감쇠를 위한
        DUOLMath::Vector3 _att;

        float _pad;

        DUOLMath::Matrix _viewProjectionMatrix;
    };

    struct Light
    {
        uint32_t _directionalLightCnt;

        uint32_t _pointLightCnt;

        uint32_t _spotLightCnt;

        uint32_t pad;

        DirectionalLight _dirLight;

        PointLight _pointLight[10];

        SpotLight _spotLight[10];
    };

	struct ConstantBufferPerFrame
	{
		Camera _camera;

        Light _light;
	};

    struct Transfrom
    {
        DUOLMath::Matrix _world;

        DUOLMath::Matrix _worldInvTranspose;
    };

    struct Material;

    struct ContantBufferPerObject
    {
        Transfrom* _transform;

        Material* _material;
    };

}
