#pragma once
/// <summary>
/// 메쉬를 대신 생성할 클래스.
/// 생성자를 사용하지 않고 팩토리가 생성할 것.
/// 파서에서 읽은 데이터를 잘 정리해서 알아서 생성해주세용~
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

