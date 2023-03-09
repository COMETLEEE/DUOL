#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/Navigation/DetourCrowd/DetourCrowd.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 네비게이션 메쉬 시스템을 사용하여 이동을 제어할 객체를 서술하는 컴포넌트입니다.
	 */
	class DUOL_GAMEENGINE_API NavMeshAgent : public DUOLGameEngine::BehaviourBase
	{
	public:
		NavMeshAgent(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("NavMeshAgent"));

		virtual ~NavMeshAgent() override;

	private:
		/**
		 * \brief The relative vertical displacement of the owning GameObject. (게임 오브젝트가 공중에서 떠서 움직이는 것처럼 보이는 상태를 방지하기 위해 사용한다.)
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
		 * \brief 0 ~ 8까지만 부여 가능
		 */
		unsigned char _obstacleAvoidanceType;

		/**
		 * \brief 해당 에이전트의 
		 */
		float _separationWeight;


		/**
		 * \brief 
		 */
		bool _anticipateTurns;

		/**
		 * \brief 
		 */
		bool _optimizeVis;

		/**
		 * \brief 
		 */
		bool _optimizeTopo;

		/**
		 * \brief Using separation property.
		 */
		bool _separation;

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

		float _destinationPos[3];

	private:
		// TODO : 우리가 FBX Exporter에 받아올 때 수정해서 받아오니까 .. 다시 수정해서 넘겨주기 위한 것 ..!
		DUOLMath::Vector3 ConvertForFBXBinaryExporter(float x, float y, float z) const;

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

		bool GetOptimizeVis() const;

		void SetOptimizeVis(bool value);

		bool GetOptimizeTopo() const;

		void SetOptimizeTopo(bool value);

		bool GetSeparation() const;

		void SetSeparation(bool value);

		// TODO : 이걸로 오브젝트의 방향을 돌리거나 하고 싶을텐데 ..
		DUOLMath::Vector3 GetVelocity() const;






		
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