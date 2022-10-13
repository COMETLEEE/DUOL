#pragma once
#include <tuple>

#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"


namespace DUOLGraphicsLibrary
{
    union D3D11NativeTexture
    {
        inline D3D11NativeTexture() :
            resource{ nullptr }
        {
        }
        inline D3D11NativeTexture(const D3D11NativeTexture& rhs) :
            resource{ rhs.resource }
        {
        }
        inline D3D11NativeTexture& operator = (const D3D11NativeTexture& rhs)
        {
            resource = rhs.resource;
            return *this;
        }
        inline ~D3D11NativeTexture()
        {
            resource.Reset();
        }

        ComPtr<ID3D11Resource>  resource;
        ComPtr<ID3D11Texture1D> tex1D;
        ComPtr<ID3D11Texture2D> tex2D;
        ComPtr<ID3D11Texture3D> tex3D;
    };

	class D3D11Texture : public Texture
	{
	public:
        D3D11Texture(ID3D11Device* device, const TextureDesc& textureDesc);

	private:
        D3D11NativeTexture _texture;

        ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

        std::tuple<int, int, int> _textureExtend;

        DXGI_FORMAT _format;

        UINT _mipLevel;

        UINT _arrayLayer;

    public:
        const D3D11NativeTexture& GetNativeTexture() const
        {
	        return _texture;
        }

        ComPtr<ID3D11ShaderResourceView> GetShadowResourceView() const
        {
	        return _shaderResourceView;
        }

        std::tuple<int, int, int> GetTextureExtends() const
        {
	        return _textureExtend;
        }

        DXGI_FORMAT GetFormat() const
        {
	        return _format;
        }

        UINT GetMipLevel() const
        {
	        return _mipLevel;
        }

        UINT GetArrayLayer() const
        {
	        return _arrayLayer;
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

        void UpdateSubresource(
            ID3D11DeviceContext* context,
            UINT                        mipLevel,
            UINT                        arrayLayer,
            const D3D11_BOX& region,
            //const SrcImageDescriptor& imageDesc,
            std::size_t                 threadCount
        );

	};
}

