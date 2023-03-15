#pragma once
#include "d3d11.h"

#include "..\\..\\Common\\LightHelper.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

namespace MuscleGrapics
{
	namespace Structure
	{
		struct CounterBuffer
		{
			CounterBuffer() :particleCounter(0),
				EmiiterCounter(0),
				AllGroupSync(0),
				pad2(0)
			{

			}
			int particleCounter;
			int EmiiterCounter;
			int AllGroupSync;
			int pad2;
		};
		struct PixelData
		{
			PixelData() :Color(0xffffffff), Depth(0xffffffff), BlendType(0xffffffff)
			{}
			unsigned int Color;
			float Depth;
			unsigned BlendType;
		};
		struct PixelNode
		{
			PixelNode() :Data(), Next(0xffffffff)
			{}
			PixelData Data;
			unsigned int Next;
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
				InitialVel(0, 0, 0, 1.0f),
				Size_StartSize(1, 1, 1, 1),
				Age_LifeTime_Rotation_Gravity(0, 0, 0, 0),
				Type(0),
				StartColor(),
				Color(),
				TexIndex()
			{
			}
			unsigned int Type;
			DUOLMath::Vector3 InitialPos;

			DUOLMath::Vector4 InitialVel;

			DUOLMath::Vector4 Size_StartSize;

			DUOLMath::Vector4 Age_LifeTime_Rotation_Gravity;

			DUOLMath::Vector4 StartColor;
			DUOLMath::Vector4 Color;

			DUOLMath::Vector2 TexIndex[4];

			DUOLMath::Vector4 InitEmitterPos;

			DUOLMath::Vector4 PrevPos[30];

			DUOLMath::Vector4 LastestPrevPos;
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
		__declspec(align(16)) struct CB_DynamicBuffer
		{
			CB_DynamicBuffer() : g_EmiitionTime(0), g_dim(0), pad1(0), pad2(0)
			{

			}
			float g_EmiitionTime;
			int g_dim;
			float pad1;
			float pad2;
		};

