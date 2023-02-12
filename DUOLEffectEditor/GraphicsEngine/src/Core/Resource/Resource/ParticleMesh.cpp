#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/DirectX11/DXEngine.h"
#include "util/VertexDesc.h"

namespace MuscleGrapics
{
	ParticleMesh::ParticleMesh() :
		_device(nullptr),
		_maxParticles(0),
		_dim(0)
	{
		_device = DXEngine::GetInstance()->GetD3dDevice();
		_d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		InitCounterBuffer();
		SetMaxParticleSize(1000);
	}

	ParticleMesh::~ParticleMesh()
	{
		ReleaseCOM(_indexVB);

		ReleaseCOM(_particleBufferUAV);
		ReleaseCOM(_particleBufferSRV);
		ReleaseCOM(_particleBuffer);

		ReleaseCOM(_particleEmitterCountBufferUAV);
		ReleaseCOM(_particleEmitterCountBuffer);
	}

	void ParticleMesh::InitCounterBuffer()
	{
		ReleaseCOM(_particleEmitterCountBufferUAV);
		ReleaseCOM(_particleEmitterCountBuffer);

		D3D11_BUFFER_DESC buffDesc;
		ZeroMemory(&buffDesc, sizeof(buffDesc));
		buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		buffDesc.ByteWidth = sizeof(Vertex::CountAndEmitterTime);
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffDesc.StructureByteStride = sizeof(Vertex::CountAndEmitterTime);
		buffDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA	data;
		ZeroMemory(&data, sizeof(data));

		unsigned int initData = 0;

		data.pSysMem = &initData;

		if (FAILED(_device->CreateBuffer(&buffDesc, &data, &_particleEmitterCountBuffer))) {
			assert(false);
		}

		ZeroMemory(&buffDesc, sizeof(buffDesc));
		_particleEmitterCountBuffer->GetDesc(&buffDesc);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		uavDesc.Buffer.NumElements = 1;

		if (FAILED(_device->CreateUnorderedAccessView(_particleEmitterCountBuffer, &uavDesc, &_particleEmitterCountBufferUAV))) {
			assert(false);
		}
	}
	void ParticleMesh::SetMaxParticleSize(unsigned int size)
	{
		if (size == _maxParticles)
			return;

		_maxParticles = size;

		ReleaseCOM(_particleBufferUAV);
		ReleaseCOM(_particleBufferSRV);
		ReleaseCOM(_particleBuffer);

		ReleaseCOM(_indexVB);

		_vbd.ByteWidth = sizeof(unsigned int) * (_maxParticles);

		_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

		HR(_device->CreateBuffer(&_vbd, 0, &_indexVB));

		std::vector<Vertex::Particle> initVertex(_maxParticles);

		D3D11_BUFFER_DESC buffDesc;
		ZeroMemory(&buffDesc, sizeof(buffDesc));
		buffDesc.ByteWidth = sizeof(Vertex::Particle) * _maxParticles;
		buffDesc.Usage = D3D11_USAGE_DEFAULT;
		buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.StructureByteStride = sizeof(Vertex::Particle);

		D3D11_SUBRESOURCE_DATA	data;
		ZeroMemory(&data, sizeof(data));

		data.pSysMem = initVertex.data();
		data.SysMemPitch = sizeof(Vertex::Particle) * _maxParticles;
		data.SysMemSlicePitch = 0;

		if (FAILED(_device->CreateBuffer(&buffDesc, &data, &_particleBuffer))) {
			assert(false);
		}

		ZeroMemory(&buffDesc, sizeof(buffDesc));
		_particleBuffer->GetDesc(&buffDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		srvDesc.Buffer.NumElements = _maxParticles;

		if (FAILED(_device->CreateShaderResourceView(_particleBuffer, &srvDesc, &_particleBufferSRV))) {
			assert(false);
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		uavDesc.Buffer.NumElements = _maxParticles;

		if (FAILED(_device->CreateUnorderedAccessView(_particleBuffer, &uavDesc, &_particleBufferUAV))) {
			assert(false);
		}
	}
	void ParticleMesh::UpdateCounter(float timer)
	{
		if (_maxParticles % 1024 == 0)
			_dim = _maxParticles / 1024;
		else
			_dim = _maxParticles / 1024 + 1;

		_dim = static_cast<int>(ceil(sqrt(_dim)));

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		_d3dImmediateContext->Map(_particleEmitterCountBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);

		Vertex::CountAndEmitterTime* dataPtr = static_cast<Vertex::CountAndEmitterTime*>(mappedResource.pData);

		dataPtr->value = 0;
		dataPtr->time = timer;
		dataPtr->dimx = _dim;
		dataPtr->dimy = _dim;

		_d3dImmediateContext->Unmap(_particleEmitterCountBuffer, 0);
	}
	void ParticleMesh::ResetParticleBuffer()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		_d3dImmediateContext->Map(_particleBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);

		Vertex::Particle** dataPtr = static_cast<Vertex::Particle**>(mappedResource.pData);

		memset(dataPtr, 0, sizeof(Vertex::Particle) * _maxParticles);

		_d3dImmediateContext->Unmap(_particleBuffer, 0);
	}
	void ParticleMesh::ExecuteDraw()
	{
		ID3D11ShaderResourceView* pNullSRV = NULL;

		_d3dImmediateContext->VSSetShaderResources(0, 1, &pNullSRV);

		_d3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &_particleBufferUAV, NULL);
		_d3dImmediateContext->CSSetUnorderedAccessViews(1, 1, &_particleEmitterCountBufferUAV, NULL);

		_d3dImmediateContext->Dispatch(_dim, _dim, 1); // ½ÇÇà.

		ID3D11UnorderedAccessView* pNullUAV = NULL;

		_d3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, NULL);
		_d3dImmediateContext->CSSetUnorderedAccessViews(1, 1, &pNullUAV, NULL);
	}
	void ParticleMesh::VSSetResource()
	{
		UINT stride = sizeof(unsigned int);

		UINT offset = 0;

		_d3dImmediateContext->VSSetShaderResources(0, 1, &_particleBufferSRV);

		_d3dImmediateContext->IASetVertexBuffers(0, 1, &_indexVB, &stride, &offset);
	}
}
