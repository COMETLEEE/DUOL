#pragma once
class TextureMesh : public IMeshComponent
{
public:
	TextureMesh(Mesh* _MeshObject);
	~TextureMesh();
public:
	virtual void Darw() override;
	void Init(string _Path);
	virtual void WakeMesh() override;
	virtual void SleepMesh() override;
private:
	string m_Path;
	ID3D11ShaderResourceView* m_DiffuseMapSRV;
};

