#include "Core/Resource/ResourceManager.h"
#include <filesystem>

#include "..\\..\\Library\\inc\\DirectXTex.h"
#include "..\\..\\Library\\inc\\SpriteFont.h"
#include "..\\..\\Library\\inc\\SpriteBatch.h"

#include "Core/Pass/Particle/BasicParticlePass.h"
#include "Core/Pass/3DObject/BasicPass.h"
#include "Core/Pass/3DObject/WirePass.h"
#include "Core/Pass/3DObject/NoLit_OrthoGraphicsPass.h"
#include "Core/Resource/Factory.h"
#include "Core/Pass/Particle/BasicParticleObjectIDPass.h"

#include "Core/Resource/VBIBMesh.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/Pass/Particle/OITParticlePass.h"
#include "Core/Pass/Particle/ParticleOutLinePass.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/Pass/OutLinePass.h"
#include "Core/Pass/BlurPass.h"

namespace MuscleGrapics
{
	ResourceManager::ResourceManager() :
		_factory(nullptr), _textureMapIDs(), _mesh_VBIB_IDs(),
		_meshId(0), _textureId(1), _mesh_VBIB_ID_Maps(), _particleMapIDs()
		, _3DShaderIDs(), _particleShaderIDs()
	{
		_factory = new Factory();
	}
	ResourceManager::~ResourceManager()
	{
		for (auto& iter : _textureMapIDs)
			ReleaseCOM(iter.second);

		_textureMapIDs.clear();

		for (auto& iter : _mesh_VBIB_IDs)
			delete iter.second;

		_mesh_VBIB_IDs.clear();

		for (auto& iter : _3DShaderIDs)
			delete iter.second;

		_3DShaderIDs.clear();

		for (auto& iter : _particleShaderIDs)
			delete iter.second;

		_particleShaderIDs.clear();

		for (auto& iter : _particleMapIDs)
			delete iter.second;

		for (auto& iter : _textureRenderShaderIDs)
			delete iter.second;

		_particleMapIDs.clear();

		_mesh_VBIB_ID_Maps.clear();

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
		_textureRenderShaderIDs.insert({ TEXT("TextureRenderPass") ,new TextureRenderPass() });

		_textureRenderShaderIDs.insert({ TEXT("OutLinePass") ,new OutLinePass() });

		_textureRenderShaderIDs.insert({ TEXT("BlurPass") ,new BlurPass() });

		_3DShaderIDs.insert({ TEXT("Wire"), new WirePass() });

		_3DShaderIDs.insert({ TEXT("Basic"), new BasicPass() });

		_3DShaderIDs.insert({ TEXT("NoLightOrthoGraphics"), new NoLit_OrthoGraphicsPass() });

		_particleShaderIDs.insert({ TEXT("BasicParticle"), new BasicParticlePass() });

		_particleShaderIDs.insert({ TEXT("BasicParticleObjectID"), new BasicParticleObjectIDPass() });

		_particleShaderIDs.insert({ TEXT("OITParticlePass"), new OITParticlePass() });

		_particleShaderIDs.insert({ TEXT("ParticleOutLinePass"), new ParticleOutLinePass() });
#pragma endregion
		_textureMapIDs.insert({ TEXT("RandomTex"), _factory->CreateRandomTexture1DSRV() }); // 랜덤텍스쳐는 특별한친구니까...

	}
	unsigned int ResourceManager::InsertVBIBMesh(tstring name, VBIBMesh* mesh)
	{
		_mesh_VBIB_ID_Maps.insert({ name,_meshId });

		_mesh_VBIB_IDs.insert({ _meshId, mesh });

		return _meshId++;
	}
	VBIBMesh* ResourceManager::GetVBIBMesh(unsigned int meshID)
	{
		auto mesh = _mesh_VBIB_IDs.at(meshID);

		assert(mesh);

		return mesh;
	}
	unsigned int ResourceManager::GetVBIBMesh(tstring meshName)
	{
		return _mesh_VBIB_ID_Maps[meshName];
	}
	void* ResourceManager::InsertTexture(tstring path)
	{
		ScratchImage image;

		ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		ID3D11Texture2D* texture2D = nullptr;

		ID3D11ShaderResourceView* srv = nullptr;

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

		_textureMapIDs.insert({ path, srv });

		return srv;
	}
	ID3D11ShaderResourceView* ResourceManager::GetTexture(tstring name)
	{
		if (_textureMapIDs.end() != _textureMapIDs.find(name))
			return _textureMapIDs[name];
		else
			return static_cast<ID3D11ShaderResourceView*>(InsertTexture(name));
	}
	void ResourceManager::InsertParticleMesh(unsigned int objectID)
	{
		if (_particleMapIDs.find(objectID) == _particleMapIDs.end())
			_particleMapIDs.insert({ objectID, new ParticleMesh() });
	}
	ParticleMesh* ResourceManager::GetParticleMesh(unsigned int meshID)
	{
		return _particleMapIDs[meshID];
	}
	void ResourceManager::DeleteParticleMesh(unsigned int objectID)
	{
		delete _particleMapIDs[objectID];

		_particleMapIDs.erase(objectID);
	}
	PassBase<RenderingData_3D>* ResourceManager::Get3DShader(tstring name)
	{
		return _3DShaderIDs[name];
	}
	PassBase<RenderingData_Particle>* ResourceManager::GetParticleShader(tstring name)
	{
		return _particleShaderIDs[name];
	}

	PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>* ResourceManager::GetTextureRenderShader(tstring name)
	{
		return _textureRenderShaderIDs[name];
	}
}
