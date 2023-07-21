#include "D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLCommon/StringHelper.h"
#include <algorithm>

namespace DUOLGraphicsLibrary
{
	D3D11Texture::D3D11Texture(const UINT64& guid, ID3D11Device* device, const TextureDesc& textureDesc) :
		Texture(guid, textureDesc)
		, _mipGenerate(false)
	{
		//if (textureDesc._texturePath == nullptr)
		//{
		//	if (textureDesc._mipLevels == 0)
		//		_mipGenerate = true;

		//	if (textureDesc._initData != nullptr)
		//	{
		//		D3D11_SUBRESOURCE_DATA subresource;
		//		subresource.pSysMem = textureDesc._initData;

		//		// R8G8B8A8
		//		subresource.SysMemPitch = textureDesc._textureExtent.x * 4;
		//		subresource.SysMemSlicePitch = 0;

		//		switch (textureDesc._type)
		//		{
		//		case TextureType::TEXTURE1D:
		//		case TextureType::TEXTURE1DARRAY:
		//		{

		//			CreateTexture1D(device, textureDesc, &subresource);
		//			break;
		//		}
		//		case TextureType::TEXTURE2D:
		//		case TextureType::TEXTURE2DARRAY:
		//		case TextureType::TEXTURECUBE:
		//		case TextureType::TEXTURECUBEARRAY:
		//		{
		//			CreateTexture2D(device, textureDesc, &subresource);
		//			break;
		//		}
		//		case TextureType::TEXTURE3D:
		//		{
		//			CreateTexture3D(device, textureDesc, &subresource);
		//			break;
		//		}
		//		case TextureType::TEXTURE2DMS:
		//		case TextureType::TEXTURE2DMSARRAY:
		//		{
		//			CreateTexture2D(device, textureDesc, &subresource);
		//			break;
		//		}
		//		default:
		//			DUOLGRAPHICS_ASSERT("failed to create texture with invalid texture type")
		//				break;
		//		}
		//	}
		//	else
		//	{
		//		switch (textureDesc._type)
		//		{
		//		case TextureType::TEXTURE1D:
		//		case TextureType::TEXTURE1DARRAY:
		//		{

		//			CreateTexture1D(device, textureDesc);
		//			break;
		//		}
		//		case TextureType::TEXTURE2D:
		//		case TextureType::TEXTURE2DARRAY:
		//		case TextureType::TEXTURECUBE:
		//		case TextureType::TEXTURECUBEARRAY:
		//		{
		//			CreateTexture2D(device, textureDesc);
		//			break;
		//		}
		//		case TextureType::TEXTURE3D:
		//		{
		//			CreateTexture3D(device, textureDesc);
		//			break;
		//		}
		//		case TextureType::TEXTURE2DMS:
		//		case TextureType::TEXTURE2DMSARRAY:
		//		{
		//			CreateTexture2D(device, textureDesc);
		//			break;
		//		}
		//		default:
		//			DUOLGRAPHICS_ASSERT("failed to create texture with invalid texture type")
		//				break;
		//		}
		//	}

		//	if (textureDesc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE))
		//	{
		//		CreateShaderResourceView(device);
		//	}
		//	if (textureDesc._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS))
		//	{
		//		CreateUnorderedAccessView(device);
		//	}

		//}
		//else
		//{
		//	CreateTextureFromFile(device, textureDesc);
		//}

		//_textureDesc._textureExtent = DUOLMath::Vector3{ floor(_textureDesc._textureExtent.x), floor(_textureDesc._textureExtent.y) , floor(_textureDesc._textureExtent.z) };
	}

	D3D11Texture::D3D11Texture(const UINT64& guid, const TextureDesc& textureDesc) :
		Texture(guid, textureDesc)
		, _mipGenerate(false)
	{
	}

	D3D11Texture::~D3D11Texture()
	{
	}


