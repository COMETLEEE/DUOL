#pragma once
#include "d3d11.h"

#include "..\\..\\Common\\LightHelper.h"
#include "..\\..\\Library\\inc\\SimpleMath.h"

namespace MuscleGrapics
{
	namespace VertexDesc
	{
		static constexpr UINT BasicVertexSize = 1;
		static D3D11_INPUT_ELEMENT_DESC BasicVertex[BasicVertexSize] =
		{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		static constexpr UINT DeferredVertexSize = 2;
		static D3D11_INPUT_ELEMENT_DESC DeferredVertexDesc[DeferredVertexSize] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		static constexpr UINT BasicLightVertexSize = 7;
		static D3D11_INPUT_ELEMENT_DESC BasicLightVertex[BasicLightVertexSize] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WEIGHT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BONEINDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 72,
				D3D11_INPUT_PER_VERTEX_DATA, 0}

		};

		static constexpr UINT BasicParticleVertexSize = 11;
		static D3D11_INPUT_ELEMENT_DESC BasicParticleVertex[BasicParticleVertexSize] =
		{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SV_VertexID",     0, DXGI_FORMAT_R32_UINT,        0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LIFETIME",     0, DXGI_FORMAT_R32_FLOAT,        0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ROTATION",     0, DXGI_FORMAT_R32_FLOAT,        0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,        0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"GRAVITY",     0, DXGI_FORMAT_R32_FLOAT,        0, 68, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXINDEX",     0, DXGI_FORMAT_R32G32_UINT,        0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}
	namespace Vertex
	{
		struct Basic
		{
			DirectX::SimpleMath::Vector3 Pos;
			DirectX::SimpleMath::Vector2 Texture;
		};
		struct BasicLight
		{
			BasicLight() : Pos(), Texture(), Nomal(), Color(), Weights(), BoneIndex{ 0,0,0,0 }, Tangent()
			{}
			DirectX::SimpleMath::Vector3 Pos;
			DirectX::SimpleMath::Vector2 Texture;
			DirectX::SimpleMath::Vector3 Nomal;
			DirectX::SimpleMath::Vector4 Color;
			DirectX::SimpleMath::Vector3 Weights;
			UINT BoneIndex[4];
			DirectX::SimpleMath::Vector3 Tangent;

		};

		struct Particle
		{
			Particle() :InitialPos(0, 0, 0),
				InitialVel(0, 0, 0),
				Size(1, 1),
				Age(0),
				Type(0),
				VertexID(0),
				LifeTime(0),
				Rotation(0),
				Color(),
				Gravity(0),
				TexIndex{ 0,0 }
			{
			}
			DirectX::SimpleMath::Vector3 InitialPos;
			DirectX::SimpleMath::Vector3 InitialVel;
			DirectX::SimpleMath::Vector2 Size;
			float Age;
			unsigned int Type;
			unsigned int VertexID;

			float LifeTime;
			float Rotation;
			DirectX::SimpleMath::Vector4 Color;
			float Gravity;
			XMINT2 TexIndex;
		};
		struct Texture
		{
			DirectX::SimpleMath::Vector3 Pos;
			DirectX::SimpleMath::Vector2 Uv;
		};
	}

	struct index3
	{
		index3() : m_Index{ 0,0,0 }
		{}
		index3(int x, int y, int z) :m_Index{ x,y,z }
		{}
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



	namespace ConstantBuffDesc
	{
		__declspec(align(16)) struct CommonInfo
		{
			DirectX::SimpleMath::Vector3 gEmitDirW;
			float	gDuration;				// �� �� ���� ��ƼŬ ��ü�� ����� ��.					

			DirectX::SimpleMath::Vector3 gEmitPosW;
			int		gMaxParticles;				// ��ƼŬ �ִ� ��� ������.							

			float	gStartDelay[2];				// �� �� �ڿ� ��ƼŬ�� ����� ��.					
			float	gStartLifeTime[2];				// �� ��ƼŬ�� ���� �ð�.						

			float	gStartSpeed[2];				// ��ƼŬ ������ ���� �ӵ�.
			float	gStartRotation[2];			// ��ƼŬ�� ���� ȸ��.							

			float gStartSize[4];				// ��ƼŬ�� ���� ũ��.							

			DirectX::SimpleMath::Vector4 gStartColor[2];			// ��ƼŬ�� ���� ����									

			float	gGravityModifier[2];			// ��ƼŬ�� �������� �߷�.						
			float gParticlePlayTime;
			float	pad4;			// ��ƼŬ�� �������� �߷�.						

			int	gisLooping;					// �ݺ�����.
			int	pad2[3];					// �����ΰ��� ..?						
		};
		__declspec(align(16)) struct Emission
		{
			float	gEmissiveCount;			// �ѹ��� ��� ���� ��ų��.
			float	gEmissiveTime;			// ���� ������� �ɸ��� �ð�.
			DirectX::SimpleMath::Vector2 pad5;
		};
		__declspec(align(16)) struct Shape
		{

		};
		__declspec(align(16)) struct Color_over_Lifetime
		{
			DirectX::SimpleMath::Vector4 gStartColor;

			DirectX::SimpleMath::Vector4 gEndColor;
		};
		__declspec(align(16)) struct Velocity_over_Lifetime
		{
			DirectX::SimpleMath::Vector3 gVelocity;
			float pad;
		};
		__declspec(align(16)) struct Size_Over_Lifetime
		{
			float gStartSize;
			float gEndSize;
			float gStartOffset;
			float gEndOffset;
		};
		__declspec(align(16)) struct Rotation_Over_Lifetime
		{
			float gAngularVelocity;

			DirectX::SimpleMath::Vector3 pad;
		};
		__declspec(align(16)) struct Texture_Sheet_Animation
		{
			int gGrid_XY[2];
			int gTimeMode;
			float pad;
		};
		/**
		 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������. 16 ����Ʈ ���� ��������.
		 */
		__declspec(align(16)) struct CB_PerObject
		{
			DirectX::SimpleMath::Matrix worldViewProj;

			DirectX::SimpleMath::Matrix gWorld;

			DirectX::SimpleMath::Matrix gWorldInvTranspose;
		};

		__declspec(align(16)) struct CB_PerFream
		{
			DirectionalLight gDirLight;

			PointLight gPointLight[10];

			SpotLight gSpotLight;

			DirectX::SimpleMath::Vector3 gEyePosW;

			int gPointCount;

			DirectX::SimpleMath::Matrix gLightViewProj;

			DirectX::SimpleMath::Matrix gCurrentViewProj; // ���� ���� ��! �� �ʿ������?

			DirectX::SimpleMath::Matrix gPrevViewProj; // ���� ���� ��!
		};

		__declspec(align(16)) struct CB_PerObject_Particle
		{
			CommonInfo _commonInfo;

			Emission _emission;

			Velocity_over_Lifetime _velocityoverLifetime;

			Color_over_Lifetime _coloroverLifetime;

			Size_Over_Lifetime _sizeoverLifetime;

			Rotation_Over_Lifetime _rotationoverLifetime;

			Texture_Sheet_Animation _textureSheetAnimation;
		};

		__declspec(align(16)) struct CB_PerFream_Particle
		{
			DirectX::SimpleMath::Vector3 gCameraPosW; // ī�޶��� ��ǥ
			float	pad999;

			DirectX::SimpleMath::Vector2 gScreenXY;
			float	gTimeStep; // 1�����Ӵ� �ð�
			float	pad2;

			DirectX::SimpleMath::Matrix	gViewProj;
		};
	}
}

