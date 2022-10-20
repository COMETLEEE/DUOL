#include "D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include <algorithm>

namespace  DUOLGraphicsLibrary
{
	D3D11Texture::D3D11Texture(ID3D11Device* device, const TextureDesc& textureDesc) :
		Texture(textureDesc._type, textureDesc._bindFlags)
		, _format(DXGI_FORMAT_UNKNOWN)
		, _arrayLayer(0)
		, _mipLevel(0)
	{

		switch (textureDesc._type)
		{
		case TextureType::TEXTURE1D:
		case TextureType::TEXTURE1DARRAY:
		{
			CreateTexture1D(device, textureDesc);
			break;
		}
		case TextureType::TEXTURE2D:
		case TextureType::TEXTURE2DARRAY:
		case TextureType::TEXTURECUBE:
		case TextureType::TEXTURECUBEARRAY:
		{
			CreateTexture2D(device, textureDesc);
			break;
		}
		case TextureType::TEXTURE3D:
		{
			CreateTexture3D(device, textureDesc);
			break;
		}
		case TextureType::TEXTURE2DMS:
		case TextureType::TEXTURE2DMSARRAY:
		{
			CreateTexture2D(device, textureDesc);
			break;
		}
		default:
			DUOLGRAPHICS_ASSERT("failed to create texture with invalid texture type")
				break;
		}

	}

	void D3D11Texture::CreateTexture1D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE1D_DESC texture1DDesc;
		{
			texture1DDesc.Width = static_cast<UINT>(textureDesc._textureExtend.x);
			texture1DDesc.MipLevels = textureDesc._mipLevels;
			texture1DDesc.ArraySize = textureDesc._arraySize;
			texture1DDesc.Format = MapFormat(textureDesc._format);
			texture1DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture1DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture1DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture1DDesc.MiscFlags = textureDesc._miscFlags;
		}

		device->CreateTexture1D(&texture1DDesc, initialData, _texture._tex1D.ReleaseAndGetAddressOf());
	}

	void D3D11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE2D_DESC texture2DDesc;
		{
			texture2DDesc.Width = static_cast<UINT>(textureDesc._textureExtend.x);
			texture2DDesc.Height = static_cast<UINT>(textureDesc._textureExtend.y);
			texture2DDesc.MipLevels = textureDesc._mipLevels;
			texture2DDesc.ArraySize = textureDesc._arraySize;
			texture2DDesc.Format = MapFormat(textureDesc._format);

			texture2DDesc.SampleDesc.Count = ((textureDesc._type >= TextureType::TEXTURECUBE) ? max(1u, textureDesc._sample) : 1u);			texture2DDesc.SampleDesc.Quality = 0;
			texture2DDesc.SampleDesc.Quality = 0;

			texture2DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture2DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture2DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture2DDesc.MiscFlags = textureDesc._miscFlags;
		}

		device->CreateTexture2D(&texture2DDesc, initialData, _texture._tex2D.ReleaseAndGetAddressOf());
	}

	void D3D11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE3D_DESC texture3DDesc;
		{
			texture3DDesc.Width = static_cast<UINT>(textureDesc._textureExtend.x);
			texture3DDesc.Height = static_cast<UINT>(textureDesc._textureExtend.y);
			texture3DDesc.Depth = static_cast<UINT>(textureDesc._textureExtend.z);
			texture3DDesc.MipLevels = textureDesc._mipLevels;
			texture3DDesc.Format = MapFormat(textureDesc._format);
			texture3DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture3DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture3DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture3DDesc.MiscFlags = textureDesc._miscFlags;
		}

		device->CreateTexture3D(&texture3DDesc, initialData, _texture._tex3D.ReleaseAndGetAddressOf());
	}
}
