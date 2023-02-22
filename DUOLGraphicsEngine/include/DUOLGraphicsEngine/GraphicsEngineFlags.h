#pragma once
#include "Export.h"
#include "DUOLMath/DUOLMath.h"
#include "ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLCommon/StringHelper.h"

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
	struct ConstantBufferPerCamera;

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

	struct DUOLGRAPHICSENGINE_EXPORT RenderingPipelineLayout
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

	struct DUOLGRAPHICSENGINE_EXPORT RenderingPipelinesList
	{
		RenderingPipelinesList():
			_cameraData(nullptr)
			, _drawSkybox(true)
			, _drawDebug(false)
		{
			
		}

		//바인딩한 Pipeline은 타입이 Render인 것을 우선적으로 작동합니다. 이외에는 바인딩한 순서대로 작동합니다.
		std::vector<RenderingPipelineLayout> _opaquePipelines;

		std::vector<RenderingPipelineLayout> _transparencyPipelines;

		Camera* _cameraData;

		bool _drawSkybox;

		bool _drawDebug;

		// 파이프라인 리스트 (묶음) 의 이름
		DUOLCommon::tstring _pipelineListName;
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
