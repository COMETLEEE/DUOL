#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// 미리 이펙트와 메쉬를 생성하고 땡겨쓸듯?
/// </summary>
class CASEParser;
class Mesh;
class Effects;
class Factory;
class ResourceManager
{

public:
	ResourceManager();
	~ResourceManager();
public:
	CASEParser* m_ASEParser;
	void init();
private:
	unordered_map<string, IMesh*> m_Mesh;

	unordered_map<string, ID3D11ShaderResourceView*> _textureMapIDs;

	unordered_map<string, VBIBMesh*> _VBIBMesh_IDs;

	Effects* m_Effects;
	Factory* m_Factory;
private:
	void LoadTexture(string name, string path);
	void LoadASE(string name, string path);
public:
	Factory* GetFactory() const { return m_Factory; }
	unordered_map<string, IMesh*>& GetMeshs() { return m_Mesh; }
};

