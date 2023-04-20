#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{

	struct ToneMapping
	{
		ToneMapping()
		{
			
		}

		float _exposure;

		DUOLMath::Vector3 pad;
	};

	struct ScreenSpaceReflection
	{
		ScreenSpaceReflection() :
			_stride(1)
			, _maxSteps(1000)
			, _maxDistance(300)
			, _strideZCutoff(0.f)
			, _numMips(7)
			, _fadeStart(0.09)
			, _fadeEnd(1)
			, _zThickness(0.2f)
		{

		}
		float _stride;
		// Step in horizontal or vertical pixels between samples. This is a float
		// because integer math is slow on GPUs, but should be set to an integer >= 1.
		float _maxSteps;    // Maximum number of iterations. Higher gives better images but may be slow.
		float _maxDistance; // Maximum camera-space distance to trace before returning a miss.
		float _strideZCutoff;
		// More distant pixels are smaller in screen space. This value tells at what point to
		// start relaxing the stride to give higher quality reflections for objects far from
		// the camera.

		float _numMips;       // the number of mip levelses in the convolved color buffer
		float _fadeStart;     // determines where to start screen edge fading of effect
		float _fadeEnd;       // determines where to end screen edge fading of effect
		float _sslr_padding0; // padding for alignment

		float _zThickness; // thickness to ascribe to each pixel in the depth buffer

	};

	struct ExponentialHeightFog
	{
		float _fogDensity;
		DUOLMath::Vector3 _fogScatteringColor;

		float _fogHeightFalloff;
		float _fogMaxOpacity;
		float _fogStartDistance;
		float _fogCutOffDistance;
	};

	struct PostProcessOption
	{
		ScreenSpaceReflection _screenSpaceReflection;

		ToneMapping _toneMapping;
	};

}
