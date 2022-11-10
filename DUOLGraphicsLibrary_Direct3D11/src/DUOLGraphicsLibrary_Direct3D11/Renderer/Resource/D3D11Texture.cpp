#include "D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include "DUOLCommon/StringHelper.h"

#include <algorithm>
#include <cmath>

namespace DUOLGraphicsLibrary
{
	D3D11Texture::D3D11Texture(const UINT64& guid, ID3D11Device* device, const TextureDesc& textureDesc) :
		Texture(guid, textureDesc)
	{
		if (textureDesc._texturePath == nullptr)
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

			if(textureDesc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE))
			{
				CreateShaderResourceView(device);
			}
		}
		else
		{
			CreateTextureFromFile(device, textureDesc);
		}
	}

	D3D11Texture::FileFormat D3D11Texture::CheckFileFormat(const char* path)
	{
		std::string pathstr(path);

		if (pathstr.length() > 1)
		{
			int extensionStartPoint = pathstr.find_last_of('.') + 1;

			std::string fileFormat = pathstr.substr(extensionStartPoint);

			if (fileFormat == "dds" || fileFormat == "DDS")
			{
				return D3D11Texture::FileFormat::DDS;
			}
			else if (fileFormat == "tga" || fileFormat == "TGA")
			{
				return D3D11Texture::FileFormat::TGA;
			}
			else if (fileFormat == "hdr" || fileFormat == "HDR")
			{
				return D3D11Texture::FileFormat::HDR;
			}
			else
			{
				return D3D11Texture::FileFormat::WIC;
			}
		}

		return D3D11Texture::FileFormat::WIC;
	}

	void D3D11Texture::CreateTexture1D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
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

		HRESULT hr = device->CreateTexture1D(&texture1DDesc, initialData, _texture._tex1D.ReleaseAndGetAddressOf());
		DXThrowError(hr, "failed to create texture");
	}

	void D3D11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE2D_DESC texture2DDesc;
		{
			texture2DDesc.Width = static_cast<UINT>(textureDesc._textureExtend.x);
			texture2DDesc.Height = static_cast<UINT>(textureDesc._textureExtend.y);
			texture2DDesc.MipLevels = textureDesc._mipLevels;
			texture2DDesc.ArraySize = textureDesc._arraySize;
			texture2DDesc.Format = MapFormat(textureDesc._format);

			texture2DDesc.SampleDesc.Count = ((textureDesc._type >= TextureType::TEXTURECUBE)
				? max(1u, textureDesc._sample)
				: 1u);
			texture2DDesc.SampleDesc.Quality = 0;
			texture2DDesc.SampleDesc.Quality = 0;

			texture2DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture2DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture2DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture2DDesc.MiscFlags = textureDesc._miscFlags;
		}

		HRESULT hr = device->CreateTexture2D(&texture2DDesc, initialData, _texture._tex2D.ReleaseAndGetAddressOf());
		DXThrowError(hr, "failed to create texture");

	}

	void D3D11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
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

		HRESULT hr = device->CreateTexture3D(&texture3DDesc, initialData, _texture._tex3D.ReleaseAndGetAddressOf());
		DXThrowError(hr, "failed to create texture");

	}

	void D3D11Texture::CreateShaderResourceView(ID3D11Device* device)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = MapFormat(_textureDesc._format);
		if(srvDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
		{
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}

		srvDesc.Texture2D.MipLevels = _textureDesc._mipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (_textureDesc._type == TextureType::TEXTURECUBE)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		}

		HRESULT hr = device->CreateShaderResourceView(_texture._resource.Get(), &srvDesc, _shaderResourceView.ReleaseAndGetAddressOf());

		DXThrowError(hr, "D3D11Texture CreateShadeResourceView ERROR");

	}

	void D3D11Texture::CreateTextureFromFile(ID3D11Device* device, const TextureDesc& desc)
	{
		auto wcharTexturePath = DUOLCommon::StringHelper::StringToWString(desc._texturePath);

		DirectX::ScratchImage image;

		switch (CheckFileFormat(desc._texturePath))
		{
		case FileFormat::DDS:
		{
			DirectX::LoadFromDDSFile(wcharTexturePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
			break;
		}
		case FileFormat::TGA:
		{
			DirectX::LoadFromTGAFile(wcharTexturePath.c_str(), DirectX::TGA_FLAGS_NONE, nullptr, image);
			break;
		}
		case FileFormat::HDR:
		{
			DirectX::LoadFromHDRFile(wcharTexturePath.c_str(), nullptr, image);
			break;
		}
		case FileFormat::WIC:
		default:
		{
			DirectX::LoadFromWICFile(wcharTexturePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
			break;
		}
		}

		HRESULT hr;

		if (image.GetImageCount() == 0)
		{
			DUOLGRAPHICS_ASSERT("D3D11Texture LoadFaile Error");
		}

		bool isCubeMap = image.GetMetadata().IsCubemap();

		if (isCubeMap)
		{
			DirectX::ScratchImage mipChain;

			hr = DirectX::GenerateMipMaps(
				image.GetImages()
				, image.GetImageCount()
				, image.GetMetadata()
				, DirectX::TEX_FILTER_DEFAULT
				, 0
				, mipChain);

			DXThrowError(hr, "D3D11Texture GenerateMipmaps Error");

			hr = DirectX::CreateTexture(device, mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), _texture._resource.GetAddressOf());
			DXThrowError(hr, "D3D11Texture CreateTexture Error");
		}
		else
		{
			hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture._resource.GetAddressOf());
			DXThrowError(hr, "D3D11Texture CreateTexture Error");
		}

		D3D11_TEXTURE2D_DESC textureDesc;
		_texture._tex2D->GetDesc(&textureDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = _textureDesc._mipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (isCubeMap)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		}

		hr = device->CreateShaderResourceView(_texture._resource.Get(), &srvDesc, _shaderResourceView.ReleaseAndGetAddressOf());
		DXThrowError(hr, "D3D11Texture CreateShadeResourceView ERROR");
	}

	int D3D11Texture::NumMipLevels(const DUOLMath::Vector3& textureExtend)
	{
		const float maxSize = std::max<float>({ textureExtend.x, textureExtend.y , textureExtend.z });
		const auto log2Size = static_cast<UINT>(std::log2(maxSize));

		return (1u + log2Size);
	}
}