	D3D11Texture::FileFormat D3D11Texture::CheckFileFormat(const char* path)
	{
		std::string pathstr(path);

		if (pathstr.length() > 1)
		{
			auto extensionStartPoint = pathstr.find_last_of('.') + 1;

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

	void D3D11Texture::SetTextureDesc(DXGI_FORMAT format, const DUOLMath::Vector3& extent, UINT mipLevels, UINT arraySize)
	{
		_textureDesc._textureExtent = DUOLMath::Vector3{ floor(extent.x), floor(extent.y) , floor(extent.z) };
		_textureDesc._mipLevels = mipLevels;
		_textureDesc._arraySize = arraySize;
		_textureDesc._format = static_cast<ResourceFormat>(format);
	}

	UINT D3D11Texture::SetTextureMiscFlags(const TextureDesc& textureDesc)
	{
		UINT flagsD3D = 0;

		if (IsMipMappedTexture(textureDesc))
		{
			const long requiredFlags = static_cast<long>(BindFlags::SHADERRESOURCE) | static_cast<long>(BindFlags::SAMPLER);
			const long disallowedFlags = static_cast<long>(BindFlags::DEPTHSTENCIL);
			if ((textureDesc._bindFlags & (requiredFlags | disallowedFlags)) | requiredFlags)
				flagsD3D |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		if (IsCubeTexture(textureDesc._type))
			flagsD3D |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		return flagsD3D;
	}

	ID3D11ShaderResourceView* D3D11Texture::GetSubResourceShaderResourceView(int startMip, int mipSize, int startArray,
		int arraySize, ID3D11Device* device)
	{
		//해당 텍스쳐에서 "일부분"만 쉐이더리소스를 등록한다. ex) Array or Mips같은...
		startMip = std::min<int>(startMip, 255);
		mipSize = std::min<int>(mipSize, 255);
		startArray = std::min<int>(startArray, 255);
		arraySize = std::min<int>(arraySize, 255);

		int packedid = (startMip << 24) | (mipSize << 16) | (startArray << 8) | (arraySize);

		auto ret = _subresourceShaderResourceView.find(packedid);

		if (ret == _subresourceShaderResourceView.end())
		{
			auto srv = CreateSubResourceShaderResourceView(device, startMip, mipSize, startArray, arraySize);

			_subresourceShaderResourceView.emplace(packedid, srv);

			return srv.Get();
		}

		return ret->second.Get();
	}

	void D3D11Texture::Initialize(ID3D11Device* device, const TextureDesc& textureDesc)
	{
		if (textureDesc._texturePath == nullptr)
		{
			if (textureDesc._mipLevels == 0)
				_mipGenerate = true;


			if (textureDesc._initData != nullptr)
			{
				D3D11_SUBRESOURCE_DATA subresource;
				subresource.pSysMem = textureDesc._initData;

				// R8G8B8A8
				subresource.SysMemPitch = textureDesc._textureExtent.x * 4;
				subresource.SysMemSlicePitch = 0;

				switch (textureDesc._type)
				{
				case TextureType::TEXTURE1D:
				case TextureType::TEXTURE1DARRAY:
				{

					CreateTexture1D(device, textureDesc, &subresource);
					break;
				}
				case TextureType::TEXTURE2D:
				case TextureType::TEXTURE2DARRAY:
				case TextureType::TEXTURECUBE:
				case TextureType::TEXTURECUBEARRAY:
				{
					CreateTexture2D(device, textureDesc, &subresource);
					break;
				}
				case TextureType::TEXTURE3D:
				{
					CreateTexture3D(device, textureDesc, &subresource);
					break;
				}
				case TextureType::TEXTURE2DMS:
				case TextureType::TEXTURE2DMSARRAY:
				{
					CreateTexture2D(device, textureDesc, &subresource);
					break;
				}
				default:
					DUOLGRAPHICS_ASSERT("failed to create texture with invalid texture type")
						break;
				}
			}
			else
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

			if (textureDesc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE))
			{
				CreateShaderResourceView(device);
			}
			if (textureDesc._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS))
			{
				CreateUnorderedAccessView(device);
			}

		}
		else
		{
			CreateTextureFromFile(device, textureDesc);
		}

		_textureDesc._textureExtent = DUOLMath::Vector3{ floor(_textureDesc._textureExtent.x), floor(_textureDesc._textureExtent.y) , floor(_textureDesc._textureExtent.z) };
	}

	void D3D11Texture::CreateTexture1D(ID3D11Device* device, const TextureDesc& textureDesc,
	                                   const D3D11_SUBRESOURCE_DATA* initialData,
	                                   const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
	                                   const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE1D_DESC texture1DDesc;
		{
			texture1DDesc.Width = static_cast<UINT>(textureDesc._textureExtent.x);
			texture1DDesc.MipLevels = NumMipLevels(textureDesc, _mipGenerate);
			texture1DDesc.ArraySize = textureDesc._arraySize;
			texture1DDesc.Format = MapFormat(textureDesc._format);
			texture1DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture1DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture1DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture1DDesc.MiscFlags = SetTextureMiscFlags(textureDesc);
		}
		_textureDesc = textureDesc;
		_textureDesc._mipLevels = texture1DDesc.MipLevels;

		HRESULT hr = device->CreateTexture1D(&texture1DDesc, initialData, _texture._tex1D.GetAddressOf());
		DXThrowError(hr, "failed to create texture");
	}

	void D3D11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE2D_DESC texture2DDesc;
		{
			texture2DDesc.Width = static_cast<UINT>(textureDesc._textureExtent.x);
			texture2DDesc.Height = static_cast<UINT>(textureDesc._textureExtent.y);
			texture2DDesc.MipLevels = NumMipLevels(textureDesc, _mipGenerate);
			texture2DDesc.ArraySize = textureDesc._arraySize;
			texture2DDesc.Format = MapFormat(textureDesc._format);

			texture2DDesc.SampleDesc.Count = ((textureDesc._type >= TextureType::TEXTURECUBE)
				? max(1u, textureDesc._sample)
				: 1u);
			texture2DDesc.SampleDesc.Quality = 0;

			texture2DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture2DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture2DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture2DDesc.MiscFlags = SetTextureMiscFlags(textureDesc);

		}
		_textureDesc = textureDesc;
		_textureDesc._mipLevels = texture2DDesc.MipLevels;

		HRESULT hr = device->CreateTexture2D(&texture2DDesc, initialData, _texture._tex2D.GetAddressOf());
		DXThrowError(hr, "failed to create texture");
	}

