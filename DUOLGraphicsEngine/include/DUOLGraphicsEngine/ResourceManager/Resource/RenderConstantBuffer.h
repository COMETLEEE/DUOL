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

		DUOLMath::Matrix _inverseProjectionMatrix;

        DUOLMath::Vector3 _cameraPosition;
		float _cameraVerticalFOV;

		DUOLMath::Vector3 _cameraLook;
		float _cameraFar;

		DUOLMath::Vector3 _cameraUp;
		float _cameraNear;

		float _aspectRatio;
		DUOLMath::Vector3 pad;
	};

	struct CurrentSceneInfo
	{
		float _gamePlayTime; // 게임 시작부터 흐른 시간.

		int _screenSize[2];

		float _timeStep;

		Light* _lights[30];

		int _lightCount;
	};

	struct ConstantBufferPerFrame
	{
		ConstantBufferPerFrame() = default;

		ConstantBufferPerFrame(const CurrentSceneInfo& info);

		float _gamePlayTime; // 게임 시작부터 흐른 시간.

		int _screenSize[2];

		float _timeStep;
	};

	inline ConstantBufferPerFrame::ConstantBufferPerFrame(const CurrentSceneInfo& info)
	{
		_gamePlayTime = info._gamePlayTime;
		_screenSize[0] = info._screenSize[0];
		_screenSize[1] = info._screenSize[1];
		_timeStep = info._timeStep;
	}

	struct ConstantBufferPerCamera
	{
		Camera _camera;

		CascadeShadowInfo _cascadeShadowInfo;

		LightData _light[30];

		int _lightCount;

		int pad[3];
	};

	//ConstantBuffer Per Object Info는 RenderObject의 Struct입니다.

}
