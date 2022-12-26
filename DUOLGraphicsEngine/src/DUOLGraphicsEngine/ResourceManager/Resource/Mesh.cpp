#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"

DUOLGraphicsEngine::SubMesh* DUOLGraphicsEngine::MeshBase::GetSubMesh(int subMeshIdx)
{
	if(subMeshIdx >= _subMeshCount)
	{
		return nullptr;
	}

	return &_subMeshs.at(subMeshIdx);
}

DUOLGraphicsEngine::MeshBase* DUOLGraphicsEngine::Model::GetMesh(unsigned int MeshIdx) const
{
	if (MeshIdx >= _meshCount)
	{
		return nullptr;
	}

	return _meshs.at(MeshIdx);
}

void DUOLGraphicsEngine::Model::SetIsSkinningModel(bool value)
{
	_isSkinningModel = value;
}


void DUOLGraphicsEngine::Model::SetMeshCount(int count)
{
	_meshCount = count;
	_meshs.reserve(count);
}

void DUOLGraphicsEngine::Model::AddMesh(MeshBase* mesh)
{
	_meshs.emplace_back(mesh);
}