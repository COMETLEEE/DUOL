#include "D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace   DUOLGraphicsLibrary
{
	D3D11RenderTarget::~D3D11RenderTarget()
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, _renderTargetDesc._texture);
		castedTexture->DeleteRenderTarget(this);
	}

	void DUOLGraphicsLibrary::D3D11RenderTarget::CreateRenderTargetViews(ID3D11Device* device,
	                                                                     const RenderTargetDesc& renderTargetDesc)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, renderTargetDesc._texture);
		castedTexture->CreateRenderTarget(this);

		long textureBindFlags = MapDXBindFlag(castedTexture->GetTextureDesc()._bindFlags);

		SetRenderTargetDesc(castedTexture);		

		HRESULT hr;

		switch (renderTargetDesc._type)
		{
		case RenderTargetType::Color:
		{
			D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
			d3dRenderTargetViewDesc.Format = MapFormat(castedTexture->GetTextureDesc()._format);

			switch (castedTexture->GetTextureDesc()._type)
			{
			case TextureType::TEXTURE1D:
			{
				FillTexture1DViewDesc(renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			case TextureType::TEXTURE2D:
			{
				FillTexture2DViewDesc(renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			case TextureType::TEXTURE3D:
			{
				FillTexture3DViewDesc(renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			//todo
			case TextureType::TEXTURECUBE:
			{
				FillTexture2DArrayViewDesc(renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			case TextureType::TEXTURE1DARRAY: break;
			case TextureType::TEXTURE2DARRAY: break;
			case TextureType::TEXTURECUBEARRAY: break;
			case TextureType::TEXTURE2DMS: break;
			case TextureType::TEXTURE2DMSARRAY: break;
			default:;
			}

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			hr = device->CreateRenderTargetView(nativeTexture, &d3dRenderTargetViewDesc, _nativeRenderTarget._renderTargetView.GetAddressOf());
			DXThrowError(hr, "Create RenderTargetView Failed");

			break;
		}
		case RenderTargetType::Depth:
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;

			//todo
			switch (castedTexture->GetTextureDesc()._type)
			{
			case TextureType::TEXTURE1D:
			{
				d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				break;
			}
			case TextureType::TEXTURE2D:
			{
				d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				d3dDepthStencilViewDesc.Texture2D.MipSlice = renderTargetDesc._mipLevel;
				break;
			}
			case TextureType::TEXTURE1DARRAY: break;
			case TextureType::TEXTURE2DARRAY:
			{
				d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				d3dDepthStencilViewDesc.Texture2DArray.MipSlice = renderTargetDesc._mipLevel;
				d3dDepthStencilViewDesc.Texture2DArray.FirstArraySlice = renderTargetDesc._arrayLayer;
				d3dDepthStencilViewDesc.Texture2DArray.ArraySize = renderTargetDesc._arraySize;

				break;
			}
			case TextureType::TEXTURE2DMS: break;
			case TextureType::TEXTURE2DMSARRAY: break;
			default:;
			}

			FillDepthStencilViewDesc(renderTargetDesc, d3dDepthStencilViewDesc);


			d3dDepthStencilViewDesc.Texture2D.MipSlice = renderTargetDesc._mipLevel;
			d3dDepthStencilViewDesc.Flags = 0;

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			hr = device->CreateDepthStencilView(nativeTexture, &d3dDepthStencilViewDesc, _nativeRenderTarget._depthStencilView.GetAddressOf());
			DXThrowError(hr, "Create DepthStencilView Failed");

			break;
		}
		default:;
		}
	}

	void D3D11RenderTarget::SetRenderTargetDesc(D3D11Texture* texture)
	{
		auto& info = texture->GetTextureDesc();

		_renderTargetDesc._resolution.x = static_cast<float>(info._textureExtent.x * powf(0.5f, _renderTargetDesc._mipLevel));
		_renderTargetDesc._resolution.y = static_cast<float>(info._textureExtent.y * powf(0.5f, _renderTargetDesc._mipLevel));

		_renderTargetDesc._mipLevel = std::min<int>(_renderTargetDesc._mipLevel, info._mipLevels);

	}

	void D3D11RenderTarget::UnloadRenderTargetView()
	{
		_nativeRenderTarget._renderTargetView.Reset();
	}

	DUOLMath::Vector2 DUOLGraphicsLibrary::D3D11RenderTarget::GetResolution() const
	{
		return _renderTargetDesc._resolution;
	}

	void D3D11RenderTarget::SetResolution(ID3D11Device* device, D3D11Texture* texture, const DUOLMath::Vector2& resolution)
	{
		HRESULT hr;

		_renderTargetDesc._texture = texture;
		SetRenderTargetDesc(texture);
		auto desc = _renderTargetDesc._texture->GetTextureDesc();

		switch (_renderTargetDesc._type)
		{
		case RenderTargetType::Color:
		{
			D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
			ZeroMemory(&d3dRenderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
			d3dRenderTargetViewDesc.Format = MapFormat(desc._format);

			switch (desc._type)
			{
			case TextureType::TEXTURE1D:
			{
				FillTexture1DViewDesc(_renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			case TextureType::TEXTURE2D:
			{
				FillTexture2DViewDesc(_renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			case TextureType::TEXTURE3D:
			{
				FillTexture3DViewDesc(_renderTargetDesc, d3dRenderTargetViewDesc);
				break;
			}
			//todo
			case TextureType::TEXTURECUBE: break;
			case TextureType::TEXTURE1DARRAY: break;
			case TextureType::TEXTURE2DARRAY: break;
			case TextureType::TEXTURECUBEARRAY: break;
			case TextureType::TEXTURE2DMS: break;
			case TextureType::TEXTURE2DMSARRAY: break;
			default:;
			}

			ID3D11Resource* nativeTexture = texture->GetNativeTexture()._resource.Get();
			hr = device->CreateRenderTargetView(nativeTexture, &d3dRenderTargetViewDesc, _nativeRenderTarget._renderTargetView.GetAddressOf());
			DXThrowError(hr, "Create RenderTargetView Failed");

			break;
		}
		case RenderTargetType::Depth:
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;

			FillDepthStencilViewDesc(_renderTargetDesc, d3dDepthStencilViewDesc);

			d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			d3dDepthStencilViewDesc.Texture2D.MipSlice = _renderTargetDesc._mipLevel;
			d3dDepthStencilViewDesc.Flags = 0;

			ID3D11Resource* nativeTexture = texture->GetNativeTexture()._resource.Get();
			hr = device->CreateDepthStencilView(nativeTexture, &d3dDepthStencilViewDesc, _nativeRenderTarget._depthStencilView.GetAddressOf());
			DXThrowError(hr, "Create DepthStencilView Failed");

			break;
		}
		default:;
		}
	}

	int DUOLGraphicsLibrary::D3D11RenderTarget::GetNumberOfRenderTargets()
	{
		return 1;
	}

	bool DUOLGraphicsLibrary::D3D11RenderTarget::IsDepthStencil() const
	{
		return ((_renderTargetDesc._type > RenderTargetType::Color) ? true : false);
	}

	bool DUOLGraphicsLibrary::D3D11RenderTarget::IsColor() const
	{
		return ((_renderTargetDesc._type == RenderTargetType::Color) ? true : false);
	}

	/**
	 * \brief
	 * \param context
	 * \param color
	 */
	void DUOLGraphicsLibrary::D3D11RenderTarget::ClearRenderTarget(ID3D11DeviceContext* context, DUOLMath::Vector4 color)
	{
		switch (_renderTargetDesc._type)
		{
		case RenderTargetType::Color:
		{
			float fcolor[4]{ color.x, color.y, color.z, color.w };

			context->ClearRenderTargetView(_nativeRenderTarget._renderTargetView.Get(), fcolor);
			break;
		}
		case RenderTargetType::Depth:
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			context->ClearDepthStencilView(_nativeRenderTarget._depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			break;
		}
		default:;
		}

	}

	void D3D11RenderTarget::FillDepthStencilViewDesc(const RenderTargetDesc& renderTargetDesc,
		D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc)
	{
		switch (renderTargetDesc._type)
		{
		case RenderTargetType::Depth:
		{
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
		}
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		}
		case RenderTargetType::Color:
		default:
		{
			DUOLGRAPHICS_ASSERT("D3D11RendetTarget FillDepthStencil ERROR")
		}
		}
	}
}
