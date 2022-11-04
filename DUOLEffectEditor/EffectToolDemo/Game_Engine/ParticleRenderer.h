#pragma once

namespace Muscle
{
	class ParticleRenderer : public IComponents
	{
	public:
		ParticleRenderer(std::shared_ptr<GameObject> _GameObject);

		~ParticleRenderer();

	private:
		std::shared_ptr<Muscle::Transform> _transform;

		std::shared_ptr<RenderingData_Particle> _particleData;

		bool _isFirstUpdate;

	public:
		// 필살기 Getter
		inline std::shared_ptr<RenderingData_Particle> GetParticleData() { return _particleData; }

	public:
		// Getter & Setter
		void SetParticleType(PARTICLE_EFFECT_TYPE type) { _particleData->_initInfo->_particleEffectType = type; }

		const PARTICLE_EFFECT_TYPE& GetParticleType() { return _particleData->_initInfo->_particleEffectType; }
		
		const uint32& GetMaxParticleCount() { return _particleData->_initInfo->_maxParticleCount; }

		void SetMaxParticleCount(const uint32& maxCount) { _particleData->_initInfo->_maxParticleCount = maxCount; }

		const Vector3& GetEmitVelocity() { return _particleData->_shapeInfo->_emitVelocity; }

		void SetEmitVelocity(const Vector3& velocity) { _particleData->_shapeInfo->_emitVelocity = velocity; }
	
		const float& GetRadius() { return _particleData->_shapeInfo->_radius; }

		void SetRadius(const float& radius) { _particleData->_shapeInfo->_radius = radius; }

		const float& GetEmitTime() { return  _particleData->_shapeInfo->_emitTime; }

		void SetEmitTime(const float& time) { _particleData->_shapeInfo->_emitTime = time; }

		const float& GetPeriod() { return _particleData->_shapeInfo->_period; }

		void SetPeriod(const float& period) { _particleData->_shapeInfo->_period = period; }

		const Vector2& GetParticleAverageSize() { return _particleData->_shapeInfo->_particleAverageSize; }

		void SetParticleAverageSize(const Vector2& size) { _particleData->_shapeInfo->_particleAverageSize = size; }

		const uint64& GetParticleTextureID() { return _particleData->_shapeInfo->_refTextureID; }

		void SetParticleTextureID(const uint64& refID) { _particleData->_shapeInfo->_refTextureID = refID; }

		const Vector3& GetAcceleration() { return _particleData->_shapeInfo->_acceleration; }

		void SetAcceleration(const Vector3& acceleration) { _particleData->_shapeInfo->_acceleration = acceleration; }

		const float& GetLifeSpan() { return _particleData->_shapeInfo->_lifeSpan; }

		void SetLifeSpan(const float& lifeSpan) { _particleData->_shapeInfo->_lifeSpan = lifeSpan; }

		bool GetIsLoop() { return _particleData->_initInfo->_isLoop; }

		void SetIsLoop(bool value) { _particleData->_initInfo->_isLoop = value; }

		// Component, 모델 별의 차이를 극복할 수 있도록 오프셋 매트릭스를 둔다. (보통 회전만 쓰일텐데 .. 매트릭스로 놔두자)
		const Matrix& GetOffsetMatrix() { return _offsetMatrix; }

		void SetOffsetMatrix(const Matrix& offsetMatrix) { _offsetMatrix = offsetMatrix; }

	public:
		// 달려있는 오브젝트에서 어느 정도의 오프셋에서 Transform을 할 것입니까.
		Matrix _offsetMatrix;

		// 플레이 중입니까 ?
		bool _isPlay;

		// 파티클 이펙트 시작
		void Play();

		// 파티클 이펙트 멈춤
		void Stop();
			
	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;
	};
}