	void D3D11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& textureDesc,
		const D3D11_SUBRESOURCE_DATA* initialData,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc,
		const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
	{
		D3D11_TEXTURE3D_DESC texture3DDesc;
		{
			texture3DDesc.Width = static_cast<UINT>(textureDesc._textureExtent.x);
			texture3DDesc.Height = static_cast<UINT>(textureDesc._textureExtent.y);
			texture3DDesc.Depth = static_cast<UINT>(textureDesc._textureExtent.z);
			texture3DDesc.MipLevels = NumMipLevels(textureDesc, _mipGenerate);
			texture3DDesc.Format = MapFormat(textureDesc._format);
			texture3DDesc.Usage = MapDXUsageFlag(textureDesc._usage);
			texture3DDesc.BindFlags = MapDXBindFlag(textureDesc._bindFlags);
			texture3DDesc.CPUAccessFlags = MapDXCPUAccessFlag(textureDesc._cpuAccessFlags);
			texture3DDesc.MiscFlags = SetTextureMiscFlags(textureDesc);
		}
		_textureDesc = textureDesc;
		_textureDesc._mipLevels = texture3DDesc.MipLevels;

		HRESULT hr = device->CreateTexture3D(&texture3DDesc, initialData, _texture._tex3D.GetAddressOf());
		DXThrowError(hr, "failed to create texture");

	}

	ComPtr<ID3D11ShaderResourceView> D3D11Texture::CreateSubResourceShaderResourceView(
		ID3D11Device* device, int startMip, int mipSize,
		int startArray, int arraySize)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = MapFormat(_textureDesc._format);
		ComPtr<ID3D11ShaderResourceView> srv;

