#pragma once
class InputLayout
{
public:
	static void Initialize();
	static void Finalize();

	static ID3D11InputLayout* Light;
	static ID3D11InputLayout* Wire;
	static ID3D11InputLayout* Sky;
	static ID3D11InputLayout* BasicParticle;

private:
	static void CreateWire();
	static void CreateSky();
	static void CreateLight();
	static void CreateBasicParticle();
};

