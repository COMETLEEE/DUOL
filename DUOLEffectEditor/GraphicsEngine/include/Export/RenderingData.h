#pragma once
#include "dllExport.h"
#include "util/STLInclude.h"
#include "util/TypeDefine.h"

#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

#pragma region PerFrameData
/**
 * \brief �� �����ӿ� �ѹ��� ������ ����
 */
namespace MuscleGrapics
{
	struct CameraInfo
	{

		CameraInfo() : _cameraWorldPosition(DUOLMath::Vector3::Zero),
			_viewMatrix(DUOLMath::Matrix::Identity),
			_projMatrix(DUOLMath::Matrix::Identity),
			_prevViewMatrix(DUOLMath::Matrix::Identity),
			_prevProjMatrix(DUOLMath::Matrix::Identity)
		{}

		DUOLMath::Vector3 _cameraWorldPosition;

	private:
		float _pad;

	public:
		DUOLMath::Matrix _viewMatrix;

		DUOLMath::Matrix _projMatrix;

		DUOLMath::Matrix _prevViewMatrix;

		DUOLMath::Matrix _prevProjMatrix;

		DUOLMath::Matrix _orthographicProj;
	};

	struct LightInfo
	{
		LightInfo() : _color(DUOLMath::Vector3(0.7f, 0.7f, 0.7f)), _lumen(1.f)
		{}

		DUOLMath::Vector3 _color;

		float _lumen;
	};

	struct DirectionalLightInfo final : public LightInfo
	{
		DirectionalLightInfo() : LightInfo(), _direction(DUOLMath::Vector3(1.f, 0.f, 0.f)), _pad(0.f)
		{}

		// == In Game Engine, World Rotation Look Vector
		DUOLMath::Vector3 _direction;

	private:
		float _pad;
	};

	struct PointLightInfo final : public LightInfo
	{
		PointLightInfo() : LightInfo(), _position(DUOLMath::Vector3(0.f, 0.f, 0.f)),
			_range(25.f)
		{}

		DUOLMath::Vector3 _position;

		float _range;
	};

	struct SpotLightInfo final : public LightInfo
	{
		SpotLightInfo() : LightInfo(), _direction(DUOLMath::Vector3(1.f, 0.f, 0.f)), _halfAngle(3.141592f / 4),
			_position(DUOLMath::Vector3::Zero), _range(25.f)
		{}

		DUOLMath::Vector3 _direction;
		float _halfAngle;

		DUOLMath::Vector3 _position;
		float _range;
	};

	constexpr uint32 LIGHT_INFO_MAX = 100;

	enum class POSTPROCESS_OPTION
	{
		ON_SSAO = 0b00000001,
		ON_GAUSSIAN_BLUR = 0b00000010,
		ON_CAM_BLUR = 0b00000100,
		ON_BLOOM = 0b00001000,
		ON_FXAA = 0b00010000,
		ON_VIGNETTING = 0b00100000,
		ON_DEBUG_PANEL = 0b01000000
	};

	struct PerFrameData
	{
		PerFrameData() : _cameraInfo(),
			_directionalLightInfos(),
			_spotLightInfos(),
			_pointLightInfos(),
			_postProcessOption(static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA) | static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR))),
			_deltaTime(0.f)
		{}

		CameraInfo _cameraInfo;

		std::vector<DirectionalLightInfo> _directionalLightInfos;

		std::vector<SpotLightInfo> _spotLightInfos;

		std::vector<PointLightInfo> _pointLightInfos;

		POSTPROCESS_OPTION _postProcessOption;

		float _deltaTime;						// �ð��� �󸶳� �귶�°�
	};
#pragma endregion

