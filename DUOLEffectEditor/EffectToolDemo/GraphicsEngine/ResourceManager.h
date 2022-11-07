#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// 미리 이펙트와 메쉬를 생성하고 땡겨쓸듯?
/// </summary>

class Effects;
class Factory;
class IPass;

class ResourceManager
{

public:
	ResourceManager();

	~ResourceManager();

	void init();

private:

	unordered_map<tstring, ID3D11ShaderResourceView*> _textureMapIDs;

	unordered_map<unsigned int, VBIBMesh*> _VBIBMesh_IDs;

	unordered_map<tstring, unsigned int> _VBIBMesh_IDMaps; // 탐색의 속도가 string 과 int의 차이가 있기 때문에 속도 향상을 위해 매핑을 해서 사용한다.
	// 이 매핑 정보는 게임 엔진에서 가지고 있는게 좋을듯 하다..
	// 나중에 위치 수정하자..!!

	unordered_map<tstring, IPass*> _shaderIDs; // 쉐이더는 종류가 mesh 만큼 많을 것 같지 않으니 매핑을 할 필요는 없을듯 하다.. 스트링으로 저장하자..
	// 어떤 fx파일과 그리고 어떤 tech를 사용할지 이름에 따라 저장한다.

	Effects* m_Effects;

	Factory* m_Factory;

public:

	void InsertVBIBMesh(tstring name, VBIBMesh* mesh);

	VBIBMesh* GetVBIBMesh(unsigned int meshID);

	IPass* GetShader(tstring name);

	Factory* GetFactory() const { return m_Factory; }

};

