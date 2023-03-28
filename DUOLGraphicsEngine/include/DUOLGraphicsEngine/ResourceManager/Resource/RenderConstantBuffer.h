#pragma once
#include "DUOLMath/DUOLMath.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/ShadowHelper.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Light.h"

namespace DUOLGraphicsEngine
{
	struct Camera
	{
		DUOLMath::Matrix _viewMatrix;

		DUOLMath::Matrix _projectionMatrix;

		DUOLMath::Matrix _prevViewMatrix;				// ��� ���� ���� ������ .. ��ǻ� �� ���� ���� �� ����.

		DUOLMath::Matrix _prevProjectionMatrix;			// ��� ���� ���� ������ .. ��ǻ� �� ���� ���� �� ����.

		DUOLMath::Matrix _viewProjectionMatrix;

		DUOLMath::Matrix _viewProjectionInverseTransposeMatrix;

        DUOLMath::Vector3 _cameraPosition;

		float _pad;

		float _cameraFar;

		float _cameraNear;

		float _cameraVerticalFOV;

		float _aspectRatio;
	};

	struct ConstantBufferPerFrame
	{
		float _gamePlayTime; // ���� ���ۺ��� �帥 �ð�.

		int _screenSize[2];

		float _timeStep;

		LightData _light[30];

		int _lightCount;

		int pad[3];
	};

	struct ConstantBufferPerCamera
	{
		Camera _camera;

		CascadeShadowInfo _cascadeShadowInfo;
	};

	//ConstantBuffer Per Object Info�� RenderObject�� Struct�Դϴ�.

}