#pragma region 3D Rendering
	enum class RASTERIZER_STATE
	{
		SOLID = 0,
		WIREFRAME = 1,
	};
	struct ObjectInfo
	{
		ObjectInfo() : _meshID(0), _objectID(0), _usingLighting(true), _usingShadow(true)
		{}

		uint64 _meshID;

		// ����
		uint32 _objectID;

		bool _usingLighting;

		bool _usingShadow;
	};

	struct MaterialInfo
	{
		MaterialInfo() : _color(1.0f, 1.0f, 1.0f, 1.0f)
		{}

		DUOLMath::Vector4 _color;
	};

	struct ShaderInfo
	{

		ShaderInfo() : _shaderName(), _rasterizerState(RASTERIZER_STATE::SOLID)
		{
		}

		std::vector<tstring> _shaderName; // �������� �н��� ���� ��ų �� �ִ�.

		RASTERIZER_STATE _rasterizerState;
	};

	struct GeometryInfo
	{
		GeometryInfo() : _world(DUOLMath::Matrix::Identity), _worldInvTranspose(DUOLMath::Matrix::Identity),
			_worldViewProj(DUOLMath::Matrix::Identity), _texTransform(DUOLMath::Matrix::Identity)
		{}

		DUOLMath::Matrix _world;

		DUOLMath::Matrix _worldInvTranspose;

		DUOLMath::Matrix _worldViewProj;

		DUOLMath::Matrix _texTransform;
	};

	struct AnimationInfo
	{
		AnimationInfo() :
			_boneMatrixList()
		{}

		DUOLMath::Matrix _boneMatrixList[70];
	};

	enum class RENDERINGDATA_TYPE
	{
		// Static Mesh
		STATIC_MESH_OBJECT,

		// Skinned Mesh
		SKINNED_MESH_OBJECT,

		// Sky Box (Cube Map)
		SKY_BOX
	};

	struct RenderingData_3D
	{
		RenderingData_3D() : _dataType(RENDERINGDATA_TYPE::STATIC_MESH_OBJECT),
			_objectInfo(std::make_shared<ObjectInfo>()), _materialInfo(std::make_shared<MaterialInfo>()),
			_shaderInfo(std::make_shared<ShaderInfo>()), _geoInfo(std::make_shared<GeometryInfo>()), _animInfo(std::make_shared<AnimationInfo>())
		{}

		~RenderingData_3D()
		{
			_objectInfo.reset();

			_materialInfo.reset();

			_shaderInfo.reset();

			_geoInfo.reset();

			_animInfo.reset();
		}

		RENDERINGDATA_TYPE _dataType;

		std::shared_ptr<ObjectInfo> _objectInfo;

		std::shared_ptr<MaterialInfo> _materialInfo;

		std::shared_ptr<ShaderInfo> _shaderInfo;

		std::shared_ptr<GeometryInfo> _geoInfo;

		std::shared_ptr<AnimationInfo> _animInfo;
	};
#pragma endregion

