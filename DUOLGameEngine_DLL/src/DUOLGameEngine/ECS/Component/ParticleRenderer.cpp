#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLCommon/Log/LogHelper.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	//rttr::registration::class_<DUOLGraphicsEngine::RenderingData_Particle>("RenderingData_Particle")
	//.constructor()
	//(
	//rttr::policy::ctor::as_raw_ptr
	//).property("_objectID",&DUOLGraphicsEngine::RenderingData_Particle::_objectID)
	//(
	//	metadata(DUOLCommon::MetaDataType::Serializable, true)
	//	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	//	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	//);

	rttr::registration::class_<DUOLGameEngine::ParticleRenderer>("ParticleRenderer")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_particleInitData",&DUOLGameEngine::ParticleRenderer::_particleInitData)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true));

}

namespace DUOLGameEngine
{
	ParticleRenderer::ParticleRenderer(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
		, _particleInfo(),
		_prevMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	{

		_renderObjectInfo._renderInfo = &_particleInfo;

		_isPlay = false;

		_isDelayStart = false;

		_isFirstRun = false;
	}

	ParticleRenderer::~ParticleRenderer()
	{
	}


	void ParticleRenderer::SetQuatAndScale()
	{
		DUOLMath::Matrix world = GetTransform()->GetWorldMatrix();// 월트 메트릭스

		// --------------------------------- Set_QuatAndScale ----------------------------------------------
		// ------------------------- 회전 혹은 스케일에 영향을 받는 옵션들. -----------------------------

		auto prevTemp = _prevMatrix;


		if (world != _prevMatrix)
		{
			// 파티클 관련 버그를 고쳐야한다...! 주말 동안 고치자...!

			world.m[3][0] = 0; world.m[3][1] = 0; world.m[3][2] = 0;
			prevTemp.m[3][0] = 0; prevTemp.m[3][1] = 0; prevTemp.m[3][2] = 0;

			DUOLMath::Vector3 s;
			DUOLMath::Quaternion r;
			DUOLMath::Vector3 t; // t는 사용하지 않음.

			world.Decompose(s, r, t);

			_particleInfo._particleData._commonInfo.gStartSize[0] = _particleInitData->_commonInfo._startSize[0].x * s.x;
			_particleInfo._particleData._commonInfo.gStartSize[1] = _particleInitData->_commonInfo._startSize[0].y * s.y;
			_particleInfo._particleData._commonInfo.gStartSize[2] = _particleInitData->_commonInfo._startSize[1].x * s.x;
			_particleInfo._particleData._commonInfo.gStartSize[3] = _particleInitData->_commonInfo._startSize[1].y * s.y;

			if (_particleInitData->_velocity_Over_Lifetime._space == DUOLGraphicsEngine::Space::Local)
				_particleInfo._particleData._velocityoverLifetime.gVelocity = DUOLMath::Vector3::Transform(_particleInitData->_velocity_Over_Lifetime._linearVelocity, world);

			if (_particleInitData->_force_Over_Lifetime._space == DUOLGraphicsEngine::Space::Local)
				_particleInfo._particleData._forceoverLifetime.gForce = DUOLMath::Vector3::Transform(_particleInitData->_force_Over_Lifetime._force, world);


			//auto& shape = _particleInitData->_shape;

			//DUOLMath::Matrix shapeTM = DUOLMath::Matrix::CreateScale(shape._scale) *
			//	DUOLMath::Matrix::CreateFromYawPitchRoll(shape._rotation.z, shape._rotation.x, shape._rotation.y) *
			//	DUOLMath::Matrix::CreateTranslation(shape._position);

			//world = shapeTM * GetTransform()->GetWorldMatrix();

			world = GetTransform()->GetWorldMatrix();

			memcpy(&_particleInfo._particleData._commonInfo.gTransformMatrix, &world, sizeof(DUOLMath::Matrix));

			_particleInfo._particleData._commonInfo.gDeltaMatrix = _prevMatrix.Invert() * world;
		}
		else
		{
			world = GetTransform()->GetWorldMatrix();

			memcpy(&_particleInfo._particleData._commonInfo.gTransformMatrix, &world, sizeof(DUOLMath::Matrix));

			_particleInfo._particleData._commonInfo.gDeltaMatrix = DUOLMath::Matrix::Identity;
		}

		_prevMatrix = GetTransform()->GetWorldMatrix();
	}

	void ParticleRenderer::OnUpdate(float deltaTime)
	{

	}

	void ParticleRenderer::OnLateUpdate(float deltaTime)
	{


		//_particleInfo._particleData._isDelete = GetGameObject()->GetIsDelete(); // 원래는 그래픽스 엔진에서도 버퍼를 삭제할 때 확인 용도 였지만, 경민 그래픽스는 어떻게 할지 모르겠다.
		// 실행 중일 때만 정보를 업데이트한다.
		if (_isPlay)
		{

			_delayTime += deltaTime ;
			if (_delayTime <= _particleInfo._particleData._commonInfo.gStartDelay[0])
				return;

			//파티클 데이터를 받아올때. 갱신 하세요 + 그 머시기 타임까지
			if (_particleInfo._particleData._commonInfo.gMaxParticles % 1024 == 0)
				_particleInfo._particleData._dim = _particleInfo._particleData._commonInfo.gMaxParticles / 1024;
			else
				_particleInfo._particleData._dim = _particleInfo._particleData._commonInfo.gMaxParticles / 1024 + 1;

			_particleInfo._particleData._dim = static_cast<int>(ceil(pow((double)_particleInfo._particleData._dim, double(1.0 / 3.0))));

			if (_particleInfo._particleData._EmissionTime >= _particleInfo._particleData._emission.gEmissiveTime)
				_particleInfo._particleData._EmissionTime = 0;

			_particleInfo._particleData._EmissionTime += deltaTime * _particleInfo._particleData._commonInfo.gSimulationSpeed;

			_particleInfo._particleData._emission.gEmissiveCount = DUOLMath::MathHelper::RandF(_particleInitData->_emission._emissiveCount[0], _particleInitData->_emission._emissiveCount[1]);

			_isDelayStart = true;

			_playTime += deltaTime ;

			SetQuatAndScale();

			_particleInfo._particleData._commonInfo.gParticlePlayTime = _playTime;

			_particleInfo._isComputed = false;

			if (!_isFirstRun)
				_particleInfo._particleData._EmissionTime = std::numeric_limits<float>::max();

		}
	}

	void ParticleRenderer::OnStart()
	{
		if (!_renderObjectInfo._mesh)
		{
			CreateParticleBuffer();
		}
		Play();
	}

	void ParticleRenderer::Render()
	{
		if (_materials.empty())
			return;

		if (_isPlay && _isDelayStart)
		{
			GraphicsManager::GetInstance()->ReserveRenderObject(&_renderObjectInfo);

			_isFirstRun = true;
		}
	}

	void ParticleRenderer::Play()
	{
		Stop();

		_isPlay = true;

		_isFirstRun = false;

		_playTime = 0;

		_delayTime = 0;

		_isDelayStart = false;

		_particleInfo._particleData._flag |= 1 << 28;

		for (auto iter : GetGameObject()->GetTransform()->GetChildGameObjects())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Play();
		}
	}

