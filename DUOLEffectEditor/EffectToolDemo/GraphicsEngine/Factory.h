#pragma once
/// <summary>
/// 메쉬를 대신 생성할 클래스.
/// 생성자를 사용하지 않고 팩토리가 생성할 것.
/// 파서에서 읽은 데이터를 잘 정리해서 알아서 생성해주세용~
/// </summary>
namespace MuscleGrapics
{
	class Factory
	{

	public:
		Factory();

		~Factory() = default;

	public:
		ID3D11ShaderResourceView* LoadTexture(string path);

		pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(string path);

		VBIBMesh* CreateGridMesh();

		VBIBMesh* CreateSphereMesh();

		VBIBMesh* CreateBoxMesh();

		VBIBMesh* CreateTextureMesh();

		ID3D11ShaderResourceView* CreateRandomTexture1DSRV();
	private:
		void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, vector<Vertex::BasicLight>& _vertices, vector<index3>& _Indices);

	};
}
