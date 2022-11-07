#pragma once
#include <map>
#include <unordered_map>

/// <summary>
/// �̸� ����Ʈ�� �޽��� �����ϰ� ���ܾ���?
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

	unordered_map<tstring, unsigned int> _VBIBMesh_IDMaps; // Ž���� �ӵ��� string �� int�� ���̰� �ֱ� ������ �ӵ� ����� ���� ������ �ؼ� ����Ѵ�.
	// �� ���� ������ ���� �������� ������ �ִ°� ������ �ϴ�..
	// ���߿� ��ġ ��������..!!

	unordered_map<tstring, IPass*> _shaderIDs; // ���̴��� ������ mesh ��ŭ ���� �� ���� ������ ������ �� �ʿ�� ������ �ϴ�.. ��Ʈ������ ��������..
	// � fx���ϰ� �׸��� � tech�� ������� �̸��� ���� �����Ѵ�.

	Effects* m_Effects;

	Factory* m_Factory;

public:

	void InsertVBIBMesh(tstring name, VBIBMesh* mesh);

	VBIBMesh* GetVBIBMesh(unsigned int meshID);

	IPass* GetShader(tstring name);

	Factory* GetFactory() const { return m_Factory; }

};

