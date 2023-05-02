#pragma once

#include "DUOLMath/DUOLMath.h"
#include "DUOLGameEngine/Export_Engine.h"
#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	struct DUOL_GAMEENGINE_API ToneMapping /*: ObjectBase*/
	{
		ToneMapping() : 
			_exposure(1.0f)
			//, ObjectBase(_T(""), ObjectType::Resource)
		{
			
		}

		DEFINE_DEFAULT_COPY_MOVE(ToneMapping)


		float GetExposure() const;

		void SetExposure(float exposure);

		float _exposure;
		DUOLMath::Vector3 pad;

		RTTR_REGISTRATION_FRIEND

		//RTTR_ENABLE(ObjectBase)
		RTTR_ENABLE()
	};

	struct DUOL_GAMEENGINE_API ScreenSpaceReflection/* : ObjectBase*/
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
			//, ObjectBase(_T(""), ObjectType::Resource)
		{

		}

		DEFINE_DEFAULT_COPY_MOVE(ScreenSpaceReflection)


		float GetStride() const;

		void SetStride(float stride);

		float GetMaxSteps() const;

		void SetMaxSteps(float maxSteps);

		float GetMaxDistance() const;

		void SetMaxDistance(float maxDistance);

		float GetStrideZCutoff() const;

		void SetStrideZCutoff(float strideZCutoff);

		float GetNumMips() const;

		void SetNumMips(float numMips);

		float GetFadeStart() const;

		void SetFadeStart(float fadeStart);

		float GetFadeEnd() const;

		void SetFadeEnd(float fadeEnd);

		float GetZThickness() const;

		void SetZThickness(float zThickness);

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

		RTTR_REGISTRATION_FRIEND

		//RTTR_ENABLE(ObjectBase)
		RTTR_ENABLE()
	};

	struct DUOL_GAMEENGINE_API ExponentialHeightFog /*: ObjectBase*/
	{
		ExponentialHeightFog():
			_fogDensity(0.02f)
			,_fogHeightFalloff(0.2f)
			,_fogCutOffDistance(0.f)
			,_fogStartDistance(0.f)
			,_fogMaxOpacity(1.f)
			,_fogHeight(0.f, 10.f, 0.f)
			,_fogScatteringColor(0.7f, 0.7f, 0.7f)
			//, ObjectBase(_T(""), ObjectType::Resource)
		{
		}

		DEFINE_DEFAULT_COPY_MOVE(ExponentialHeightFog)


		float GetFogDensity() const;

		void SetFogDensity(float fogDensity);

		float GetFogMaxOpacity() const;

		void SetFogMaxOpacity(float fogMaxOpacity);

		float GetFogStartDistance() const;

		void SetFogStartDistance(float fogStartDistance);

		float GetFogCutOffDistance() const;

		void SetFogCutOffDistance(float fogCutOffDistance);

		float GetFogHeightFalloff() const;

		void SetFogHeightFalloff(float fogHeightFalloff);

		DUOLMath::Vector3 GetFogHeight() const;

		void SetFogHeight(const DUOLMath::Vector3& fogHeight);

		DUOLMath::Vector3 GetFogScatteringColor() const;

		void SetFogScatteringColor(const DUOLMath::Vector3& fogScatteringColor);

		float GetLightScatterFlag() const;

		void SetLightScatterFlag(float lightScatterFlag);

		float _fogDensity;		  //밀도
		float _fogMaxOpacity;     //최대 안개 수치(0~1사이의 값)
		float _fogStartDistance;  //카메라로부터의 거리
		float _fogCutOffDistance; //카메라로부터 안개가 끝나는 지점의 거리

		float _fogHeightFalloff;  //높이 exponential의 수치.
		DUOLMath::Vector3 _fogHeight;

		DUOLMath::Vector3 _fogScatteringColor;
		float _lightScatterFlag; //directional light가 있을떄 넣어준다. 아니면 -1

		RTTR_REGISTRATION_FRIEND

		//RTTR_ENABLE(ObjectBase)
		RTTR_ENABLE()
	};

	struct DUOL_GAMEENGINE_API GraphicsSetting 
	{
		GraphicsSetting();

		~GraphicsSetting();

		std::shared_ptr<ScreenSpaceReflection> _screenSpaceReflection;

		std::shared_ptr<ToneMapping> _toneMapping;

		std::shared_ptr<ExponentialHeightFog> _exponentialHeightFog;

		/*ScreenSpaceReflection* _screenSpaceReflection;

		ToneMapping* _toneMapping;

		ExponentialHeightFog* _exponentialHeightFog;*/

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE()
	};


	struct DUOL_GAMEENGINE_API GraphicsSettingData
	{

		ScreenSpaceReflection _screenSpaceReflection;

		ToneMapping _toneMapping;

		ExponentialHeightFog _exponentialHeightFog;

		/*ScreenSpaceReflection* _screenSpaceReflection;

		ToneMapping* _toneMapping;

		ExponentialHeightFog* _exponentialHeightFog;*/
	};
}