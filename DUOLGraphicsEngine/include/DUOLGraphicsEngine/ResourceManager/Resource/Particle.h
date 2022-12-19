#pragma once

#include <vector>
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	enum class RASTERIZER_STATE
	{
		WIRE = 0,
		SOLID = 1
	};

	enum class PARTICLE_EFFECT_TYPE
	{
		CIRCLE,
		SPHERE,
	};

	struct Particle_CommonInfo
	{
		enum class Option_Particle
		{
			Constant,
			Curve, // 커브는 후순위로 미루자..!
			RandomBetweenTwoConstant,
			RandomBetweenTwoCurve // 커브는 후순위로 미루자..!
		};
		Particle_CommonInfo() :
			_firstRun(true),
			_duration(5.0f),
			_looping(true),
			_startDelayOption(Option_Particle::Constant),
			_startDelay{ 0,0 },
			_startLifeTimeOption(Option_Particle::Constant),
			_startLifeTime{ 5.0f,5.0f },
			_startSpeedOption(Option_Particle::Constant),
			_startSpeed{ 4.0f,4.0f },
			_startSizeOption(Option_Particle::Constant),
			_startSize{ DUOLMath::Vector2(1.0f, 1.0f),DUOLMath::Vector2(1.0f, 1.0f) },
			_startRotationOption(Option_Particle::Constant),
			_startRotation{ 0,0 },
			_startColorOption(Option_Particle::Constant),
			_startColor{ DUOLMath::Vector4(1.0f, 1.0f, 1.0f,1.0f),DUOLMath::Vector4(1.0f, 1.0f, 1.0f,1.0f) },
			_gravirtModifierOption(Option_Particle::Constant),
			_gravityModifier{ 0.0f,0.0f },
			_maxParticles(1000),
			_transformMatrix(DUOLMath::Matrix::Identity),
			_refTexturePath(TEXT(""))
		{}
		//																					ShaderCode					   ImGui						
		bool _firstRun;					// 시작인가요 ..?										O							O

		float _duration;				// 몇 초 동안 파티클 객체가 재생될 지.						X							O

		bool _looping;					// 반복여부.												X							O

		Option_Particle _startDelayOption;	//													X							X

		float _startDelay[2];				// 몇 초 뒤에 파티클이 재생될 지.						X							O

		Option_Particle _startLifeTimeOption;	//												X							X

		float _startLifeTime[2];				// 한 파티클의 생존 시간.							O							O

		Option_Particle _startSpeedOption;//													X							X

		float _startSpeed[2];				// 파티클 생성시 시작 속도.							O							O

		Option_Particle _startSizeOption;//														X							X

		DUOLMath::Vector2 _startSize[2];				// 파티클의 시작 크기.								O							O

		Option_Particle _startRotationOption;//													X							X

		float _startRotation[2];			// 파티클의 시작 회전.								X							O

		Option_Particle _startColorOption;//													X							X

		DUOLMath::Vector4 _startColor[2];			// 파티클의 시작 색상										X							O

		Option_Particle _gravirtModifierOption;//												X							X

		float _gravityModifier[2];			// 파티클에 가해지는 중력.							O							O

		int _maxParticles;				// 파티클 최대 출력 사이즈.								X							O
		// ContantBuffer 16정렬 후 등록 완료..

		DUOLMath::Matrix _transformMatrix;		// 파티클의 생성 위치 및 각을 조정							X							X

		DUOLCommon::tstring _refTexturePath;			// 파티클 이펙트가 텍스쳐의 주소.

		float _playTime;				// play 시작후 흐른 시간.

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _firstRun;

			ar& _duration;

			ar& _looping;

			ar& _startDelayOption;

			ar& _startDelay;

			ar& _startLifeTimeOption;

			ar& _startLifeTime;

			ar& _startSpeedOption;

			ar& _startSpeed;

			ar& _startSizeOption;

			ar& _startSize;

			ar& _startRotationOption;

			ar& _startRotation;

			ar& _startColorOption;

			ar& _startColor;

			ar& _gravirtModifierOption;

			ar& _gravityModifier;

			ar& _maxParticles;

			ar& _transformMatrix;

			ar& _refTexturePath;
		}

	};
	struct Particle_Emission
	{
		Particle_Emission() : _emissiveCount(1), _emissiveTime(0.1f)
		{
		}

		int _emissiveCount;			// 한번에 몇개를 방출 시킬지.

		float _emissiveTime;			// 다음 방출까지 걸리는 시간.

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _emissiveCount;
			ar& _emissiveTime;

		}
	};
	struct Particle_Color_over_Lifetime
	{
		Particle_Color_over_Lifetime() : _startColor(1.0f, 1.0f, 1.0f, 1.0f), _endColor(1.0f, 1.0f, 1.0f, 1.0f)
		{
		}

		DUOLMath::Vector4 _startColor;

		DUOLMath::Vector4 _endColor;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _startColor;
			ar& _endColor;

		}
	};
	struct Particle_Velocity_over_Lifetime
	{
		Particle_Velocity_over_Lifetime() : _linearVelocity(0, 0, 0)
		{
		}

		DUOLMath::Vector3 _linearVelocity;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _linearVelocity;
		}
	};
	struct Particle_Size_Over_Lifetime
	{
		Particle_Size_Over_Lifetime() :
			_startSize(1), _endSize(1),
			_startOffset(0), _endOffset(0)
		{

		}
		float _startSize;
		float _endSize;
		float _startOffset;
		float _endOffset;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _startSize;
			ar& _endSize;
			ar& _startOffset;
			ar& _endOffset;
		}
	};
	struct Particle_Texture_Sheet_Animation
	{
		Particle_Texture_Sheet_Animation() :
			_grid_XY{ 1, 1 },
			_timeMode(0)
		{

		}

		int _grid_XY[2];

		int _timeMode;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _grid_XY;
			ar& _timeMode;
		}
	};
	struct Particle_Rotation_Over_Lifetime
	{
		Particle_Rotation_Over_Lifetime() :
			_AngularVelocity(0)
		{
		}
		float _AngularVelocity;

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _AngularVelocity;
		}
	};
	// 파티클 시스템을 사용하기 위한 인터페이스
	// 한 개의 입자 시스템을 묘사한다.
	struct RenderingData_Particle
	{
		RenderingData_Particle() :
			_commonInfo(),
			_emission(),
			_color_Over_Lifetime(),
			_velocity_Over_Lifetime(),
			_size_Over_Lifetime(),
			_rotation_Over_Lifetime(),
			_texture_Sheet_Animaition(),
			_objectID(0),
			_isDelete(false),
			_childrens()
			, shaderName()
		{}

		Particle_CommonInfo _commonInfo;

		Particle_Emission _emission;

		Particle_Color_over_Lifetime _color_Over_Lifetime;

		Particle_Velocity_over_Lifetime _velocity_Over_Lifetime;

		Particle_Size_Over_Lifetime _size_Over_Lifetime;

		Particle_Rotation_Over_Lifetime _rotation_Over_Lifetime;

		Particle_Texture_Sheet_Animation _texture_Sheet_Animaition;

		unsigned int _objectID; // 파티클 ID 리소스 매니저에 맵핑한 아이디, 오브젝트 ID로 사용하자.

		std::vector<DUOLCommon::tstring> shaderName; // 파티클 ID 리소스 매니저에 맵핑한 아이디

		std::vector<RenderingData_Particle> _childrens;

		bool _isDelete; // 파티클을 다 사용했으면 할당 해제 하기 위함. 파티클을 내부에서 오브젝트 풀 등으로 관리 안하는 이유는 파티클마다 버퍼의 크기가 다르기 때문이다.

		RASTERIZER_STATE _rasterizerState;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _commonInfo;
			ar& _emission;
			ar& _color_Over_Lifetime;
			ar& _velocity_Over_Lifetime;
			ar& _size_Over_Lifetime;
			ar& _rotation_Over_Lifetime;
			ar& _texture_Sheet_Animaition;
			ar& _objectID;
			ar& shaderName;
			ar& _childrens;
			ar& _isDelete;
			ar& _rasterizerState;
		}
	};
}