	void ParticleRenderer::Stop()
	{
		_isPlay = false;
		for (auto iter : GetGameObject()->GetTransform()->GetChildGameObjects())
		{
			auto renderer = iter->GetComponent<ParticleRenderer>();
			if (renderer)
				renderer->Stop();
		}
	}

	bool ParticleRenderer::GetIsPlay() const
	{
		if (!GetIsEnabled())
			return false;

		return _isPlay;
	}

	DUOLGraphicsEngine::ConstantBuffDesc::CB_PerObject_Particle& ParticleRenderer::GetParticleData()
	{
		return _particleInfo._particleData;
	}

	void ParticleRenderer::CreateParticleBuffer()
	{
		if (_materials.empty() || _renderObjectInfo._mesh)
			return;

		auto pathStr = _materials[0]->GetName();

		auto indexStr = _materials[0]->GetName();

		auto pos = pathStr.find(TEXT(".dfx"));

		pathStr.resize(pos + 4);

		indexStr = indexStr.substr(pos + 4);

		auto index = std::stoi(indexStr);

		auto particleDataVector = ResourceManager::GetInstance()->LoadRenderingData_Particle(pathStr);

		if (!particleDataVector)
		{
			DUOL_TRACE(DUOL_CONSOLE, "no particleData. Name: {0}", DUOLCommon::StringHelper::ToString(pathStr));

			_renderObjectInfo._mesh = nullptr;

			while (!_materials.empty())
				DeleteBackMaterial();
		}

		_particleInitData = (*particleDataVector)[index].get();

		_particleInfo = *_particleInitData;

		_particleInfo._objectID = GetGameObject()->GetUUID();

		const auto& _graphicsEngine = DUOLGameEngine::GraphicsManager::GetInstance()->_graphicsEngine;
		auto gameObject = GetGameObject();
		auto uuid = gameObject->GetUUID();
		DUOLCommon::tstring str = _T("Particle") + DUOLCommon::StringHelper::ToTString(uuid);

		_renderObjectInfo._mesh = _graphicsEngine->CreateParticle(
			str,
			_particleInfo._particleData._commonInfo.gMaxParticles);
		_renderObjectInfo._materials = &_primitiveMaterials;

		_renderObjectInfo._renderInfo = &_particleInfo;
	}
}
