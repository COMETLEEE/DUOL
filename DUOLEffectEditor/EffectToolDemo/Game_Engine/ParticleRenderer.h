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
		// �ʻ�� Getter
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

		// Component, �� ���� ���̸� �غ��� �� �ֵ��� ������ ��Ʈ������ �д�. (���� ȸ���� �����ٵ� .. ��Ʈ������ ������)
		const Matrix& GetOffsetMatrix() { return _offsetMatrix; }

		void SetOffsetMatrix(const Matrix& offsetMatrix) { _offsetMatrix = offsetMatrix; }

	public:
		// �޷��ִ� ������Ʈ���� ��� ������ �����¿��� Transform�� �� ���Դϱ�.
		Matrix _offsetMatrix;

		// �÷��� ���Դϱ� ?
		bool _isPlay;

		// ��ƼŬ ����Ʈ ����
		void Play();

		// ��ƼŬ ����Ʈ ����
		void Stop();
			
	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;
	};
}