#include "D3D11BufferWithRV.h"

#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"

DUOLGraphicsLibrary::D3D11BufferWithRV::D3D11BufferWithRV(const UINT64& guid, ID3D11Device* device,
	const BufferDesc& bufferDesc, const void* initialData) :
	D3D11Buffer(guid, device, bufferDesc, initialData)
{
	if (bufferDesc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE))
	{
		CreateShaderResourceView(device, bufferDesc);
	}
	if (bufferDesc._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS))
	{
		CreateUnorderedAccessView(device, bufferDesc);
	}
}

DUOLGraphicsLibrary::D3D11BufferWithRV::~D3D11BufferWithRV()
{
}

void DUOLGraphicsLibrary::D3D11BufferWithRV::CreateShaderResourceView(ID3D11Device* device, const BufferDesc& bufferDesc)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	{
		if (IsTypedBuffer(bufferDesc))
		{
			desc.Format = MapFormat(bufferDesc._format);
		}
		else if (IsByteAddressBuffer(bufferDesc))
		{
			desc.Format = DXGI_FORMAT_R32_TYPELESS; ///raw buffer have to r32 typeless format  
		}
		else
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
		}

		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		desc.BufferEx.FirstElement = 0;
		desc.BufferEx.Flags = GetSRVFlags(bufferDesc);

		if (desc.BufferEx.Flags == D3D11_BUFFEREX_SRV_FLAG_RAW)
		{
			desc.BufferEx.NumElements = static_cast<UINT>(bufferDesc._size) / 4;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		}
		else
		{
			desc.Buffer.NumElements = static_cast<UINT>(bufferDesc._size) / bufferDesc._stride;
		}

	}

	auto hr = device->CreateShaderResourceView(_buffer.Get(), &desc, _shaderResourceView.ReleaseAndGetAddressOf());
	DXThrowError(hr, "D3D11BUFFER ID3D11ShaderResourceView create failed");
}

void DUOLGraphicsLibrary::D3D11BufferWithRV::CreateUnorderedAccessView(ID3D11Device* device, const BufferDesc& bufferDesc)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	{
		if (IsTypedBuffer(bufferDesc))
		{
			desc.Format = MapFormat(bufferDesc._format);
		}
		else if (IsByteAddressBuffer(bufferDesc))
		{
			desc.Format = DXGI_FORMAT_R32_TYPELESS; ///raw buffer have to r32 typeless format  
		}
		else
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
		}

		desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.Flags = GetUAVFlags(bufferDesc);

		if(desc.Buffer.Flags == D3D11_BUFFER_UAV_FLAG_RAW)
		{
			desc.Buffer.NumElements = static_cast<UINT>(bufferDesc._size) / 4;
		}
		else
		{
			desc.Buffer.NumElements = static_cast<UINT>(bufferDesc._size) / bufferDesc._stride;
		}
	}

	auto hr = device->CreateUnorderedAccessView(_buffer.Get(), &desc, _unorderedAccessView.ReleaseAndGetAddressOf());
	DXThrowError(hr, "D3D11BUFFER ID3D11UnorderedAccessView create failed");
}

ID3D11ShaderResourceView** DUOLGraphicsLibrary::D3D11BufferWithRV::GetSRV()
{
	return _shaderResourceView.GetAddressOf();
}

ID3D11UnorderedAccessView** DUOLGraphicsLibrary::D3D11BufferWithRV::GetUAV()
{
	return _unorderedAccessView.GetAddressOf();
}

UINT DUOLGraphicsLibrary::D3D11BufferWithRV::GetUAVFlags(const BufferDesc& desc)
{
	if ((desc._bindFlags & static_cast<long>(BindFlags::UNORDEREDACCESS)) != 0)
	{
		if (IsStructuredBuffer(desc))
		{
			if ((desc._miscFlags & static_cast<long>(MiscFlags::RESOURCE_MISC_APPEND)) != 0)
				return D3D11_BUFFER_UAV_FLAG_APPEND;
			if ((desc._miscFlags & static_cast<long>(MiscFlags::RESOURCE_MISC_COUNTER)) != 0)
				return D3D11_BUFFER_UAV_FLAG_COUNTER;
		}
		else if (IsByteAddressBuffer(desc))
			return D3D11_BUFFER_UAV_FLAG_RAW;
	}
	return 0;
}

UINT DUOLGraphicsLibrary::D3D11BufferWithRV::GetSRVFlags(const BufferDesc& desc)
{
	if ((desc._bindFlags & static_cast<long>(BindFlags::SHADERRESOURCE)) != 0)
	{
		if (IsByteAddressBuffer(desc))
			return D3D11_BUFFEREX_SRV_FLAG_RAW;
	}
	return 0;

}
