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
	// �����ϴٰ� ���Ҵ�..!!
// ASE�� �ʿ������� �ٽ� ����..
// ������ �ڽ����� ������ �ϴ°� �켱�ϵ� �ϴ�.
	return pair<ID3D11Buffer*, ID3D11Buffer*>();
}

VBIBMesh* Factory::CreateGridMesh()
{
	//���ؽ� ���ۿ� �ε��� ���� ����..

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

	const int _Size = 120;
	// ���ؽ� ���� �ε��� ���� ����
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

