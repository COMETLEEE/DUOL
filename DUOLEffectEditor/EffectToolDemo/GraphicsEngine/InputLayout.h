#pragma once
class InputLayout
{
public:
	static void Initialize();
	static void Finalize();

	static ID3D11InputLayout* m_Light;
	static ID3D11InputLayout* m_Wire;
	static ID3D11InputLayout* m_Sky;

private:
	static void CreateWire();
	static void CreateSky();
	static void CreateLight();
};

