#pragma once
/// <summary>
/// �޽��� ��� ������ Ŭ����.
/// �����ڸ� ������� �ʰ� ���丮�� ������ ��.
/// �ļ����� ���� �����͸� �� �����ؼ� �˾Ƽ� �������ּ���~
/// </summary>
class Factory
{

public:
	Factory();

	~Factory();

public:
	ID3D11ShaderResourceView* LoadTexture(string path);
	
	pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(string path);

	VBIBMesh* CreateGridMesh();

	VBIBMesh* CreateSphereMesh();

	VBIBMesh* CreateBoxMesh();
private:

	void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, vector<Vertex::Basic>& _vertices, vector<index3>& _Indices);

};

