#include "pch.h"
#include "VBIBMesh.h"

VBIBMesh::VBIBMesh() :
	_VB(nullptr), _IB(nullptr), _indexSize()
{
}

VBIBMesh::~VBIBMesh()
{
	ReleaseCOM(_VB);
	ReleaseCOM(_IB);
}

void VBIBMesh::BuildGeometryBuffers(vector<Vertex>& _vertex, vector<index3>& _indices)
{
	//���ؽ� ���ۿ� �ε��� ���� ����..

	if (_vertex.size() <= 0) return;

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();


	D3D11_BUFFER_DESC vbd; //���ؽ� ���� ����ü
	vbd.Usage = D3D11_USAGE_IMMUTABLE; // ����Ҷ����� å ã�ƺ���
	vbd.ByteWidth = sizeof(Vertex) * _vertex.size(); // ������
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���ؽ� ���� �÷��� ����
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	HRESULT hr;
	D3D11_SUBRESOURCE_DATA vinitData; //���ؽ�
	vinitData.pSysMem = _vertex.data(); //���ؽ� ������ ������ �ִ� �迭�� ������
	HR(_d3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));


	D3D11_BUFFER_DESC ibd; //�ε���(����) ���� ����ü
	ibd.Usage = D3D11_USAGE_IMMUTABLE; // ����Ҷ����� å ã�ƺ���
	ibd.ByteWidth = sizeof(int) * _indices.size() * 3;  // ������
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���� �÷��� ����
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData; //�ε���
	iinitData.pSysMem = _indices.data(); //�ε��� ������ ������ �ִ� �迭�� ������
	HR(_d3dDevice->CreateBuffer(&ibd, &iinitData, &_IB));

	_indexSize = _indices.size() * 3;
	_vertex.clear();
	_indices.clear();
}




void VBIBMesh::Init(vector<Vertex>& _vertex, vector<index3>& _indices)
{
	BuildGeometryBuffers(_vertex, _indices);
}
