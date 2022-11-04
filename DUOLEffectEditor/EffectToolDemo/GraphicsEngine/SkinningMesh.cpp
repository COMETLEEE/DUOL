#include "pch.h"
#include "SkinningMesh.h"

SkinningMesh::SkinningMesh(Mesh* _MeshObject) : IMeshComponent(_MeshObject)
{
}

SkinningMesh::~SkinningMesh()
{
}

void SkinningMesh::WakeMesh()
{
}

void SkinningMesh::SleepMesh()
{
}

void SkinningMesh::Darw()
{
	if (m_MeshObject->GetFinalBoneList().size())
		Effects::LightFX->SetBoneTM(*m_MeshObject->GetFinalBoneList().data(), m_MeshObject->GetFinalBoneList().size());
}
