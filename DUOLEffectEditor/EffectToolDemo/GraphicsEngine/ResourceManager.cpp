#include "pch.h"
#include "ResourceManager.h"

#include <filesystem>


ResourceManager::ResourceManager() : _factory(nullptr), _textureMapIDs(), _VBIBMesh_IDs(),
_meshId(0), _textureId(1), _VBIBMesh_ID_Maps(), _textureMapID_Maps(), _particleMapIDs()
, _3DShaderIDs(), _particleShaderIDs(), _particleId(0)
{
	_factory = new Factory();
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



	delete _factory;

}

void ResourceManager::init()
{
#pragma region VBIBMesh
	//순서 중요함..!!!!
	InsertVBIBMesh(TEXT("Grid"), _factory->CreateGridMesh());		// 0;

	InsertVBIBMesh(TEXT("Box"), _factory->CreateBoxMesh());			// 1;

	InsertVBIBMesh(TEXT("Sphere"), _factory->CreateSphereMesh());	// 2;

	InsertVBIBMesh(TEXT("Texture"), _factory->CreateTextureMesh());	// 3; // 텍스처 렌더링을 위한.. 디퍼드 렌더링을 위한 텍스처.
	//순서 중요함..!!!!
#pragma endregion

#pragma region Shader

	_3DShaderIDs.insert({ TEXT("Wire"), new WirePass() });
	_3DShaderIDs.insert({ TEXT("Basic"), new BasicPass() });

	_particleShaderIDs.insert({ TEXT("BasicParticle"), new BasicParticlePass() });
#pragma endregion


#pragma region TextureLoad
	_textureMapIDs.insert({ 0, _factory->CreateRandomTexture1DSRV() }); // 랜덤텍스쳐는 특별한친구니까... 일단 0에 넣어두자..!
	InsertTexture(TEXT("ParticleNone"), TEXT("Resource/Image/Particle.dds"));
	InsertTexture(TEXT("test1"), TEXT("Resource/Image/test1.png"));
	InsertTexture(TEXT("test2"), TEXT("Resource/Image/test2.png"));
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
	return _VBIBMesh_ID_Maps[meshName];
}

void* ResourceManager::InsertTexture(tstring name, tstring path)
{
	ScratchImage image;

	ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();

	ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

	ID3D11Texture2D* texture2D;

	ID3D11ShaderResourceView* srv;

	// 텍스쳐 파일 로드 후 그래픽 카드에 전달
	std::wstring ext = std::filesystem::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
	else if (ext == L".tga" || ext == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, image);
	else if (ext == L".hdr" || ext == L".HDR")
		::LoadFromHDRFile(path.c_str(), nullptr, image);
	else// png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);



	ScratchImage mipChain;

	bool isCubeMap = image.GetMetadata().IsCubemap();

	HRESULT hr;

	if (isCubeMap)
	{
		hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			TEX_FILTER_DEFAULT, 0, mipChain);

		hr = ::CreateTexture(device, mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(),
			reinterpret_cast<ID3D11Resource**>(&texture2D));
	}
	else
		hr = ::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			reinterpret_cast<ID3D11Resource**>(&texture2D));

	if (FAILED(hr))
		::MessageBoxA(nullptr, "Texture Create Failed !", nullptr, MB_OK);

	D3D11_TEXTURE2D_DESC textureDesc;

	texture2D->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (!isCubeMap)
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	else
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

	hr = device->CreateShaderResourceView(texture2D, &srvDesc, &srv);

	// Mip Level이 만약 0이면 .. Mip Map 형성해주자.
	if (textureDesc.MipLevels == 0)
		deviceContext->GenerateMips(srv);


	texture2D->Release();

	void* id = reinterpret_cast<void*>(srv);
	_textureMapID_Maps.insert({ name, id });

	_textureMapIDs.insert({ id, srv });

	return id;
}

void* ResourceManager::InsertTexture(tstring name, std::vector<tstring>& path)
{
	std::vector<std::wstring> text;
	//auto texture = d3dHelper::CreateTexture2DArraySRV(
	//	DXEngine::GetInstance()->GetD3dDevice()
	//	, DXEngine::GetInstance()->Getd3dImmediateContext(), path);
	// 텍스쳐 배열 구현 해야됨...

	//_textureMapID_Maps.insert({ name,_textureId });

	//_textureMapIDs.insert({ _textureId, texture });

	return 0;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(void* textureID)
{
	if (_textureMapIDs.end() != _textureMapIDs.find(textureID))
		return _textureMapIDs[textureID];
	else
	{
		assert(false);
		return 0;
	}
}

void* ResourceManager::GetTexture(tstring textureMap)
{
	if (_textureMapID_Maps.end() != _textureMapID_Maps.find(textureMap))
		return _textureMapID_Maps[textureMap];
	else
	{
		assert(false);
		return 0;
	}
}

unsigned int ResourceManager::InsertParticleMesh(ParticleMesh* mesh)
{

	_particleMapIDs.insert({ _particleId, mesh });

	return _particleId++;
}

ParticleMesh* ResourceManager::GetParticleMesh(unsigned int meshID)
{
	return _particleMapIDs[meshID];
}

PassBase<RenderingData_3D>* ResourceManager::Get3DShader(tstring name)
{
	return _3DShaderIDs[name];
}

PassBase<RenderingData_Particle>* ResourceManager::GetParticleShader(tstring name)
{
	return _particleShaderIDs[name];
}


