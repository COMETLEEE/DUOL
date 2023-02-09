#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/DirectX11/DXEngine.h"
#include "util/VertexDesc.h"

namespace MuscleGrapics
{
	ParticleMesh::ParticleMesh() :
		_initVB(nullptr),
		_drawVB(nullptr),
		_streamOutVB(nullptr),
		_device(nullptr),
		_maxParticles(0)
	{
		_device = DXEngine::GetInstance()->GetD3dDevice();

		SetEmitterCount(1, false);

		SetMaxParticleSize(1000);
	}

	ParticleMesh::~ParticleMesh()
	{
		ReleaseCOM(_initVB);

		ReleaseCOM(_drawVB);

		ReleaseCOM(_streamOutVB);
	}

	void ParticleMesh::SetMaxParticleSize(unsigned int size, bool isChangeEmitterCount)
	{
		if (size == _maxParticles && !isChangeEmitterCount)
			return;

		_maxParticles = size;

		ReleaseCOM(_drawVB);

		ReleaseCOM(_streamOutVB);

		_vbd.ByteWidth = sizeof(Vertex::Particle) * (_maxParticles + _emitterCount); // 1개는 방출기다.

		_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

		HR(_device->CreateBuffer(&_vbd, 0, &_drawVB));

		HR(_device->CreateBuffer(&_vbd, 0, &_streamOutVB));
	}
	void ParticleMesh::SetEmitterCount(unsigned int size, bool isChangeEmitterCount)
	{
		if (size == _emitterCount)
			return;

		_emitterCount = size;

		SetMaxParticleSize(_maxParticles, isChangeEmitterCount);

		ReleaseCOM(_initVB);

		std::vector<Vertex::Particle> initVertex(_emitterCount);

		_vbd.Usage = D3D11_USAGE_DEFAULT;

		_vbd.ByteWidth = sizeof(Vertex::Particle) * _emitterCount;

		_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		_vbd.CPUAccessFlags = 0;

		_vbd.MiscFlags = 0;

		_vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;

		vinitData.pSysMem = initVertex.data();

		HR(_device->CreateBuffer(&_vbd, &vinitData, &_initVB));
	}
	ID3D11Buffer** ParticleMesh::GetInitVB()
	{
		return &_initVB;
	}

	ID3D11Buffer** ParticleMesh::GetDrawVB()
	{
		return &_drawVB;
	}

	ID3D11Buffer** ParticleMesh::GetStreamOutVB()
	{
		return &_streamOutVB;
	}
}
