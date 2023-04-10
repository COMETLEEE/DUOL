/**

	@file    PhysicsShapeBase.h
	@brief	 Shape들의 부모 클래스
	@details Shape들을 묶어서 관리하고 다형적 생성을 위해 상속 구조 사용
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../Util/PhysicsDescriptions.h"

/* etc */
#include "../Util/PhysicsDataStructure.h"

#include <memory>

#include "DUOLPhysics/Util/CollisionLayer.h"

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsActorBase;

	/**

		@class   PhysicsShapeBase
		@brief	 Shape들의 부모 클래스
		@details -

	**/
	class PhysicsShapeBase
	{
	protected:
		class Impl;

	private:
		friend PhysicsSystem;
		friend PhysicsActorBase;

	protected:
		/**
			@brief   PhysicsShapeBase 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsShapeBase();

		/**
			@brief   PhysicsShapeBase 클래스 소멸자
			@details -
		**/
		~PhysicsShapeBase();

	protected:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Shape 생성
			@details -
			@param   system    - System에 포함된 Physics를 쓰기 위해 전달
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Shape 생성
			@details Scene에서 생성하고 바로 Actor에 부착 후 해제됨
			@param   scene    - Scene에 포함된 Physics를 쓰기 위해 전달
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		virtual void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Trigger On/Off
			@details Trigger가 활성화 되면 Simulation Flag는 비활성화 된다.
			@param   enable - True면 활성화, false면 비활성화
		**/
		void SetTriggerEnable(bool enable);

		/**
			@brief	 SceneQuery On/Off
			@details -
			@param   enable - True면 활성화, false면 비활성화
		**/
		void SetSceneQueryEnable(bool enable);

		/**
			@brief	 Shape의 Local Transform Getter
			@details -
			@retval  Physics Pose
		**/
		PhysicsPose GetLocalPose() const;

		/**
			@brief	 Local Transform Setter
			@details -
			@param   position - position 값
		**/
		void SetLocalPose(const DUOLMath::Vector3& position);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   quat - quaternion 값
		**/
		void SetLocalPose(const DUOLMath::Quaternion& quat);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   transform - transform Matrix 값
		**/
		void SetLocalPose(const DUOLMath::Matrix& transform);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   globalPose - position과 quaternion 값
		**/
		void SetLocalPose(const PhysicsPose& globalPose);


		/**
			@brief	 Material Setter
			@details -
			@param   material - 피직스 Material
		**/
		void SetMaterial(const std::weak_ptr<PhysicsMaterial>& material);

		/**
		 * \brief 쉐이프가 액터에게 소유되었는지 유무를 판단합니다.
		 * \return 액터를 가지고 있나요.
		 */
		bool HasActor() const;

		/**
		 * \brief 
		 * \param layer 사용하는 레이어
		 */
		void SetSimulationLayer(const DUOLCommon::tstring& layer);

		/**
			@brief	 Shape 할당 해제
			@details -
		**/
		virtual void Release() final;
	};
}
