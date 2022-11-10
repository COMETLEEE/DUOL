#pragma once
class ID3DX11EffectTechnique;
class Effect;

/// <summary>
/// 버텍스와 인덱스를 저장하고 있는 버퍼 클래스.
/// </summary>
class VBIBMesh
{
public:
	VBIBMesh();

	~VBIBMesh();
public:
	// 기본 메쉬의 초기화
	void Init(vector<Vertex::Basic>& _vertex, vector<index3>& _indices, D3D11_USAGE vertexUsage = D3D11_USAGE_IMMUTABLE, D3D11_USAGE indexUsage = D3D11_USAGE_IMMUTABLE, D3D11_CPU_ACCESS_FLAG cpuAccessFlag = (D3D11_CPU_ACCESS_FLAG)0);

	ID3D11Buffer** GetVB();

	ID3D11Buffer** GetIB();

	int GetIndexSize();

private:
	ID3D11Buffer* _VB; // 버텍스 버퍼

	ID3D11Buffer* _IB; // 인덱스(색인) 버퍼

	int _indexSize;
};

