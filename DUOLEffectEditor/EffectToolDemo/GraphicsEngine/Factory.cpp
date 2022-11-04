#include "pch.h"
#include "Factory.h"
// 2022. 06. 16 그래픽 엔진 구조 변경 중 
// 1. 일단 Mesh의 생성자에서 하던일을 전부 팩토리로 옮길 것. 오버로딩으로 하는게 좋을듯?
// 2. 매쉬에서 분리할 수 있는것들 다른 클래스로 분리 할 것.
// 3. 파서의 파싱 결과에 따라 적절한 컴포넌트를 부착할 수 있도록 구조를 변경할 것.
// 4. MeshComponents의 내용을 채워 넣을 것.
// 5. 리소스 매니저에 팩토리를 사용해서 리소스를 저장할 것.

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
	// 구현하다가 말았다..!!
// ASE가 필요해지면 다시 하자..
// 지금은 박스부터 나오게 하는게 우선일듯 하다.
	return m_Bulider->LoadASE(path);
}

