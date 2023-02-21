#pragma once
#include <tuple>
#include <vector>

#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"


namespace DUOLGraphicsLibrary
{
	class D3D11RenderTarget;

	union D3D11NativeTexture
	{
		inline D3D11NativeTexture() :
			_resource{ nullptr }
		{
		}
		inline D3D11NativeTexture(const D3D11NativeTexture& texture) :
			_resource{ texture._resource }
		{
		}
		inline D3D11NativeTexture& operator = (const D3D11NativeTexture& texture)
		{
			_resource = texture._resource;
			return *this;
		}
		inline ~D3D11NativeTexture()
		{
			_resource.Reset();
		}

		ComPtr<ID3D11Resource>  _resource;
		ComPtr<ID3D11Texture1D> _tex1D;
		ComPtr<ID3D11Texture2D> _tex2D;
		ComPtr<ID3D11Texture3D> _tex3D;
	};

	class D3D11Texture : public Texture
	{
	public:
		D3D11Texture(const UINT64& guid, ID3D11Device* device, const TextureDesc& textureDesc);

		//for RenderContextTexture
		D3D11Texture(const UINT64& guid, const TextureDesc& textureDesc);

		enum class FileFormat
		{
			DDS,
			TGA,
			HDR,
			WIC
		};

	private:
		D3D11NativeTexture _texture;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

		ComPtr<ID3D11UnorderedAccessView> _unorderdAccessView;

		//이 텍스쳐로부터 생성된 렌더타겟입니다.
		//텍스쳐가 resize 되었을때, 렌더타겟들 또한 resizing 해줍니다
		std::vector<D3D11RenderTarget*> _renderTargets;

		bool _mipGenerate;

	private:
		FileFormat CheckFileFormat(const char* path);

		void SetTextureDesc(DXGI_FORMAT format, const DUOLMath::Vector3& extent, UINT mipLevels, UINT arraySize);

		UINT SetTextureMiscFlags(const TextureDesc& textureDesc);

	public:
		const D3D11NativeTexture& GetNativeTexture() const
		{
			return _texture;
		}

		ID3D11ShaderResourceView* GetShaderResourceView() const
		{
			return _shaderResourceView.Get();
		}

		ID3D11UnorderedAccessView* GetUnorderedAccessView() const
		{
			return _unorderdAccessView.Get();
		}

		virtual void* GetShaderResourceAddress() override
		{
			return _shaderResourceView.Get();
		}

	public:

		void CreateTexture1D(
			ID3D11Device* device,
			const TextureDesc& desc,
			const D3D11_SUBRESOURCE_DATA* initialData = nullptr,
			const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr,
			const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr
		);

		void CreateTexture2D(
			ID3D11Device* device,
			const TextureDesc& desc,
			const D3D11_SUBRESOURCE_DATA* initialData = nullptr,
			const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr,
			const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr
		);

		void CreateTexture3D(
			ID3D11Device* device,
			const TextureDesc& desc,
			const D3D11_SUBRESOURCE_DATA* initialData = nullptr,
			const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr,
			const D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr
		);


		void CreateShaderResourceView(ID3D11Device* device);

		void CreateUnorderedAccessView(ID3D11Device* device);

		void CreateTextureFromFile(ID3D11Device* device, const TextureDesc& desc);

		void CreateSubresourceCopyForCPUAccess(ID3D11Device* device, ID3D11DeviceContext* context, D3D11NativeTexture& copy, UINT accessFlag, const TextureLocation&
			location);

		void CreateRenderTarget(D3D11RenderTarget* renderTarget);

		void DeleteRenderTarget(D3D11RenderTarget* renderTarget);

		std::vector<D3D11RenderTarget*>& GetRenderTargets();

		bool SetResolution(ID3D11Device* device, const DUOLMath::Vector2& resolution);

		/**
	@brief Context의 backbuffer를 위한 함수입니다.
	@param device  -
	@param texture -
	@param format  -
**/
		void SetTexture(ID3D11Device* device, ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT format, const DUOLMath::Vector2& resolution);

		/**
			@brief Context의 backbuffer를 위한 함수입니다.
		**/
		void UnloadTexture();
	};
}

