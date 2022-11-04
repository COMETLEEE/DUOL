#pragma once
#include "ResourceFlags.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class Shader;

	enum class PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
		PRIMITIVE_TOPOLOGY_POINTLIST = 1,
		PRIMITIVE_TOPOLOGY_LINELIST = 2,
		PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
		PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
		PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
		PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
		PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
		PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
		PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
		PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
		PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
		PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
		PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
		PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
		PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
		PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
		PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
		PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
		PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
		PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
		PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
		PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
		PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
		PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
		PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
		PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
		PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
		PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
		PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
		PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
		PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
		PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
		PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
		PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
		PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
		PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
		PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
		PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
		PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
		PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
		PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64
	};

	struct PipelineLayoutDesc
	{
		PipelineLayoutDesc(ResourceType resourceType, BindFlags bindFlag, StageFlags stageFlag) :
			_resourceType(resourceType)
			, _bindFlags(bindFlag)
			, _stageFlags(stageFlag)
		{

		}

		~PipelineLayoutDesc() = default;

		PipelineLayoutDesc() :
			_resourceType(ResourceType::UNDEFINED)
			, _bindFlags(BindFlags::UNDEFINED)
			, _stageFlags(StageFlags::UNDEFINED)
		{

		}

		ResourceType _resourceType;

		BindFlags _bindFlags;

		StageFlags  _stageFlags;
	};

	struct DepthStencilStateDesc
	{

	};

	struct RasterizerStateDesc
	{
		RasterizerStateDesc() :
			_fillMode(FillMode::SOLID)
			, _cullMode(CullMode::CULL_BACK)
			, _depthBias(0)
			, _depthBiasClamp(0.0f)
			, _slopeScaledDepthBias(0.0f)
			, _frontCounterClockWise(false)
			, _depthClipEnable(true)
			, _scissorEnable(false)
		{

		}

		enum class FillMode
		{
			WIRE = 2,
			SOLID = 3
		};

		enum class CullMode
		{
			CULL_NONE = 1,
			CULL_FRONT = 2,
			CULL_BACK = 3
		};

		FillMode _fillMode = FillMode::SOLID;

		CullMode _cullMode = CullMode::CULL_BACK;

		int _depthBias = 0;

		float _depthBiasClamp = 0.0f;

		float _slopeScaledDepthBias = 0.0f;

		bool _frontCounterClockWise = false;

		bool _depthClipEnable = true;

		bool _scissorEnable = false;

		bool _multiSampleEnable = false;

		bool _antialiasedLineEnable = false;
	};

	struct BlendStateDesc
	{

	};

	struct PipelineStateDesc
	{
		PipelineStateDesc() :
			_vertexShader(nullptr)
			, _pixelShader(nullptr)
			, _hullShader(nullptr)
			, _domainShader(nullptr)
			, _geometryShader(nullptr)
			, _primitiveTopology(PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		{

		}

		Shader* _vertexShader;

		Shader* _pixelShader;

		Shader* _hullShader;

		Shader* _domainShader;

		Shader* _geometryShader;

		DepthStencilStateDesc _depthStencilStateDesc;

		RasterizerStateDesc _rasterizerStateDesc;

		BlendStateDesc _blendStateDesc;

		PrimitiveTopology _primitiveTopology;
	};

	struct Viewport
	{
		inline Viewport(DUOLMath::Vector2 resolution) :
			_left(0.f)
			, _top(0.f)
			, _width(resolution.x)
			, _height(resolution.y)
			, _minDepth(0.0f)
			, _maxDepth(1.0f)
		{

		}

		inline Viewport(float left, float top, float width, float height, float minDepth, float maxDepth) :
			_left(left)
			, _top(top)
			, _width(width)
			, _height(height)
			, _minDepth(minDepth)
			, _maxDepth(maxDepth)
		{

		}

		float _left;

		float _top;

		float _width;

		float _height;

		float _minDepth;

		float _maxDepth;
	};
}
