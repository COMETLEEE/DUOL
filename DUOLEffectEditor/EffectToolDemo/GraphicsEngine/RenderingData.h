#pragma once

#include "Typedef.h"

#pragma region PerFrameData
/**
 * \brief 한 프레임에 한번만 보내는 정보
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

	float _deltaTime;						// 시간이 얼마나 흘렀는가
};
#pragma endregion

#pragma region 3D Rendering

struct ObjectInfo
{
	ObjectInfo() : _meshID(0), _objectID(0), _usingLighting(true), _usingShadow(true)
	{}

	uint64 _meshID;

	// 게임
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

	//PARTICLE_EFFECT_TYPE _particleEffectType;	// 그리는데 사용하는 쉐이더가 다를 것 같고 ..
	// 모델이 달라지면 Particle 구조체 또한 달라질 수 있다.

	//bool _firstRun;					// 시작인가요 ..?

	//bool _isLoop;					// 계속 반복하나요 ? => False이면 

	//float _duration;				// 지속 시간 (isLoop False이면 이 시간만큼만 진행 후 삭제

	//uint32 _maxParticleCount;		// 최대 파티클의 갯수

	//uint64 _particlePlayID;			// First Run을 할 때 부여받음. 이 녀석으로 Curr와 연결됨.
};

// 초기 속도, 초기 위치, 상수 가속도 (시간에 대한 함수도 가능할지도 .. ?!) 에 의해서 결정되는 모듈
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
	bool _firstRun;					// 시작인가요 ..?										O							X

	float _duration;				// 몇 초 동안 파티클 객체가 재생될 지.						X							X

	bool _looping;					// 반복여부.												X							X

	Option_Particle _startDelayOption;	//													X							X

	float _startDelay[2];				// 몇 초 뒤에 파티클이 재생될 지.						X							X

	Option_Particle _startLifeTimeOption;	//												X							X

	float _startLifeTime[2];				// 한 파티클의 생존 시간.							X							X

	Option_Particle _startSpeedOption;//													X							X

	float _startSpeed[2];				// 파티클 생성시 시작 속도.							X							X

	Option_Particle _startSizeOption;//														X							X

	Vector2 _startSize[2];				// 파티클의 시작 크기.								X							X

	Option_Particle _startRotationOption;//													X							X

	float _startRotation[2];			// 파티클의 시작 회전.								X							X

	Option_Particle _startColorOption;//													X							X

	Vector3 _startColor[2];			// 파티클의 시작 색상										X							X

	Option_Particle _gravirtModifierOption;//												X							X

	float _gravityModifier[2];			// 파티클에 가해지는 중력.							X							X

	UINT _maxParticles;				// 파티클 최대 출력 사이즈.								X							X




	Matrix _transformMatrix;		// 파티클의 생성 위치 및 각을 조정							X							X

	void* _refTextureID;			// 파티클 이펙트가 사용하는 아이디




	float _emissiveCount;			// 한번에 몇개를 방출 시킬지.

	float _emissiveTime;			// 다음 방출까지 걸리는 시간.

};

// 파티클 시스템을 사용하기 위한 인터페이스
// 한 개의 입자 시스템을 묘사한다.
struct RenderingData_Particle
{
	RenderingData_Particle() : _initInfo(std::make_shared<Particle_InitInfo>()), _commonInfo(std::make_shared<Particle_CommonInfo>()), _objectID(0)
		, shaderName(TEXT("BasicParticle"))
	{}

	std::shared_ptr<Particle_InitInfo> _initInfo;

	std::shared_ptr<Particle_CommonInfo> _commonInfo;

	unsigned int _objectID; // 파티클 ID 리소스 매니저에 맵핑한 아이디

	tstring shaderName; // 파티클 ID 리소스 매니저에 맵핑한 아이디
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

// NDC 공간에서의 LeftTop, Right Bottom
// (-1, 1)
struct NDCInfo
{
	Vector2 _leftTop;

	Vector2 _rightBottom;
};

// 화면 해상도 변화에 대해 좌표에 빠르게 대응하기 위해 NDC 공간 사용
struct RenderingData_UI
{
	RenderingData_UI() : _drawInfo(std::make_shared<DrawInfo>()), _ndcInfo(std::make_shared<NDCInfo>())
	{}

	std::shared_ptr<DrawInfo> _drawInfo;

	std::shared_ptr<NDCInfo> _ndcInfo;
};

// 화면 해상도에 맞춰서 앵커에 따라서 부모가 늘어나면 나도 늘어나야한다.

// 파티클 쉐이더 만들어야하니까 빠른 개발 메타로 간다.
#pragma endregion


#pragma region Text Rendering
// 텍스트는 특수하니까 따로 보내라.
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