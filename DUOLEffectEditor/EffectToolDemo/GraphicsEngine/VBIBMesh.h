#pragma once

/// <summary>
/// ���ؽ��� �ε����� �����ϰ� �ִ� ���� Ŭ����.
/// </summary>
namespace MuscleGrapics
{
	class VBIBMesh
	{
	public:
		VBIBMesh();

		~VBIBMesh();
	public:
		template<class T>
		void Init(vector<T>& _vertex, vector<index3>& _indices, D3D11_USAGE vertexUsage = D3D11_USAGE_IMMUTABLE, D3D11_USAGE indexUsage = D3D11_USAGE_IMMUTABLE, D3D11_CPU_ACCESS_FLAG cpuAccessFlag = (D3D11_CPU_ACCESS_FLAG)0);

		ID3D11Buffer** GetVB();

		ID3D11Buffer** GetIB();

		int GetIndexSize();

	private:
		ID3D11Buffer* _VB; // ���ؽ� ����

		ID3D11Buffer* _IB; // �ε���(����) ����

		int _indexSize;
	};

	template <class T>
	void VBIBMesh::Init(vector<T>& _vertex, vector<index3>& _indices, D3D11_USAGE vertexUsage, D3D11_USAGE indexUsage,
		D3D11_CPU_ACCESS_FLAG cpuAccessFlag)
	{
		assert(_vertex.size() >= 0);

		ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

		D3D11_BUFFER_DESC vbd; //���ؽ� ���� ����ü

		vbd.Usage = vertexUsage; // ����Ҷ����� å ã�ƺ���
		vbd.ByteWidth = sizeof(T) * _vertex.size(); // ������
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // ���ؽ� ���� �÷��� ����
		vbd.CPUAccessFlags = cpuAccessFlag;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData; //���ؽ�

		vinitData.pSysMem = _vertex.data(); //���ؽ� ������ ������ �ִ� �迭�� ������

		HR(_d3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));

		D3D11_BUFFER_DESC ibd; //�ε���(����) ���� ����ü

		ibd.Usage = indexUsage; // ����Ҷ����� å ã�ƺ���
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
}

