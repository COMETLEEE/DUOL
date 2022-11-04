#include "pch.h"
#include "Mesh.h"
#include "CASEParser.h"

Mesh::Mesh() :IMesh(), m_Mtl(nullptr), m_pNowRenderstate(nullptr)
{
}

Mesh::~Mesh()
{
	if (!m_Parent)
		delete m_Mtl;

	for (auto& iter : m_MeshComponents)
	{
		delete iter;
	}
	m_MeshComponents.clear();

}

void Mesh::SetConstants()
{
	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	// 렌더스테이트
	_d3dImmediateContext->RSSetState(m_pNowRenderstate); //어떻게 만들지..
	if (m_Mtl)
		Effects::LightFX->SetMaterial(m_Mtl);
	for (auto& iter : m_MeshComponents)
	{
		iter->Darw();
	}

	
	//m_LightFX->SetCubeMap(SkyBox::m_CubeMapSRV);
}

void Mesh::DrawMesh(XMMATRIX _WorldTM)
{
	m_WorldTM = _WorldTM;
	SetConstants();
}

void Mesh::SetColor(XMFLOAT4 _Color)
{
	GetMeshComponent<VBIBMesh>()->SetColor(_Color);
}

void Mesh::WakeMesh()
{
	Count++;
	if (Count == 1)
	{
		for (auto iter : m_MeshComponents)
		{
			iter->WakeMesh();
		}
	}
}

void Mesh::SleepMesh()
{
	Count--;
	if (Count == 0)
	{
		for (auto iter : m_MeshComponents)
		{
			iter->SleepMesh();
		}
	}
}

void Mesh::SetMaterial(Material* _Mtl)
{
	if(m_Mtl)
		delete m_Mtl;
	m_Mtl = _Mtl;
}

void Mesh::SetRasterizerState(ID3D11RasterizerState* _State)
{
	m_pNowRenderstate = _State;
}
