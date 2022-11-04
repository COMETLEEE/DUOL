#include "pch.h"
#include "CubeMapMesh.h"
#include "DDSTextureLoader.h"
CubeMapMesh::CubeMapMesh(Mesh* _MeshObject) :IMeshComponent(_MeshObject), m_CubeMapSRV(nullptr)
{
}

CubeMapMesh::~CubeMapMesh()
{
	ReleaseCOM(m_CubeMapSRV)
}

void CubeMapMesh::Darw()
{
	Effects::SkyFX->SetCubeMap(m_CubeMapSRV);
	Effects::LightFX->SetCubeMap(m_CubeMapSRV);
}

void CubeMapMesh::Init(string _Path)
{
	m_Path = _Path;
}

void CubeMapMesh::WakeMesh()
{
	HRESULT hr;
	ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();
	wstring cubemapFilename;
	cubemapFilename.assign(m_Path.begin(), m_Path.end());

	ID3D11Resource* texResource = nullptr;
	HR(CreateDDSTextureFromFile(device, cubemapFilename.c_str(), &texResource, &m_CubeMapSRV));
	ReleaseCOM(texResource); // view saves reference
}

void CubeMapMesh::SleepMesh()
{
	ReleaseCOM(m_CubeMapSRV)
}
