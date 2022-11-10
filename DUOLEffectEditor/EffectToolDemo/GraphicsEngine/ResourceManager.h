#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// �̸� ����Ʈ�� �޽��� �����ϰ� ���ܾ���?
/// </summary>

class Factory;

class ResourceManager
{
public:
	ResourceManager();

	~ResourceManager();

public:
	void init();

	unsigned int InsertVBIBMesh(tstring name, VBIBMesh* mesh);

	VBIBMesh* GetVBIBMesh(unsigned int meshID);

	unsigned int GetVBIBMesh(tstring meshName);

	unsigned int InsertTexture(tstring name, tstring path);
	unsigned int InsertTexture(tstring name, std::vector<tstring>& path);

	ID3D11ShaderResourceView* GetTexture(unsigned int textureID);

	unsigned int GetTexture(tstring textureMap);

	unsigned int InsertParticleMesh( ParticleMesh* mesh);

	ParticleMesh* GetParticleMesh(unsigned int meshID);

	unsigned int GetParticleMesh(tstring meshName);

	PassBase<RenderingData_3D>* Get3DShader(tstring name);

	PassBase<RenderingData_Particle>* GetParticleShader(tstring name);

private:
	unordered_map<unsigned int, VBIBMesh*> _VBIBMesh_IDs;

	unordered_map<tstring, unsigned int> _VBIBMesh_ID_Maps; // Ž���� �ӵ��� string �� int�� ���̰� �ֱ� ������ �ӵ� ����� ���� ������ �ؼ� ����Ѵ�.
	// ���߿� ��ġ ��������..!!

	unordered_map<unsigned int, ID3D11ShaderResourceView*> _textureMapIDs;

	unordered_map<tstring, unsigned int> _textureMapID_Maps;

	unordered_map<unsigned int, ParticleMesh*> _particleMapIDs;

	unordered_map<tstring, PassBase<RenderingData_3D>*> _3DShaderIDs; // ���̴��� ������ mesh ��ŭ ���� �� ���� ������ ������ �� �ʿ�� ������ �ϴ�.. ��Ʈ������ ��������..
	// � fx���ϰ� �׸��� � tech�� ������� �̸��� ���� �����Ѵ�.

	unordered_map<tstring, PassBase<RenderingData_Particle>*> _particleShaderIDs;


	Factory* _factory;

	unsigned int _textureId;
	unsigned int _meshId;
	unsigned int _particleId;
};

