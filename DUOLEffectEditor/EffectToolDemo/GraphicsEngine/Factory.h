#pragma once
/// <summary>
/// �޽��� ��� ������ Ŭ����.
/// �����ڸ� ������� �ʰ� ���丮�� ������ ��.
/// �ļ����� ���� �����͸� �� �����ؼ� �˾Ƽ� �������ּ���~
/// </summary>
class Factory
{
public:
	Factory();
	~Factory();

public:
	Mesh* CreateMesh(BulidInfo& _BulidInfo);
	IMesh* CreateGizmoMesh(GizmoInfo& _Info);

	void CreateSpecialMeshs();

	ID3D11ShaderResourceView* LoadTexture(string path);
	
	pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(string path);
private:
	Bulider* m_Bulider;
};

