#pragma once

#define HR(x) hr = x;\
if(FAILED(hr)) return 


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }



#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>
#include <atltrace.h> // 디버그용 .
#include <queue>
#include "LightHelper.h"
#include "MathHelper.h"

using namespace std;
using namespace DirectX;

struct Vertex
{
	Vertex(XMFLOAT3 _pos, XMFLOAT2 _Tex, XMFLOAT3 _Nomal, XMFLOAT4 _Color, XMFLOAT3 _Weights) : 
	Tangent(),m_IndexIndex(),m_vertexIndex(),m_vertexLastIndex()
	{
		Pos = _pos;
		Texture = _Tex;
		Nomal = _Nomal;
		Color = _Color;
		Weights = _Weights;
		BoneIndex[0] = 0;
		BoneIndex[1] = 0;
		BoneIndex[2] = 0;
		BoneIndex[3] = 0;
	}
	Vertex() :Pos(), Texture(), Nomal(), Color(0.0f, 0.0f, 0.0f, 1.0f), Weights(), Tangent(),
		 m_IndexIndex(), m_vertexIndex(), m_vertexLastIndex()
	{
		for (int i = 0; i < 4; i++)
		{
			BoneIndex[i] = 0;
		}
	}
	XMFLOAT3 Pos;
	XMFLOAT2 Texture;
	XMFLOAT3 Nomal;
	XMFLOAT4 Color;
	XMFLOAT3 Weights;
	int BoneIndex[4];
	XMFLOAT3 Tangent;



	int m_IndexIndex; //버텍스 쪼개고 합칠 때 쓰는 인덱스.. fx에는 안넘긴다!.
	int m_vertexLastIndex; //버텍스 쪼개고 합칠 때 쓰는 인덱스.. fx에는 안넘긴다!.
	int m_vertexIndex; //버텍스 쪼개고 합칠 때 쓰는 인덱스.. fx에는 안넘긴다!.

	bool _isDelete = false;
};

struct index3
{
	index3()
	{
		m_Index[0] = 0;
		m_Index[1] = 0;
		m_Index[2] = 0;
	}
	index3(int x, int y, int z)
	{
		m_Index[0] = x;
		m_Index[1] = y;
		m_Index[2] = z;
	}
	int m_Index[3];
};
struct Face
{

};
struct Kd
{
	Kd() :r(1.0f), g(1.0f), b(1.0f) {}
	float r, g, b;
};

struct MTL //머터리얼
{
	string m_Name;
	Kd m_Kd;
};

struct Plane
{
	Plane(const XMFLOAT3& _point1, const XMFLOAT3& _point2, const XMFLOAT3& _point3)
	{
		XMVECTOR V1 = XMVectorSubtract(XMLoadFloat3(&_point1), XMLoadFloat3(&_point2));
		XMVECTOR V2 = XMVectorSubtract(XMLoadFloat3(&_point1), XMLoadFloat3(&_point3));

		m_Normal = XMVector3Cross(V1, V2);
		m_Normal = XMVector3Normalize(m_Normal);
		m_Point = XMLoadFloat3(&_point1);
	}
	XMVECTOR m_Normal;
	XMVECTOR m_Point;
};

enum GizmoType
{
	Shphere,
	Box,
	Line
};
struct GizmoInfo
{
	GizmoType m_Type;
	float m_Radius;
	XMFLOAT3* m_BoxPos;
	XMFLOAT3* m_LinePos;
};
