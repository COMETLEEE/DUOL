#include "pch.h"
#include "NormalMesh.h"
#include "DDSTextureLoader.h"

NormalMesh::NormalMesh(Mesh* _MeshObject) : IMeshComponent(_MeshObject), m_NormalMap(nullptr)
{
}

NormalMesh::~NormalMesh()
{
	ReleaseCOM(m_NormalMap);
}

void NormalMesh::Darw()
{
	Effects::LightFX->SetNormal(m_NormalMap);
}

void NormalMesh::Init(string _Path)
{
	m_Path = _Path;
}


void NormalMesh::WakeMesh()
{
	ID3D11Resource* texResource = nullptr;
	wstring _str;
	_str.assign(m_Path.begin(), m_Path.end());
	CreateDDSTextureFromFile(DXEngine::GetInstance()->GetD3dDevice(), _str.c_str(), &texResource, &m_NormalMap);
	ReleaseCOM(texResource)
}

void NormalMesh::SleepMesh()
{
	ReleaseCOM(m_NormalMap)
}
