#include "pch.h"
#include "Bulider.h"
#include "CASEParser.h"
#include "DDSTextureLoader.h"

//#include "..\Loader\Loader_Interface.h"


Bulider::Bulider()
{
}

Mesh* Bulider::CreateMesh(BulidInfo& _BulidInfo)
{
	Mesh* _Result = new Mesh();


	char* writable = new char[_BulidInfo.m_Path.size() + 1];
	std::copy(_BulidInfo.m_Path.begin(), _BulidInfo.m_Path.end(), writable);
	writable[_BulidInfo.m_Path.size()] = '\0';

	//MeshData* _MeshData = DXEngine::GetInstance()->GetResourceManager()->m_ASEParser->Load(writable);
	MeshData* _MeshData = DXEngine::GetInstance()->GetResourceManager()->m_ASEParser->Load(writable);
	
	wstring _test;
	_test.assign(_BulidInfo.m_Path.begin() , _BulidInfo.m_Path.end());
	//ASEParsingData _data = ->LoadASE(_test);
	// �ļ��� �޽� �����͸� �о��.
	delete[] writable;


	// �о�� ������ ���� �ٸ��� ������Ʈ ����.
	_Result->SetRasterizerState(RasterizerState::m_SolidRS);
	CreateTexture(_Result, _MeshData->m_Path);

	_Result->m_BoneNameList = _MeshData->m_BoneNameList;
	_Result->m_MinPos = _MeshData->m_MinPos;
	_Result->m_MaxPos = _MeshData->m_MaxPos;

	_Result->SetMaterial(_MeshData->m_Mtl);
	_Result->AddMeshComponent<VBIBMesh>()->Init(_MeshData->m_vertices, _MeshData->m_indices);
	CreateMeshChild(_Result, _MeshData, _BulidInfo);


	delete _MeshData;


	return _Result;
}

void Bulider::CreateMeshChild(Mesh* _TopParent, MeshData* _MeshData, BulidInfo& _BulidInfo)
{

	for (auto& iter : _MeshData->m_Childrens)
	{
		Mesh* _Child = new Mesh();

		_Child->SetName(iter->m_Name);
		_Child->SetParent(_TopParent->FindChildren(iter->m_Parent->m_Name));
		_Child->SetMaterial(_TopParent->m_Mtl);

		_Child->m_AniKeys = iter->m_AniKeys;
		if (_TopParent->m_BoneNameList.size() > 0)
			_Child->AddMeshComponent<SkinningMesh>();


		_Child->SetRasterizerState(RasterizerState::m_SolidRS);

		_Child->m_NodeWorldTM = iter->m_WorldTM;
		_Child->m_NodeLocalTM = iter->m_LocalMT;




		_Child->AddMeshComponent<VBIBMesh>()->Init(iter->m_vertices, iter->m_indices);

		CreateMeshChild(_TopParent, iter, _BulidInfo);
	}

}

void Bulider::CreateTexture(Mesh* _Mesh, string Path)
{
	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* _DiffuseMapSRV;
	ID3D11ShaderResourceView* _NormalMapSRV;


	string _Path = "Resource/Mesh/";
	_Path += Path;
	_Path += ".dds";
	wstring _str;
	_str.assign(_Path.begin(), _Path.end());

	HRESULT hr;
	hr = CreateDDSTextureFromFile(DXEngine::GetInstance()->GetD3dDevice(), _str.c_str(), &texResource, &_DiffuseMapSRV);
	ReleaseCOM(texResource)	// view saves reference
		ReleaseCOM(_DiffuseMapSRV)	// view saves reference
		if (FAILED(hr)) return;
	_Mesh->AddMeshComponent<TextureMesh>()->Init(_Path);



	_str.clear();
	_Path = "Resource/Mesh/";
	_Path += Path;
	_Path += "_n.dds";
	_str;
	_str.assign(_Path.begin(), _Path.end());
	hr = CreateDDSTextureFromFile(DXEngine::GetInstance()->GetD3dDevice(), _str.c_str(), &texResource, &_NormalMapSRV);
	ReleaseCOM(texResource)	// view saves reference
		ReleaseCOM(_NormalMapSRV)	// view saves reference
		if (FAILED(hr)) return;
	_Mesh->AddMeshComponent<NormalMesh>()->Init(_Path);

}

