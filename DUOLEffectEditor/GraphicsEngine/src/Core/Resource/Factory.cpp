#include "Core/Resource/Factory.h"
#include "Core/Resource/Resource/VBIBMesh.h"
#include "util/VertexDesc.h"
#include "util/TypeDefine.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/Resource/Resource/ParticleMesh.h"
#include "Core/DirectX11/geometrygenerator.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/Pass/PBRBake/CubeMapToIrradiance_Bake_Pass.h"
#include "Core/Pass/PBRBake/CubeMapToRadiance_Bake_Pass.h"
#include "Core/Resource/PerlinNoise.hpp"
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource/ShaderClassInstance.h"

// 2022. 06. 16 그래픽 엔진 구조 변경 중 
// 1. 일단 Mesh의 생성자에서 하던일을 전부 팩토리로 옮길 것. 오버로딩으로 하는게 좋을듯?
// 2. 매쉬에서 분리할 수 있는것들 다른 클래스로 분리 할 것.
// 3. 파서의 파싱 결과에 따라 적절한 컴포넌트를 부착할 수 있도록 구조를 변경할 것.
// 4. MeshComponents의 내용을 채워 넣을 것.
// 5. 리소스 매니저에 팩토리를 사용해서 리소스를 저장할 것.
namespace MuscleGrapics
{
	Factory::Factory()
	{
	}
	ID3D11ShaderResourceView* Factory::LoadTexture(std::string path)
	{
		return nullptr;
	}

	std::pair<ID3D11Buffer*, ID3D11Buffer*> Factory::LoadASE(std::string path)
	{
		return std::pair<ID3D11Buffer*, ID3D11Buffer*>();
	}

	VBIBMesh* Factory::CreateGridMesh()
	{
		ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();

		const int _Size = 120;

		std::vector<Vertex::Basic> vertices;

		vertices.resize(_Size);

		for (int i = 0; i < _Size / 4; i++)
			vertices[i].Pos = DirectX::XMFLOAT3(5.0f * i, 0, 0);

		for (int i = 0; i < _Size / 4; i++)
			vertices[i + _Size / 4].Pos = DirectX::XMFLOAT3(5.0f * i, 0, 5.0f * (_Size / 4 - 1));

		for (int i = 0; i < _Size / 4; i++)
			vertices[i + _Size / 2].Pos = DirectX::XMFLOAT3(0, 0, 5.0f * i);

		for (int i = 0; i < _Size / 4; i++)
			vertices[i + _Size / 4 * 3].Pos = DirectX::XMFLOAT3(5.0f * (_Size / 4 - 1), 0, 5.0f * i);

		for (int i = 0; i < _Size; i++)
		{
			vertices[i].Pos.x -= 5.0f * (_Size / 8 - 1);

			vertices[i].Pos.z -= 5.0f * (_Size / 8 - 1);
		}

		UINT indices[_Size];

		for (int i = 0; i < _Size / 2; i++, i++)
		{
			indices[i] = i / 2;

			indices[i + 1] = i / 2 + _Size / 4;
		}

		for (int i = 0; i < _Size / 2; i++, i++)
		{
			indices[i + _Size / 2] = i / 2 + _Size / 2;

			indices[i + _Size / 2 + 1] = i / 2 + _Size / 4 * 3;
		}

		std::vector<index3> _Temp;

		_Temp.resize(_Size / 3);

		for (unsigned int i = 0; i < _Temp.size(); i++)
		{
			_Temp[i].m_Index[0] = indices[i * 3];

			_Temp[i].m_Index[1] = indices[i * 3 + 1];

			_Temp[i].m_Index[2] = indices[i * 3 + 2];
		}

		auto mesh = new VBIBMesh();

		mesh->Init(vertices, _Temp);

		return mesh;
	}