#pragma region Particle Rendering
	/**
		@enum  RenderingData_Particle
		@brief ver_0.1 �������� ������ �������. ��� ������ ����� ������ ��ȹ�̴�. ������ ���̴� �ڵ忡�� ���ʿ��� ������ ���̱� ����.
	**/
	enum class Space
	{
		Local,
		World
	};
	/**
		@struct Particle_CommonInfo
		@brief  Ŀ�������� ��� ��ƼŬ�� ����ϴ� ���� �⺻���� ���, ������ �����Ų��.
	**/
	struct Particle_CommonInfo
	{
		enum class Option_Particle
		{
			Constant,
			Curve, // Ŀ��� �ļ����� �̷���..!
			RandomBetweenTwoConstant,
			RandomBetweenTwoCurve // Ŀ��� �ļ����� �̷���..!
		};
		Particle_CommonInfo() :
			_useModule(true),
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
			_simulationSpeed(1.0f)
		{}
		bool operator==(const Particle_CommonInfo& other) const
		{
			if (
				_useModule != other._useModule ||
				_firstRun != other._firstRun ||
				_duration != other._duration ||
				_looping != other._looping ||
				_startDelayOption != other._startDelayOption ||
				_startDelay[0] != other._startDelay[0] ||
				_startDelay[1] != other._startDelay[1] ||
				_startLifeTimeOption != other._startLifeTimeOption ||
				_startLifeTime[0] != other._startLifeTime[0] ||
				_startLifeTime[1] != other._startLifeTime[1] ||
				_startSpeedOption != other._startSpeedOption ||
				_startSpeed[0] != other._startSpeed[0] ||
				_startSpeed[1] != other._startSpeed[1] ||
				_startSizeOption != other._startSizeOption ||
				_startSize[0] != other._startSize[0] ||
				_startSize[1] != other._startSize[1] ||
				_startRotationOption != other._startRotationOption ||
				_startRotation[0] != other._startRotation[0] ||
				_startRotation[1] != other._startRotation[1] ||
				_startColorOption != other._startColorOption ||
				_startColor[0] != other._startColor[0] ||
				_startColor[1] != other._startColor[1] ||
				_gravirtModifierOption != other._gravirtModifierOption ||
				_gravityModifier[0] != other._gravityModifier[0] ||
				_gravityModifier[1] != other._gravityModifier[1] ||
				_maxParticles != other._maxParticles ||
				_transformMatrix != other._transformMatrix ||
				_simulationSpeed != other._simulationSpeed

				)
				return false;
			else
				return true;
		}
		bool _useModule;
		//																					ShaderCode					   ImGui						
		bool _firstRun;					// �����ΰ��� ..?										O							O

		float _duration;				// �� �� ���� ��ƼŬ ��ü�� ����� ��.						X							O

		bool _looping;					// �ݺ�����.												X							O

		Option_Particle _startDelayOption;	//													X							X

		float _startDelay[2];				// �� �� �ڿ� ��ƼŬ�� ����� ��.						X							O

		Option_Particle _startLifeTimeOption;	//												X							X

		float _startLifeTime[2];				// �� ��ƼŬ�� ���� �ð�.							O							O

		Option_Particle _startSpeedOption;//													X							X

		float _startSpeed[2];				// ��ƼŬ ������ ���� �ӵ�.							O							O

		Option_Particle _startSizeOption;//														X							X

		DUOLMath::Vector2 _startSize[2];				// ��ƼŬ�� ���� ũ��.								O							O

		Option_Particle _startRotationOption;//													X							X

		float _startRotation[2];			// ��ƼŬ�� ���� ȸ��.								X							O

		Option_Particle _startColorOption;//													X							X

		DUOLMath::Vector4 _startColor[2];			// ��ƼŬ�� ���� ����										X							O

		Option_Particle _gravirtModifierOption;//												X							X

		float _gravityModifier[2];			// ��ƼŬ�� �������� �߷�.							O							O

		int _maxParticles;				// ��ƼŬ �ִ� ��� ������.								X							O
		// ContantBuffer 16���� �� ��� �Ϸ�..

		DUOLMath::Matrix _transformMatrix;		// ��ƼŬ�� ���� ��ġ �� ���� ����							X							X

		float _playTime;				// play ������ �帥 �ð�.

		float _simulationSpeed;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

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

			ar& _simulationSpeed;

		}

	};
	struct Particle_Emission
	{
		Particle_Emission() : _useModule(false),
			_emissiveCount(1), _emissiveTime(0.1f)
		{
		}
		bool operator==(const Particle_Emission& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Emission)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		int _emissiveCount;			// �ѹ��� ��� ���� ��ų��.

		float _emissiveTime;			// ���� ������� �ɸ��� �ð�.

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;
			ar& _emissiveCount;
			ar& _emissiveTime;

		}
	};
	struct Particle_Shape
	{
		Particle_Shape() :
			_useModule(false),
			_shape(Shape::Sphere),
			_angle(0), _radius(0), _donutRadius(0), _arc(0),
			_position(0, 0, 0),
			_rotation(0, 0, 0),
			_scale(1, 1, 1)
		{
		}
		bool operator==(const Particle_Shape& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Shape)) == 0)
				return true;
			else
				return false;
		}
		enum class Shape
		{
			Sphere,
			Hemisphere,
			Cone,
			Donut,
			Box,
			Circle,
			Rectangle,
			Edge

		};
		bool _useModule;

		Shape _shape;

		float _angle;
		float _radius;
		float _donutRadius;
		float _arc;

		DUOLMath::Vector3 _position;
		float pad0;
		DUOLMath::Vector3 _rotation;
		float pad1;
		DUOLMath::Vector3 _scale;
		float pad2;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _shape;

			ar& _angle;
			ar& _radius;
			ar& _donutRadius;
			ar& _arc;

			ar& _position;
			ar& pad0;
			ar& _rotation;
			ar& pad1;
			ar& _scale;
			ar& pad2;
		}

	};
	struct Particle_Velocity_over_Lifetime
	{
		Particle_Velocity_over_Lifetime() : _useModule(false),
			_linearVelocity(0, 0, 0), _space(Space::Local),
			_orbital(0, 0, 0), _offset(0, 0, 0)
		{
		}
		bool operator==(const Particle_Velocity_over_Lifetime& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Velocity_over_Lifetime)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		DUOLMath::Vector3 _linearVelocity;
		Space _space;

		DUOLMath::Vector3 _orbital;
		float pad1;

		DUOLMath::Vector3 _offset;
		float pad2;


	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _space;

			ar& _linearVelocity;

			ar& _orbital;

			ar& pad1;

			ar& _offset;

			ar& pad2;
		}
	};
	struct Particle_Force_over_LifeTime
	{
		Particle_Force_over_LifeTime() :_useModule(false), _force(0, 0, 0),
			_space(Space::Local)
		{
		}
		bool operator==(const Particle_Force_over_LifeTime& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Velocity_over_Lifetime)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		DUOLMath::Vector3 _force;

		Space _space;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _force;

			ar& _space;
		}
	};
	struct Particle_Color_over_Lifetime
	{
		/**
			@brief Cpu���� ���� ���� �̸� ���ؼ� ������ �ʴ� ������ ��ƼŬ���� ������Ÿ���� �ٸ��� �����̴�.
			gpu �ȿ��� ����� �� �ִ� ����� �����غ���.
		**/
		Particle_Color_over_Lifetime() : _useModule(false)
		{
			for (int i = 0; i < 8; i++)
			{
				_alpha_Ratio[i] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
				_color_Ratio[i] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			}


			_alpha_Ratio[0] = DUOLMath::Vector4(0.0f, 1.0f, 1.0f, 0.0f);
			_alpha_Ratio[1] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 0.2f);
			_alpha_Ratio[2] = DUOLMath::Vector4(0.5f, 1.0f, 1.0f, 0.4f);
			_alpha_Ratio[3] = DUOLMath::Vector4(0.1f, 1.0f, 1.0f, 0.6f);
			_alpha_Ratio[4] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			_color_Ratio[0] = DUOLMath::Vector4(1.0f, 0, 0, 0.0f);
			_color_Ratio[1] = DUOLMath::Vector4(1.0f, 1.0f, 0, 0.2f);
			_color_Ratio[2] = DUOLMath::Vector4(0, 0, 1.0f, 0.4f);
			_color_Ratio[3] = DUOLMath::Vector4(0, 1.0f, 1.0f, 0.6f);
			_color_Ratio[4] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);


		}
		bool operator==(const Particle_Color_over_Lifetime& other) const
		{
			for (int i = 0; i < 8; i++)
			{
				if (_alpha_Ratio[i] != other._alpha_Ratio[i])
					return false;

				if (_alpha_Ratio[i] != other._alpha_Ratio[i])
					return false;

			}
			if (_useModule != other._useModule)
				return false;
			return true;
		}
		bool _useModule;

		DUOLMath::Vector4 _alpha_Ratio[8];

		DUOLMath::Vector4 _color_Ratio[8];

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _alpha_Ratio;

			ar& _color_Ratio;
		}
	};
	struct Particle_Size_Over_Lifetime
	{
		Particle_Size_Over_Lifetime() :
			_useModule(false),
			_startSize(1), _endSize(1),
			_startOffset(0), _endOffset(0)
		{
		}
		bool operator==(const Particle_Size_Over_Lifetime& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Size_Over_Lifetime)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		float _startSize;
		float _endSize;
		float _startOffset;
		float _endOffset;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;
			ar& _startSize;
			ar& _endSize;
			ar& _startOffset;
			ar& _endOffset;
		}
	};
	struct Particle_Rotation_Over_Lifetime
	{
		Particle_Rotation_Over_Lifetime() :
			_useModule(false),
			_AngularVelocity(0)
		{
		}
		bool operator==(const Particle_Rotation_Over_Lifetime& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Rotation_Over_Lifetime)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		float _AngularVelocity;

		DUOLMath::Vector3 pad0;

		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _AngularVelocity;

			ar& pad0;
		}
	};
	struct Particle_Texture_Sheet_Animation
	{
		Particle_Texture_Sheet_Animation() :
			_useModule(false),
			_grid_XY{ 1, 1 },
			_timeMode(0)
		{
		}
		bool operator==(const Particle_Texture_Sheet_Animation& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Texture_Sheet_Animation)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		int _grid_XY[2];

		int _timeMode;

		float pad0;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _grid_XY;

			ar& _timeMode;

			ar& pad0;
		}
	};
	struct Particle_Noise
	{
		enum class Quality
		{
			Low,
			Middle,
			High
		};
		Particle_Noise() : _useModule(false),
			_strength(1.0f), _frequency(0.5f), _scrollSpeed(0), _damping(true), _octaves(1),
			_octaveMultiplier(0.5), _octaveScale(2), _quality(Quality::High), _positionAmount(1), _rotationAmount(0),
			_sizeAmount(0)
		{}
		bool operator==(const Particle_Noise& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Noise)) == 0)
				return true;
			else
				return false;
		}
		bool _useModule;

		float _strength;
		float _frequency;
		float _scrollSpeed;
		bool _damping;
		int _octaves;
		float _octaveMultiplier;
		float _octaveScale;
		Quality _quality;

		float _positionAmount;
		float _rotationAmount;
		float _sizeAmount;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;
			ar& _strength;
			ar& _frequency;
			ar& _scrollSpeed;
			ar& _damping;
			ar& _octaves;
			ar& _octaveMultiplier;
			ar& _octaveScale;
			ar& _quality;

			ar& _positionAmount;
			ar& _rotationAmount;
			ar& _sizeAmount;
		}
	};



	struct Particle_Collision
	{
		Particle_Collision() : _useModule(false),
			_height(0), _boundce(1), _lifeTimeLoss(0)
		{}
		bool operator==(const Particle_Collision& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Noise)) == 0)
				return true;
			else
				return false;
		}

	bool _useModule;

	float _height;

	float _boundce;

	float _lifeTimeLoss;

	float _pad;
	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _height;

			ar& _boundce;

			ar& _lifeTimeLoss;

			ar& _pad;
		}
	};
	struct Particle_Trails
	{
		Particle_Trails() :_useModule(false), _ratio(1.0f), _lifeTime(1.0f), _minimumVertexDistance(0.1f),
			_worldSpace(false), _dieWithParticle(false), _textureMode(TextureMode::Stretch),
			_sizeAffectsWidth(false), _sizeAffectsLifeTime(false),
			_inheritParticleColor(false), _colorOverLifeTime(DUOLMath::Vector4(1)), _widthOverTrail(0),
			_colorOverTrail(DUOLMath::Vector4(1)), _generateLightingData(false),
			_shadowBias(0)
		{
		}
		bool operator==(const Particle_Trails& other) const
		{
			if (memcmp(this, &other, sizeof(Particle_Trails)) == 0)
				return true;
			else
				return false;
		}
		enum class TextureMode
		{
			Stretch,
			Tile,
			DistributePerSegment,
			RepeatPerSegment

		};
		bool _useModule;

		float _ratio; // 0 ~ 1 ��
		float _lifeTime; // 0 ~ 1 ��
		float _minimumVertexDistance;
		bool _worldSpace;

		bool _dieWithParticle;
		TextureMode _textureMode;
		bool _sizeAffectsWidth;
		bool _sizeAffectsLifeTime;

		bool _inheritParticleColor;
		DUOLMath::Vector4 _colorOverLifeTime;
		float _widthOverTrail;
		DUOLMath::Vector4 _colorOverTrail;

		bool _generateLightingData;
		float _shadowBias;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _ratio;
			ar& _lifeTime;
			ar& _minimumVertexDistance;
			ar& _worldSpace;

			ar& _dieWithParticle;
			ar& _textureMode;
			ar& _sizeAffectsWidth;
			ar& _sizeAffectsLifeTime;

			ar& _inheritParticleColor;
			ar& _colorOverLifeTime;
			ar& _widthOverTrail;
			ar& _colorOverTrail;

			ar& _generateLightingData;
			ar& _shadowBias;
		}

	};
	struct Particle_Renderer
	{
		Particle_Renderer() :_useModule(true),
			_renderMode(RenderMode::Billboard), _blendState(BlendState::AdditiveState),
			_meshName(_T("")), _texturePath(_T("")), _traillTexturePath(_T("")),
			_renderAlignment(RenderAlignment::Local), _speedScale(0), _lengthScale(2)
		{
		}
		bool operator==(const Particle_Renderer& other) const
		{
			if (
				_useModule != other._useModule ||
				_renderMode != other._renderMode ||
				_blendState != other._blendState ||
				_meshName != other._meshName ||
				_texturePath != other._texturePath ||
				_traillTexturePath != other._traillTexturePath ||
				_renderAlignment != other._renderAlignment ||
				_speedScale != other._speedScale ||
				_lengthScale != other._lengthScale ||
				_masking != other._masking
				)
				return false;
			return true;
		}
		enum class RenderMode
		{
			Billboard,
			StretchedBillboard,
			HorizontalBillboard,
			VerticalBillboard,
			Mesh
		};
		enum class BlendState
		{
			AdditiveState,
			UiState,
		};
		enum class RenderAlignment
		{
			View,
			World,
			Local,
			Velocity
		};
		enum class Masking
		{
			NoMasking,
			VisibleInsideMask,
			OutsideMask
		};
		bool _useModule;

		RenderMode _renderMode;

		BlendState _blendState;

		tstring _meshName;

		tstring _texturePath;			// ��ƼŬ ����Ʈ�� �ؽ����� �ּ�.

		tstring _traillTexturePath;			// ��ƼŬ ����Ʈ�� �ؽ����� �ּ�.

		RenderAlignment _renderAlignment;

		float _speedScale;

		float _lengthScale;

		Masking _masking;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _useModule;

			ar& _renderMode;

			ar& _blendState;

			ar& _meshName;

			ar& _texturePath;

			ar& _traillTexturePath;

			ar& _renderAlignment;

			ar& _speedScale;

			ar& _lengthScale;

			ar& _masking;
		}
	};
	// ��ƼŬ �ý����� ����ϱ� ���� �������̽�
	// �� ���� ���� �ý����� �����Ѵ�.
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
			, shaderName(),
			_rasterizerState(RASTERIZER_STATE::SOLID)
		{}
		bool operator==(MuscleGrapics::RenderingData_Particle& other)
		{
			if (this->_commonInfo != other._commonInfo ||
				this->_emission != other._emission ||
				this->_shape != other._shape ||
				this->_velocity_Over_Lifetime != other._velocity_Over_Lifetime ||
				this->_force_Over_Lifetime != other._force_Over_Lifetime ||
				this->_color_Over_Lifetime != other._color_Over_Lifetime ||
				this->_size_Over_Lifetime != other._size_Over_Lifetime ||
				this->_rotation_Over_Lifetime != other._rotation_Over_Lifetime ||
				this->_texture_Sheet_Animaition != other._texture_Sheet_Animaition ||
				this->_noise != other._noise ||
				this->_collision != other._collision ||
				this->_trails != other._trails ||
				this->_renderer != other._renderer
				)
				return false;
			else
				return true;
		}
		//------------------------------------------------ ��� -------------------------------------------
		Particle_CommonInfo _commonInfo;

		Particle_Emission _emission;

		Particle_Shape _shape;

		Particle_Velocity_over_Lifetime _velocity_Over_Lifetime;

		Particle_Force_over_LifeTime _force_Over_Lifetime;

		Particle_Color_over_Lifetime _color_Over_Lifetime;

		Particle_Size_Over_Lifetime _size_Over_Lifetime;

		Particle_Rotation_Over_Lifetime _rotation_Over_Lifetime;

		Particle_Texture_Sheet_Animation _texture_Sheet_Animaition;

		Particle_Noise _noise;

		Particle_Collision _collision;

		Particle_Trails _trails;

		Particle_Renderer _renderer;
		//------------------------------------------------ ��� -------------------------------------------

		unsigned int _objectID; // ��ƼŬ ID ���ҽ� �Ŵ����� ������ ���̵�, ������Ʈ ID�� �������.

		std::vector<tstring> shaderName; // � ���̴��� ����ϴ���.

		std::vector<RenderingData_Particle> _childrens;

		bool _isDelete; // ��ƼŬ�� �� ��������� �Ҵ� ���� �ϱ� ����. ��ƼŬ�� ���ο��� ������Ʈ Ǯ ������ ���� ���ϴ� ������ ��ƼŬ���� ������ ũ�Ⱑ �ٸ��� �����̴�.

		RASTERIZER_STATE _rasterizerState;

	protected:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _commonInfo;

			ar& _emission;

			ar& _shape;

			ar& _velocity_Over_Lifetime;

			ar& _force_Over_Lifetime;

			ar& _color_Over_Lifetime;

			ar& _size_Over_Lifetime;

			ar& _rotation_Over_Lifetime;

			ar& _texture_Sheet_Animaition;

			ar& _noise;

			ar& _collision;

			ar& _trails;

			ar& _renderer;

			ar& _objectID;
			ar& shaderName;
			ar& _childrens;
			ar& _isDelete;
			ar& _rasterizerState;
		}
	};
