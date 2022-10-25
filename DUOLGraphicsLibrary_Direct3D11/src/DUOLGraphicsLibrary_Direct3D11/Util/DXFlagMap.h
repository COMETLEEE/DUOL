#pragma once
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/SamplerFlags.h"

namespace DUOLGraphicsLibrary
{
	D3D11_USAGE MapDXUsageFlag(const ResourceUsage& input);

	long MapDXCPUAccessFlag(const long& input);

	long MapDXBindFlag(const long& input);

	DXGI_FORMAT MapFormat(const ResourceFormat& format);

	D3D11_PRIMITIVE_TOPOLOGY MapDXPrimitiveTopology(const PrimitiveTopology& topology);

	D3D11_FILL_MODE MapDXFillMode(const RasterizerStateDesc::FillMode& fillMode);

	D3D11_CULL_MODE MapDXCullMode(const RasterizerStateDesc::CullMode& cullMode);

	D3D11_TEXTURE_ADDRESS_MODE MapDXTextureAdressMode(const SamplerAddressMode& samplerAddressMode);

	D3D11_FILTER MapDXFilter(const SamplerFilter& samplerFilter);

	D3D11_COMPARISON_FUNC MapDXComparisonFunc(const ComparisonFunc& comparisonFunc);

}
