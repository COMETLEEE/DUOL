#pragma once
class Mesh;

class IMeshComponent
{
public:
	IMeshComponent(Mesh* _MeshObject);
	virtual ~IMeshComponent();

	Mesh* m_MeshObject;
public:
	virtual void Darw() abstract;
	virtual void WakeMesh() abstract;
	virtual void SleepMesh() abstract;

};

