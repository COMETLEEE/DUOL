#pragma once
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"

namespace DUOLGraphicsLibrary
{
	class D3D11BufferWithRV : public D3D11Buffer
	{
	public:
		D3D11BufferWithRV(const UINT64& guid, ID3D11Device* device, const BufferDesc& bufferDesc, const void* initialData = nullptr);

		~D3D11BufferWithRV() override;

		void CreateShaderResourceView(ID3D11Device* device, const BufferDesc& bufferDesc);

		void CreateUnorderedAccessView(ID3D11Device* device, const BufferDesc& bufferDesc);

		ID3D11ShaderResourceView** GetSRV();

		// Returns the native UAV object.
		 ID3D11UnorderedAccessView** GetUAV();
	private:
		static UINT GetUAVFlags(const BufferDesc& desc);

		static UINT GetSRVFlags(const BufferDesc& desc);

	private:
		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

		ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView;
	};

}