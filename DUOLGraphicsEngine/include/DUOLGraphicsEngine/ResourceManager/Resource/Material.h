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
			, _roughness(0.5f)
			, _metalicSmoothnessMap(nullptr)
			, _normalMap(nullptr)
		{

		}

		DUOLMath::Vector4 _albedo;

		DUOLMath::Vector4 _emissive;

		float _metalic;

		float _roughness;

		DUOLMath::Vector2  _tiling;

		DUOLGraphicsLibrary::Texture* _albedoMap;

		DUOLGraphicsLibrary::Texture* _metalicSmoothnessMap;

		DUOLGraphicsLibrary::Texture* _normalMap;

		//shader;
		DUOLGraphicsLibrary::PipelineState* _shaders;
	};

	struct MaterialDesc
	{
	public:
		MaterialDesc() :
			_albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap()
			, _metalic(0.5f)
			, _smoothness(0.5f)
			, _metalicSmoothnessMap()
			, _normalMap()
		{

		}

		DUOLMath::Vector4 _albedo;

		DUOLCommon::tstring _albedoMap;

		float _metalic;

		float _smoothness;

		DUOLCommon::tstring _metalicSmoothnessMap;

		DUOLCommon::tstring _normalMap;

		//shader;
		DUOLCommon::tstring  _renderPass;
	};
}