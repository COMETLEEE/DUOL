#pragma once
class ID3DX11EffectTechnique;
class Effect;

/// <summary>
/// ���ؽ��� �ε����� �����ϰ� �ִ� ���� Ŭ����.
/// </summary>
class VBIBMesh
{
public:
	VBIBMesh();

	~VBIBMesh();
public:
	// �⺻ �޽��� �ʱ�ȭ
	void Init(vector<Vertex::Basic>& _vertex, vector<index3>& _indices);

	ID3D11Buffer** GetVB();

	ID3D11Buffer** GetIB();

	int GetIndexSize();

private:
	ID3D11Buffer* _VB; // ���ؽ� ����

	ID3D11Buffer* _IB; // �ε���(����) ����

	int _indexSize;
};

