#pragma once
#include "Export.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
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
}