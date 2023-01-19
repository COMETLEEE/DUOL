#pragma once
#include <string>
#include <vector>

#include "Core/DirectX11/geometrygenerator.h"



/// <summary>
/// 메쉬를 대신 생성할 클래스.
/// 생성자를 사용하지 않고 팩토리가 생성할 것.
/// 파서에서 읽은 데이터를 잘 정리해서 알아서 생성해주세용~
/// </summary>

class ID3D11ShaderResourceView;

class ID3D11Buffer;

namespace MuscleGrapics
{
	namespace Vertex
	{
		struct BasicLight;
	}
	struct index3;

	class GeometryGenerator::MeshData;

	class VBIBMesh;

	class Factory
	{

	public:
		Factory();

		~Factory() = default;

	public:
		ID3D11ShaderResourceView* LoadTexture(std::string path);

		std::pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(std::string path);

		VBIBMesh* CreateGridMesh();

		VBIBMesh* CreateSphereMesh();

		VBIBMesh* CreateBoxMesh();

		VBIBMesh* CreateTextureMesh();

		ID3D11ShaderResourceView* CreateRandomTexture1DSRV();

		ID3D11ShaderResourceView* CreatePerlinNoiseTexture(float frequency/*주파수*/, int octaves/*레이어 수*/, std::uint32_t seed/*randSeed*/, float width, float height);
	private:
		void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, std::vector<Vertex::BasicLight>& _vertices, std::vector<index3>& _Indices);

	};
}