	VBIBMesh* Factory::CreateSphereMesh()
	{
		GeometryGenerator::MeshData sphere;

		GeometryGenerator geoGen;

		geoGen.CreateSphere(0.5f, 30, 30, sphere);

		std::vector<Vertex::BasicLight> vertices(sphere.Vertices.size());

		std::vector<index3> indices(sphere.Indices.size() / 3);

		ChangeGeometry(&sphere, vertices, indices);

		VBIBMesh* _Temp = new VBIBMesh();

		_Temp->Init(vertices, indices);

		return _Temp;
	}

	VBIBMesh* Factory::CreateBoxMesh()
	{
		GeometryGenerator::MeshData Box;

		GeometryGenerator geoGen;

		geoGen.CreateBox(1.0f, 1.0f, 1.0f, Box);

		std::vector<Vertex::BasicLight> vertices(Box.Vertices.size());

		std::vector<index3> indices(Box.Indices.size() / 3);

		ChangeGeometry(&Box, vertices, indices);

		VBIBMesh* _Temp = new VBIBMesh();

		_Temp->Init<Vertex::BasicLight>(vertices, indices);

		return _Temp;
	}

	/**
	 * \brief 텍스처 이미지를 만드는 것이 아닌 화면에 출력하기 위한 버텍스 6개를 정의 하는 것.
	 */
	VBIBMesh* Factory::CreateTextureMesh()
	{
		std::vector<Vertex::Basic> vertices(6);

		std::vector<index3> indices(2);

		indices[0].m_Index[0] = 0;
		indices[0].m_Index[1] = 1;
		indices[0].m_Index[2] = 2;
		indices[1].m_Index[0] = 3;
		indices[1].m_Index[1] = 4;
		indices[1].m_Index[2] = 5;

		VBIBMesh* _Temp = new VBIBMesh();

		_Temp->Init<Vertex::Basic>(vertices, indices, D3D11_USAGE_DYNAMIC, D3D11_USAGE_IMMUTABLE, D3D11_CPU_ACCESS_WRITE);

		return _Temp;
	}