		//todo:: 흠.... 뎁스는 이걸로밖에 안되네.. 우짜냐 맵함수를 고도화시키는 방법밖엔 없지않을까싶다.
		if (srvDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
		{
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		if (srvDesc.Format == DXGI_FORMAT_R32_TYPELESS)
		{
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		switch (_textureDesc._type) {

		case TextureType::TEXTURE1D:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MostDetailedMip = startMip;
			srvDesc.Texture1D.MipLevels = mipSize;
			break;
		}
		case TextureType::TEXTURE2D:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = mipSize;
			srvDesc.Texture2D.MostDetailedMip = startMip;
			break;
		}
		case TextureType::TEXTURE3D: break;
		case TextureType::TEXTURECUBE:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = mipSize;
			srvDesc.TextureCube.MostDetailedMip = startMip;
			break;
		case TextureType::TEXTURE1DARRAY: break;
		case TextureType::TEXTURE2DARRAY:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.ArraySize = arraySize;
			srvDesc.Texture2DArray.FirstArraySlice = startArray;
			srvDesc.Texture2DArray.MipLevels = mipSize;
			srvDesc.Texture2DArray.MostDetailedMip = startMip;
			break;
		}
		case TextureType::TEXTURECUBEARRAY:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			srvDesc.TextureCubeArray.NumCubes = arraySize / 6;
			srvDesc.TextureCubeArray.First2DArrayFace = startArray;
			srvDesc.TextureCubeArray.MipLevels = mipSize;
			srvDesc.TextureCubeArray.MostDetailedMip = startMip;
			break;
		}
		case TextureType::TEXTURE2DMS: break;
		case TextureType::TEXTURE2DMSARRAY: break;
		default:;
		}

		HRESULT hr = device->CreateShaderResourceView(_texture._resource.Get(), &srvDesc, srv.GetAddressOf());

		DXThrowError(hr, "D3D11Texture CreateShadeResourceView ERROR");

		return srv;
	}

	void D3D11Texture::SetTexture(ID3D11Device* device, ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT format, const DUOLMath::Vector2& resolution)
	{
		//백버퍼 텍스쳐 바인딩
		_texture._tex2D = texture;

		_textureDesc._textureExtent = DUOLMath::Vector3{ floorf(resolution.x), floor(resolution.y), 1.f };
		CreateShaderResourceView(device);

		for (auto rtv : _renderTargets)
		{
			rtv->SetResolution(device, this, resolution);
		}
	}

	void D3D11Texture::CreateShaderResourceView(ID3D11Device* device)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = MapFormat(_textureDesc._format);
		_shaderResourceView.Reset();

		//todo:: 흠.... 뎁스는 이걸로밖에 안되네.. 우짜냐 맵함수를 고도화시키는 방법밖엔 없지않을까싶다.
		if (srvDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
		{
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		if (srvDesc.Format == DXGI_FORMAT_R32_TYPELESS)
		{
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		switch (_textureDesc._type) {

		case TextureType::TEXTURE1D:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MipLevels = _textureDesc._mipLevels;
			srvDesc.Texture1D.MostDetailedMip = 0;
			break;
		}
		case TextureType::TEXTURE2D:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = _textureDesc._mipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			break;
		}
		case TextureType::TEXTURE3D: break;
		case TextureType::TEXTURECUBE:
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = _textureDesc._mipLevels;
			srvDesc.TextureCube.MostDetailedMip = 0;
			break;
		case TextureType::TEXTURE1DARRAY: break;
		case TextureType::TEXTURE2DARRAY:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.ArraySize = _textureDesc._arraySize;
			srvDesc.Texture2DArray.MipLevels = _textureDesc._mipLevels;
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			break;
		}
		case TextureType::TEXTURECUBEARRAY:
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			srvDesc.TextureCubeArray.NumCubes = _textureDesc._arraySize / 6;
			srvDesc.TextureCubeArray.MipLevels = _textureDesc._mipLevels;
			srvDesc.TextureCubeArray.MostDetailedMip = 0;
			srvDesc.TextureCubeArray.First2DArrayFace = 0;
			break;
		}
		case TextureType::TEXTURE2DMS: break;
		case TextureType::TEXTURE2DMSARRAY: break;
		default:;
		}

		HRESULT hr = device->CreateShaderResourceView(_texture._resource.Get(), &srvDesc, _shaderResourceView.GetAddressOf());

		DXThrowError(hr, "D3D11Texture CreateShadeResourceView ERROR");

	}

	void D3D11Texture::CreateUnorderedAccessView(ID3D11Device* device)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		uavDesc.Format = MapFormat(_textureDesc._format);

		//todo:: 흠.... 뎁스는 이걸로밖에 안되네.. 우짜냐 맵함수를 고도화시키는 방법밖엔 없지않을까싶다.
		if (uavDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
		{
			uavDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}
		if (uavDesc.Format == DXGI_FORMAT_R32_TYPELESS)
		{
			uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		switch (_textureDesc._type) {

		case TextureType::TEXTURE1D:
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
			break;
		}
		case TextureType::TEXTURE2D:
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			break;
		}
		case TextureType::TEXTURE3D: break;
		case TextureType::TEXTURECUBE:
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
			break;
		case TextureType::TEXTURE1DARRAY: break;
		case TextureType::TEXTURE2DARRAY:
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			break;
		}
		case TextureType::TEXTURECUBEARRAY: break;
		case TextureType::TEXTURE2DMS: break;
		case TextureType::TEXTURE2DMSARRAY: break;
		default:;
		}

		device->CreateUnorderedAccessView(_texture._resource.Get(), &uavDesc, _unorderdAccessView.GetAddressOf());
	}

	void D3D11Texture::CreateTextureFromFile(ID3D11Device* device, const TextureDesc& desc)
	{
		auto wcharTexturePath = DUOLCommon::StringHelper::StringToWString(desc._texturePath);

		auto spos = wcharTexturePath.find(TEXT("<UDIM>."));
		if (spos != DUOLCommon::tstring::npos)
		{
			wcharTexturePath.erase(spos, 7);
		}
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

			/*hr = DirectX::GenerateMipMaps(
				image.GetImages()
				, image.GetImageCount()
				, image.GetMetadata()
				, DirectX::TEX_FILTER_DEFAULT
				, 0
				, mipChain);*/

				//DXThrowError(hr, "D3D11Texture GenerateMipmaps Error");

			hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture._resource.GetAddressOf());

			std::string errer = desc._texturePath;
			errer += "\nD3D11Texture CreateTexture Error";
			/*if (FAILED(hr))
				DUOL_ENGINE_ERROR(errer.c_str());*/
			DXThrowError(hr, errer.c_str());
		}
		else
		{
			hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture._resource.GetAddressOf());
			std::string errer = desc._texturePath;
			errer += "\nD3D11Texture CreateTexture Error";
			/*if (FAILED(hr))
				DUOL_ENGINE_ERROR(errer.c_str());*/
			DXThrowError(hr, errer.c_str());
		}

		D3D11_TEXTURE2D_DESC textureDesc;

		if (!_texture._tex2D) // 만약 텍스쳐를 못 불러왔다면...!
		{
			auto noneTexturePath = ("Asset/Texture/NoneTexture.png");

			wcharTexturePath = DUOLCommon::StringHelper::StringToWString(noneTexturePath);

			switch (CheckFileFormat(noneTexturePath))
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

				/*hr = DirectX::GenerateMipMaps(
					image.GetImages()
					, image.GetImageCount()
					, image.GetMetadata()
					, DirectX::TEX_FILTER_DEFAULT
					, 0
					, mipChain);*/

					//DXThrowError(hr, "D3D11Texture GenerateMipmaps Error");

				hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture._resource.GetAddressOf());
				DXThrowError(hr, "D3D11Texture CreateTexture Error");
			}
			else
			{
				hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _texture._resource.GetAddressOf());
				DXThrowError(hr, "D3D11Texture CreateTexture Error");
			}
		}

		_texture._tex2D->GetDesc(&textureDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = _textureDesc._mipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (isCubeMap)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			_textureDesc._type = TextureType::TEXTURECUBE;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			_textureDesc._type = TextureType::TEXTURE2D;
		}

		hr = device->CreateShaderResourceView(_texture._resource.Get(), &srvDesc, _shaderResourceView.GetAddressOf());
		DXThrowError(hr, "D3D11Texture CreateShadeResourceView ERROR");

		SetTextureDesc(textureDesc.Format, { static_cast<float>(textureDesc.Width), static_cast<float>(textureDesc.Height), 0.f }, textureDesc.MipLevels, textureDesc.MipLevels);

	}

	void D3D11Texture::CreateSubresourceCopyForCPUAccess(ID3D11Device* device, ID3D11DeviceContext* context, D3D11NativeTexture& copy, UINT accessFlag, const TextureLocation& location)
	{
		D3D11_RESOURCE_DIMENSION dimension;
		_texture._resource->GetType(&dimension);

		switch (dimension)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			/* Create temporary 1D texture with a similar descriptor */
			D3D11_TEXTURE1D_DESC desc;
			_texture._tex1D->GetDesc(&desc);
			{
				desc.Width = 1;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Usage = D3D11_USAGE_STAGING;
				desc.BindFlags = 0;
				desc.CPUAccessFlags = accessFlag;
				desc.MiscFlags = 0;
			}
			device->CreateTexture1D(&desc, nullptr, copy._tex1D.GetAddressOf());
		}
		break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			/* Query and modify descriptor for 2D texture */
			D3D11_TEXTURE2D_DESC desc;
			_texture._tex2D->GetDesc(&desc);
			{
				desc.Width = 1;
				desc.Height = 1;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Usage = D3D11_USAGE_STAGING;
				desc.BindFlags = 0;
				desc.CPUAccessFlags = accessFlag;
				desc.MiscFlags = (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE);
			}
			device->CreateTexture2D(&desc, nullptr, copy._tex2D.GetAddressOf());
		}
		break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			/* Query and modify descriptor for 3D texture */
			D3D11_TEXTURE3D_DESC desc;
			_texture._tex3D->GetDesc(&desc);
			{
				desc.Width = 1;
				desc.Height = 1;
				desc.Depth = 1;
				desc.MipLevels = 1;
				desc.Usage = D3D11_USAGE_STAGING;
				desc.BindFlags = 0;
				desc.CPUAccessFlags = accessFlag;
				desc.MiscFlags = 0;
			}
			device->CreateTexture3D(&desc, nullptr, copy._tex3D.GetAddressOf());
		}
		break;
		}

		const UINT mipLevel = location._mipLevel;

		const D3D11_BOX srcBox
		{
			static_cast<UINT>(location._offset.x),
			static_cast<UINT>(location._offset.y),
			static_cast<UINT>(location._offset.z),
			static_cast<UINT>(location._offset.x) + 1,
			static_cast<UINT>(location._offset.y) + 1,
			static_cast<UINT>(location._offset.z) + 1,
		};

		context->CopySubresourceRegion(
			copy._resource.Get(),
			D3D11CalcSubresource(0, 0, 0),
			0, // DstX
			0, // DstY
			0, // DstZ
			_texture._resource.Get(),
			//D3D11CalcSubresource(mipLevel, 0, 0),
			D3D11CalcSubresource(0, 0, 0),
			&srcBox
		);
	}

	void D3D11Texture::CreateRenderTarget(D3D11RenderTarget* renderTarget)
	{
		_renderTargets.emplace_back(renderTarget);
	}

	void D3D11Texture::DeleteRenderTarget(D3D11RenderTarget* renderTarget)
	{
		//O(n)이지만 렌더타겟 수가 그렇게 많지 않다. 많아봐야 mipmap * arraysize 인데... 

		std::vector<D3D11RenderTarget*>::iterator find;

		for (find = _renderTargets.begin(); find != _renderTargets.end(); ++find)
		{
			if (*find == renderTarget)
			{
				break;;
			}
		}

		if (find == _renderTargets.end())
		{
			return;
		}
		else
		{
			_renderTargets.erase(find);
		}
	}

	void D3D11Texture::UnloadTexture()
	{
		for (auto& rtv : _renderTargets)
		{
			rtv->UnloadRenderTargetView();
		}

		_subresourceShaderResourceView.clear();
		_shaderResourceView.Reset();
		_texture._tex2D.Reset();
		;
	}

	std::vector<D3D11RenderTarget*>& D3D11Texture::GetRenderTargets()
	{
		return _renderTargets;
	}

	bool D3D11Texture::SetResolution(ID3D11Device* device, const DUOLMath::Vector2& resolution)
	{
		UnloadTexture();

		switch (_textureDesc._type)
		{
		case TextureType::TEXTURE2D:
		{
			_textureDesc._textureExtent.x = floor(resolution.x);
			_textureDesc._textureExtent.y = floor(resolution.y);

			CreateTexture2D(device, _textureDesc);
			break;
		}
		case TextureType::TEXTURE1D:
		case TextureType::TEXTURE3D:
		case TextureType::TEXTURECUBE:
		case TextureType::TEXTURE1DARRAY:
		case TextureType::TEXTURE2DARRAY:
		case TextureType::TEXTURECUBEARRAY:
		case TextureType::TEXTURE2DMS:
		case TextureType::TEXTURE2DMSARRAY:
		default:
		{
			return false;
			break;
		}
		}

		if (_textureDesc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE))
		{
			CreateShaderResourceView(device);
		}
		if (_textureDesc._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS))
		{
			CreateUnorderedAccessView(device);
		}

		//렌더타겟들 또한 Resize 해줍니다.
		for (auto rtv : _renderTargets)
		{
			rtv->SetResolution(device, this, resolution);
		}

		return true;
	}

}
