#pragma once

#include "Typedef.h"

#pragma region PerFrameData
/**
 * \brief �� �����ӿ� �ѹ��� ������ ����
 */
struct CameraInfo
{
	CameraInfo() : _cameraWorldPosition(Vector3::Zero), _viewMatrix(Matrix::Identity), _projMatrix(Matrix::Identity),
		_prevViewMatrix(Matrix::Identity), _prevProjMatrix(Matrix::Identity)
	{}

	Vector3 _cameraWorldPosition;

private:
	float _pad;

public:
	Matrix _viewMatrix;

	Matrix _projMatrix;

	Matrix _prevViewMatrix;

	Matrix _prevProjMatrix;
};

struct LightInfo
{
	LightInfo() : _color(Vector3(0.7f, 0.7f, 0.7f)), _lumen(1.f)
	{}

	Vector3 _color;

	float _lumen;
};

struct DirectionalLightInfo final : public LightInfo
{
	DirectionalLightInfo() : LightInfo(), _direction(Vector3(1.f, 0.f, 0.f)), _pad(0.f)
	{}

	// == In Game Engine, World Rotation Look Vector
	Vector3 _direction;

private:
	float _pad;
};

struct PointLightInfo final : public LightInfo
{
	PointLightInfo() : LightInfo(), _position(Vector3(0.f, 0.f, 0.f)),
		_range(25.f)
	{}

	Vector3 _position;

	float _range;
};

struct SpotLightInfo final : public LightInfo
{
	SpotLightInfo() : LightInfo(), _direction(Vector3(1.f, 0.f, 0.f)), _halfAngle(3.141592f / 4),
		_position(Vector3::Zero), _range(25.f)
	{}

	Vector3 _direction;
	float _halfAngle;

	Vector3 _position;
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
		, _metallic(1.f), _roughness(0.3f), _emissive(Vector4::Zero)
	{}

	float _metallic;

	float _roughness;

	Vector4 _emissive;

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
	GeometryInfo() : _world(Matrix::Identity), _worldInvTranspose(Matrix::Identity),
		_worldViewProj(Matrix::Identity), _texTransform(Matrix::Identity)
	{}

	Matrix _world;

	Matrix _worldInvTranspose;

	Matrix _worldViewProj;

	Matrix _texTransform;
};

struct AnimationInfo
{
	AnimationInfo() :
		_boneMatrixList()
	{}

	Matrix _boneMatrixList[70];
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

struct Particle_InitInfo
{
	Particle_InitInfo() //: /*_particleEffectType(PARTICLE_EFFECT_TYPE::CIRCLE), _firstRun(true), _isLoop(true),
		//_duration(5.f), _particlePlayID(ULLONG_MAX), _maxParticleCount(500)*/
	{}

	//PARTICLE_EFFECT_TYPE _particleEffectType;	// �׸��µ� ����ϴ� ���̴��� �ٸ� �� ���� ..
	// ���� �޶����� Particle ����ü ���� �޶��� �� �ִ�.

	//bool _firstRun;					// �����ΰ��� ..?

	//bool _isLoop;					// ��� �ݺ��ϳ��� ? => False�̸� 

	//float _duration;				// ���� �ð� (isLoop False�̸� �� �ð���ŭ�� ���� �� ����

	//uint32 _maxParticleCount;		// �ִ� ��ƼŬ�� ����

	//uint64 _particlePlayID;			// First Run�� �� �� �ο�����. �� �༮���� Curr�� �����.
};

// �ʱ� �ӵ�, �ʱ� ��ġ, ��� ���ӵ� (�ð��� ���� �Լ��� ���������� .. ?!) �� ���ؼ� �����Ǵ� ���
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
		_startSize{ Vector2(1.0f, 1.0f),Vector2(1.0f, 1.0f) },
		_startRotationOption(Option_Particle::Constant),
		_startRotation{ 0,0 },
		_startColorOption(Option_Particle::Constant),
		_startColor{ Vector3(1.0f, 1.0f, 1.0f),Vector3(1.0f, 1.0f, 1.0f) },
		_gravirtModifierOption(Option_Particle::Constant),
		_gravityModifier{ 0.0f,0.0f },
		_maxParticles(1000),
		_transformMatrix(Matrix::Identity),
		_refTextureID(0),
		_emissiveCount(1),
		_emissiveTime(0.01f)
	{}
	//																					ShaderCode					   ImGui						
	bool _firstRun;					// �����ΰ��� ..?										O							X

