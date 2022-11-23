#pragma once
/// <summary>
/// �޽��� ��� ������ Ŭ����.
/// �����ڸ� ������� �ʰ� ���丮�� ������ ��.
/// �ļ����� ���� �����͸� �� �����ؼ� �˾Ƽ� �������ּ���~
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
