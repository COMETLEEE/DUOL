#pragma once
#include "include/util/STLInclude.h"
#include "Core/DirectX11/DXEngine.h"
#include "util/VertexDesc.h"
#include "Core/Resource/Resource/ResourceBase.h"

/// <summary>
/// 버텍스와 인덱스를 저장하고 있는 버퍼 클래스.
/// </summary>
namespace MuscleGrapics
{
	class VBIBMesh :public ResourceBase
	{
	public:
		VBIBMesh();

		~VBIBMesh();
	public:
		template<class T>
		void Init(std::vector<T>& _vertex, std::vector<index3>& _indices, D3D11_USAGE vertexUsage = D3D11_USAGE_IMMUTABLE, D3D11_USAGE indexUsage = D3D11_USAGE_IMMUTABLE, D3D11_CPU_ACCESS_FLAG cpuAccessFlag = (D3D11_CPU_ACCESS_FLAG)0);

		ID3D11Buffer** GetVB();

		ID3D11Buffer** GetIB();

		int GetIndexSize();

	private:
		ID3D11Buffer* _VB; // 버텍스 버퍼

		ID3D11Buffer* _IB; // 인덱스(색인) 버퍼

		int _indexSize;
	};

	template <class T>
	void VBIBMesh::Init(std::vector<T>& _vertex, std::vector<index3>& _indices, D3D11_USAGE vertexUsage, D3D11_USAGE indexUsage,
		D3D11_CPU_ACCESS_FLAG cpuAccessFlag)
	{
		assert(_vertex.size() >= 0);

		ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

		D3D11_BUFFER_DESC vbd; //버텍스 버퍼 구조체

		vbd.Usage = vertexUsage; // 사용할때마다 책 찾아보기
		vbd.ByteWidth = sizeof(T) * _vertex.size(); // 사이즈
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // 버텍스 버퍼 플래그 설정
		vbd.CPUAccessFlags = cpuAccessFlag;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData; //버텍스

		vinitData.pSysMem = _vertex.data(); //버텍스 정보를 가지고 있는 배열의 포인터

		HR(_d3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));

		D3D11_BUFFER_DESC ibd; //인덱스(색인) 버퍼 구조체

		ibd.Usage = indexUsage; // 사용할때마다 책 찾아보기
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
}

