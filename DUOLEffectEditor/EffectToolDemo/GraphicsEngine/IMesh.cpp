#include "pch.h"
#include "IMesh.h"

IMesh::IMesh() :m_Parent(nullptr), m_MaxPos(), m_MinPos(), m_WorldTM()
{
	m_NodeWorldTM = XMMatrixIdentity();
	m_NodeLocalTM = XMMatrixIdentity();
}

IMesh::~IMesh()
{
	for (auto& iter : m_Childrens)
	{
		delete iter;
	}
	m_Childrens.clear();

	m_BoneNameList.clear();
	m_FinalBoneList.clear();
}



void IMesh::SetParent(IMesh* _Parent)
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

void IMesh::SetName(string _Name)
{
	m_Name = _Name;
}

IMesh* IMesh::FindChildren(string _Name)
{
	if (m_Name == _Name)
		return this;

	for (auto iter : m_Childrens)
	{
		if (iter->m_Name == _Name)
			return iter;
		else
		{
			IMesh* Temp = iter->FindChildren(_Name);
			if (Temp)
				return Temp;
		}
	}

	return nullptr;
}

std::vector<IMesh*>& IMesh::GetChilrens()
{
	return m_Childrens;
}