	float _duration;				// �� �� ���� ��ƼŬ ��ü�� ����� ��.						X							X

	bool _looping;					// �ݺ�����.												X							X

	Option_Particle _startDelayOption;	//													X							X

	float _startDelay[2];				// �� �� �ڿ� ��ƼŬ�� ����� ��.						X							X

	Option_Particle _startLifeTimeOption;	//												X							X

	float _startLifeTime[2];				// �� ��ƼŬ�� ���� �ð�.							X							X

	Option_Particle _startSpeedOption;//													X							X

	float _startSpeed[2];				// ��ƼŬ ������ ���� �ӵ�.							X							X

	Option_Particle _startSizeOption;//														X							X

	Vector2 _startSize[2];				// ��ƼŬ�� ���� ũ��.								X							X

	Option_Particle _startRotationOption;//													X							X

	float _startRotation[2];			// ��ƼŬ�� ���� ȸ��.								X							X

	Option_Particle _startColorOption;//													X							X

	Vector3 _startColor[2];			// ��ƼŬ�� ���� ����										X							X

	Option_Particle _gravirtModifierOption;//												X							X

	float _gravityModifier[2];			// ��ƼŬ�� �������� �߷�.							X							X

	UINT _maxParticles;				// ��ƼŬ �ִ� ��� ������.								X							X




	Matrix _transformMatrix;		// ��ƼŬ�� ���� ��ġ �� ���� ����							X							X

	void* _refTextureID;			// ��ƼŬ ����Ʈ�� ����ϴ� ���̵�




	float _emissiveCount;			// �ѹ��� ��� ���� ��ų��.

	float _emissiveTime;			// ���� ������� �ɸ��� �ð�.

};

// ��ƼŬ �ý����� ����ϱ� ���� �������̽�
// �� ���� ���� �ý����� �����Ѵ�.
struct RenderingData_Particle
{
	RenderingData_Particle() : _initInfo(std::make_shared<Particle_InitInfo>()), _commonInfo(std::make_shared<Particle_CommonInfo>()), _objectID(0)
		, shaderName(TEXT("BasicParticle"))
	{}

	std::shared_ptr<Particle_InitInfo> _initInfo;

	std::shared_ptr<Particle_CommonInfo> _commonInfo;

	unsigned int _objectID; // ��ƼŬ ID ���ҽ� �Ŵ����� ������ ���̵�

	tstring shaderName; // ��ƼŬ ID ���ҽ� �Ŵ����� ������ ���̵�
};
#pragma endregion


#pragma region UI Rendering

struct DrawInfo
{
	DrawInfo() : _textureID(ULLONG_MAX), _sortOrder(5),
		_color(Vector4(1.f, 1.f, 1.f, 1.f)), _useAlphaBlend(true)
	{
	}

	uint64 _textureID;

	uint32 _sortOrder;

	Vector4 _color;

	bool _useAlphaBlend;
};

// NDC ���������� LeftTop, Right Bottom
// (-1, 1)
struct NDCInfo
{
	Vector2 _leftTop;

	Vector2 _rightBottom;
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
	TextData() : _fontID(7), _startCoord(Vector2(10.f, 10.f))
		, _color(Vector4(0.3f, 0.8f, 0.8f, 1.f)), _outText(TEXT("Text"))
	{ }

	uint64 _fontID;

	Vector2 _startCoord;

	Vector4 _color;

	tstring _outText;
};

#pragma endregion