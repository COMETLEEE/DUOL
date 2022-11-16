#include "pch.h"
#include "ParticleMesh.h"


ParticleMesh::ParticleMesh() :_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr), _device(nullptr),
_maxParticles(0)
{
	_device = DXEngine::GetInstance()->GetD3dDevice();

	_vbd.Usage = D3D11_USAGE_DEFAULT;
	_vbd.ByteWidth = sizeof(Vertex::Particle) * 1;
	_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_vbd.CPUAccessFlags = 0;
	_vbd.MiscFlags = 0;
	_vbd.StructureByteStride = 0;

	Vertex::Particle p;
	ZeroMemory(&p, sizeof(Vertex::Particle));
	p.Age = 0.0f;
	p.Type = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	HR(_device->CreateBuffer(&_vbd, &vinitData, &_initVB));

	SetMaxParticleSize(1000);

}

ParticleMesh::~ParticleMesh()
{
	ReleaseCOM(_initVB);
	ReleaseCOM(_drawVB);
	ReleaseCOM(_streamOutVB);

	// 텍스쳐는 참조.. 다른 곳에서 할당 해제 할 것이다.
	//ReleaseCOM(_texArraySRV);
	//ReleaseCOM(_randomTexSRV);
}

void ParticleMesh::SetMaxParticleSize(unsigned int size)
{
	if (size == _maxParticles)
		return;

	_maxParticles = size;

	ReleaseCOM(_drawVB);
	ReleaseCOM(_streamOutVB);

	_vbd.ByteWidth = sizeof(Vertex::Particle) * _maxParticles;
	_vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	HR(_device->CreateBuffer(&_vbd, 0, &_drawVB));
	HR(_device->CreateBuffer(&_vbd, 0, &_streamOutVB));
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
