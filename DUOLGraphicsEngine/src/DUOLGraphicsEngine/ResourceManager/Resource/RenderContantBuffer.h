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
        //�е��� �ϴ� ����. hlsl�� 4���� ���Ϳ� ä���ִ� ������ ���.
        //ä�� �ֵ�, �ΰ��� 4���� ���� ���̿� ���� ���������� �ȵȴ�.
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

        //�Ÿ��� ���� ���踦 ����
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

        //���̶���Ʈ�� ������ ������ �����ϴµ� ���̴� ����.
        DUOLMath::Vector3 Direction;

        float _spot;

        //�Ÿ��� ���� ���踦 ����
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
