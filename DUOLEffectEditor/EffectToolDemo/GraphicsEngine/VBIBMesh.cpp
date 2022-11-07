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



void VBIBMesh::Init(vector<Vertex::Basic>& _vertex, vector<index3>& _indices)
{
	assert(_vertex.size() >= 0);

	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();


	D3D11_BUFFER_DESC vbd; //버텍스 버퍼 구조체
	vbd.Usage = D3D11_USAGE_IMMUTABLE; // 사용할때마다 책 찾아보기
	vbd.ByteWidth = sizeof(Vertex::Basic) * _vertex.size(); // 사이즈
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 버텍스 버퍼 플래그 설정
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	HRESULT hr;
	D3D11_SUBRESOURCE_DATA vinitData; //버텍스
	vinitData.pSysMem = _vertex.data(); //버텍스 정보를 가지고 있는 배열의 포인터
	HR(_d3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));


	D3D11_BUFFER_DESC ibd; //인덱스(색인) 버퍼 구조체
	ibd.Usage = D3D11_USAGE_IMMUTABLE; // 사용할때마다 책 찾아보기
	ibd.ByteWidth = sizeof(int) * _indices.size() * 3;  // 사이즈
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼 플래그 설정
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData; //인덱스
	iinitData.pSysMem = _indices.data(); //인덱스 정보를 가지고 있는 배열의 포인터
	HR(_d3dDevice->CreateBuffer(&ibd, &iinitData, &_IB));

	_indexSize = _indices.size() * 3;
	_vertex.clear();
	_indices.clear();
}

ID3D11Buffer** VBIBMesh::GetVB()
{
	return &_VB;
}

ID3D11Buffer** VBIBMesh::GetIB()
{
	return &_IB;
}

int VBIBMesh::GetIndexSize()
{
	return _indexSize;
}
