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

		SetMaxParticleSize(1000);

		D3D11_BUFFER_DESC buffDesc;
		ZeroMemory(&buffDesc, sizeof(buffDesc));
		buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		buffDesc.ByteWidth = sizeof(Structure::CounterBuffer);
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.CPUAccessFlags = 0;
		buffDesc.StructureByteStride = sizeof(Structure::CounterBuffer);
		buffDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA	data;
		ZeroMemory(&data, sizeof(data));

		Structure::CounterBuffer initData;

		data.pSysMem = &initData;

		if (FAILED(_device->CreateBuffer(&buffDesc, &data, &_counterBuffer))) {
			assert(false);
		}

		ZeroMemory(&buffDesc, sizeof(buffDesc));
		_counterBuffer->GetDesc(&buffDesc);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		uavDesc.Buffer.NumElements = 1;

		if (FAILED(_device->CreateUnorderedAccessView(_counterBuffer, &uavDesc, &_counterUAV))) {
			assert(false);
		}

		/// -----------------------------_copyCounterBuffer-----------------------------------
		buffDesc.BindFlags = 0;
		buffDesc.ByteWidth = sizeof(Structure::CounterBuffer);
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		buffDesc.StructureByteStride = sizeof(Structure::CounterBuffer);
		buffDesc.Usage = D3D11_USAGE_STAGING;

		data.pSysMem = &initData;

		if (FAILED(_device->CreateBuffer(&buffDesc, &data, &_copyCounterBuffer))) {
			assert(false);
		}


	}

	ParticleMesh::~ParticleMesh()
	{
		ReleaseCOM(_indexVB);

		ReleaseCOM(_particleBufferUAV);
		ReleaseCOM(_particleBufferSRV);
		ReleaseCOM(_particleBuffer);

		ReleaseCOM(_counterBuffer);
		ReleaseCOM(_counterUAV);

		ReleaseCOM(_copyCounterBuffer);


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
		buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffDesc.ByteWidth = sizeof(Vertex::Particle) * _maxParticles;
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.StructureByteStride = sizeof(Vertex::Particle);
		buffDesc.Usage = D3D11_USAGE_DEFAULT;
		buffDesc.CPUAccessFlags = 0;

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
		uavDesc.Buffer.Flags = 0;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		uavDesc.Buffer.NumElements = _maxParticles;

		if (FAILED(_device->CreateUnorderedAccessView(_particleBuffer, &uavDesc, &_particleBufferUAV))) {
			assert(false);
		}
	}
	int ParticleMesh::GetDim()
	{
		if (_maxParticles % 1024 == 0)
			_dim = _maxParticles / 1024;
		else
			_dim = _maxParticles / 1024 + 1;

		_dim = static_cast<int>(ceil(pow((double)_dim, double(1.0 / 3.0))));

		return _dim;
	}
	void ParticleMesh::ResetParticleBuffer()
	{
		UINT clearNum = 0;

		_d3dImmediateContext->ClearUnorderedAccessViewUint(_particleBufferUAV, &clearNum);
	}
	void ParticleMesh::ParticleUpdate()
	{
		ID3D11ShaderResourceView* pNullSRV = NULL;

		_d3dImmediateContext->GSSetShaderResources(2, 1, &pNullSRV);

		_d3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &_particleBufferUAV, NULL);
		_d3dImmediateContext->CSSetUnorderedAccessViews(1, 1, &_counterUAV, NULL);

		_d3dImmediateContext->Dispatch(_dim, _dim, _dim); // ½ÇÇà.

		ID3D11UnorderedAccessView* pNullUAV = NULL;

		_d3dImmediateContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, NULL);
		_d3dImmediateContext->CSSetUnorderedAccessViews(1, 1, &pNullUAV, NULL);

		_d3dImmediateContext->CopyResource(_copyCounterBuffer, _counterBuffer);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		_d3dImmediateContext->Map(_copyCounterBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

		auto temp = (Structure::CounterBuffer*)mappedResource.pData;

		_particleCount = temp->particleCounter;

		_d3dImmediateContext->Unmap(_copyCounterBuffer, 0);
	}
	void ParticleMesh::VSSetResource()
	{
		UINT stride = sizeof(unsigned int);

		UINT offset = 0;

		_d3dImmediateContext->GSSetShaderResources(2, 1, &_particleBufferSRV);

		_d3dImmediateContext->IASetVertexBuffers(0, 1, &_indexVB, &stride, &offset);
	}

	void ParticleMesh::ResetCounter()
	{
		UINT clearNum = 0;

		_d3dImmediateContext->ClearUnorderedAccessViewUint(_counterUAV, &clearNum);
	}

	unsigned ParticleMesh::GetParticleCount()
	{
		return _particleCount;
	}
}
