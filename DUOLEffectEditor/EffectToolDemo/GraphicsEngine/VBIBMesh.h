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

private:
	ID3D11Buffer* _VB; // 버텍스 버퍼

	ID3D11Buffer* _IB; // 인덱스(색인) 버퍼

	int _indexSize;

	void BuildGeometryBuffers(vector<Vertex>& _vertex, vector<index3>& _indices); //버텍스 버퍼와 인덱스 버퍼 초기화

public:
	void Init(vector<Vertex>& _vertex, vector<index3>& _indices);

	ID3D11Buffer** GetVB();

	ID3D11Buffer** GetIB();

	int GetIndexSize();
};

