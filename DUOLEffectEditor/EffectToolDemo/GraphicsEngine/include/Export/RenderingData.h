#pragma once
#include "dllExport.h"
#include "include/util/STLInclude.h"
#include "include/util/TypeDefine.h"

#include "..\\..\\Library/inc/SimpleMath.h"

#pragma region PerFrameData
/**
 * \brief �� �����ӿ� �ѹ��� ������ ����
 */
namespace MuscleGrapics
{
	struct CameraInfo
	{

		CameraInfo() : _cameraWorldPosition(DirectX::SimpleMath::Vector3::Zero),
			_viewMatrix(DirectX::SimpleMath::Matrix::Identity),
			_projMatrix(DirectX::SimpleMath::Matrix::Identity),
			_prevViewMatrix(DirectX::SimpleMath::Matrix::Identity),
			_prevProjMatrix(DirectX::SimpleMath::Matrix::Identity)
		{}

		DirectX::SimpleMath::Vector3 _cameraWorldPosition;

	private:
		float _pad;

	public:
		DirectX::SimpleMath::Matrix _viewMatrix;

		DirectX::SimpleMath::Matrix _projMatrix;

		DirectX::SimpleMath::Matrix _prevViewMatrix;

		DirectX::SimpleMath::Matrix _prevProjMatrix;
	};

	struct LightInfo
	{
		LightInfo() : _color(DirectX::SimpleMath::Vector3(0.7f, 0.7f, 0.7f)), _lumen(1.f)
		{}

		DirectX::SimpleMath::Vector3 _color;

		float _lumen;
	};

	struct DirectionalLightInfo final : public LightInfo
	{
		DirectionalLightInfo() : LightInfo(), _direction(DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f)), _pad(0.f)
		{}

		// == In Game Engine, World Rotation Look Vector
		DirectX::SimpleMath::Vector3 _direction;

	private:
		float _pad;
	};

	struct PointLightInfo final : public LightInfo
	{
		PointLightInfo() : LightInfo(), _position(DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f)),
			_range(25.f)
		{}

		DirectX::SimpleMath::Vector3 _position;

		float _range;
	};

	struct SpotLightInfo final : public LightInfo
	{
		SpotLightInfo() : LightInfo(), _direction(DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f)), _halfAngle(3.141592f / 4),
			_position(DirectX::SimpleMath::Vector3::Zero), _range(25.f)
		{}

		DirectX::SimpleMath::Vector3 _direction;
		float _halfAngle;

		DirectX::SimpleMath::Vector3 _position;
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
		PerFrameData() : _cameraInfo(std::make_shared<CameraInfo>()),
			_directionalLightInfos(std::vector<std::shared_ptr<DirectionalLightInfo>>()),
			_spotLightInfos(std::vector<std::shared_ptr<SpotLightInfo>>()),
			_pointLightInfos(std::vector<std::shared_ptr<PointLightInfo>>()),
			_postProcessOption(static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA) | static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR))),
			_deltaTime(0.f)
		{}

		std::shared_ptr<CameraInfo> _cameraInfo;

		std::vector<std::shared_ptr<DirectionalLightInfo>> _directionalLightInfos;

		std::vector<std::shared_ptr<SpotLightInfo>> _spotLightInfos;

		std::vector<std::shared_ptr<PointLightInfo>> _pointLightInfos;

		POSTPROCESS_OPTION _postProcessOption;

		float _deltaTime;						// �ð��� �󸶳� �귶�°�
	};
#pragma endregion

