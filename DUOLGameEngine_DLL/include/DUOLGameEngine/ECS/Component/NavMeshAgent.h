#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/Navigation/DetourCrowd/DetourCrowd.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	/**
	 * \brief �׺���̼� �޽� �ý����� ����Ͽ� �̵��� ������ ��ü�� �����ϴ� ������Ʈ�Դϴ�.
	 */
	class DUOL_GAMEENGINE_API NavMeshAgent : public DUOLGameEngine::BehaviourBase
	{
	public:
		NavMeshAgent(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("NavMeshAgent"));

		virtual ~NavMeshAgent() override;

	private:
		/**
		 * \brief The relative vertical displacement of the owning GameObject. (���� ������Ʈ�� ���߿��� ���� �����̴� ��ó�� ���̴� ���¸� �����ϱ� ���� ����Ѵ�.)
		 */
		DUOLMath::Vector3 _baseOffset;

		/**
		 * \brief Agent's radius used in obstacle avoidance.
		 */
		float _radius;

		/**
		 * \brief Height uisng in obstacle avoidance.
		 */
		float _height;

		/**
		 * \brief Agent's max acceleration.
		 */
		float _maxAcceleration;

		/**
		 * \brief Agent's max speed.
		 */
		float _maxSpeed;

		/**
		 * \brief Using obstacle avoidance.
		 */
		bool _obstacleAvoidance;

		/**
		 * \brief 0 ~ 8������ �ο� ����
		 */
		unsigned char _obstacleAvoidanceType;

		/**
		 * \brief Using separation property.
		 */
		bool _separation;

		/**
		 * \brief How aggresive the agent manager should be at avoiding collisions with this agent.
		 */
		float _separationWeight;

		/**
		 * \brief Use acticipate turns in steering behavior.
		 */
		bool _anticipateTurns;

		/**
		 * \brief Use #dtPathCorridor::optimizePathVisibility() to optimize the agent path.
		 */
		bool _optimizeVisibility;

		/**
		 * \brief Use dtPathCorridor::optimizePathTopology() to optimize the agent path.
		 */
		bool _optimizeTopology;

		/**
		 * \brief Recast-Detour primitive agent.
		 */
		const dtCrowdAgent* _primitiveAgent;

		/**
		 * \brief Recast-Detour primitive agent's index.
		 */
		int _primitiveAgentIndex;

		/**
		 * \brief Recast - Detour target Poly's ref.
		 */
		dtPolyRef _destinationRef;

		/**
		 * \brief Destination Position. (Read Only)
		 */
		float _destinationPos[3];

	private:
		// TODO : �츮�� FBX Exporter�� �޾ƿ� �� �����ؼ� �޾ƿ��ϱ� .. �ٽ� �����ؼ� �Ѱ��ֱ� ���� �� ..!
		DUOLMath::Vector3 ConvertForFBXBinaryExporter(float x, float y, float z) const;

		/**
		 * \brief ���� ������Ʈ �Ķ���͸� �̿��� �ùķ��̼ǿ� ���� ������ �����մϴ�.
		 */
		void UpdateAgentParameters();

	public:
		const DUOLMath::Vector3& GetBaseOffset() const;

		void SetBaseOffset(const DUOLMath::Vector3& baseOffset);

		float GetRadius() const;

		void SetRadius(float value);

		float GetHeight() const;

		void SetHeight(float value);

		float GetMaxAcceleration() const;

		void SetMaxAcceleration(float value);

		float GetMaxSpeed() const;

		void SetMaxSpeed(float value);

		unsigned char GetObstacleAvoidanceType() const;

		void SetObstacleAvoidanceType(unsigned char value);

		float GetSeparationWeight() const;

		void SetSeparationWeight(float value);

		bool GetAnticipateTurns() const;

		void SetAnticipateTurns(bool value);

		bool GetOptimizeVisibility() const;

		void SetOptimizeVisibility(bool value);

		bool GetOptimizeTopology() const;

		void SetOptimizeTopology(bool value);

		bool GetSeparation() const;

		void SetSeparation(bool value);

		bool GetObstacleAvoidance() const;

		void SetObstacleAvoidance(bool value);

		// TODO : �̰ɷ� ������Ʈ�� ������ �����ų� �ϰ� �����ٵ� ..
		DUOLMath::Vector3 GetVelocity() const;

		bool SetVelocity(const DUOLMath::Vector3& newVelo);

		DUOLMath::Vector3 GetDestination() const;
		
		bool SetDestination(const DUOLMath::Vector3& dest);

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

#pragma region FRIEND_CLASS
		friend class NavigationManager;
#pragma endregion
	};
}