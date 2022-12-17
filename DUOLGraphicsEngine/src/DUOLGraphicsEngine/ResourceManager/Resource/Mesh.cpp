#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"


const DUOLGraphicsEngine::SubMesh* DUOLGraphicsEngine::MeshBase::GetSubMesh(int subMeshIdx) const
{
	if(subMeshIdx >= _subMeshCount)
	{
		return nullptr;
	}

	return &_subMeshs.at(subMeshIdx);
}

const DUOLGraphicsEngine::MeshBase* DUOLGraphicsEngine::Model::GetMesh(unsigned MeshIdx) const
{
	if (MeshIdx >= _meshCount)
	{
		return nullptr;
	}

	return _meshs.at(_meshCount);
}

void DUOLGraphicsEngine::Model::SetIsSkinningModel(bool value)
{
	_isSkinningModel = true;
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
