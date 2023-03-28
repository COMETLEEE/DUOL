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

		DUOLMath::Matrix _prevViewMatrix;				// 모션 블러를 위해 넣지만 .. 사실상 쓸 곳은 많을 것 같다.

		DUOLMath::Matrix _prevProjectionMatrix;			// 모션 블러를 위해 넣지만 .. 사실상 쓸 곳은 많을 것 같다.

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
		float _gamePlayTime; // 게임 시작부터 흐른 시간.

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

	//ConstantBuffer Per Object Info는 RenderObject의 Struct입니다.

}
