#include "DXFlagMap.h"
#include "DXHelper.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"

namespace  DUOLGraphicsLibrary
{
	D3D11_USAGE MapDXUsageFlag(const ResourceUsage& input)
	{
		switch (input)
		{
		case ResourceUsage::USAGE_DEFAULT:
		{
			return D3D11_USAGE_DEFAULT;
		}
		case ResourceUsage::USAGE_IMMUTABLE:
		{
			return D3D11_USAGE_IMMUTABLE;
		}
		case ResourceUsage::USAGE_DYNAMIC:
		{
			return D3D11_USAGE_DYNAMIC;
		}
		case ResourceUsage::USAGE_STAGING:
		{
			return D3D11_USAGE_STAGING;
		}
		default:
		{
			DUOLGRAPHICS_ASSERT("DXFlagMap MappingError");
			return D3D11_USAGE_DEFAULT;
		}
		}
	}

	UINT MapDXCPUAccessFlag(const long& input)
	{
		long flag = 0;

		if ((input & (long)CPUAccessFlags::READ) != 0)
		{
			flag |= D3D11_CPU_ACCESS_READ;
		}
		if ((input & (long)CPUAccessFlags::WRITE) != 0)
		{
			flag |= D3D11_CPU_ACCESS_WRITE;
		}

		return flag;
	}

	D3D11_MAP MapDXCPUAccessFlagToMap(const long& input)
	{

		if ((input & (long)CPUAccessFlags::READ) != 0)
		{
			return D3D11_MAP_READ;
		}
		if ((input & (long)CPUAccessFlags::WRITE) != 0)
		{
			return D3D11_MAP_WRITE;
		}
		if ((input & (long)CPUAccessFlags::WRITEDISCARD) != 0)
		{
			return D3D11_MAP_WRITE_DISCARD;
		}
		if ((input & (long)CPUAccessFlags::READWRITE) != 0)
		{
			return D3D11_MAP_READ_WRITE;
		}
	}

	UINT MapDXBindFlag(const long& input)
	{
		long flag = 0;

		if ((input & (long)BindFlags::VERTEXBUFFER) != 0)
		{
			flag |= D3D11_BIND_VERTEX_BUFFER;
		}
		if ((input & (long)BindFlags::INDEXBUFFER) != 0)
		{
			flag |= D3D11_BIND_INDEX_BUFFER;
		}
		if ((input & (long)BindFlags::CONSTANTBUFFER) != 0)
		{
			flag |= D3D11_BIND_CONSTANT_BUFFER;
		}
		if ((input & (long)BindFlags::SHADERRESOURCE) != 0)
		{
			flag |= D3D11_BIND_SHADER_RESOURCE;
		}
		if ((input & (long)BindFlags::RENDERTARGET) != 0)
		{
			flag |= D3D11_BIND_RENDER_TARGET;
		}
		if ((input & (long)BindFlags::DEPTHSTENCIL) != 0)
		{
			flag |= D3D11_BIND_DEPTH_STENCIL;
		}
		if ((input & (long)BindFlags::UNRODERED) != 0)
		{
			flag |= D3D11_BIND_UNORDERED_ACCESS;
		}
		if ((input & (long)BindFlags::STREAMOUTPUTBUFFER) != 0)
		{
			flag |= D3D11_BIND_STREAM_OUTPUT;
		}

		return flag;
	}

