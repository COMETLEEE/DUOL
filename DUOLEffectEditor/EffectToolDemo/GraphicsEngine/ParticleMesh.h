#pragma once
namespace MuscleGrapics
{
	class ParticleMesh
	{
	public:
		ParticleMesh();

		~ParticleMesh();

	public:
		void SetMaxParticleSize(unsigned int size, bool isChangeEmitterCount = false);

		void SetEmitterCount(unsigned int size, bool isChangeEmitterCount = true);

		ID3D11Buffer** GetInitVB();

		ID3D11Buffer** GetDrawVB();

		ID3D11Buffer** GetStreamOutVB();

	private:
		ID3D11Buffer* _initVB;

		ID3D11Buffer* _drawVB;

		ID3D11Buffer* _streamOutVB;

		ID3D11Device* _device; // Ä³½Ì

		D3D11_BUFFER_DESC _vbd;

		unsigned int _maxParticles;

		unsigned int _emitterCount;
	};
}
