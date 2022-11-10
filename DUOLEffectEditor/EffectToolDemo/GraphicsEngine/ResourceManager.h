#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// 미리 이펙트와 메쉬를 생성하고 땡겨쓸듯?
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

	unordered_map<tstring, unsigned int> _VBIBMesh_ID_Maps; // 탐색의 속도가 string 과 int의 차이가 있기 때문에 속도 향상을 위해 매핑을 해서 사용한다.
	// 나중에 위치 수정하자..!!

	unordered_map<unsigned int, ID3D11ShaderResourceView*> _textureMapIDs;

	unordered_map<tstring, unsigned int> _textureMapID_Maps;

	unordered_map<unsigned int, ParticleMesh*> _particleMapIDs;

	unordered_map<tstring, PassBase<RenderingData_3D>*> _3DShaderIDs; // 쉐이더는 종류가 mesh 만큼 많을 것 같지 않으니 매핑을 할 필요는 없을듯 하다.. 스트링으로 저장하자..
	// 어떤 fx파일과 그리고 어떤 tech를 사용할지 이름에 따라 저장한다.

	unordered_map<tstring, PassBase<RenderingData_Particle>*> _particleShaderIDs;


	Factory* _factory;

	unsigned int _textureId;
	unsigned int _meshId;
	unsigned int _particleId;
};