	//todo 나중에 매핑하자.. 
	DXGI_FORMAT MapFormat(const ResourceFormat& format)
	{
		switch (format)
		{
		case ResourceFormat::FORMAT_UNKNOWN: break;
		case ResourceFormat::FORMAT_R32G32B32A32_TYPELESS: break;
		case ResourceFormat::FORMAT_R32G32B32A32_FLOAT: break;
		case ResourceFormat::FORMAT_R32G32B32A32_UINT: break;
		case ResourceFormat::FORMAT_R32G32B32A32_SINT: break;
		case ResourceFormat::FORMAT_R32G32B32_TYPELESS: break;
		case ResourceFormat::FORMAT_R32G32B32_FLOAT: break;
		case ResourceFormat::FORMAT_R32G32B32_UINT: break;
		case ResourceFormat::FORMAT_R32G32B32_SINT: break;
		case ResourceFormat::FORMAT_R16G16B16A16_TYPELESS: break;
		case ResourceFormat::FORMAT_R16G16B16A16_FLOAT: break;
		case ResourceFormat::FORMAT_R16G16B16A16_UNORM: break;
		case ResourceFormat::FORMAT_R16G16B16A16_UINT: break;
		case ResourceFormat::FORMAT_R16G16B16A16_SNORM: break;
		case ResourceFormat::FORMAT_R16G16B16A16_SINT: break;
		case ResourceFormat::FORMAT_R32G32_TYPELESS: break;
		case ResourceFormat::FORMAT_R32G32_FLOAT: break;
		case ResourceFormat::FORMAT_R32G32_UINT: break;
		case ResourceFormat::FORMAT_R32G32_SINT: break;
		case ResourceFormat::FORMAT_R32G8X24_TYPELESS: break;
		case ResourceFormat::FORMAT_D32_FLOAT_S8X24_UINT: break;
		case ResourceFormat::FORMAT_R32_FLOAT_X8X24_TYPELESS: break;
		case ResourceFormat::FORMAT_X32_TYPELESS_G8X24_UINT: break;
		case ResourceFormat::FORMAT_R10G10B10A2_TYPELESS: break;
		case ResourceFormat::FORMAT_R10G10B10A2_UNORM: break;
		case ResourceFormat::FORMAT_R10G10B10A2_UINT: break;
		case ResourceFormat::FORMAT_R11G11B10_FLOAT: break;
		case ResourceFormat::FORMAT_R8G8B8A8_TYPELESS: break;
		case ResourceFormat::FORMAT_R8G8B8A8_UNORM: break;
		case ResourceFormat::FORMAT_R8G8B8A8_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_R8G8B8A8_UINT: break;
		case ResourceFormat::FORMAT_R8G8B8A8_SNORM: break;
		case ResourceFormat::FORMAT_R8G8B8A8_SINT: break;
		case ResourceFormat::FORMAT_R16G16_TYPELESS: break;
		case ResourceFormat::FORMAT_R16G16_FLOAT: break;
		case ResourceFormat::FORMAT_R16G16_UNORM: break;
		case ResourceFormat::FORMAT_R16G16_UINT: break;
		case ResourceFormat::FORMAT_R16G16_SNORM: break;
		case ResourceFormat::FORMAT_R16G16_SINT: break;
		case ResourceFormat::FORMAT_R32_TYPELESS: break;
		case ResourceFormat::FORMAT_D32_FLOAT: break;
		case ResourceFormat::FORMAT_R32_FLOAT: break;
		case ResourceFormat::FORMAT_R32_UINT: break;
		case ResourceFormat::FORMAT_R32_SINT: break;
		case ResourceFormat::FORMAT_R24G8_TYPELESS: break;
		case ResourceFormat::FORMAT_D24_UNORM_S8_UINT: break;
		case ResourceFormat::FORMAT_R24_UNORM_X8_TYPELESS: break;
		case ResourceFormat::FORMAT_X24_TYPELESS_G8_UINT: break;
		case ResourceFormat::FORMAT_R8G8_TYPELESS: break;
		case ResourceFormat::FORMAT_R8G8_UNORM: break;
		case ResourceFormat::FORMAT_R8G8_UINT: break;
		case ResourceFormat::FORMAT_R8G8_SNORM: break;
		case ResourceFormat::FORMAT_R8G8_SINT: break;
		case ResourceFormat::FORMAT_R16_TYPELESS: break;
		case ResourceFormat::FORMAT_R16_FLOAT: break;
		case ResourceFormat::FORMAT_D16_UNORM: break;
		case ResourceFormat::FORMAT_R16_UNORM: break;
		case ResourceFormat::FORMAT_R16_UINT: break;
		case ResourceFormat::FORMAT_R16_SNORM: break;
		case ResourceFormat::FORMAT_R16_SINT: break;
		case ResourceFormat::FORMAT_R8_TYPELESS: break;
		case ResourceFormat::FORMAT_R8_UNORM: break;
		case ResourceFormat::FORMAT_R8_UINT: break;
		case ResourceFormat::FORMAT_R8_SNORM: break;
		case ResourceFormat::FORMAT_R8_SINT: break;
		case ResourceFormat::FORMAT_A8_UNORM: break;
		case ResourceFormat::FORMAT_R1_UNORM: break;
		case ResourceFormat::FORMAT_R9G9B9E5_SHAREDEXP: break;
		case ResourceFormat::FORMAT_R8G8_B8G8_UNORM: break;
		case ResourceFormat::FORMAT_G8R8_G8B8_UNORM: break;
		case ResourceFormat::FORMAT_BC1_TYPELESS: break;
		case ResourceFormat::FORMAT_BC1_UNORM: break;
		case ResourceFormat::FORMAT_BC1_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_BC2_TYPELESS: break;
		case ResourceFormat::FORMAT_BC2_UNORM: break;
		case ResourceFormat::FORMAT_BC2_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_BC3_TYPELESS: break;
		case ResourceFormat::FORMAT_BC3_UNORM: break;
		case ResourceFormat::FORMAT_BC3_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_BC4_TYPELESS: break;
		case ResourceFormat::FORMAT_BC4_UNORM: break;
		case ResourceFormat::FORMAT_BC4_SNORM: break;
		case ResourceFormat::FORMAT_BC5_TYPELESS: break;
		case ResourceFormat::FORMAT_BC5_UNORM: break;
		case ResourceFormat::FORMAT_BC5_SNORM: break;
		case ResourceFormat::FORMAT_B5G6R5_UNORM: break;
		case ResourceFormat::FORMAT_B5G5R5A1_UNORM: break;
		case ResourceFormat::FORMAT_B8G8R8A8_UNORM: break;
		case ResourceFormat::FORMAT_B8G8R8X8_UNORM: break;
		case ResourceFormat::FORMAT_R10G10B10_XR_BIAS_A2_UNORM: break;
		case ResourceFormat::FORMAT_B8G8R8A8_TYPELESS: break;
		case ResourceFormat::FORMAT_B8G8R8A8_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_B8G8R8X8_TYPELESS: break;
		case ResourceFormat::FORMAT_B8G8R8X8_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_BC6H_TYPELESS: break;
		case ResourceFormat::FORMAT_BC6H_UF16: break;
		case ResourceFormat::FORMAT_BC6H_SF16: break;
		case ResourceFormat::FORMAT_BC7_TYPELESS: break;
		case ResourceFormat::FORMAT_BC7_UNORM: break;
		case ResourceFormat::FORMAT_BC7_UNORM_SRGB: break;
		case ResourceFormat::FORMAT_AYUV: break;
		case ResourceFormat::FORMAT_Y410: break;
		case ResourceFormat::FORMAT_Y416: break;
		case ResourceFormat::FORMAT_NV12: break;
		case ResourceFormat::FORMAT_P010: break;
		case ResourceFormat::FORMAT_P016: break;
		case ResourceFormat::FORMAT_420_OPAQUE: break;
		case ResourceFormat::FORMAT_YUY2: break;
		case ResourceFormat::FORMAT_Y210: break;
		case ResourceFormat::FORMAT_Y216: break;
		case ResourceFormat::FORMAT_NV11: break;
		case ResourceFormat::FORMAT_AI44: break;
		case ResourceFormat::FORMAT_IA44: break;
		case ResourceFormat::FORMAT_P8: break;
		case ResourceFormat::FORMAT_A8P8: break;
		case ResourceFormat::FORMAT_B4G4R4A4_UNORM: break;
		case ResourceFormat::FORMAT_P208: break;
		case ResourceFormat::FORMAT_V208: break;
		case ResourceFormat::FORMAT_V408: break;
		case ResourceFormat::FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE: break;
		case ResourceFormat::FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE: break;
		case ResourceFormat::FORMAT_FORCE_UINT: break;
		default:;
		}

		return static_cast<DXGI_FORMAT>(format);
	}

