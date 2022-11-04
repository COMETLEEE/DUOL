#pragma once
#include <DUOLCommon/StringHelper.h>
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class Texture;
	class PipelineState;
}

namespace DUOLGraphicsEngine
{
	struct Material
	{
	public:
		Material() :
			_albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap(nullptr)
			, _metalic(0.f)
			, _smoothness(0.5f)
			, _metalicSmoothnessMap(nullptr)
			, _normalMap(nullptr)
		{

		}

		DUOLMath::Vector4 _albedo;

		DUOLGraphicsLibrary::Texture* _albedoMap;

		float _metalic;

		float _smoothness;

		DUOLGraphicsLibrary::Texture* _metalicSmoothnessMap;

		DUOLGraphicsLibrary::Texture* _normalMap;

		//shader;
		DUOLGraphicsLibrary::PipelineState* _renderPass;
	};

	struct MaterialDesc
	{
	public:
		MaterialDesc() :
			_albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap()
			, _metalic(0.f)
			, _smoothness(0.5f)
			, _metalicSmoothnessMap()
			, _normalMap()
		{

		}

		DUOLMath::Vector4 _albedo;

		const DUOLCommon::tstring _albedoMap;

		float _metalic;

		float _smoothness;

		const DUOLCommon::tstring _metalicSmoothnessMap;

		const DUOLCommon::tstring _normalMap;

		//shader;
		const DUOLCommon::tstring  _renderPass;
	};
}