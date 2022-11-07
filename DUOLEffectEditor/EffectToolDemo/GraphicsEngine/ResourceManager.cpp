#include "pch.h"
#include "ResourceManager.h"
#include "DDSTextureLoader.h"


ResourceManager::ResourceManager() :_effects(), _factory(nullptr), _textureMapIDs(), _VBIBMesh_IDs(),
_meshId(0), _textureId(0), _VBIBMesh_ID_Maps(), _textureMapID_Maps(), _particleMapIDs()
, _3DShaderIDs(), _particleShaderIDs(), _particleId(0)
{
	_factory = new Factory();

	_effects = new Effects();

	InputLayout::Initialize();
}
// 매핑이 되는 uv 좌표 그리드 만들어보기.
// 쉐이더 디버깅. 점 찍어보기
// 
ResourceManager::~ResourceManager()
{
	for (auto& iter : _textureMapIDs)
	{
		ReleaseCOM(iter.second);
	}
	_textureMapIDs.clear();

	for (auto& iter : _VBIBMesh_IDs)
	{
		delete iter.second;
	}
	_VBIBMesh_IDs.clear();

	for (auto& iter : _3DShaderIDs)
	{
		delete iter.second;
	}
	_3DShaderIDs.clear();

	for (auto& iter : _particleShaderIDs)
	{
		delete iter.second;
	}
	_particleShaderIDs.clear();

	for (auto& iter : _particleMapIDs)
	{
		delete iter.second;
	}
	_particleMapIDs.clear();

	_VBIBMesh_ID_Maps.clear();
	_textureMapID_Maps.clear();


	delete _effects;

	delete _factory;

	InputLayout::Finalize();
}

void ResourceManager::init()
{
#pragma region VBIBMesh

	InsertVBIBMesh(TEXT("Grid"), _factory->CreateGridMesh());		// 0;
	InsertVBIBMesh(TEXT("Box"), _factory->CreateBoxMesh());			// 1;
	InsertVBIBMesh(TEXT("Sphere"), _factory->CreateSphereMesh());	// 2;

#pragma endregion

#pragma region Shader

	_3DShaderIDs.insert({ TEXT("Wire"), new WirePass() });
	_3DShaderIDs.insert({ TEXT("Basic"), new BasicPass() });

	_particleShaderIDs.insert({ TEXT("BasicParticle"), new BasicParticlePass() });
#pragma endregion


#pragma region TextureLoad
	//_textureMapIDs.insert({ 1, d3dHelper::CreateRandomTexture1DSRV(DXEngine::GetInstance()->GetD3dDevice()) }); // 랜덤텍스쳐는 특별한친구니까... 일단 -1에 넣어두자..!
	//InsertTexture(TEXT("Particle"), TEXT("Resource/Image/Particle.png"));
#pragma endregion

#pragma region Particle

	InsertParticleMesh(new ParticleMesh());
#pragma endregion


}

unsigned int ResourceManager::InsertVBIBMesh(tstring name, VBIBMesh* mesh)
{
	_VBIBMesh_ID_Maps.insert({ name,_meshId });

	_VBIBMesh_IDs.insert({ _meshId, mesh });

	return _meshId++;
}

VBIBMesh* ResourceManager::GetVBIBMesh(unsigned int meshID)
{
	auto mesh = _VBIBMesh_IDs.at(meshID);

	assert(mesh);

	return mesh;
}

unsigned int ResourceManager::GetVBIBMesh(tstring meshName)
{
	return _textureMapID_Maps[meshName];
}

unsigned int ResourceManager::InsertTexture(tstring name, tstring path)
{
	std::wstring text = path;

	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* texture;

	HRTex(CreateDDSTextureFromFile(
		DXEngine::GetInstance()->GetD3dDevice(),
		text.c_str(),
		&texResource,
		&texture));

	ReleaseCOM(texResource);

	_textureMapID_Maps.insert({ name,_textureId });

	_textureMapIDs.insert({ _textureId, texture });

	return _textureId++;
}

unsigned int ResourceManager::InsertTexture(tstring name, std::vector<tstring>& path)
{
	std::vector<std::wstring> text;
	auto texture = d3dHelper::CreateTexture2DArraySRV(
		DXEngine::GetInstance()->GetD3dDevice()
		, DXEngine::GetInstance()->Getd3dImmediateContext(), path);

	_textureMapID_Maps.insert({ name,_textureId });

	_textureMapIDs.insert({ _textureId, texture });

	return _textureId++;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(unsigned int textureID)
{
	return _textureMapIDs[textureID];
}

unsigned int ResourceManager::GetTexture(tstring textureMap)
{
	return _textureMapID_Maps[textureMap];
}

unsigned int ResourceManager::InsertParticleMesh(ParticleMesh* mesh)
{

	_particleMapIDs.insert({ _particleId, mesh });

	return _particleId++;
}

ParticleMesh* ResourceManager::GetParticleMesh(unsigned meshID)
{
	return _particleMapIDs[meshID];
}

unsigned int ResourceManager::GetParticleMesh(tstring meshName)
{
	return _textureMapID_Maps[meshName];
}

IPass<RenderingData_3D>* ResourceManager::Get3DShader(tstring name)
{
	return _3DShaderIDs[name];
}

IPass<RenderingData_Particle>* ResourceManager::GetParticleShader(tstring name)
{
	return _particleShaderIDs[name];
}