	//todo 나중에 매핑하자.. 
	D3D11_PRIMITIVE_TOPOLOGY MapDXPrimitiveTopology(const PrimitiveTopology& topology)
	{
		switch (topology)
		{
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_UNDEFINED:
		{
			return D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_POINTLIST:
		{
			return D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST:
		{
			return D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		}
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP:
		{
			return D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		}
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		{
			return D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST_ADJ: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST: break;
		case PrimitiveTopology::PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST: break;
		default:;
		}

		return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(topology);
	}

	D3D11_TEXTURE_ADDRESS_MODE MapDXTextureAdressMode(const SamplerAddressMode& samplerAddressMode)
	{
		switch (samplerAddressMode)
		{
		case SamplerAddressMode::ADDRESS_WRAP:
		{
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}
		case SamplerAddressMode::ADDRESS_MIRROR:
		{
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		case SamplerAddressMode::ADDRESS_CLAMP:
		{
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		case SamplerAddressMode::ADDRESS_BORDER:
		{
			return D3D11_TEXTURE_ADDRESS_BORDER;
		}
		case SamplerAddressMode::ADDRESS_MIRROR_ONCE:
		{
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}
		default:
			break;
		}

		return D3D11_TEXTURE_ADDRESS_WRAP;
	}

	//todo 나중에 매핑하자.. 
	D3D11_FILTER MapDXFilter(const SamplerFilter& samplerFilter)
	{
		switch (samplerFilter)
		{
		case SamplerFilter::FILTER_MIN_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MIN_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MIN_POINT_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MIN_LINEAR_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MIN_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MIN_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_ANISOTROPIC: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_COMPARISON_MIN_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_COMPARISON_ANISOTROPIC: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MINIMUM_MIN_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MINIMUM_ANISOTROPIC: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT: break;
		case SamplerFilter::FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR: break;
		case SamplerFilter::FILTER_MAXIMUM_ANISOTROPIC: break;
		default:;
		}

		//todo 미래의 나에게 맡긴다.
		return static_cast<D3D11_FILTER>(samplerFilter);
	}

	D3D11_COMPARISON_FUNC MapDXComparisonFunc(const ComparisonFunc& comparisonFunc)
	{
		switch (comparisonFunc)
		{
		case ComparisonFunc::COMPARISON_NEVER:
		{
			return D3D11_COMPARISON_NEVER;
		}
		case ComparisonFunc::COMPARISON_LESS:
		{
			return D3D11_COMPARISON_LESS;
		}
		case ComparisonFunc::COMPARISON_EQUAL:
		{
			return D3D11_COMPARISON_EQUAL;
		}
		case ComparisonFunc::COMPARISON_LESS_EQUAL:
		{
			return D3D11_COMPARISON_LESS_EQUAL;
		}
		case ComparisonFunc::COMPARISON_GREATER:
		{
			return D3D11_COMPARISON_GREATER;
		}
		case ComparisonFunc::COMPARISON_NOT_EQUAL:
		{
			return D3D11_COMPARISON_NOT_EQUAL;
		}
		case ComparisonFunc::COMPARISON_GREATER_EQUAL:
		{
			return D3D11_COMPARISON_GREATER_EQUAL;
		}
		case ComparisonFunc::COMPARISON_ALWAYS:
		{
			return D3D11_COMPARISON_ALWAYS;
		}
		default:
			break;
		}

		return D3D11_COMPARISON_NEVER;
	}

	D3D11_BLEND_OP MapDXBlendOp(const BlendStateDesc::RenderTagetBlendFactor::BlendOp& blendOp)
	{
		switch (blendOp)
		{
		case BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_ADD:
		{
			return D3D11_BLEND_OP_ADD;
		}
		case BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_SUBTRACT:
		{
			return D3D11_BLEND_OP_SUBTRACT;
		}
		case BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_REV_SUBTRACT:
		{
			return D3D11_BLEND_OP_REV_SUBTRACT;
		}
		case BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_MIN:
		{
			return D3D11_BLEND_OP_MIN;
		}
		case BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_MAX:
		{
			return D3D11_BLEND_OP_MAX;
		}
		default:
		{
			return D3D11_BLEND_OP_ADD;
		}
		}
	}

	//todo 미래의 나에게 맡긴다.
	D3D11_BLEND MapDXBlend(const BlendStateDesc::RenderTagetBlendFactor::Blend& blend)
	{
		switch (blend) {
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_ZERO: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_ONE: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_SRC_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC_ALPHA: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_SRC_ALPHA: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_DEST_ALPHA: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_DEST_ALPHA: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_DEST_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_DEST_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC_ALPHA_SAT: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_BLEND_FACTOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_BLEND_FACTOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC1_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_SRC1_COLOR: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC1_ALPHA: break;
		case BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_SRC1_ALPHA: break;
		default:;
		}

		return static_cast<D3D11_BLEND>(blend);
	}

	D3D11_STENCIL_OP MapDXStencil(const StencilOp& stencilOp)
	{
		switch (stencilOp)
		{
		case StencilOp::STENCIL_OP_KEEP:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		}
		case StencilOp::STENCIL_OP_ZERO:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_ZERO;
		}
		case StencilOp::STENCIL_OP_REPLACE:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_REPLACE;
		}
		case StencilOp::STENCIL_OP_INCR_SAT:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;
		}
		case StencilOp::STENCIL_OP_DECR_SAT:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR_SAT;
		}
		case StencilOp::STENCIL_OP_INVERT:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INVERT;
		}
		case StencilOp::STENCIL_OP_INCR:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
		}
		case StencilOp::STENCIL_OP_DECR:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_DECR;
		}
		default:
		{
			return D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		}
		}
	}

	D3D11_FILL_MODE MapDXFillMode(const RasterizerStateDesc::FillMode& fillMode)
	{
		switch (fillMode)
		{
		case RasterizerStateDesc::FillMode::WIRE:
			return D3D11_FILL_WIREFRAME;
		case RasterizerStateDesc::FillMode::SOLID:
			return D3D11_FILL_SOLID;
		default:;
		}

		return D3D11_FILL_SOLID;
	}

	D3D11_CULL_MODE MapDXCullMode(const RasterizerStateDesc::CullMode& cullMode)
	{
		switch (cullMode)
		{
		case RasterizerStateDesc::CullMode::CULL_NONE:
			return D3D11_CULL_NONE;
		case RasterizerStateDesc::CullMode::CULL_FRONT:
			return D3D11_CULL_FRONT;
		case RasterizerStateDesc::CullMode::CULL_BACK:
			return D3D11_CULL_BACK;
		default:;
		}

		return D3D11_CULL_NONE;
	}
}
