#include "pch.h"
#include "Factory.h"
// 2022. 06. 16 �׷��� ���� ���� ���� �� 
// 1. �ϴ� Mesh�� �����ڿ��� �ϴ����� ���� ���丮�� �ű� ��. �����ε����� �ϴ°� ������?
// 2. �Ž����� �и��� �� �ִ°͵� �ٸ� Ŭ������ �и� �� ��.
// 3. �ļ��� �Ľ� ����� ���� ������ ������Ʈ�� ������ �� �ֵ��� ������ ������ ��.
// 4. MeshComponents�� ������ ä�� ���� ��.
// 5. ���ҽ� �Ŵ����� ���丮�� ����ؼ� ���ҽ��� ������ ��.

Factory::Factory()
{
	m_Bulider = new Bulider();
}

Factory::~Factory()
{
	delete m_Bulider;
}

Mesh* Factory::CreateMesh(BulidInfo& _BulidInfo)
{
	Mesh* _Result = m_Bulider->CreateMesh(_BulidInfo);
	DXEngine::GetInstance()->GetResourceManager()->GetMeshs().insert(make_pair(_BulidInfo.m_Name, _Result));
	return _Result;
}

IMesh* Factory::CreateGizmoMesh(GizmoInfo& _Info)
{
	return m_Bulider->CreateGizmoMesh(_Info);
}

void Factory::CreateSpecialMeshs()
{
	m_Bulider->CreateGridMesh();
	m_Bulider->CreateCubeMapMesh();
	m_Bulider->CreateSolidBoxMesh();
	m_Bulider->CreateSolidSphereMesh();
}

ID3D11ShaderResourceView* Factory::LoadTexture(string path)
{



}

pair<ID3D11Buffer*, ID3D11Buffer*> Factory::LoadASE(string path)
{
	// �����ϴٰ� ���Ҵ�..!!
// ASE�� �ʿ������� �ٽ� ����..
// ������ �ڽ����� ������ �ϴ°� �켱�ϵ� �ϴ�.
	return m_Bulider->LoadASE(path);
}

