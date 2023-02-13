#pragma once
#include <d3d11.h>
#include "Core/Resource/Resource/ResourceBase.h"

class ID3D11Buffer;

class ID3D11Device;

namespace MuscleGrapics
{
	class ParticleMesh : public ResourceBase
	{
	public:
		ParticleMesh();

		~ParticleMesh();

	public:
		void SetMaxParticleSize(unsigned int size);

		int GetDim();

		void ResetParticleBuffer();

		void ParticleUpdate();

		void VSSetResource();

		void ResetCounter();

		unsigned int GetParticleCount();
	private:
		ID3D11Buffer* _indexVB;

		ID3D11UnorderedAccessView* _particleBufferUAV;
		ID3D11ShaderResourceView* _particleBufferSRV;
		ID3D11Buffer* _particleBuffer;

		ID3D11UnorderedAccessView* _counterUAV;
		ID3D11Buffer* _counterBuffer;

		ID3D11Buffer* _copyCounterBuffer; // ī���͸� ������ ���� Ȯ���ϱ����� ����

		ID3D11Device* _device; // ĳ��

		ID3D11DeviceContext* _d3dImmediateContext; // ĳ��

		D3D11_BUFFER_DESC _vbd;

		unsigned int _maxParticles;

		unsigned int _emitterCount;

		unsigned int _particleCount;

		int _dim;
	};
}
