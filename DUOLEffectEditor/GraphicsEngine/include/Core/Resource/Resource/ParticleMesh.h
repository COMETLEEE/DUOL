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

		void UpdateCounter(float timer);

		void ResetParticleBuffer();

		void ExecuteDraw();

		void VSSetResource();

	private:
		void InitCounterBuffer();

	private:
		ID3D11Buffer* _indexVB;

		ID3D11UnorderedAccessView* _particleBufferUAV;
		ID3D11ShaderResourceView* _particleBufferSRV;
		ID3D11Buffer* _particleBuffer;

		ID3D11UnorderedAccessView* _particleEmitterCountBufferUAV;
		ID3D11Buffer* _particleEmitterCountBuffer;

		ID3D11Device* _device; // Ä³½Ì

		ID3D11DeviceContext* _d3dImmediateContext; // Ä³½Ì

		D3D11_BUFFER_DESC _vbd;

		unsigned int _maxParticles;

		unsigned int _emitterCount;

		int _dim;
	};
}
