#pragma once
#include "AniKey.h"
class IMesh
{
public:
	IMesh();
	virtual ~IMesh();
public:
	GRPHICHS_API virtual void DrawMesh(XMMATRIX _WorldTM) abstract;
protected:
	IMesh* m_Parent;
	vector<IMesh*> m_Childrens;
	string m_Name;
	XMMATRIX m_WorldTM;

	XMMATRIX m_NodeWorldTM;
	XMMATRIX m_NodeLocalTM;
	XMFLOAT3 m_MinPos;
	XMFLOAT3 m_MaxPos;
	// 스키닝 애니메이션을 위한 것.
	vector<string> m_BoneNameList; // 본 탐색을 위한 것.
	vector<XMMATRIX>  m_FinalBoneList; // 쉐이더에 넘기기 위한 것.

	vector<AnimationKey> m_AniKeys; // 애니메이션을 위한것.. Mesh인데 그냥 니가 가져라~


public:
	GRPHICHS_API string GetName() { return m_Name; }
	GRPHICHS_API IMesh* GetParent() { return m_Parent; }
	GRPHICHS_API vector<IMesh*>& GetChilrens();
	GRPHICHS_API XMMATRIX& GetWorldTM() { return m_WorldTM; }

	GRPHICHS_API XMMATRIX& GetNodeWorldTM() { return m_NodeWorldTM; }
	GRPHICHS_API XMMATRIX& GetNodeLocalTM() { return m_NodeLocalTM; }
	GRPHICHS_API XMFLOAT3& GetMinPos() { return m_MinPos; }
	GRPHICHS_API XMFLOAT3& GetMaxPos() { return m_MaxPos; }
	GRPHICHS_API vector<string>& GetBoneNameList() { return m_BoneNameList; }
	GRPHICHS_API vector<XMMATRIX>& GetFinalBoneList() { return m_FinalBoneList; }
	GRPHICHS_API vector<AnimationKey>& GetAnimationKey() { return m_AniKeys; }

	GRPHICHS_API void SetParent(IMesh* _Parent);
	GRPHICHS_API void SetName(string _Name);
	GRPHICHS_API virtual void SetColor(XMFLOAT4 _Color) abstract;
	GRPHICHS_API virtual void WakeMesh() abstract;
	GRPHICHS_API virtual void SleepMesh() abstract;
	GRPHICHS_API IMesh* FindChildren(string _Name);

};