#pragma region 3D Rendering

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
		MaterialInfo() : _diffuseMapID(ULLONG_MAX), _normalMapID(ULLONG_MAX), _metallicMapID(ULLONG_MAX),
			_roughnessMapID(ULLONG_MAX), _ambientOcclusionMapID(ULLONG_MAX), _emissiveMapID(ULLONG_MAX)
			, _metallic(1.f), _roughness(0.3f), _emissive(DirectX::SimpleMath::Vector4::Zero)
		{}

		float _metallic;

		float _roughness;

		DirectX::SimpleMath::Vector4 _emissive;

		uint64 _diffuseMapID;

		uint64 _normalMapID;

		uint64 _metallicMapID;

		uint64 _roughnessMapID;

		uint64 _ambientOcclusionMapID;

		uint64 _emissiveMapID;
	};

	struct ShaderInfo
	{
		enum class RASTERIZER_STATE
		{
			SOLID = 0,
			WIREFRAME = 1,
		};

		ShaderInfo() : _shaderName(TEXT("Wire")), _rasterizerState(RASTERIZER_STATE::SOLID)
		{}

		tstring _shaderName;

		RASTERIZER_STATE _rasterizerState;
	};

	struct GeometryInfo
	{
		GeometryInfo() : _world(DirectX::SimpleMath::Matrix::Identity), _worldInvTranspose(DirectX::SimpleMath::Matrix::Identity),
			_worldViewProj(DirectX::SimpleMath::Matrix::Identity), _texTransform(DirectX::SimpleMath::Matrix::Identity)
		{}

		DirectX::SimpleMath::Matrix _world;

		DirectX::SimpleMath::Matrix _worldInvTranspose;

		DirectX::SimpleMath::Matrix _worldViewProj;

		DirectX::SimpleMath::Matrix _texTransform;
	};

	struct AnimationInfo
	{
		AnimationInfo() :
			_boneMatrixList()
		{}

		DirectX::SimpleMath::Matrix _boneMatrixList[70];
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
			Curve, // Ŀ��� �ļ����� �̷���..!
			RandomBetweenTwoConstant,
			RandomBetweenTwoCurve // Ŀ��� �ļ����� �̷���..!
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
			_startSize{ DirectX::SimpleMath::Vector2(1.0f, 1.0f),DirectX::SimpleMath::Vector2(1.0f, 1.0f) },
			_startRotationOption(Option_Particle::Constant),
			_startRotation{ 0,0 },
			_startColorOption(Option_Particle::Constant),
			_startColor{ DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f,1.0f),DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f,1.0f) },
			_gravirtModifierOption(Option_Particle::Constant),
			_gravityModifier{ 0.0f,0.0f },
			_maxParticles(1000),
			_transformMatrix(DirectX::SimpleMath::Matrix::Identity),
			_refTextureID(0)
		{}
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

		DirectX::SimpleMath::Vector2 _startSize[2];				// ��ƼŬ�� ���� ũ��.								O							O

		Option_Particle _startRotationOption;//													X							X

		float _startRotation[2];			// ��ƼŬ�� ���� ȸ��.								X							O

		Option_Particle _startColorOption;//													X							X

		DirectX::SimpleMath::Vector4 _startColor[2];			// ��ƼŬ�� ���� ����										X							O

		Option_Particle _gravirtModifierOption;//												X							X

		float _gravityModifier[2];			// ��ƼŬ�� �������� �߷�.							O							O

		int _maxParticles;				// ��ƼŬ �ִ� ��� ������.								X							O
		// ContantBuffer 16���� �� ��� �Ϸ�..

		DirectX::SimpleMath::Matrix _transformMatrix;		// ��ƼŬ�� ���� ��ġ �� ���� ����							X							X

		void* _refTextureID;			// ��ƼŬ ����Ʈ�� ����ϴ� ���̵�

		float _playTime;				// play ������ �帥 �ð�.

	};
	struct Particle_Emission
	{
		Particle_Emission() : _emissiveCount(1), _emissiveTime(0.1f)
		{
		}

		int _emissiveCount;			// �ѹ��� ��� ���� ��ų��.

		float _emissiveTime;			// ���� ������� �ɸ��� �ð�.
	};
	struct Particle_Color_over_Lifetime
	{
		Particle_Color_over_Lifetime() : _startColor(1.0f, 1.0f, 1.0f, 1.0f), _endColor(1.0f, 1.0f, 1.0f, 1.0f)
		{
		}

		DirectX::SimpleMath::Vector4 _startColor;

		DirectX::SimpleMath::Vector4 _endColor;
	};
	struct Particle_Velocity_over_Lifetime
	{
		Particle_Velocity_over_Lifetime() : _linearVelocity(0, 0, 0)
		{
		}

		DirectX::SimpleMath::Vector3 _linearVelocity;
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
	};
	struct Particle_Rotation_Over_Lifetime
	{
		Particle_Rotation_Over_Lifetime() :
			_AngularVelocity(0)
		{
		}
		float _AngularVelocity;
	};
	// ��ƼŬ �ý����� ����ϱ� ���� �������̽�
	// �� ���� ���� �ý����� �����Ѵ�.
	struct RenderingData_Particle
	{
		RenderingData_Particle() :
			_commonInfo(std::make_shared<Particle_CommonInfo>()),
			_emission(std::make_shared<Particle_Emission>()),
			_color_Over_Lifetime(std::make_shared<Particle_Color_over_Lifetime>()),
			_velocity_Over_Lifetime(std::make_shared<Particle_Velocity_over_Lifetime>()),
			_size_Over_Lifetime(std::make_shared<Particle_Size_Over_Lifetime>()),
			_rotation_Over_Lifetime(std::make_shared<Particle_Rotation_Over_Lifetime>()),
			_texture_Sheet_Animaition(std::make_shared<Particle_Texture_Sheet_Animation>()),
			_objectID(0)
			, shaderName(TEXT("BasicParticle"))
		{}

		std::shared_ptr<Particle_CommonInfo> _commonInfo;

		std::shared_ptr<Particle_Emission> _emission;

		std::shared_ptr<Particle_Color_over_Lifetime> _color_Over_Lifetime;

		std::shared_ptr<Particle_Velocity_over_Lifetime> _velocity_Over_Lifetime;

		std::shared_ptr<Particle_Size_Over_Lifetime> _size_Over_Lifetime;

		std::shared_ptr<Particle_Rotation_Over_Lifetime> _rotation_Over_Lifetime;

		std::shared_ptr<Particle_Texture_Sheet_Animation> _texture_Sheet_Animaition;

		unsigned int _objectID; // ��ƼŬ ID ���ҽ� �Ŵ����� ������ ���̵�

		tstring shaderName; // ��ƼŬ ID ���ҽ� �Ŵ����� ������ ���̵�
	};
#pragma endregion


#pragma region UI Rendering

	struct DrawInfo
	{
		DrawInfo() : _textureID(ULLONG_MAX), _sortOrder(5),
			_color(DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f)), _useAlphaBlend(true)
		{
		}

		uint64 _textureID;

		uint32 _sortOrder;

		DirectX::SimpleMath::Vector4 _color;

		bool _useAlphaBlend;
	};

	// NDC ���������� LeftTop, Right Bottom
	// (-1, 1)
	struct NDCInfo
	{
		DirectX::SimpleMath::Vector2 _leftTop;

		DirectX::SimpleMath::Vector2 _rightBottom;
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
		TextData() : _fontID(7), _startCoord(DirectX::SimpleMath::Vector2(10.f, 10.f))
			, _color(DirectX::SimpleMath::Vector4(0.3f, 0.8f, 0.8f, 1.f)), _outText(TEXT("Text"))
		{ }

		uint64 _fontID;

		DirectX::SimpleMath::Vector2 _startCoord;

		DirectX::SimpleMath::Vector4 _color;

		tstring _outText;
	};
}

#pragma endregion