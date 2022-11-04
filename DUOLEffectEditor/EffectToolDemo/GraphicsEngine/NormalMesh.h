#pragma once
class NormalMesh : public IMeshComponent
{
public:
	NormalMesh(Mesh* _MeshObject);
	~NormalMesh();
	
public:
	virtual void Darw() override;
	void Init(string _Path);
	virtual void WakeMesh() override;
	virtual void SleepMesh() override;
private:
	string m_Path;
	ID3D11ShaderResourceView* m_NormalMap;
};

