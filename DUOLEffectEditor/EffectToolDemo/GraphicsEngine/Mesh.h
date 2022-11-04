#pragma once

/// <summary>
/// 정보, 변수를 저장하는 용도.
/// 애니메이션도 저장할 듯
/// 거의 메타정보 저장용.
/// </summary>
/// 게임 오브젝트 처럼 컴포넌트 기반으로 만들자.
/// 애니메이션 + 스키닝 + 노말 + 텍스쳐 + ... 원하는 걸 조합해서 사용할 수 있도록..
/// 
/// 일단 화면에 오브젝트를 띄우고 수정할까?
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
	string m_Path; // 텍스쳐 주소
	MeshData* m_Parent;
	Material* m_Mtl; // 머터리얼

	XMMATRIX m_WorldTM;
	XMMATRIX m_LocalMT;
	XMFLOAT3 m_MinPos;
	XMFLOAT3 m_MaxPos;

	vector<AnimationKey> m_AniKeys;

	vector<MeshData*> m_Childrens;
	vector<Vertex> m_vertices; // 버텍스
	vector<index3> m_indices; // 인덱스

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
