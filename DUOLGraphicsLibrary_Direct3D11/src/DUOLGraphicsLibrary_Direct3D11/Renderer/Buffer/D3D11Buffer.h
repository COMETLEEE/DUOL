#pragma once
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11Buffer : public Buffer
	{
	public:
		D3D11Buffer(const UINT64& guid, ID3D11Device* device, const BufferDesc& bufferDesc, const void* initialData = nullptr);

	private:
		ComPtr<ID3D11Buffer> _buffer;

		UINT _bufferSize;

		UINT _stride;

	private:
		DXGI_FORMAT _format;

		D3D11_USAGE _usage;

	private:
		void CreateBuffer(ID3D11Device* device, const BufferDesc& bufferDesc, const void* initialData);

	public:
		ID3D11Buffer* GetNativeBuffer() const
		{
			return _buffer.Get();
		}

		UINT GetBufferSize() const
		{
			return _bufferSize;
		}

		UINT GetStride() const
		{
			return _stride;
		}

		DXGI_FORMAT GetFormat() const
		{
			return _format;
		}

		D3D11_USAGE GetUsage() const
		{
			return _usage;
		}

	public:
		void UpdateSubresource(ID3D11DeviceContext* context, const void* data, UINT dataSize, UINT offset);

	};
}