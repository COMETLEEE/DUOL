#pragma once
class SkinningMesh : public IMeshComponent
{
public:
	SkinningMesh(Mesh* _MeshObject);
	~SkinningMesh();
	virtual void WakeMesh() override;
	virtual void SleepMesh() override;
public:
	virtual void Darw() override;
};

