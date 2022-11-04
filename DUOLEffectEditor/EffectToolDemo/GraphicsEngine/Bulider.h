#pragma once


struct BulidInfo
{
	string m_Name;
	string m_Path;
	ID3DX11EffectTechnique* m_Tech;
	Shader* m_Shader;
};

class Bulider
{
public:
	Bulider();
	~Bulider() = default;
public:
	Mesh* CreateMesh(BulidInfo& _BulidInfo);
	IMesh* CreateGizmoMesh(GizmoInfo& _Info);

	void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, vector<Vertex>& _vertices, vector<index3>& _Indices);

	pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(string path);


private:
	void CreateMeshChild(Mesh* _TopParent, MeshData* _MeshData, BulidInfo& _BulidInfo);
	void CreateTexture(Mesh* _Mesh, string Path);

	

	Mesh* SphereGizmoCreate(float _Radius);
	Mesh* BoxGizmoCreate(XMFLOAT3 _Pos[8]);
	Mesh* LineGizmoCreate(XMFLOAT3 _Pos[2]);

public:
	// 특별제작? 얘네도 빌더가 만들까?
	VBIBMesh* CreateGridMesh();
	// Axis는 그냥 기즈모 렌더러로 선 3개 생성하자~
	VBIBMesh* CreateCubeMapMesh();
	VBIBMesh* CreateSolidBoxMesh();
	VBIBMesh* CreateSolidSphereMesh();
};

