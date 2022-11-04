#pragma once

/// <summary>
/// ����, ������ �����ϴ� �뵵.
/// �ִϸ��̼ǵ� ������ ��
/// ���� ��Ÿ���� �����.
/// </summary>
/// ���� ������Ʈ ó�� ������Ʈ ������� ������.
/// �ִϸ��̼� + ��Ű�� + �븻 + �ؽ��� + ... ���ϴ� �� �����ؼ� ����� �� �ֵ���..
/// 
/// �ϴ� ȭ�鿡 ������Ʈ�� ���� �����ұ�?
#include "d3dx11effect.h"
#include "IMesh.h"
class Effect;
class Shader;
class WireShader;

__declspec(align(16)) class Mesh : public IMesh
{
public:
	Mesh();
	virtual ~Mesh();
public:
	void SetConstants();
private:
	int Count = 0;
	ID3D11RasterizerState* m_pNowRenderstate;
	Material* m_Mtl;

	vector<IMeshComponent*> m_MeshComponents;

public:
	void SetRasterizerState(ID3D11RasterizerState* _State);
	void SetMaterial(Material* _Mtl);
	virtual void DrawMesh(XMMATRIX _WorldTM) override;
	virtual void SetColor(XMFLOAT4 _Color) override;
	virtual void WakeMesh() override;
	virtual void SleepMesh() override;

	friend class Bulider;
public:
	template<typename T>
	T* AddMeshComponent();
	template<typename T>
	T* GetMeshComponent();

public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

















template<typename T>
T* Mesh::AddMeshComponent()
{
	T* _NewCom = new T(this);
	m_MeshComponents.emplace_back(_NewCom);
	return _NewCom;
}

template<typename T>
inline T* Mesh::GetMeshComponent()
{
	for (auto& iter : m_MeshComponents)
	{
		T* _Temp = dynamic_cast<T*>(iter);
		if (_Temp)
			return _Temp;
	}

	return NULL;
}


__declspec(align(16)) struct MeshData
{
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}

	MeshData()
	{
		m_Name = "";
		m_Path = "";
		m_Parent = nullptr;
		m_Mtl = nullptr;
		m_WorldTM = XMMatrixIdentity();
		m_LocalMT = XMMatrixIdentity();

	}
	~MeshData()
	{
		for (auto& iter : m_Childrens)
		{
			delete iter;
		}
	}
	string m_Name;
	string m_Path; // �ؽ��� �ּ�
	MeshData* m_Parent;
	Material* m_Mtl; // ���͸���

	XMMATRIX m_WorldTM;
	XMMATRIX m_LocalMT;
	XMFLOAT3 m_MinPos;
	XMFLOAT3 m_MaxPos;

	vector<AnimationKey> m_AniKeys;

	vector<MeshData*> m_Childrens;
	vector<Vertex> m_vertices; // ���ؽ�
	vector<index3> m_indices; // �ε���

	vector<string> m_BoneNameList;

	void SetParent(MeshData* _Parent)
	{
		if (_Parent)
		{
			m_Parent = _Parent;
			m_Parent->m_Childrens.push_back(this);
		}
		else
		{
			m_Parent = nullptr;
			m_Childrens.push_back(this);
		}
	}


	MeshData* FindChildren(string _Name)
	{
		if (m_Name == _Name)
			return this;

		for (auto iter : m_Childrens)
		{
			if (iter->m_Name == _Name)
				return iter;
			else
			{
				MeshData* Temp = iter->FindChildren(_Name);
				if (Temp)
					return Temp;
			}
		}

		return nullptr;
	}
};
