#include "D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace  DUOLGraphicsLibrary
{
	D3D11Texture::D3D11Texture(ID3D11Device* device, const TextureDesc& textureDesc) :
		Texture(textureDesc._type, textureDesc._bindFlags)
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

	void D3D11Texture::CreateTexture1D(ID3D11Device* device, const TextureDesc& desc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
	}

	void D3D11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& desc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
	}

	void D3D11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& desc,
		const D3D11_SUBRESOURCE_DATA* initialData, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
	}

	void D3D11Texture::UpdateSubresource(ID3D11DeviceContext* context, UINT mipLevel, UINT arrayLayer,
		const D3D11_BOX& region, std::size_t threadCount)
	{
	}
}
