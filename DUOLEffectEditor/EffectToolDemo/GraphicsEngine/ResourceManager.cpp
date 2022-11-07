#include "pch.h"
#include "ResourceManager.h"
#include "DDSTextureLoader.h"

ResourceManager::ResourceManager() :m_Effects(), m_Factory(nullptr), _textureMapIDs(), _VBIBMesh_IDs()
{
	m_Factory = new Factory();

	m_Effects = new Effects();

	InputLayout::Initialize();
}
// 매핑이 되는 uv 좌표 그리드 만들어보기.
// 쉐이더 디버깅. 점 찍어보기
// 
ResourceManager::~ResourceManager()
{
	for (auto& iter : _textureMapIDs)
	{
		delete iter.second;
	}
	_textureMapIDs.clear();

	for (auto& iter : _VBIBMesh_IDs)
	{
		delete iter.second;
	}
	_VBIBMesh_IDs.clear();

	for (auto& iter : _shaderIDs)
	{
		delete iter.second;
	}
	_shaderIDs.clear();


	delete m_Effects;

	delete m_Factory;

	InputLayout::Finalize();
}

void ResourceManager::init()
{
#pragma region VBIBMesh

	InsertVBIBMesh(TEXT("Grid"), m_Factory->CreateGridMesh());		// 0;
	InsertVBIBMesh(TEXT("Box"), m_Factory->CreateBoxMesh());			// 1;
	InsertVBIBMesh(TEXT("Sphere"), m_Factory->CreateSphereMesh());	// 2;

#pragma endregion

#pragma region Shader

	_shaderIDs.insert({ TEXT("Wire"), new WirePass() });
	_shaderIDs.insert({ TEXT("Basic"), new BasicPass() });
#pragma endregion
}

void ResourceManager::InsertVBIBMesh(tstring name, VBIBMesh* mesh)
{
	static unsigned int meshId = 0;

	_VBIBMesh_IDMaps.insert({ name,meshId });

	_VBIBMesh_IDs.insert({ meshId++, mesh });
}

VBIBMesh* ResourceManager::GetVBIBMesh(unsigned int meshID)
{
	auto mesh = _VBIBMesh_IDs.at(meshID);

	assert(mesh);

	return mesh;
}

IPass* ResourceManager::GetShader(tstring name)
{
	return _shaderIDs[name];
}


