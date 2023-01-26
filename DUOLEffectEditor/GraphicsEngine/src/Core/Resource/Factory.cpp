#include "Core/Resource/Factory.h"
#include "Core/Resource/VBIBMesh.h"
#include "util/VertexDesc.h"
#include "util/TypeDefine.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/Resource/ParticleMesh.h"
#include "Core/DirectX11/geometrygenerator.h"
#include "Core/Resource/PerlinNoise.hpp"

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
		D3D11_TEXTURE1D_DESC texDesc;

		texDesc.Width = 1024;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;

		ID3D11Texture1D* randomTex = 0;

		HR(device->CreateTexture1D(&texDesc, &initData, &randomTex));

		// Create the resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
		viewDesc.Texture1D.MostDetailedMip = 0;

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
}
