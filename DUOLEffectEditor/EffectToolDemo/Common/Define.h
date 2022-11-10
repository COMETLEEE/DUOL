#pragma once

#define HR(x) {HRESULT hr;\
	hr = x;\
if(FAILED(hr)) return;}

#define HRTex(x) {HRESULT hr;\
	hr = x;\
if(FAILED(hr)) return 0;}

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

namespace Vertex
{
	using namespace DirectX;
	struct Basic
	{
		Basic(XMFLOAT3 _pos, XMFLOAT2 _Tex, XMFLOAT3 _Nomal, XMFLOAT4 _Color, XMFLOAT3 _Weights) :
			Tangent()
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
		Basic() :Pos(), Texture(), Nomal(), Color(0.0f, 0.0f, 0.0f, 1.0f), Weights(), Tangent()
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

	};


	struct Particle
	{
		Particle() :InitialPos(), InitialVel(), Size(), Age(), Type()
		{

		}
		XMFLOAT3 InitialPos;
		XMFLOAT3 InitialVel;
		XMFLOAT2 Size;
		float Age;
		unsigned int Type;
	};
}

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
