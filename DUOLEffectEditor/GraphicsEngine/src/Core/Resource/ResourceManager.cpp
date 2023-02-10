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

#include "Core/Resource/Resource/VBIBMesh.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/Pass/Particle/OITParticlePass.h"
#include "Core/Pass/Particle/ParticleOutLinePass.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/Pass/OutLinePass.h"
#include "Core/Pass/BlurPass.h"
#include "Core/Pass/3DObject/SkyBoxPass.h"

#include <memory>

#include "Core/Resource/Resource/GeometryShader.h"
#include "Core/Resource/Resource/PixelShader.h"
#include "Core/Resource/Resource/VertexShader.h"
#include "Core/Resource/Resource/Texture.h"

#include "..\..\DUOLCommon/include/DUOLCommon/StringHelper.h"
namespace MuscleGrapics
{
	ResourceManager::ResourceManager() :
		_factory(nullptr),
		_meshId(0), _textureId(1),
		_3DShaderIDs(), _particleShaderIDs()
	{
		_factory = new Factory();
	}
	ResourceManager::~ResourceManager()
	{
		for (auto& iter : _3DShaderIDs)
			delete iter.second;

		_3DShaderIDs.clear();

		for (auto& iter : _particleShaderIDs)
			delete iter.second;

		_particleShaderIDs.clear();


		for (auto& iter : _textureRenderShaderIDs)
			delete iter.second;

		for (auto& iter : _shaderClassInstanceStorage)
			iter.second.second->Release();

		for (auto& iter1 : _resourceStorage)
		{
			for (auto& iter2 : iter1.second)
			{
				delete iter2.second;
			}
		}

		delete _factory;
	}

	void ResourceManager::CompileVertexShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto vertexShader = GetResource<VertexShader>(TupleToString(key));

