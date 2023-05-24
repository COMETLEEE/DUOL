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
#include "Core/Pass/OITBlendPass.h"

#include <memory>

#include "Core/Resource/Resource/GeometryShader.h"
#include "Core/Resource/Resource/PixelShader.h"
#include "Core/Resource/Resource/VertexShader.h"
#include "Core/Resource/Resource/ComputeShader.h"
#include "Core/Resource/Resource/Texture.h"

#include "..\..\DUOLCommon/include/DUOLCommon/StringHelper.h"
#include "Core/Pass/PBRBake/BRDFLookUpTable_Bake_Pass.h"
#include "Core/Pass/PBRBake/CubeMapToIrradiance_Bake_Pass.h"
#include "Core/Pass/PBRBake/CubeMapToRadiance_Bake_Pass.h"

namespace MuscleGrapics
{
	ResourceManager::ResourceManager() :
		_factory(nullptr),
		_meshId(0), _textureId(1)
	{
		_factory = new Factory();
	}
	ResourceManager::~ResourceManager()
	{
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
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto vertexShader = GetResource<VertexShader>(TupleToString(key));

		if (!vertexShader)
		{
			_factory->CompileVertexShader(pipeLineDesc, fileName, entryName, macro, _shaderLikingDescs);

			vertexShader = new VertexShader(pipeLineDesc._vs, pipeLineDesc._il, pipeLineDesc._vsDynamicLinkageArray, pipeLineDesc._numVsInstance);

			AddResource(TupleToString(key), vertexShader);
		}
		else
		{
			pipeLineDesc._vs = vertexShader->Get();

			pipeLineDesc._il = vertexShader->GetInputLayOut();
		}

	}

	void ResourceManager::CompilePixelShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto pixelShader = GetResource<PixelShader>(TupleToString(key));

		if (!pixelShader)
		{
			_factory->CompilePixelShader(pipeLineDesc, fileName, entryName, macro, _shaderLikingDescs);

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

	void ResourceManager::CompileGeometryShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName, bool useStreamOut,
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto geometryShader = GetResource<GeometryShader>(TupleToString(key));

		if (!geometryShader)
		{
			_factory->CompileGeometryShader(pipeLineDesc, fileName, entryName, useStreamOut, macro, _shaderLikingDescs);

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
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		//ComputeShader는 셰이더 리플렉션에서 버그가 발생한다.. 이유를 도저히 못찾겠다.. 나중에 시간날 때 다시 찾아보자..

		std::tuple<tstring, std::string, std::string> key = { fileName ,entryName,D3DMacroToString(macro) };

		auto computeShader = GetResource<ComputeShader>(TupleToString(key));

		if (!computeShader)
		{
			_factory->CompileComputeShader(pipeLineDesc, fileName, entryName, macro, _shaderLikingDescs);

			computeShader = new ComputeShader(pipeLineDesc._cs, pipeLineDesc._csDynamicLinkageArray, pipeLineDesc._numPsInstance);

			AddResource(TupleToString(key), computeShader);
		}
		else
		{
			pipeLineDesc._cs = computeShader->Get();

			pipeLineDesc._csDynamicLinkageArray = computeShader->GetDynamicLinkageArray();

			pipeLineDesc._numCsInstance = computeShader->GetInstanceNum();
		}
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
		AddResource<Pass_Texture>("BRDFLookUpTable_Bake_Pass", new BRDFLookUpTable_Bake_Pass());

		AddResource<Pass_Texture>("CubeMapToIrradiance_Bake_Pass", new CubeMapToIrradiance_Bake_Pass());

		AddResource<Pass_Texture>("CubeMapToRadiance_Bake_Pass", new CubeMapToRadiance_Bake_Pass());

		AddResource<Pass_Texture>("TextureRenderPass", new TextureRenderPass());

		AddResource<Pass_Texture>("OITBlendPass", new OITBlendPass());

		AddResource<Pass_Texture>("OutLinePass", new OutLinePass());

		AddResource<Pass_Texture>("BlurPass", new BlurPass());

		AddResource<Pass_3D>("Wire", new WirePass());

		AddResource<Pass_3D>("Basic", new BasicPass());

		AddResource<Pass_3D>("NoLightOrthoGraphics", new NoLit_OrthoGraphicsPass());

		AddResource<Pass_3D>("SkyBoxPass", new SkyBoxPass());

		AddResource<Pass_Particle>("BasicParticle", new BasicParticlePass());

		AddResource<Pass_Particle>("BasicParticleObjectID", new BasicParticleObjectIDPass());

		AddResource<Pass_Particle>("OITParticlePass", new OITParticlePass());

		AddResource<Pass_Particle>("ParticleOutLinePass", new ParticleOutLinePass());
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

		AddResource(DUOLCommon::StringHelper::WStringToString(path), new ShaderResourceView(srv));
		if (isCubeMap)
		{
			// PBR Todo :
			//_factory->Bake_BRDF_PreFilter_Irradiance(DUOLCommon::StringHelper::WStringToString(path), texture2D, srv);
		}
		texture2D->Release();

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

	void ResourceManager::CreateParticleMesh(std::string name)
	{
		if (!GetResource<ParticleMesh>(name))
			AddResource(name, _factory->CreateParticleMesh());
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
