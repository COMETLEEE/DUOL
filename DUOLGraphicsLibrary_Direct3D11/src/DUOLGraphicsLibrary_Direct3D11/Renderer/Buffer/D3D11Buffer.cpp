#include "D3D11Buffer.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

namespace DUOLGraphicsLibrary
{
	D3D11Buffer::D3D11Buffer(ID3D11Device* device, const BufferDesc& bufferDesc, const void* initialData)
	{
		CreateBuffer(device, bufferDesc, initialData);
	}

	void D3D11Buffer::CreateBuffer(ID3D11Device* device, const BufferDesc& bufferDesc, const void* initialData)
	{
		D3D11_BUFFER_DESC d3dBufferDesc;
		{
			d3dBufferDesc.ByteWidth = bufferDesc._size;
			d3dBufferDesc.Usage = (D3D11_USAGE)bufferDesc._usage;
			d3dBufferDesc.BindFlags = bufferDesc._bindFlags;
			d3dBufferDesc.CPUAccessFlags = bufferDesc._cpuAccessFlags;
			d3dBufferDesc.MiscFlags = bufferDesc._miscFlags;
			d3dBufferDesc.StructureByteStride = bufferDesc._stride;
		}

		if(initialData != nullptr)
		{
			D3D11_SUBRESOURCE_DATA subresourceData;
			{
				subresourceData.pSysMem = initialData;
				subresourceData.SysMemPitch = 0;
				subresourceData.SysMemSlicePitch = 0;
			}

			auto hr = device->CreateBuffer(&d3dBufferDesc, &subresourceData, _buffer.ReleaseAndGetAddressOf());
			DXThrowError(hr, "D3D11Buffer CreateBuffer Failed");
		}

	}

	void D3D11Buffer::UpdateSubresource(ID3D11DeviceContext* context, const void* data, UINT dataSize, UINT offset)
	{
		if(_bufferSize < dataSize + offset)
		{
			DUOLGRAPHICS_ASSERT("D3D11Buffer UpdateSubresource Buffer Size Over")
		}

		if(_usage == D3D11_USAGE_DYNAMIC)
		{
			D3D11_MAPPED_SUBRESOURCE  subResource;
			context->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

			memcpy(reinterpret_cast<char*>(subResource.pData)+ offset, data, dataSize);

			context->Unmap(_buffer.Get(), 0);
		}
		else
		{
			//일단 buffer가 dynamic이 아니면 의심하라.
			const D3D11_BOX dstBox{ offset, 0, 0, offset + dataSize, 1, 1 };
			context->UpdateSubresource(_buffer.Get(), 0, &dstBox, data, 0, 0);

		}
	}
}
