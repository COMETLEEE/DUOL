#pragma once
#include <string>
#include <vector>

#include "Core/DirectX11/geometrygenerator.h"



/// <summary>
/// �޽��� ��� ������ Ŭ����.
/// �����ڸ� ������� �ʰ� ���丮�� ������ ��.
/// �ļ����� ���� �����͸� �� �����ؼ� �˾Ƽ� �������ּ���~
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

		ID3D11ShaderResourceView* CreatePerlinNoiseTexture(float frequency/*���ļ�*/, int octaves/*���̾� ��*/, std::uint32_t seed/*randSeed*/, float width, float height);
	private:
		void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, std::vector<Vertex::BasicLight>& _vertices, std::vector<index3>& _Indices);

	};
}
