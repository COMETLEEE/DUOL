#pragma once
#include "d3d11.h"

#include "..\\..\\Common\\LightHelper.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

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

		static constexpr UINT BasicParticleVertexSize = 14;
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
		{"QUADTEX",     0, DXGI_FORMAT_R32G32_FLOAT,        0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"QUADTEX",     1, DXGI_FORMAT_R32G32_FLOAT,        0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"QUADTEX",     2, DXGI_FORMAT_R32G32_FLOAT,        0, 88, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"QUADTEX",     3, DXGI_FORMAT_R32G32_FLOAT,        0, 96, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}
	namespace Vertex
	{
		struct Basic
		{
			DUOLMath::Vector3 Pos;
			DUOLMath::Vector2 Texture;
		};
		struct BasicLight
		{
			BasicLight() : Pos(), Texture(), Nomal(), Color(), Weights(), BoneIndex{ 0,0,0,0 }, Tangent()
			{}
			DUOLMath::Vector3 Pos;
			DUOLMath::Vector2 Texture;
			DUOLMath::Vector3 Nomal;
			DUOLMath::Vector4 Color;
			DUOLMath::Vector3 Weights;
			UINT BoneIndex[4];
			DUOLMath::Vector3 Tangent;

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
				TexIndex()
			{
			}
			DUOLMath::Vector3 InitialPos;
			DUOLMath::Vector3 InitialVel;
			DUOLMath::Vector2 Size;
			float Age;
			unsigned int Type;
			unsigned int VertexID;

			float LifeTime;
			float Rotation;
			DUOLMath::Vector4 Color;
			float Gravity;
			DUOLMath::Vector2 TexIndex[4];
		};
		struct Texture
		{
			DUOLMath::Vector3 Pos;
			DUOLMath::Vector2 Uv;
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
		__declspec(align(16)) struct CommonInfo // 0~8 36
		{
			CommonInfo(Particle_CommonInfo& renderingData)
			{


				memcpy(gStartDelay, renderingData._startDelay, sizeof(gStartDelay));
				memcpy(gStartLifeTime, renderingData._startLifeTime, sizeof(gStartLifeTime));

				memcpy(gStartSpeed, renderingData._startSpeed, sizeof(gStartSpeed));
				memcpy(gStartRotation, renderingData._startRotation, sizeof(gStartRotation));

				memcpy(gStartSize, renderingData._startSize, sizeof(gStartSize));

				memcpy(gStartColor, renderingData._startColor, sizeof(gStartColor));

				memcpy(gGravityModifier, renderingData._gravityModifier, sizeof(gGravityModifier));
				gParticlePlayTime = renderingData._playTime; // ���� �ð�

				gisLooping = static_cast<int>(renderingData._looping);
				gSimulationSpeed = renderingData._simulationSpeed;
				gDuration = renderingData._duration;
				gMaxParticles = renderingData._maxParticles;

			}
			DUOLMath::Matrix gTransformMatrix;


			float	gStartDelay[2];				// �� �� �ڿ� ��ƼŬ�� ����� ��.					
			float	gStartLifeTime[2];				// �� ��ƼŬ�� ���� �ð�.						

			float	gStartSpeed[2];				// ��ƼŬ ������ ���� �ӵ�.
			float	gStartRotation[2];			// ��ƼŬ�� ���� ȸ��.							

			float gStartSize[4];				// ��ƼŬ�� ���� ũ��.							

			DUOLMath::Vector4 gStartColor[2];			// ��ƼŬ�� ���� ����									

			float	gGravityModifier[2];			// ��ƼŬ�� �������� �߷�.						
			float gParticlePlayTime;
			float	gObjectID;			// ��ƼŬ�� ������ �ִ� ������Ʈ ID.						

			int	gisLooping;					// �ݺ�����.
			float gSimulationSpeed;
			float	gDuration;				// �� �� ���� ��ƼŬ ��ü�� ����� ��.					
			int		gMaxParticles;				// ��ƼŬ �ִ� ��� ������.							

		};
		__declspec(align(16)) struct Emission // 9
		{
			Emission(Particle_Emission& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Emission) - sizeof(int));
			}
			int	gEmissiveCount;			// �ѹ��� ��� ���� ��ų��.
			float	gEmissiveTime;			// ���� ������� �ɸ��� �ð�.
			DUOLMath::Vector2 pad5;
		};
		__declspec(align(16)) struct Shape // 10 ~ 13
		{
			Shape(Particle_Shape& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 2, sizeof(Particle_Shape) - sizeof(int) * 2);
			}

			float gAngle;
			float gRadius;
			float gDonutRadius;
			float gArc;

			DUOLMath::Vector3 gPosition;
			float pad0;

			DUOLMath::Vector3 gRotation;
			float pad1;

			DUOLMath::Vector3 gScale;
			float pad2;
		};
		__declspec(align(16)) struct Velocity_over_Lifetime // 14 // 4
		{
			Velocity_over_Lifetime(Particle_Velocity_over_Lifetime& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Velocity_over_Lifetime) - sizeof(int));
			}
			DUOLMath::Vector3 gVelocity;
			float pad;
			DUOLMath::Vector3 gOrbital;
			float pad2;
			DUOLMath::Vector3 gOffset;
			float pad3;
		};
		__declspec(align(16)) struct Force_over_LifeTime // 15 // 4
		{
			Force_over_LifeTime(Particle_Force_over_LifeTime& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Force_over_LifeTime) - sizeof(int));
			}
			DUOLMath::Vector3 gForce;
			float pad;
		};
		__declspec(align(16)) struct Color_over_Lifetime //
		{
			Color_over_Lifetime(Particle_Color_over_Lifetime& renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&renderingData) + 1, sizeof(Particle_Color_over_Lifetime) - sizeof(int));
			}

			DUOLMath::Vector4 gAlpha_Ratio[8];

			DUOLMath::Vector4 gColor_Ratio[8];
		};
		__declspec(align(16)) struct Size_Over_Lifetime
		{
			Size_Over_Lifetime(Particle_Size_Over_Lifetime& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Size_Over_Lifetime) - sizeof(int));
			}
			float gStartSize;
			float gEndSize;
			float gStartOffset;
			float gEndOffset;
		};
		__declspec(align(16)) struct Rotation_Over_Lifetime
		{
			Rotation_Over_Lifetime(Particle_Rotation_Over_Lifetime& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Rotation_Over_Lifetime) - sizeof(int));
			}

			float gAngularVelocity;

			DUOLMath::Vector3 pad;
		};
		__declspec(align(16)) struct Texture_Sheet_Animation // 30
		{
			Texture_Sheet_Animation(Particle_Texture_Sheet_Animation& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Texture_Sheet_Animation) - sizeof(int));
			}
			int gGrid_XY[2];
			int gTimeMode;
			float pad;
		};
		__declspec(align(16)) struct Noise
		{
		};
		__declspec(align(16)) struct Trails
		{
		};
		__declspec(align(16)) struct paticle_Renderer
		{
			paticle_Renderer(Particle_Renderer& _renderingData)
			{
				gSpeedScale = _renderingData._speedScale;

				gLengthScale = _renderingData._lengthScale;

				pad[0] = 0;
				pad[1] = 0;
			}
			float gSpeedScale;
			float gLengthScale;
			float pad[2];
		};
		/**
		 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������. 16 ����Ʈ ���� ��������.
		 */
		__declspec(align(16)) struct CB_PerObject
		{
			DUOLMath::Matrix worldViewProj;

			DUOLMath::Matrix gWorld;

			DUOLMath::Matrix gWorldInvTranspose;

			DUOLMath::Vector4 gObjectID;

			DUOLMath::Vector4 gColor;

		};

		__declspec(align(16)) struct CB_PerFream
		{
			DirectionalLight gDirLight;

			PointLight gPointLight[10];

			SpotLight gSpotLight;

			DUOLMath::Vector3 gEyePosW;

			int gPointCount;

			DUOLMath::Matrix gLightViewProj;

			DUOLMath::Matrix gCurrentViewProj; // ���� ���� ��! �� �ʿ������?

			DUOLMath::Matrix gPrevViewProj; // ���� ���� ��!
		};

		__declspec(align(16)) struct CB_PerObject_Particle
		{
			CB_PerObject_Particle(RenderingData_Particle& renderingData);

			CommonInfo _commonInfo;

			Emission _emission;

			Shape _shape;

			Velocity_over_Lifetime _velocityoverLifetime;

			Force_over_LifeTime _forceoverLifetime;

			Color_over_Lifetime _coloroverLifetime;

			Size_Over_Lifetime _sizeoverLifetime;

			Rotation_Over_Lifetime _rotationoverLifetime;

			Texture_Sheet_Animation _textureSheetAnimation;

			//Noise _noise;

			//Trails _trails;

			paticle_Renderer _renderer;

			unsigned int _flag;
			float Pad[3];
		};

		__declspec(align(16)) struct CB_PerFream_Particle
		{
			DUOLMath::Vector3 gCameraPosW; // ī�޶��� ��ǥ
			float	pad999;

			DUOLMath::Vector2 gScreenXY;
			float	gTimeStep; // 1�����Ӵ� �ð�
			float	pad2;

			DUOLMath::Matrix	gViewProj;
		};


		inline CB_PerObject_Particle::CB_PerObject_Particle(RenderingData_Particle& renderingData) :
			_commonInfo(renderingData._commonInfo),
			_emission(renderingData._emission),
			_shape(renderingData._shape),
			_velocityoverLifetime(renderingData._velocity_Over_Lifetime),
			_forceoverLifetime(renderingData._force_Over_Lifetime),
			_coloroverLifetime(renderingData._color_Over_Lifetime),
			_sizeoverLifetime(renderingData._size_Over_Lifetime),
			_rotationoverLifetime(renderingData._rotation_Over_Lifetime),
			_textureSheetAnimation(renderingData._texture_Sheet_Animaition),
			_renderer(renderingData._renderer)
			//_noise(),
			//_trails(),
			//_renderer()
		{

			_flag = renderingData.GetFlag();

			memcpy(&_commonInfo.gObjectID, &renderingData._objectID, sizeof(UINT));
			{
				DUOLMath::Matrix world = renderingData._commonInfo._transformMatrix; // ��Ʈ ��Ʈ����







				// --------------------------------- Set_QuatAndScale ----------------------------------------------
				// ------------------------- ȸ�� Ȥ�� �����Ͽ� ������ �޴� �ɼǵ�. -----------------------------
				DUOLMath::Vector3 s;
				DUOLMath::Quaternion r;
				DUOLMath::Vector3 t;

				world.Decompose(s, r, t);

				world.m[3][0] = 0; world.m[3][1] = 0; world.m[3][2] = 0;

				_commonInfo.gStartSize[0] = _commonInfo.gStartSize[0] * s.x;
				_commonInfo.gStartSize[1] = _commonInfo.gStartSize[1] * s.y;
				_commonInfo.gStartSize[2] = _commonInfo.gStartSize[2] * s.x;
				_commonInfo.gStartSize[3] = _commonInfo.gStartSize[3] * s.y;

				if (renderingData._velocity_Over_Lifetime._space == Space::Local)
					_velocityoverLifetime.gVelocity = DUOLMath::Vector3::Transform(_velocityoverLifetime.gVelocity, world);

				if (renderingData._force_Over_Lifetime._space == Space::Local)
					_forceoverLifetime.gForce = DUOLMath::Vector3::Transform(_forceoverLifetime.gForce, world);

				DUOLMath::Matrix shapeTM = DUOLMath::Matrix::CreateScale(_shape.gScale) * DUOLMath::Matrix::CreateFromYawPitchRoll(_shape.gRotation.z, _shape.gRotation.x, _shape.gRotation.y) * DUOLMath::Matrix::CreateTranslation(_shape.gPosition);

				world = shapeTM * renderingData._commonInfo._transformMatrix;
				memcpy(&_commonInfo.gTransformMatrix, &world, sizeof(DUOLMath::Matrix));

			}
		}

	}
}