pair<ID3D11Buffer*, ID3D11Buffer*> Bulider::LoadASE(string path)
{
	// �����ϴٰ� ���Ҵ�..!!
	// ASE�� �ʿ������� �ٽ� ����..
	// ������ �ڽ����� ������ �ϴ°� �켱�ϵ� �ϴ�.

	//Mesh* _Result = new Mesh();


	//char* writable = new char[path.size() + 1];
	//std::copy(path.begin(), path.end(), writable);
	//writable[path.size()] = '\0';

	//MeshData* _MeshData = DXEngine::GetInstance()->GetResourceManager()->m_ASEParser->Load(writable);



	////���ؽ� ���ۿ� �ε��� ���� ����..

	//if (_MeshData->m_vertices.size() <= 0) return;

	//ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();


	//D3D11_BUFFER_DESC vbd; //���ؽ� ���� ����ü
	//vbd.Usage = D3D11_USAGE_IMMUTABLE; // ����Ҷ����� å ã�ƺ���
	//vbd.ByteWidth = sizeof(Vertex) * _MeshData->m_vertices.size(); // ������
	//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���ؽ� ���� �÷��� ����
	//vbd.CPUAccessFlags = 0;
	//vbd.MiscFlags = 0;
	//vbd.StructureByteStride = 0;

	//HRESULT hr;
	//D3D11_SUBRESOURCE_DATA vinitData; //���ؽ�
	//vinitData.pSysMem = _MeshData->m_vertices.data(); //���ؽ� ������ ������ �ִ� �迭�� ������
	//HR(_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));


	//D3D11_BUFFER_DESC ibd; //�ε���(����) ���� ����ü
	//ibd.Usage = D3D11_USAGE_IMMUTABLE; // ����Ҷ����� å ã�ƺ���
	//ibd.ByteWidth = sizeof(int) * _MeshData->m_indices.size() * 3;  // ������
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���� �÷��� ����
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;

	//D3D11_SUBRESOURCE_DATA iinitData; //�ε���
	//iinitData.pSysMem = _MeshData->m_indices.data(); //�ε��� ������ ������ �ִ� �迭�� ������
	//HR(_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

	//_indexSize = _MeshData->m_indices.size() * 3;
	//_MeshData->m_vertices.clear();
	//_MeshData->m_indices.clear();

	//delete _MeshData;


}

IMesh* Bulider::CreateGizmoMesh(GizmoInfo& _Info)
{
	Mesh* _Result = nullptr;
	switch (_Info.m_Type)
	{
	case Shphere:
		_Result = SphereGizmoCreate(_Info.m_Radius);
		break;
	case Box:
		_Result = BoxGizmoCreate(_Info.m_BoxPos);
		break;
	case Line:
		_Result = LineGizmoCreate(_Info.m_LinePos);
		break;
	default:
		break;
	}
	return _Result;
}

Mesh* Bulider::SphereGizmoCreate(float _Radius)
{
	Mesh* _Result = new Mesh();
	//���ؽ� ���ۿ� �ε��� ���� ����..

	GeometryGenerator::MeshData sphere;
	GeometryGenerator geoGen;
	geoGen.CreateSphere(_Radius, 30, 30, sphere);

	std::vector<Vertex> vertices(sphere.Vertices.size());
	std::vector<index3> indices(sphere.Indices.size() / 3);

	ChangeGeometry(&sphere, vertices, indices);

	VBIBMesh* _Temp = _Result->AddMeshComponent<VBIBMesh>();
	_Temp->Init(vertices, indices);

	_Result->SetRasterizerState(RasterizerState::m_WireframeRS);
	return _Result;
}

