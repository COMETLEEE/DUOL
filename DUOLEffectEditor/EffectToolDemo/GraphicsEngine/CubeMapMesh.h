#pragma once
class CubeMapMesh : public IMeshComponent
{
public:
	CubeMapMesh(Mesh* _MeshObject);
	~CubeMapMesh();
private:
	string m_Path;
	ID3D11ShaderResourceView* m_CubeMapSRV;
public:
	virtual void Darw() override;
	void Init(string _Path);
	virtual void WakeMesh() override;
	virtual void SleepMesh() override;
};

