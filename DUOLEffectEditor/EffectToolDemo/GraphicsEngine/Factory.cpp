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

}

Factory::~Factory()
{

}


ID3D11ShaderResourceView* Factory::LoadTexture(string path)
{


	return nullptr;
}

pair<ID3D11Buffer*, ID3D11Buffer*> Factory::LoadASE(string path)
{
	// 구현하다가 말았다..!!
// ASE가 필요해지면 다시 하자..
// 지금은 박스부터 나오게 하는게 우선일듯 하다.
	return pair<ID3D11Buffer*, ID3D11Buffer*>();
}

VBIBMesh* Factory::CreateGridMesh()
{
	//버텍스 버퍼와 인덱스 버퍼 셋팅..

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

	const int _Size = 120;
	// 버텍스 버퍼 인덱스 버퍼 세팅
	vector<Vertex> vertices;
	vertices.resize(_Size);
	for (int i = 0; i < _Size / 4; i++)
	{
		vertices[i].Pos = XMFLOAT3(5.0f * i, 0, 0);
	}
	for (int i = 0; i < _Size / 4; i++)
	{
		vertices[i + _Size / 4].Pos = XMFLOAT3(5.0f * i, 0, 5.0f * (_Size / 4 - 1));
	}

	for (int i = 0; i < _Size / 4; i++)
	{
		vertices[i + _Size / 2].Pos = XMFLOAT3(0, 0, 5.0f * i);
	}
	for (int i = 0; i < _Size / 4; i++)
	{
		vertices[i + _Size / 4 * 3].Pos = XMFLOAT3(5.0f * (_Size / 4 - 1), 0, 5.0f * i);
	}

	for (int i = 0; i < _Size; i++)
	{
		vertices[i].Pos.x -= 5.0f * (_Size / 8 - 1);
		vertices[i].Pos.z -= 5.0f * (_Size / 8 - 1);
	}


	UINT indices[_Size];

	for (int i = 0; i < _Size / 2; i++, i++)
	{
		indices[i] = i / 2;
		indices[i + 1] = i / 2 + _Size / 4;
	}
	for (int i = 0; i < _Size / 2; i++, i++)
	{
		indices[i + _Size / 2] = i / 2 + _Size / 2;
		indices[i + _Size / 2 + 1] = i / 2 + _Size / 4 * 3;
	}

	vector<index3> _Temp;
	_Temp.resize(_Size / 3);
	for (unsigned int i = 0; i < _Temp.size(); i++)
	{
		_Temp[i].m_Index[0] = indices[i * 3];
		_Temp[i].m_Index[1] = indices[i * 3 + 1];
		_Temp[i].m_Index[2] = indices[i * 3 + 2];
	}
	auto mesh = new VBIBMesh();
	mesh->Init(vertices, _Temp);

	return mesh;
}

VBIBMesh* Factory::CreateSphereMesh()
{

	GeometryGenerator::MeshData sphere;
	GeometryGenerator geoGen;
	geoGen.CreateSphere(0.5f, 30, 30, sphere);

	std::vector<Vertex> vertices(sphere.Vertices.size());
	std::vector<index3> indices(sphere.Indices.size() / 3);

	ChangeGeometry(&sphere, vertices, indices);

	VBIBMesh* _Temp = new VBIBMesh();
	_Temp->Init(vertices, indices);


	return _Temp;

}

VBIBMesh* Factory::CreateBoxMesh()
{
	GeometryGenerator::MeshData Box;
	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, Box);

	std::vector<Vertex> vertices(Box.Vertices.size());
	std::vector<index3> indices(Box.Indices.size() / 3);

	ChangeGeometry(&Box, vertices, indices);

	for (auto& iter : vertices)
	{
		iter.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	VBIBMesh* _Temp = new VBIBMesh();

	_Temp->Init(vertices, indices);

	return _Temp;
}

void Factory::ChangeGeometry(GeometryGenerator::MeshData* _MeshData, vector<Vertex>& _vertices,
	vector<index3>& _Indices)
{
	for (size_t i = 0; i < _MeshData->Vertices.size(); ++i)
	{
		_vertices[i].Pos = _MeshData->Vertices[i].Position;
		_vertices[i].Nomal = _MeshData->Vertices[i].Normal;
	}

	for (unsigned int i = 0; i < _Indices.size(); i++)
	{
		_Indices[i].m_Index[0] = _MeshData->Indices[i * 3];
		_Indices[i].m_Index[1] = _MeshData->Indices[i * 3 + 1];
		_Indices[i].m_Index[2] = _MeshData->Indices[i * 3 + 2];
	}
}

