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
#include "Core/Pass/3DObject/SkyBoxPass.h"

#include <memory>

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

		for (auto& iter : _perlineNoiseMaps)
		{
			iter.second.reset();
		}

		_perlineNoiseMaps.clear();

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

		for (auto& iter : _vertexShaderStorage)
			iter.second->Release();

		for (auto& iter : _inputLayoutStorage)
			iter.second->Release();

		for (auto& iter : _pixelShaderStorage)
			iter.second->Release();

		for (auto& iter : _geometryShaderStorage)
			iter.second->Release();

		_particleMapIDs.clear();

		_mesh_VBIB_ID_Maps.clear();

		delete _factory;
	}



	ID3D11VertexShader* ResourceManager::CompileVertexShader(const WCHAR* fileName, const CHAR* entryName,
		D3D11_INPUT_ELEMENT_DESC polygonLayout[], UINT size, std::vector<D3D_SHADER_MACRO> macro)
	{
		ID3DBlob* errorMessage = nullptr;
		ID3DBlob* vertexShaderBuffer = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		auto vertexShader = GetVertexShader({ fileName ,entryName,D3DMacroToString(macro) });

		if (!vertexShader)
		{
			ID3D11VertexShader* vertexShaderTemp = nullptr;

			ID3D11InputLayout* inputLayOutTemp = nullptr;

			if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
				entryName, "vs_5_0", compileFlag, 0, &vertexShaderBuffer, &errorMessage)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "VS Shader Compile Failed ! PassBase..", nullptr, MB_OK);
			}

			if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShaderTemp)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "VS Shader Create Failed ! PassBase..", nullptr, MB_OK);
			}

			if (FAILED(device->CreateInputLayout(polygonLayout, size,
				vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayOutTemp)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "InputLayout Create Failed ! PassBase..", nullptr, MB_OK);
			}

			vertexShaderBuffer->Release();

			if (errorMessage)
				errorMessage->Release();

			InsertVertexShader({ fileName ,entryName,D3DMacroToString(macro) }, vertexShaderTemp);

			InsertInputLayOut(vertexShaderTemp, inputLayOutTemp);
			vertexShader = vertexShaderTemp;
		}

		return vertexShader;

	}

	ID3D11PixelShader* ResourceManager::CompilePixelShader(const WCHAR* fileName, const CHAR* entryName, std::vector<D3D_SHADER_MACRO> macro)
	{
		ID3DBlob* pixelShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		auto pixelShader = GetPixelShader({ fileName ,entryName,D3DMacroToString(macro) });

		if (!pixelShader)
		{
			ID3D11PixelShader* pixelShaderTemp = nullptr;

			if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE
				, entryName, "ps_5_0", compileFlag, 0, &pixelShaderBuffer, &errorMessage)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "PS Shader Compile Failed ! Shader..", nullptr, MB_OK);
			}

			if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShaderTemp)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);
			}


			pixelShaderBuffer->Release();
			if (errorMessage)
				errorMessage->Release();

			InsertPixelShader({ fileName ,entryName,D3DMacroToString(macro) }, pixelShaderTemp);

			pixelShader = pixelShaderTemp;
		}

		return pixelShader;
	}

	ID3D11GeometryShader* ResourceManager::CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName, bool useStreamOut, std::vector<D3D_SHADER_MACRO> macro)
	{
		ID3DBlob* geometryShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		auto geometryShader = GetGeometryShader({ fileName ,entryName,D3DMacroToString(macro) });

		if (!geometryShader)
		{
			ID3D11GeometryShader* geometryShaderTemp = nullptr;

			if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE
				, entryName, "gs_5_0", compileFlag, 0, &geometryShaderBuffer, &errorMessage)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
			}

			if (useStreamOut)
			{
				constexpr int size = VertexDesc::BasicParticleVertexSize;

				//define the system output declaration entry, i.e. what will be written in the SO
				D3D11_SO_DECLARATION_ENTRY pDecl[size] =
				{
					//position, semantic name, semantic index, start component, component count, output slot
					{0,"POSITION", 0, 0, 3, 0 }, // output first 3 components of SPEED
					{0, "VELOCITY", 0, 0, 3, 0 }, // output first 3 components of "POSITION"
					{0, "SIZE_STARTSIZE", 0, 0, 4, 0 }, // output first 2 components of SIZE
					{0, "AGE_LIFETIME_ROTATION_GRAVITY", 0, 0, 4, 0 }, // output AGE
					{0, "TYPE",0, 0, 1, 0 }, // output TYPE
					{0, "VERTEXID",0, 0, 1, 0 }, // output TYPE
					{0, "STARTCOLOR",0, 0, 4, 0 }, // output TYPE
					{0, "COLOR",0, 0, 4, 0 }, // output TYPE
					{0, "QUADTEX",0, 0, 2, 0 }, // output TYPE
					{0, "QUADTEX",1, 0, 2, 0 }, // output TYPE
					{0, "QUADTEX",2, 0, 2, 0 }, // output TYPE
					{0, "QUADTEX",3, 0, 2, 0 }, // output TYPE
					{0, "EMITTERPOS",0, 0, 3, 0 }, // output TYPE
				};
				int offset = 13;

				for (int i = 0; i < 15; i++)
					pDecl[offset + i] = { 0, "PREVPOS",(UINT)i, 0, 3, 0 };

				pDecl[28] = { 0, "LASTESTPREVPOS",0, 0, 3, 0 };

				UINT strides[1] = { sizeof(Vertex::Particle) };

				device->CreateGeometryShaderWithStreamOutput(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), pDecl,
					size, strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &geometryShaderTemp);
			}
			else
			{
				device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &geometryShaderTemp);
			}

			InsertGeometryShader({ fileName ,entryName,D3DMacroToString(macro) }, geometryShaderTemp);

			geometryShader = geometryShaderTemp;
		}
		return geometryShader;

	}


	void ResourceManager::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName)
	{
		char* compileErrors;
		unsigned long long bufferSize, i;
		std::ofstream fout;

		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());

		// Get the length of the message.
		bufferSize = errorMessage->GetBufferSize();

		// Open a file to write the error message to.
		fout.open("shader-error.txt");

		// Write out the error message.
		for (i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// Close the file.
		fout.close();

		// Release the error message.
		errorMessage->Release();

		errorMessage = 0;

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.",
			shaderFileName, MB_OK);

		return;

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

		_3DShaderIDs.insert({ TEXT("SkyBoxPass"), new SkyBoxPass() });

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

	ID3D11ShaderResourceView* ResourceManager::GetNoiseMap(std::tuple<float, int, float> key)
	{

		auto func = [](ID3D11ShaderResourceView* p)
		{
			ReleaseCOM(p);
		};
		if (_perlineNoiseMaps.end() == _perlineNoiseMaps.find(key))
		{

			std::shared_ptr<ID3D11ShaderResourceView> noiseMap(
				_factory->CreatePerlinNoiseTexture(std::get<0>(key), std::get<1>(key), std::get<2>(key), 0, 100.0f, 100.0f)
				, func
			);
			_perlineNoiseMaps.insert({
				key,noiseMap
				});
		}

		return _perlineNoiseMaps[key].get();
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

	ID3D11VertexShader* ResourceManager::GetVertexShader(std::tuple<tstring, std::string, std::string> key)
	{
		auto strkey = TupleToString(key);

		auto temp = _vertexShaderStorage.find(strkey);

		if (temp != _vertexShaderStorage.end())
			return temp->second;

		return nullptr;
	}

	void ResourceManager::InsertVertexShader(std::tuple<tstring, std::string, std::string> key, ID3D11VertexShader* shader)
	{
		auto strkey = TupleToString(key);

		if (_vertexShaderStorage.find(strkey) != _vertexShaderStorage.end())
			assert(false);
		_vertexShaderStorage.insert({ strkey, shader });
	}

	ID3D11PixelShader* ResourceManager::GetPixelShader(std::tuple<tstring, std::string, std::string> key)
	{
		auto strkey = TupleToString(key);

		auto temp = _pixelShaderStorage.find(strkey);

		if (temp != _pixelShaderStorage.end())
			return temp->second;

		return nullptr;
	}

	void ResourceManager::InsertPixelShader(std::tuple<tstring, std::string, std::string> key, ID3D11PixelShader* shader)
	{
		auto strkey = TupleToString(key);

		if (_pixelShaderStorage.find(strkey) != _pixelShaderStorage.end())
			assert(false);
		_pixelShaderStorage.insert({ strkey, shader });
	}

	ID3D11GeometryShader* ResourceManager::GetGeometryShader(std::tuple<tstring, std::string, std::string> key)
	{
		auto strkey = TupleToString(key);

		auto temp = _geometryShaderStorage.find(strkey);

		if (temp != _geometryShaderStorage.end())
			return temp->second;

		return nullptr;
	}

	void ResourceManager::InsertGeometryShader(std::tuple<tstring, std::string, std::string> key, ID3D11GeometryShader* shader)
	{
		auto strkey = TupleToString(key);

		if (_geometryShaderStorage.find(strkey) != _geometryShaderStorage.end())
			assert(false);
		_geometryShaderStorage.insert({ strkey, shader });
	}

	ID3D11InputLayout* ResourceManager::GetInputLayout(ID3D11VertexShader* key)
	{
		auto temp = _inputLayoutStorage.find(key);

		if (temp != _inputLayoutStorage.end())
			return temp->second;

		return nullptr;
	}

	void ResourceManager::InsertInputLayOut(ID3D11VertexShader* key, ID3D11InputLayout* inputLayout)
	{
		if (_inputLayoutStorage.find(key) != _inputLayoutStorage.end())
			assert(false);

		_inputLayoutStorage.insert({ key,inputLayout });
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