#pragma endregion


#pragma region UI Rendering

	struct DrawInfo
	{
		DrawInfo() : _textureID(ULLONG_MAX), _sortOrder(5),
			_color(DUOLMath::Vector4(1.f, 1.f, 1.f, 1.f)), _useAlphaBlend(true)
		{
		}

		uint64 _textureID;

		uint32 _sortOrder;

		DUOLMath::Vector4 _color;

		bool _useAlphaBlend;
	};

	// NDC ���������� LeftTop, Right Bottom
	// (-1, 1)
	struct NDCInfo
	{
		DUOLMath::Vector2 _leftTop;

		DUOLMath::Vector2 _rightBottom;
	};

	// ȭ�� �ػ� ��ȭ�� ���� ��ǥ�� ������ �����ϱ� ���� NDC ���� ���
	struct RenderingData_UI
	{
		RenderingData_UI() : _drawInfo(std::make_shared<DrawInfo>()), _ndcInfo(std::make_shared<NDCInfo>())
		{}

		std::shared_ptr<DrawInfo> _drawInfo;

		std::shared_ptr<NDCInfo> _ndcInfo;
	};

	// ȭ�� �ػ󵵿� ���缭 ��Ŀ�� ���� �θ� �þ�� ���� �þ���Ѵ�.

	// ��ƼŬ ���̴� �������ϴϱ� ���� ���� ��Ÿ�� ����.
#pragma endregion


#pragma region Text Rendering
// �ؽ�Ʈ�� Ư���ϴϱ� ���� ������.
	struct TextData
	{
		TextData() : _fontID(7), _startCoord(DUOLMath::Vector2(10.f, 10.f))
			, _color(DUOLMath::Vector4(0.3f, 0.8f, 0.8f, 1.f)), _outText(TEXT("Text"))
		{ }

		uint64 _fontID;

		DUOLMath::Vector2 _startCoord;

		DUOLMath::Vector4 _color;

		tstring _outText;
	};
}

#pragma endregion