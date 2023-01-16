#pragma once
#include "Export.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;

	enum class RendererModuleType
	{
		DIRECTX11 = 1
		//, DIRECTX12
	};

	struct ModuleInfo
	{
		RendererModuleType _moduleType;

		void* _device;

		void* _deviceContext;
	};

	struct GraphicsEngineDesc
	{
		GraphicsEngineDesc() :
			_handle(0)
			, _moduleType(RendererModuleType::DIRECTX11)
			, _screenSize(0.f, 0.f)
			, _isFullscreen(false)
			, _isMSAA(false)
			, _sampleCount(1)
		{
		}


		long _handle;

		RendererModuleType _moduleType;

		// width, height
		DUOLMath::Vector2 _screenSize;

		bool _isFullscreen;

		bool _isMSAA;

		int _sampleCount;
	};

	struct RenderingPipelineLayout
	{
		RenderingPipelineLayout():
			_renderingPipeline()
			,_perObjectBufferData(nullptr)
			,_dataSize(0)
		{
		}

		RenderingPipelineLayout(RenderingPipeline* pipeline, void* postProcessingDataPoint = nullptr, int _dataSize = 0) :
			_renderingPipeline(pipeline)
			, _perObjectBufferData(postProcessingDataPoint)
			, _dataSize(_dataSize)
		{
		}

		RenderingPipeline* _renderingPipeline;

		void* _perObjectBufferData; //When pipeline type is PostProcessing, bind data at perObjectBuffer
							
		int  _dataSize;
	};

	struct RenderOption
	{
		int _dummy;
	};

	struct ShadowSetting
	{
		DUOLMath::Vector2 _maxResolution;

		float _cascadeSplit[4];
		
	};
}