		if (!vertexShader)
		{
			_factory->CompileVertexShader(pipeLineDesc, fileName, entryName, macro);

			vertexShader = new VertexShader(pipeLineDesc._vs, pipeLineDesc._il, pipeLineDesc._vsDynamicLinkageArray, pipeLineDesc._numVsInstance);

			AddResource(TupleToString(key), vertexShader);
		}
		else
		{
			pipeLineDesc._vs = vertexShader->Get();

			pipeLineDesc._il = vertexShader->GetInputLayOut();
		}

	}

	void ResourceManager::CompilePixelShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName, std::vector<D3D_SHADER_MACRO> macro)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto pixelShader = GetResource<PixelShader>(TupleToString(key));

		if (!pixelShader)
		{
			_factory->CompilePixelShader(pipeLineDesc, fileName, entryName, macro);

			pixelShader = new PixelShader(pipeLineDesc._ps, pipeLineDesc._psDynamicLinkageArray, pipeLineDesc._numPsInstance);

			AddResource(TupleToString(key), pixelShader);
		}
		else
		{
			pipeLineDesc._ps = pixelShader->Get();

			pipeLineDesc._psDynamicLinkageArray = pixelShader->GetDynamicLinkageArray();

			pipeLineDesc._numPsInstance = pixelShader->GetInstanceNum();
		}
	}

	void ResourceManager::CompileGeometryShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName, bool useStreamOut, std::vector<D3D_SHADER_MACRO> macro)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto geometryShader = GetResource<GeometryShader>(TupleToString(key));

		if (!geometryShader)
		{
			_factory->CompileGeometryShader(pipeLineDesc, fileName, entryName, useStreamOut, macro);

			geometryShader = new GeometryShader(pipeLineDesc._gs, pipeLineDesc._gsDynamicLinkageArray, pipeLineDesc._numGsInstance);

			AddResource(TupleToString(key), geometryShader);
		}
		else
		{
			pipeLineDesc._gs = geometryShader->Get();

			pipeLineDesc._gsDynamicLinkageArray = geometryShader->GetDynamicLinkageArray();

			pipeLineDesc._numGsInstance = geometryShader->GetInstanceNum();
		}
	}

	void ResourceManager::CompileComputeShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro)
	{
	}

	void ResourceManager::init()
	{
#pragma region VBIBMesh

		AddResource("Grid", _factory->CreateGridMesh());

		AddResource("Box", _factory->CreateBoxMesh());

		AddResource("Sphere", _factory->CreateSphereMesh());

		AddResource("Texture", _factory->CreateTextureMesh());

#pragma endregion
#pragma region Shader
		_textureRenderShaderIDs.insert({ TEXT("TextureRenderPass") ,new TextureRenderPass() });

		_textureRenderShaderIDs.insert({ TEXT("OutLinePass") ,new OutLinePass() });

		_textureRenderShaderIDs.insert({ TEXT("BlurPass") ,new BlurPass() });

		_3DShaderIDs.insert({ TEXT("Wire"), new WirePass() });

		_3DShaderIDs.insert({ TEXT("Basic"), new BasicPass() });

		_3DShaderIDs.insert({ TEXT("NoLightOrthoGraphics"), new NoLit_OrthoGraphicsPass() });

		_3DShaderIDs.insert({ TEXT("SkyBoxPass"), new SkyBoxPass() });

		_particleShaderIDs.insert({ TEXT("BasicParticle"), new BasicParticlePass() });

		_particleShaderIDs.insert({ TEXT("BasicParticleObjectID"), new BasicParticleObjectIDPass() });

		_particleShaderIDs.insert({ TEXT("OITParticlePass"), new OITParticlePass() });

		_particleShaderIDs.insert({ TEXT("ParticleOutLinePass"), new ParticleOutLinePass() });
#pragma endregion
		AddResource("RandomTex", new ShaderResourceView(_factory->CreateRandomTexture1DSRV()));
	}
	void* ResourceManager::LoadTexture(tstring path)
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
			//hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			//	TEX_FILTER_DEFAULT, 0, mipChain);

			hr = ::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(),
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

		;
		AddResource(DUOLCommon::StringHelper::WStringToString(path), new ShaderResourceView(srv));

		return srv;
	}

	void* ResourceManager::LoadTexture(std::string path)
	{
		return LoadTexture(DUOLCommon::StringHelper::StringToWString(path));
	}

	ID3D11ShaderResourceView* ResourceManager::GetTexture(std::string name)
	{
		auto result = GetResource<ShaderResourceView>(name);

		if (!result)
			return static_cast<ID3D11ShaderResourceView*>(LoadTexture(name));

		return result->Get();
	}

	ID3D11ShaderResourceView* ResourceManager::GetTexture(tstring name)
	{
		return GetTexture(DUOLCommon::StringHelper::WStringToString(name));
	}

	ID3D11ShaderResourceView* ResourceManager::GetNoiseMap(std::tuple<float, int, float> key)
	{
		std::string strKey = std::to_string(std::get<0>(key)) + std::to_string(std::get<1>(key)) + std::to_string(std::get<2>(key));

		auto result = GetResource<ShaderResourceView>(strKey);

		if (!result)
		{
			result = new ShaderResourceView(
				_factory->CreatePerlinNoiseTexture(
					std::get<0>(key), std::get<1>(key), std::get<2>(key),
					0, 100.0f, 100.0f));

			AddResource(strKey, result);
		}

		return result->Get();
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

	void ResourceManager::CreateParticleMesh(std::string name)
	{
		if (!GetResource<ParticleMesh>(name))
			AddResource(name, _factory->CreateParticleMesh());
	}

	void ResourceManager::InsertShaderClassInstance(std::string key, std::pair<unsigned int, ID3D11ClassInstance*> instance)
	{
		if (_shaderClassInstanceStorage.find(key) != _shaderClassInstanceStorage.end())
			assert(false);

		_shaderClassInstanceStorage.insert({ key,instance });
	}

	std::pair<unsigned int, ID3D11ClassInstance*>& ResourceManager::GetShaderClassInstance(std::string key)
	{
		auto temp = _shaderClassInstanceStorage.find(key);

		if (temp == _shaderClassInstanceStorage.end())
			assert(false);

		return temp->second;
	}

	std::string ResourceManager::D3DMacroToString(std::vector<D3D_SHADER_MACRO>& macro)
	{
		std::string result;
		for (auto& iter : macro)
		{
			if (iter.Definition)
				result += iter.Definition;
			if (iter.Name)
				result += iter.Name;
		}
		return result;
	}

	std::string ResourceManager::TupleToString(std::tuple<tstring, std::string, std::string>& key)
	{
		std::string result;

		auto target = std::get<0>(key).c_str();

		const int stringLength = static_cast<int>(std::wcslen(target));

		// WCHAR 는 한 문자 당 2바이트의 공간.
		const int bufferLength = stringLength * 2 + 1;

		char* buffer = new char[bufferLength];

		// WideCharToMultiByte 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
		// 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
		int end = WideCharToMultiByte(CP_UTF8, 0, target, stringLength,
			buffer, bufferLength,
			nullptr, nullptr);

		buffer[end] = '\0';

		auto retVal = std::string{ buffer };

		delete[] buffer;

		result += retVal;

		result += std::get<1>(key);

		result += std::get<2>(key);

		return result;

	}
}
