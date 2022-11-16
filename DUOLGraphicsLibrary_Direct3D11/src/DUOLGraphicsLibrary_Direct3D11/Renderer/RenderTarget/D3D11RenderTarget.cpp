#include "D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace   DUOLGraphicsLibrary
{
	void DUOLGraphicsLibrary::D3D11RenderTarget::CreateRenderTargetViews(ID3D11Device* device,
		const RenderTargetDesc& renderTargetDesc)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, renderTargetDesc._texture);

		long textureBindFlags = MapDXBindFlag(castedTexture->GetTextureDesc()._bindFlags);

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
			case TextureType::TEXTURECUBE: break;
			case TextureType::TEXTURE1DARRAY: break;
			case TextureType::TEXTURE2DARRAY: break;
			case TextureType::TEXTURECUBEARRAY: break;
			case TextureType::TEXTURE2DMS: break;
			case TextureType::TEXTURE2DMSARRAY: break;
			default:;
			}

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			device->CreateRenderTargetView(nativeTexture, &d3dRenderTargetViewDesc, _nativeRenderTarget._renderTargetView.ReleaseAndGetAddressOf());

			break;
		}
		case RenderTargetType::Depth:
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;

			FillDepthStencilViewDesc(renderTargetDesc, d3dDepthStencilViewDesc);

			d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			d3dDepthStencilViewDesc.Texture2D.MipSlice = renderTargetDesc._mipLevel;
			d3dDepthStencilViewDesc.Flags = 0;

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			device->CreateDepthStencilView(nativeTexture, &d3dDepthStencilViewDesc, _nativeRenderTarget._depthStencilView.ReleaseAndGetAddressOf());
			break;
		}
		default:;
		}
	}

	void D3D11RenderTarget::CreateRenderTargetViews(ID3D11Device* device, ID3D11Texture2D* texture,
		RenderTargetType type, DXGI_FORMAT format)
	{
		switch (type)
		{
		case RenderTargetType::Color:
		{
			D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
			ZeroMemory(&d3dRenderTargetViewDesc, sizeof(d3dRenderTargetViewDesc));

			d3dRenderTargetViewDesc.Format = format;
			d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;

			ID3D11Resource* nativeTexture = texture;
			device->CreateRenderTargetView(nativeTexture, &d3dRenderTargetViewDesc, _nativeRenderTarget._renderTargetView.GetAddressOf());

			break;
		}
		case RenderTargetType::Depth:
		case RenderTargetType::Stencil:
		case RenderTargetType::DepthStencil:
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
			ZeroMemory(&d3dDepthStencilViewDesc, sizeof(d3dDepthStencilViewDesc));

			d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

			switch (type)
			{
			case RenderTargetType::Depth:
			{
				d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
				break;
			}
			case RenderTargetType::Stencil:
			case RenderTargetType::DepthStencil:
			{
				d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				break;
			}
			case RenderTargetType::Color:
			default:
			{
				DUOLGRAPHICS_ASSERT("D3D11RendetTarget FillDepthStencil ERROR")
			}
			}

			ID3D11Resource* nativeTexture = texture;

			device->CreateDepthStencilView(nativeTexture, &d3dDepthStencilViewDesc, _nativeRenderTarget._depthStencilView.GetAddressOf());
			break;
		}
		default:;
		}
	}

	void D3D11RenderTarget::UnloadRenderTargetView()
	{
		_nativeRenderTarget._renderTargetView->Release();
	}

	DUOLMath::Vector2 DUOLGraphicsLibrary::D3D11RenderTarget::GetResolution() const
	{
		return _renderTargetDesc._resolution;
	}

	void D3D11RenderTarget::SetResolution(ID3D11Device* device, const DUOLMath::Vector2& resolution)
	{
		D3D11Texture* castedTexture = TYPE_CAST(D3D11Texture*, _renderTargetDesc._texture);

		TextureDesc textureDesc = castedTexture->GetTextureDesc();

		switch (textureDesc._type)
		{
		case TextureType::TEXTURE2D:
		{

			textureDesc._textureExtent.x = resolution.x;
			textureDesc._textureExtent.y = resolution.y;

			castedTexture->CreateTexture2D(device, textureDesc);
			castedTexture->CreateShaderResourceView(device);

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
			break;
		}
		}

		long textureBindFlags = MapDXBindFlag(castedTexture->GetTextureDesc()._bindFlags);

		switch (_renderTargetDesc._type)
		{
		case RenderTargetType::Color:
		{
			D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
			d3dRenderTargetViewDesc.Format = MapFormat(castedTexture->GetTextureDesc()._format);

			switch (castedTexture->GetTextureDesc()._type)
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

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			device->CreateRenderTargetView(nativeTexture, &d3dRenderTargetViewDesc, _nativeRenderTarget._renderTargetView.ReleaseAndGetAddressOf());

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

			ID3D11Resource* nativeTexture = castedTexture->GetNativeTexture()._tex2D.Get();
			device->CreateDepthStencilView(nativeTexture, &d3dDepthStencilViewDesc, _nativeRenderTarget._depthStencilView.ReleaseAndGetAddressOf());
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
			//context->ClearDepthStencilView(_nativeRenderTarget._depthStencilView.Get())
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