		__declspec(align(16)) struct CommonInfo // 0~8 36
		{
			CommonInfo(Particle_CommonInfo& renderingData)
			{
				memcpy(&gDeltaMatrix, &renderingData._deltaMatrix, sizeof(DUOLMath::Matrix));

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
			DUOLMath::Matrix gDeltaMatrix;

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
			Noise(Particle_Noise& _renderingData)
			{
				gStregth = _renderingData._strength;
				gScrollSpeed = _renderingData._scrollSpeed;
				gPositionAmount = _renderingData._positionAmount;
				gRotationAmount = _renderingData._rotationAmount;

				gSizeAmount = _renderingData._sizeAmount;
			}
			float gStregth;
			float gScrollSpeed;
			float gPositionAmount;
			float gRotationAmount;

			float gSizeAmount;
			DUOLMath::Vector3 pad;
		};
		__declspec(align(16)) struct Collision
		{
			Collision(Particle_Collision& _renderingData)
			{
				memcpy(this, reinterpret_cast<int*>(&_renderingData) + 1, sizeof(Particle_Collision) - sizeof(int));

				if (_renderingData._useModule)
				{
					for (int i = 0; i < gPlaneCount; i++)
					{
						gPlanNormalVec[i].Normalize();
					}
				}

			}
			int gPlaneCount;
			float gBoundce;
			float gLifeTimeLoss;
			float pad;

			DUOLMath::Vector4 gPlanePosition[8];
			DUOLMath::Vector4 gPlanNormalVec[8];
		};
		__declspec(align(16)) struct Trails
		{
			Trails(Particle_Trails& _renderingData)
			{
				gRatio = _renderingData._ratio;
				gLifeTime = _renderingData._lifeTime;
				gMinimumVertexDistance = _renderingData._minimumVertexDistance;
				gWidthOverTrail = _renderingData._widthOverTrail;
				gTrailVertexCount = _renderingData._trailVertexCount;

				gTrailsFlag = 0;
				if (_renderingData._worldSpace)
					gTrailsFlag |= 1 << 0;
				if (_renderingData._dieWithParticle)
					gTrailsFlag |= 1 << 1;
				if (_renderingData._sizeAffectsWidth) // o
					gTrailsFlag |= 1 << 2;
				if (_renderingData._sizeAffectsLifeTime) // o
					gTrailsFlag |= 1 << 3;
				if (_renderingData._inheritParticleColor) // o
					gTrailsFlag |= 1 << 4;
				if (_renderingData._generateLightingData)
					gTrailsFlag |= 1 << 5;

				if (_renderingData._textureMode == Particle_Trails::TextureMode::Stretch)
					gTrailsFlag |= 1 << 6;
				if (_renderingData._textureMode == Particle_Trails::TextureMode::Tile)
					gTrailsFlag |= 1 << 7;
				if (_renderingData._textureMode == Particle_Trails::TextureMode::DistributePerSegment)
					gTrailsFlag |= 1 << 8;
				if (_renderingData._textureMode == Particle_Trails::TextureMode::RepeatPerSegment)
					gTrailsFlag |= 1 << 9;

				for (int i = 0; i < 8; i++)
				{
					gAlpha_Ratio_Lifetime[i] = _renderingData._alpha_Ratio_Lifetime[i];
					gColor_Ratio_Lifetime[i] = _renderingData._color_Ratio_Lifetime[i];
					gAlpha_Ratio_Trail[i] = _renderingData._alpha_Ratio_Trail[i];
					gColor_Ratio_Trail[i] = _renderingData._color_Ratio_Trail[i];
				}
			}
			float gRatio; // o
			float gLifeTime; // o
			float gMinimumVertexDistance; // o
			float gWidthOverTrail; // o

			int gTrailsFlag;
			int gTrailVertexCount;
			int pad[2];

			DUOLMath::Vector4 gAlpha_Ratio_Lifetime[8]; // o
			DUOLMath::Vector4 gColor_Ratio_Lifetime[8]; // o
			DUOLMath::Vector4 gAlpha_Ratio_Trail[8]; // o
			DUOLMath::Vector4 gColor_Ratio_Trail[8]; // o

		};
		__declspec(align(16)) struct paticle_Renderer
		{
			paticle_Renderer(Particle_Renderer& _renderingData)
			{
				gSpeedScale = _renderingData._speedScale;

				gLengthScale = _renderingData._lengthScale;

				gBlendType = static_cast<unsigned int>(_renderingData._blendState);

				pad = 0;
			}
			float gSpeedScale;
			float gLengthScale;
			unsigned int gBlendType;
			float pad;
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

			DUOLMath::Vector4 gMetalicRoughnessAoSpecular;

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

			Noise _noise;

			Collision _collision;

			Texture_Sheet_Animation _textureSheetAnimation;

			Trails _trails;

			paticle_Renderer _renderer;

			unsigned int _flag;
			float Pad[3];
		};

		__declspec(align(16)) struct Light
		{
			unsigned int Type;
			DUOLMath::Vector3 Direction;

			DUOLMath::Vector3 Position;
			float Range;

			DUOLMath::Vector3 Color;
			float Intensity;

			float Attenuation;
			float AttenuationRadius;
			DUOLMath::Vector2 pad;

		};

		__declspec(align(16)) struct CB_PerFream
		{
			CB_PerFream(PerFrameData& perFreamData)
			{
				memcpy(gLight, perFreamData._light, sizeof(Light) * 30);

				gCameraPosW = perFreamData._cameraInfo._cameraWorldPosition;
				gLightCount = perFreamData._lightCount;

				gScreenXY = perFreamData._cameraInfo._screenSize;
				gTimeStep = perFreamData._deltaTime;
				gGamePlayTime = perFreamData._gamePlayTime;

				gViewProj = perFreamData._cameraInfo._viewMatrix * perFreamData._cameraInfo._projMatrix;
			}

			Light gLight[LIGHT_INFO_MAX];

			DUOLMath::Vector3 gCameraPosW; // ī�޶��� ��ǥ
			unsigned int gLightCount;

			DUOLMath::Vector2 gScreenXY;
			float	gTimeStep; // 1�����Ӵ� �ð�
			float	gGamePlayTime;

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
			_trails(renderingData._trails),
			_renderer(renderingData._renderer),
			_noise(renderingData._noise),
			_collision(renderingData._collision)
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

