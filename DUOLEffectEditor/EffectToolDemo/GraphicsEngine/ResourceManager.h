#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// �̸� ����Ʈ�� �޽��� �����ϰ� ���ܾ���?
/// </summary>

class Effects;
class Factory;
class ResourceManager
{

public:
	ResourceManager();

	~ResourceManager();

	void init();

private:

	unordered_map<string, ID3D11ShaderResourceView*> _textureMapIDs;

	unordered_map<string, VBIBMesh*> _VBIBMesh_IDs;

	Effects* m_Effects;

	Factory* m_Factory;

private:
	void LoadTexture(string name, string path);

	void LoadASE(string name, string path);

public:
	Factory* GetFactory() const { return m_Factory; }

};