	ID3D11ShaderResourceView* Factory::CreateRandomTexture1DSRV()
	{
		auto device = DXEngine::GetInstance()->GetD3dDevice();

		XMFLOAT4 randomValues[1024];
		int count = 0;
		for (int i = 0; i < 1024; ++i)
		{
			randomValues[i].x = MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].y = MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].z = MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].w = MathHelper::RandF(-1.0f, 1.0f);

			if (0.9f <= randomValues[i].x && randomValues[i].x <= 1.0f)
				count++;
		}

		int a = 0;

		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = randomValues;
		initData.SysMemPitch = 1024 * sizeof(XMFLOAT4);
		initData.SysMemSlicePitch = 0;

		// Create the texture.
		D3D11_TEXTURE2D_DESC texDesc;

		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.ArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Width = 1024;
		texDesc.Height = 1;

		ID3D11Texture2D* randomTex = 0;

		HR(device->CreateTexture2D(&texDesc, &initData, &randomTex));

		// Create the resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = texDesc.MipLevels;
		viewDesc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* randomTexSRV;

		HR(device->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV));

		ReleaseCOM(randomTex);

		return randomTexSRV;
	}

	ID3D11ShaderResourceView* Factory::CreatePerlinNoiseTexture(float frequency/*주파수*/, int octaves/*레이어 수*/, float octaveMutiplier, std::uint32_t seed/*randSeed*/, float width, float height)
	{
		auto device = DXEngine::GetInstance()->GetD3dDevice();

		const siv::PerlinNoise perlin0{ seed };
		const siv::PerlinNoise perlin1{ seed + 1 };
		const siv::PerlinNoise perlin2{ seed + 2 };
		const double fx = (frequency / width);
		const double fy = (frequency / height);

		std::vector<XMFLOAT4> colors(width * height);

		for (std::int32_t y = 0; y < height; ++y)
		{
			for (std::int32_t x = 0; x < width; ++x)
			{
				int index = width * y + x;

				colors[index].x = perlin0.octave2D_01((x * fx), (y * fy), octaves, octaveMutiplier);
				colors[index].y = perlin1.octave2D_01((x * fx), (y * fy), octaves, octaveMutiplier);
				colors[index].z = perlin2.octave2D_01((x * fx), (y * fy), octaves, octaveMutiplier);
				colors[index].w = 1.0f;
			}
		}

		D3D11_SUBRESOURCE_DATA initData;

		initData.pSysMem = colors.data();
		initData.SysMemPitch = width * sizeof(XMFLOAT4);
		initData.SysMemSlicePitch = 0;

		// Create the texture.
		D3D11_TEXTURE2D_DESC texDesc;

		ZeroMemory(&texDesc, sizeof(texDesc));

		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_DEFAULT;

		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		texDesc.SampleDesc.Count = 1;

		ID3D11Texture2D* Tex = 0;

		HR(device->CreateTexture2D(&texDesc, &initData, &Tex));

		// Create the resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = texDesc.MipLevels;

		ID3D11ShaderResourceView* TexSRV;

		HR(device->CreateShaderResourceView(Tex, &viewDesc, &TexSRV));

		ReleaseCOM(Tex);

		return TexSRV;
	}

	ParticleMesh* Factory::CreateParticleMesh()
	{
		return new ParticleMesh();
	}

	void Factory::CompileVertexShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		ID3DBlob* errorMessage = nullptr;
		ID3DBlob* vertexShaderBuffer = nullptr;
		ID3D11VertexShader* vertexShader = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11InputLayout* inputLayOut = nullptr;


		if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryName, "vs_5_0", compileFlag, 0, &vertexShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "VS Shader Compile Failed ! PassBase..", nullptr, MB_OK);
		}

		if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "VS Shader Create Failed ! PassBase..", nullptr, MB_OK);
		}


		if (!BuildInputLayout(vertexShaderBuffer, &inputLayOut))
			assert(false);

		vertexShaderBuffer->Release();

		if (errorMessage)
			errorMessage->Release();

		pipeLineDesc._vs = vertexShader;
		pipeLineDesc._il = inputLayOut;
	}

	void Factory::CompilePixelShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		ID3DBlob* pixelShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11PixelShader* pixelShader;

		if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE
			, entryName, "ps_5_0", compileFlag, 0, &pixelShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "PS Shader Compile Failed ! Shader..", nullptr, MB_OK);
		}

		if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);
		}

		pixelShaderBuffer->Release();
		if (errorMessage)
			errorMessage->Release();

		pipeLineDesc._ps = pixelShader;
	}

	void Factory::CompileGeometryShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		bool useStreamOut, std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		ID3DBlob* geometryShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11GeometryShader* geometryShader = nullptr;

		if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE
			, entryName, "gs_5_0", compileFlag, 0, &geometryShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "Geometry Shader Compile Failed !", nullptr, MB_OK);
		}

		if (useStreamOut)
		{

			std::vector<D3D11_SO_DECLARATION_ENTRY> decl;

			std::vector<std::string> sementicNames;

			BuildGeometryShaderAttribute(geometryShaderBuffer, decl, sementicNames);

			for (int i = 0; i < decl.size(); i++)
			{
				decl[i].SemanticName = sementicNames[i].c_str();
			}

			UINT strides[1] = { decl.size() };

			if (_shaderLikingDescs.empty())
			{
				if (FAILED(device->CreateGeometryShaderWithStreamOutput(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), decl.data(),
					decl.size(), strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, nullptr, &geometryShader)))
				{
					if (errorMessage)
						OutputShaderErrorMessage(errorMessage, nullptr, fileName);
					else
						::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
				}
			}
			else
			{
				// ------------------------------------------------------------ TEST -------------------------------------------------------
	// 1. 동적 링킹 배열을 만들어서 클래스 링킹 오브젝트를 만든다.

				ID3D11ClassLinkage* _pGsClassLinkage = nullptr;
				DXEngine::GetInstance()->GetD3dDevice()->CreateClassLinkage(&_pGsClassLinkage);
				// 2. 동적 클래스 링킹을 사용할 쉐이더를 만들 때, 클래스 링킹 오브젝트를 쉐이더 생성 함수의 인자로 넘긴다.
				if (FAILED(device->CreateGeometryShaderWithStreamOutput(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), decl.data(),
					decl.size(), strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, _pGsClassLinkage, &geometryShader)))
				{
					if (errorMessage)
						OutputShaderErrorMessage(errorMessage, nullptr, fileName);
					else
						::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
				}

				// 3. D3DReflect 함수를 사용하여 ID3D11ShaderReflection 오브젝트를 만들어라.
				ID3D11ShaderReflection* pReflector = nullptr;
				D3DReflect(geometryShaderBuffer->GetBufferPointer(),
					geometryShaderBuffer->GetBufferSize(),
					IID_ID3D11ShaderReflection,
					(void**)&pReflector);

				// 4. 셰이더 내의 인터페이스 인스턴스의 숫자를 얻어라.
				pipeLineDesc._numGsInstance = pReflector->GetNumInterfaceSlots();

				// 5. 셰이더의 인터페이스 인스턴스 수를 저장할 수 있을 만큼 충분한 배열을 만든다.
				pipeLineDesc._gsDynamicLinkageArray = (ID3D11ClassInstance**)malloc(sizeof(ID3D11ClassInstance*) * pipeLineDesc._numGsInstance);

				// 6. 각 인터페이스 인스턴스에 대응하는 배열 내의 인덱스를 확인한다. // 인덱스는 하드코딩으로 하자...! 따로 저장하기 귀찮다..!
				// 인덱스는 hlsl 작성 순서가 아니였다...! offset을 저장해야한다...
				//ID3D11ShaderReflectionVariable* tempClass = pReflector->GetVariableByName("g_shapeInstance");
				//auto slotIndex = tempClass->GetInterfaceSlot(0);

				// Pass 에서 처리 중.

				// 7. 셰이더 내의 인터페이스로부터 상속된 각 클래스 오브젝트를 위한 클래스 인스턴스를 얻는다.

				// 내일 응용프로그램에서 생성하는 것이 아닌 상수 버퍼에서 생성해보자..!

				auto func = [&](std::string str, unsigned int index)
				{
					ID3D11ClassInstance* _classInstanceTemp;

					_pGsClassLinkage->CreateClassInstance(str.c_str(), 0, 0, 0, 0, &_classInstanceTemp);
					str += "_GS";
					DXEngine::GetInstance()->GetResourceManager()->AddResource<ShaderClassInstance>(str, new ShaderClassInstance(_classInstanceTemp, index));
				};

				for (auto& iter : _shaderLikingDescs)
				{
					ID3D11ShaderReflectionVariable* tempClass = pReflector->GetVariableByName(iter._interfaceName.c_str());
					func(iter._instanceName, tempClass->GetInterfaceSlot(0));
					func(iter._nullInstanceName, tempClass->GetInterfaceSlot(0));
				}

				// 8. 인터페이스 인스턴스를 클래스 인스턴스로 설정하려면 동적 연결 배열에서 해당 항목을 설정합니다.
				// pipeLineDesc._gsDynamicLinkageArray[testOffset] = _classInstanceTemp;
				// Pass 에서 처리 중.

				// 9. 동적 역결 배열을 SetShader 호출에 매개 변수로 전달 합니다.
				//DXEngine::GetInstance()->Getd3dImmediateContext()->GSSetShader(geometryShaderTemp, _dynmicLinkageArray, numGsInstance);
				// Pass 에서 처리 중.
				// ------------------------------------------------------------ TEST -------------------------------------------------------
				ReleaseCOM(pReflector);
				ReleaseCOM(_pGsClassLinkage);
			}

		}
		else
		{
			if (_shaderLikingDescs.empty())
			{
				if (FAILED(device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &geometryShader)))
				{
					if (errorMessage)
						OutputShaderErrorMessage(errorMessage, nullptr, fileName);
					else
						::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
				}
			}
			else
			{
				ID3D11ClassLinkage* _pGsClassLinkage = nullptr;
				DXEngine::GetInstance()->GetD3dDevice()->CreateClassLinkage(&_pGsClassLinkage);

				if (FAILED(device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(),
					_pGsClassLinkage, &geometryShader)))
				{
					if (errorMessage)
						OutputShaderErrorMessage(errorMessage, nullptr, fileName);
					else
						::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
				}

				ID3D11ShaderReflection* pReflector = nullptr;
				D3DReflect(geometryShaderBuffer->GetBufferPointer(),
					geometryShaderBuffer->GetBufferSize(),
					IID_ID3D11ShaderReflection,
					(void**)&pReflector);

				pipeLineDesc._numGsInstance = pReflector->GetNumInterfaceSlots();

				pipeLineDesc._gsDynamicLinkageArray = (ID3D11ClassInstance**)malloc(sizeof(ID3D11ClassInstance*) * pipeLineDesc._numGsInstance);

				auto func = [&](std::string str, unsigned int index)
				{
					ID3D11ClassInstance* _classInstanceTemp;

					_pGsClassLinkage->CreateClassInstance(str.c_str(), 0, 0, 0, 0, &_classInstanceTemp);
					str += "_GS";
					DXEngine::GetInstance()->GetResourceManager()->AddResource<ShaderClassInstance>(str, new ShaderClassInstance(_classInstanceTemp, index));
				};

				for (auto& iter : _shaderLikingDescs)
				{
					ID3D11ShaderReflectionVariable* tempClass = pReflector->GetVariableByName(iter._interfaceName.c_str());
					func(iter._instanceName, tempClass->GetInterfaceSlot(0));
					func(iter._nullInstanceName, tempClass->GetInterfaceSlot(0));
				}

				ReleaseCOM(pReflector);
				ReleaseCOM(_pGsClassLinkage);

			}




		}

		pipeLineDesc._gs = geometryShader;

	}

	void Factory::CompileComputeShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName,
		const CHAR* entryName, std::vector<D3D_SHADER_MACRO> macro, std::vector<ShaderLikingDesc> _shaderLikingDescs)
	{
		ID3DBlob* computeShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11ComputeShader* computeShader;

		if (FAILED(::D3DCompileFromFile(fileName, macro.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE
			, entryName, "cs_5_0", compileFlag, 0, &computeShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "CS Shader Compile Failed ! Shader..", nullptr, MB_OK);
		}

		if (_shaderLikingDescs.empty())
		{
			if (FAILED(device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(),
				nullptr, &computeShader)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "CS Shader Create Failed ! Shader..", nullptr, MB_OK);
			}
		}
		else
		{
			ID3D11ShaderReflection* pReflector = nullptr;
			D3DReflect(computeShaderBuffer->GetBufferPointer(),
				computeShaderBuffer->GetBufferSize(),
				IID_ID3D11ShaderReflection,
				(void**)&pReflector);

			ID3D11ClassLinkage* _pCsClassLinkage = nullptr;
			DXEngine::GetInstance()->GetD3dDevice()->CreateClassLinkage(&_pCsClassLinkage);

			if (FAILED(device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(),
				_pCsClassLinkage, &computeShader)))
			{
				if (errorMessage)
					OutputShaderErrorMessage(errorMessage, nullptr, fileName);
				else
					::MessageBoxA(nullptr, "CS Shader Create Failed ! Shader..", nullptr, MB_OK);
			}

			pipeLineDesc._numCsInstance = pReflector->GetNumInterfaceSlots();

			pipeLineDesc._csDynamicLinkageArray = (ID3D11ClassInstance**)malloc(sizeof(ID3D11ClassInstance*) * pipeLineDesc._numCsInstance);

			auto func = [&](std::string str, unsigned int index)
			{
				ID3D11ClassInstance* _classInstanceTemp;
				_pCsClassLinkage->CreateClassInstance(str.c_str(), 0, 0, 0, 0, &_classInstanceTemp);
				str += "_CS";
				DXEngine::GetInstance()->GetResourceManager()->AddResource<ShaderClassInstance>(str, new ShaderClassInstance(_classInstanceTemp, index));
			};

			for (auto& iter : _shaderLikingDescs)
			{
				ID3D11ShaderReflectionVariable* tempClass = pReflector->GetVariableByName(iter._interfaceName.c_str());
				func(iter._instanceName, tempClass->GetInterfaceSlot(0));
				func(iter._nullInstanceName, tempClass->GetInterfaceSlot(0));
			}

			ReleaseCOM(pReflector);
			ReleaseCOM(_pCsClassLinkage);
		}



		computeShaderBuffer->Release();
		if (errorMessage)
			errorMessage->Release();

		pipeLineDesc._cs = computeShader;
	}

	void Factory::ChangeGeometry(GeometryGenerator::MeshData* _MeshData, std::vector<Vertex::BasicLight>& _vertices,
		std::vector<index3>& _Indices)
	{
		for (int i = 0; i < _MeshData->Vertices.size(); i++)
		{
			_vertices[i].Pos = _MeshData->Vertices[i].Position;
			_vertices[i].Nomal = _MeshData->Vertices[i].Normal;
			_vertices[i].Tangent = _MeshData->Vertices[i].TangentU;
			_vertices[i].Texture = _MeshData->Vertices[i].TexC;
		}

		for (int i = 0; i < _Indices.size(); i++)
		{
			_Indices[i].m_Index[0] = _MeshData->Indices[i * 3];
			_Indices[i].m_Index[1] = _MeshData->Indices[i * 3 + 1];
			_Indices[i].m_Index[2] = _MeshData->Indices[i * 3 + 2];
		}

	}

	void Factory::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName)
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

	bool Factory::BuildInputLayout(ID3DBlob* shaderBlob,
		ID3D11InputLayout** ppinputLayOut)
	{

		ID3D11ShaderReflection* pReflector = nullptr;

		D3DReflect(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			IID_ID3D11ShaderReflection,
			(void**)&pReflector);


		std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc;

		D3D11_SHADER_DESC ShaderDesc;
		pReflector->GetDesc(&ShaderDesc);

		int byteOffset = 0;


		for (unsigned InputIndex = 0; InputIndex < ShaderDesc.InputParameters; InputIndex++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetInputParameterDesc(InputIndex, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc;

			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex; //동일한 시멘틱스를 사용하는 매개변수 구분
			elementDesc.InputSlot = 0; //d3d11에 정의된 0~15값 input assembler
			elementDesc.AlignedByteOffset = byteOffset; //편의를 위해 이걸로, 
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			//Mask which indicates which components of a register are used.

			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;

				byteOffset += 4;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;

				byteOffset += 8;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

				byteOffset += 12;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

				byteOffset += 16;
			}

			InputLayoutDesc.push_back(elementDesc);

#ifdef DEBUG
			std::string str = elementDesc.SemanticName;
			str += "\n";
			ATLTRACE(str.c_str());
#endif
		}

		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> temp;

		for (unsigned InputIndex = 0; InputIndex < ShaderDesc.OutputParameters; InputIndex++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetOutputParameterDesc(InputIndex, &paramDesc);
			temp.emplace_back(paramDesc);
		}



		auto device = DXEngine::GetInstance()->GetD3dDevice();
		//_shaderblob을 통해 유효성 검사를 한다
		HR(device->CreateInputLayout(&InputLayoutDesc.front(), static_cast<UINT>(InputLayoutDesc.size()), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), ppinputLayOut));

		ReleaseCOM(pReflector);

		return true;
	}

	bool Factory::BuildGeometryShaderAttribute(ID3DBlob* shaderBlob,
		std::vector<D3D11_SO_DECLARATION_ENTRY>& outputElements, std::vector<std::string>& sementicNames)
	{
		ID3D11ShaderReflection* pReflector = nullptr;

		D3DReflect(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			IID_ID3D11ShaderReflection,
			(void**)&pReflector);

		D3D11_SHADER_DESC shaderDesc;
		pReflector->GetDesc(&shaderDesc);

		outputElements.resize(shaderDesc.OutputParameters);
		sementicNames.resize(shaderDesc.OutputParameters);

		//레지스터의 총사이즈를 어떻게 알 수 있을까?
		int registerInfo[256] = { 0, };

		//이전 bit + 1
		for (unsigned InputIndex = 0; InputIndex < shaderDesc.OutputParameters; InputIndex++)
		{

			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetOutputParameterDesc(InputIndex, &paramDesc);

			D3D11_SO_DECLARATION_ENTRY& elementDesc = outputElements[InputIndex];

			elementDesc.Stream = 0;

			sementicNames[InputIndex] = paramDesc.SemanticName;

			elementDesc.SemanticIndex = paramDesc.SemanticIndex;

			elementDesc.StartComponent = 0;

			if (paramDesc.Mask & (1 << 0))
			{
				elementDesc.ComponentCount++;
			}
			if (paramDesc.Mask & (1 << 1))
			{
				elementDesc.ComponentCount++;
			}
			if (paramDesc.Mask & (1 << 2))
			{
				elementDesc.ComponentCount++;
			}
			if (paramDesc.Mask & (1 << 3))
			{
				elementDesc.ComponentCount++;
			}

			registerInfo[paramDesc.Register] += elementDesc.ComponentCount;

			elementDesc.OutputSlot = 0;

#ifdef DEBUG
			std::string str = paramDesc.SemanticName;
			str += "\n";
			ATLTRACE(str.c_str());
#endif


		}
		ReleaseCOM(pReflector);

		return false;

	}

	void Factory::Bake_BRDF_PreFilter_Irradiance(std::string path, ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv)
	{
		auto resourceManager = DXEngine::GetInstance()->GetResourceManager();

		D3D11_VIEWPORT viwport; // 뷰포트

		viwport.TopLeftX = 0;
		viwport.TopLeftY = 0;
		viwport.Width = static_cast<float>(512);
		viwport.Height = static_cast<float>(512);
		viwport.MinDepth = 0;
		viwport.MaxDepth = 1.0f;

		auto context = DXEngine::GetInstance()->Getd3dImmediateContext();

		context->RSSetViewports(1, &viwport);

		resourceManager->AddResource("BRDFLookUpTable", Bake_BRDFLookUpTable(512, 512));

		resourceManager->AddResource("SkyBoxIrradianceMap", Bake_IrradianceMap(texture2DBuffer, srv, 512, 512));

		resourceManager->AddResource("SkyBoxPreFilterMap", Bake_PreFilterMap(texture2DBuffer, srv, 512, 512, 4));

		RenderTarget::SetViewPort();
	}

	RenderTexture* Factory::Bake_BRDFLookUpTable(unsigned width, unsigned height)
	{
		RenderTexture* renderTexture = new RenderTexture();

		D3D11_TEXTURE2D_DESC  textureDesc;

		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		renderTexture->CreateTexture2D(&textureDesc);
		renderTexture->CreateSRV(&srvDesc);
		renderTexture->CreateRTV(&rtvDesc);

		auto resourceManager = DXEngine::GetInstance()->GetResourceManager();

		auto pass = resourceManager->GetResource<Pass_Texture>("BRDFLookUpTable_Bake_Pass");

		RenderTarget::SetRenderTargetView(nullptr, 1, renderTexture->GetRTV());

		std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderingData;

		pass->Draw(renderingData);

		return renderTexture;
	}

	RenderTexture* Factory::Bake_PreFilterMap(ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv, unsigned width, unsigned height, unsigned mipSize)
	{
		RenderTexture* renderTexture = new RenderTexture();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 6;
		textureDesc.MipLevels = mipSize;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 1;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 4;

		renderTexture->CreateTexture2D(&textureDesc);
		renderTexture->CreateSRV(&srvDesc);

		auto resourceManager = DXEngine::GetInstance()->GetResourceManager();

		CubeMapToRadiance_Bake_Pass* pass = reinterpret_cast<CubeMapToRadiance_Bake_Pass*>(resourceManager->GetResource<Pass_Texture>("CubeMapToRadiance_Bake_Pass"));

		ID3D11RenderTargetView** radianceRenderTarget = new ID3D11RenderTargetView * [6 * mipSize];

		for (int j = 0; j < mipSize; ++j)
		{
			for (int i = 0; i < 6; ++i)
			{
				rtvDesc.Texture2DArray.MipSlice = j;
				rtvDesc.Texture2DArray.FirstArraySlice = i;
				int arrayIdx = 6 * j + i;
				DXEngine::GetInstance()->GetD3dDevice()->CreateRenderTargetView(
					renderTexture->GetRenderTargetTexture(), &rtvDesc, &radianceRenderTarget[arrayIdx]);
			}
		}

		for (UINT mipIdx = 0; mipIdx < mipSize; mipIdx++)
		{
			float texWidth = static_cast<float>(width * pow(0.5f, mipIdx));
			float texHeight = static_cast<float>(height * pow(0.5f, mipIdx));

			D3D11_VIEWPORT viwport;

			viwport.TopLeftX = 0;
			viwport.TopLeftY = 0;
			viwport.Width = texWidth;
			viwport.Height = texHeight;
			viwport.MinDepth = 0;
			viwport.MaxDepth = 1.0f;

			auto context = DXEngine::GetInstance()->Getd3dImmediateContext();

			context->RSSetViewports(1, &viwport);

			for (int idx = 0; idx < 6; idx++)
			{
				RenderTarget::SetRenderTargetView(nullptr, 1, radianceRenderTarget[6 * mipIdx + idx]);

				std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderingData;

				renderingData.push_back({ srv,0 });

				pass->SetResource(idx, static_cast<float>(mipIdx) / static_cast<float>(mipSize));

				pass->Draw(renderingData);
			}
		}

		for (int j = 0; j < mipSize; ++j)
		{
			for (int i = 0; i < 6; ++i)
			{
				int arrayIdx = 6 * j + i;
				ReleaseCOM(radianceRenderTarget[arrayIdx]);
			}
		}

		RenderTarget::SetViewPort();

		delete[] radianceRenderTarget;

		return renderTexture;
	}

	RenderTexture* Factory::Bake_IrradianceMap(ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv, unsigned width, unsigned height)
	{
		RenderTexture* renderTexture = new RenderTexture();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.ArraySize = 6;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 1;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		renderTexture->CreateTexture2D(&textureDesc);
		renderTexture->CreateSRV(&srvDesc);

		auto resourceManager = DXEngine::GetInstance()->GetResourceManager();

		CubeMapToIrradiance_Bake_Pass* pass = reinterpret_cast<CubeMapToIrradiance_Bake_Pass*>(resourceManager->GetResource<Pass_Texture>("CubeMapToIrradiance_Bake_Pass"));

		ID3D11RenderTargetView* cubeIrradianceRenderTarget[6];

		for (int i = 0; i < 6; ++i)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;

			DXEngine::GetInstance()->GetD3dDevice()->CreateRenderTargetView(renderTexture->GetRenderTargetTexture(), &rtvDesc, &cubeIrradianceRenderTarget[i]);

			std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderingData;

			renderingData.push_back({ srv,0 });

			pass->SetIndex(i);

			RenderTarget::SetRenderTargetView(nullptr, 1, cubeIrradianceRenderTarget[i]);

			pass->Draw(renderingData);
		}

		for (int i = 0; i < 6; ++i)
		{
			ReleaseCOM(cubeIrradianceRenderTarget[i]);
		}

		return renderTexture;
	}
}
