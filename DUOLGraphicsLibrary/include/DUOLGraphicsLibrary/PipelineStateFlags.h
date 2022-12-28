#pragma once
#include "DUOLGraphicsLibrary/ResourceFlags.h"
#include "DUOLGraphicsLibrary/SamplerFlags.h"
#include "DUOLGraphicsLibrary/Core/Typedef.h"
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

	enum class StencilOp
	{
		STENCIL_OP_KEEP = 1,
		STENCIL_OP_ZERO = 2,
		STENCIL_OP_REPLACE = 3,
		STENCIL_OP_INCR_SAT = 4,
		STENCIL_OP_DECR_SAT = 5,
		STENCIL_OP_INVERT = 6,
		STENCIL_OP_INCR = 7,
		STENCIL_OP_DECR = 8
	};

	struct StencilFaceDesc
	{
		StencilFaceDesc() :
			_stencilFailOp(StencilOp::STENCIL_OP_KEEP)
			, _stencilDepthFailOp(StencilOp::STENCIL_OP_KEEP)
			, _stencilPassOp(StencilOp::STENCIL_OP_KEEP)
			, _comparisonOp(ComparisonFunc::COMPARISON_ALWAYS)
		{

		}

		StencilFaceDesc(StencilOp stencilFail, StencilOp stencilDepthFail, StencilOp stencilPass, ComparisonFunc stencilCompare) :
			_stencilFailOp(stencilFail)
			, _stencilDepthFailOp(stencilDepthFail)
			, _stencilPassOp(stencilPass)
			, _comparisonOp(stencilCompare)
		{

		}

		StencilOp _stencilFailOp;

		StencilOp _stencilDepthFailOp;

		StencilOp _stencilPassOp;

		ComparisonFunc _comparisonOp;
	};

	struct DepthStencilStateDesc
	{
		DepthStencilStateDesc() :
			_depthEnable(true)
			, _writeEnable(true)
			, _depthCompareOp(ComparisonFunc::COMPARISON_LESS)
			, _stencilEnable(false)
			, _stencilReadMask(0xf)
			, _stencilWriteMask(0xf)
			, _frontFace()
			, _backFace()
		{
		}


		bool _depthEnable;

		bool _writeEnable;//depthWriteMask

		ComparisonFunc _depthCompareOp;

		bool _stencilEnable;

		UINT8 _stencilReadMask;

		UINT8 _stencilWriteMask;

		StencilFaceDesc _frontFace;

		StencilFaceDesc _backFace;
	};

	struct RasterizerStateDesc
	{
		RasterizerStateDesc() :
			_fillMode(FillMode::SOLID)
			, _cullMode(CullMode::CULL_NONE)
			, _depthBias(0)
			, _depthBiasClamp(0.0f)
			, _slopeScaledDepthBias(0.0f)
			, _frontCounterClockWise(false)
			, _depthClipEnable(true)
			, _scissorEnable(false)
			, _multiSampleEnable(false)
			, _antialiasedLineEnable(false)
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

		FillMode _fillMode;

		CullMode _cullMode;

		int _depthBias;

		float _depthBiasClamp;

		float _slopeScaledDepthBias;

		bool _frontCounterClockWise;

		bool _depthClipEnable;

		bool _scissorEnable;

		bool _multiSampleEnable;

		bool _antialiasedLineEnable;
	};

	struct BlendStateDesc
	{
		BlendStateDesc() :
			_alphaToCoverageEnable(false)
			, _independentBlendEnable(false)
		{
		}

		struct RenderTagetBlendFactor
		{
			RenderTagetBlendFactor() :
				_blendEnable(false)
				, _srcBlend(Blend::BLEND_SRC_ALPHA)
				, _destBlend(Blend::BLEND_ONE)
				, _blendOp(BlendOp::BLEND_OP_ADD)
				, _srcBlendAlpha(Blend::BLEND_SRC_ALPHA)
				, _destBlendAlpha(Blend::BLEND_DEST_ALPHA)
				, _blendOpAlpha(BlendOp::BLEND_OP_ADD)
				, _renderTargetBlendDesc(0xf)
			{
			}

			enum class Blend
			{
				BLEND_ZERO = 1,
				BLEND_ONE = 2,
				BLEND_SRC_COLOR = 3,
				BLEND_INV_SRC_COLOR = 4,
				BLEND_SRC_ALPHA = 5,
				BLEND_INV_SRC_ALPHA = 6,
				BLEND_DEST_ALPHA = 7,
				BLEND_INV_DEST_ALPHA = 8,
				BLEND_DEST_COLOR = 9,
				BLEND_INV_DEST_COLOR = 10,
				BLEND_SRC_ALPHA_SAT = 11,
				BLEND_BLEND_FACTOR = 14,
				BLEND_INV_BLEND_FACTOR = 15,
				BLEND_SRC1_COLOR = 16,
				BLEND_INV_SRC1_COLOR = 17,
				BLEND_SRC1_ALPHA = 18,
				BLEND_INV_SRC1_ALPHA = 19
			};

			enum class BlendOp
			{
				BLEND_OP_ADD = 1,
				BLEND_OP_SUBTRACT = 2,
				BLEND_OP_REV_SUBTRACT = 3,
				BLEND_OP_MIN = 4,
				BLEND_OP_MAX = 5
			};

			bool _blendEnable;

			Blend _srcBlend;

			Blend _destBlend;

			BlendOp _blendOp;

			Blend _srcBlendAlpha;

			Blend _destBlendAlpha;

			BlendOp _blendOpAlpha;

			UINT8 _renderTargetBlendDesc;
		};

		bool _alphaToCoverageEnable;

		bool _independentBlendEnable;

		RenderTagetBlendFactor _renderTarget[8];
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
