#include "pch.h"
#include "InputLayout.h"


ID3D11InputLayout* InputLayout::Light = nullptr;
ID3D11InputLayout* InputLayout::Wire = nullptr;
ID3D11InputLayout* InputLayout::Sky = nullptr;
ID3D11InputLayout* InputLayout::BasicParticle = nullptr;

void InputLayout::Initialize()
{
	CreateLight();
	CreateWire();
	CreateSky();
	CreateBasicParticle();
}

void InputLayout::Finalize()
{
	ReleaseCOM(Light)
		ReleaseCOM(Wire)
		ReleaseCOM(Sky)
		ReleaseCOM(BasicParticle)
}

void InputLayout::CreateWire()
{
	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	Effects::WireFX->m_WireTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr;
	HR(_d3dDevice->CreateInputLayout(vertexDesc, 1, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Wire));

}

void InputLayout::CreateSky()
{
	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	Effects::SkyFX->m_SkyTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr;
	HR(_d3dDevice->CreateInputLayout(vertexDesc, 1, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Sky));
}

void InputLayout::CreateLight()
{
	//잘모르겠다 다시 책을 보자. 입력배치란 무엇인가? 패스란 무엇인가 todo:

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	HRESULT hr;
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 72,
			D3D11_INPUT_PER_VERTEX_DATA, 0}

	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	Effects::LightFX->m_LightTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HR(_d3dDevice->CreateInputLayout(vertexDesc, 7, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Light));

}

void InputLayout::CreateBasicParticle()
{

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	HRESULT hr;
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	Effects::ParticleFX->_streamOutTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HR(_d3dDevice->CreateInputLayout(vertexDesc, 5, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &BasicParticle));


}