Mesh* Bulider::BoxGizmoCreate(XMFLOAT3 _Pos[8])
{
	Mesh* _Result = new Mesh();
	//���ؽ� ���ۿ� �ε��� ���� ����..

	std::vector<Vertex> vertices(8);

	for (int i = 0; i < 8; i++)
	{
		vertices[i].Pos = _Pos[i];
	}

	std::vector<index3> indices(12);

	indices[0] = index3(3, 1, 0);
	indices[1] = index3(3, 2, 1);

	indices[2] = index3(2, 7, 6);
	indices[3] = index3(2, 3, 7);

	indices[4] = index3(3, 4, 7);
	indices[5] = index3(3, 0, 4);

	indices[6] = index3(2, 6, 5);
	indices[7] = index3(1, 2, 5);

	indices[8] = index3(6, 7, 5);
	indices[9] = index3(7, 4, 5);

	indices[10] = index3(1, 5, 4);
	indices[11] = index3(0, 1, 4);


	VBIBMesh* _Temp = _Result->AddMeshComponent<VBIBMesh>();
	_Temp->Init(vertices, indices);
	_Result->SetRasterizerState(RasterizerState::m_WireframeRS);
	return _Result;
}

Mesh* Bulider::LineGizmoCreate(XMFLOAT3 _Pos[2])
{
	Mesh* _Result = new Mesh();
	//���ؽ� ���ۿ� �ε��� ���� ����..

	std::vector<Vertex> vertices(2);

	for (int i = 0; i < 2; i++)
	{
		vertices[i].Pos = _Pos[i];
	}

	std::vector<index3> indices(1);

	indices[0] = index3(0, 1, 0);

	VBIBMesh* _Temp = _Result->AddMeshComponent<VBIBMesh>();
	_Temp->Init(vertices, indices);
	_Result->SetRasterizerState(RasterizerState::m_WireframeRS);
	return _Result;
}

void Bulider::ChangeGeometry(GeometryGenerator::MeshData* _MeshData, vector<Vertex>& _vertices, vector<index3>& _Indices)
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

VBIBMesh* Bulider::CreateGridMesh()
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

VBIBMesh* Bulider::CreateCubeMapMesh()
{
	Mesh* _Result = new Mesh();

	GeometryGenerator::MeshData sphere;
	GeometryGenerator geoGen;
	geoGen.CreateSphere(5000.0f, 30, 30, sphere);

	std::vector<Vertex> vertices(sphere.Vertices.size());
	std::vector<index3> indices(sphere.Indices.size() / 3);

	ChangeGeometry(&sphere, vertices, indices);
	_Result->AddMeshComponent<CubeMapMesh>()->Init("Resource/Mesh/sunsetcube1024.dds");

	VBIBMesh* _Temp = _Result->AddMeshComponent<VBIBMesh>();
	_Temp->Init(vertices, indices);

	DXEngine::GetInstance()->GetResourceManager()->GetMeshs().insert(make_pair("SkyBox", _Result));

	return nullptr;
}

VBIBMesh* Bulider::CreateSolidBoxMesh()
{
	Mesh* _Result = new Mesh();
	//���ؽ� ���ۿ� �ε��� ���� ����..

	GeometryGenerator::MeshData Box;
	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, Box);


	_Result->SetName("Box");
	DXEngine::GetInstance()->GetResourceManager()->GetMeshs().insert(make_pair("Box", _Result));

	std::vector<Vertex> vertices(Box.Vertices.size());
	std::vector<index3> indices(Box.Indices.size() / 3);

	ChangeGeometry(&Box, vertices, indices);

	for (auto& iter : vertices)
	{
		iter.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	VBIBMesh* _Temp = _Result->AddMeshComponent<VBIBMesh>();
	_Temp->Init(vertices, indices);

	_Result->SetRasterizerState(RasterizerState::m_SolidRS);


	Material* _Mat = new Material();
	_Mat->Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	_Mat->Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.8f);
	_Result->SetMaterial(_Mat);

	return nullptr;
}

VBIBMesh* Bulider::CreateSolidSphereMesh()
{

	return nullptr;
}
