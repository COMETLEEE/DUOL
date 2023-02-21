#pragma once
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"

namespace DUOLGraphicsLibrary
{
	class D3D11Texture;

	union D3D11NativeRenderTarget
	{
		inline D3D11NativeRenderTarget() :
			_renderTargetView{ nullptr }
		{
		}
		inline D3D11NativeRenderTarget(const D3D11NativeRenderTarget& renderTarget) :
			_renderTargetView{ renderTarget._renderTargetView }
		{
		}
		inline D3D11NativeRenderTarget& operator = (const D3D11NativeRenderTarget& renderTarget)
		{
			_renderTargetView = renderTarget._renderTargetView;
			return *this;
		}
		inline ~D3D11NativeRenderTarget()
		{
			_renderTargetView->Release();
		}

		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
	};

	class D3D11RenderTarget : public RenderTarget
	{
	public:
		D3D11RenderTarget(
			const UINT64& guid
			, ID3D11Device* device
			, const RenderTargetDesc& renderTargetDesc) :
			RenderTarget(guid)
			, _renderTargetDesc(renderTargetDesc)
		{
			CreateRenderTargetViews(device, renderTargetDesc);
		}

		//for RenderContextTexture
		D3D11RenderTarget(
			const UINT64& guid
			, ID3D11Device* device
			, ID3D11Texture2D* texture
			, RenderTargetType type, DXGI_FORMAT format, const DUOLMath::Vector2& resolution);

		D3D11RenderTarget(
			const UINT64& guid) :
			RenderTarget(guid)
		{
		}

		~D3D11RenderTarget() = default;

	private:
		RenderTargetDesc _renderTargetDesc;

		D3D11NativeRenderTarget _nativeRenderTarget;

	private:
		void CreateRenderTargetViews(ID3D11Device* device, const RenderTargetDesc& renderTargetDesc);

		void SetRenderTargetDesc(D3D11Texture* texture);

	public:
		void CreateRenderTargetViews(ID3D11Device* device, ID3D11Texture2D* texture, RenderTargetType type, DXGI_FORMAT format, const DUOLMath::Vector2& resoultion);

		void UnloadRenderTargetView();

		D3D11NativeRenderTarget GetNativeRenderTarget() const { return _nativeRenderTarget; }

		virtual DUOLMath::Vector2 GetResolution() const override final;

		virtual int GetNumberOfRenderTargets() override final;

		virtual bool IsDepthStencil() const override final;

		virtual bool IsColor() const override final;

	public:
		virtual void ClearRenderTarget(ID3D11DeviceContext* context, DUOLMath::Vector4 color);

		//for Resize
		void SetResolution(ID3D11Device* device, const DUOLMath::Vector2& resolution);

		static void FillTexture1DViewDesc(const RenderTargetDesc& renderTargetDesc, D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc);

		static void FillTexture2DViewDesc(const RenderTargetDesc& renderTargetDesc, D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc);

		static void FillTexture3DViewDesc(const RenderTargetDesc& renderTargetDesc, D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc); \

		static void FillTexture2DArrayViewDesc(const RenderTargetDesc& renderTargetDesc, D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc);

		static void FillDepthStencilViewDesc(const RenderTargetDesc& renderTargetDesc, D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc);

	};

	inline void D3D11RenderTarget::FillTexture1DViewDesc(const RenderTargetDesc& renderTargetDesc,
	                                                     D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc)
	{
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
		renderTargetViewDesc.Texture1D.MipSlice = renderTargetDesc._mipLevel;
	}

	inline void D3D11RenderTarget::FillTexture2DViewDesc(const RenderTargetDesc& renderTargetDesc,
		D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc)
	{
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = renderTargetDesc._mipLevel;
	}

	inline void D3D11RenderTarget::FillTexture3DViewDesc(const RenderTargetDesc& renderTargetDesc,
		D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc)
	{
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
		//TODO
	}

	inline void D3D11RenderTarget::FillTexture2DArrayViewDesc(const RenderTargetDesc& renderTargetDesc,
		D3D11_RENDER_TARGET_VIEW_DESC& renderTargetViewDesc)
	{
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		renderTargetViewDesc.Texture2DArray.MipSlice = renderTargetDesc._mipLevel;
		renderTargetViewDesc.Texture2DArray.FirstArraySlice = renderTargetDesc._arrayLayer;
		renderTargetViewDesc.Texture2DArray.ArraySize = renderTargetDesc._arraySize;
	}
}
