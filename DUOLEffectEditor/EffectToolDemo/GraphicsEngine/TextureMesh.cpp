#include "pch.h"
#include "TextureMesh.h"
#include "DDSTextureLoader.h"

TextureMesh::TextureMesh(Mesh* _MeshObject) : IMeshComponent(_MeshObject), m_DiffuseMapSRV(nullptr)
{
}

TextureMesh::~TextureMesh()
{
	ReleaseCOM(m_DiffuseMapSRV);
}

void TextureMesh::Darw()
{
	Effects::LightFX->SetTexure(m_DiffuseMapSRV);
}

void TextureMesh::Init(string _Path)
{
	m_Path = _Path;
}

void TextureMesh::WakeMesh()
{
	ID3D11Resource* texResource = nullptr;
	wstring _str;
	_str.assign(m_Path.begin(), m_Path.end());
	CreateDDSTextureFromFile(DXEngine::GetInstance()->GetD3dDevice(), _str.c_str(), &texResource, &m_DiffuseMapSRV);
	ReleaseCOM(texResource)
}

void TextureMesh::SleepMesh()
{
	ReleaseCOM(m_DiffuseMapSRV)
}
