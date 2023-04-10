/**

	@file    PhysicsShapeBase.h
	@brief	 Shape���� �θ� Ŭ����
	@details Shape���� ��� �����ϰ� ������ ������ ���� ��� ���� ���
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
		@brief	 Shape���� �θ� Ŭ����
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
			@brief   PhysicsShapeBase Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsShapeBase();

		/**
			@brief   PhysicsShapeBase Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsShapeBase();

	protected:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Shape ����
			@details -
			@param   system    - System�� ���Ե� Physics�� ���� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Shape ����
			@details Scene���� �����ϰ� �ٷ� Actor�� ���� �� ������
			@param   scene    - Scene�� ���Ե� Physics�� ���� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		virtual void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Trigger On/Off
			@details Trigger�� Ȱ��ȭ �Ǹ� Simulation Flag�� ��Ȱ��ȭ �ȴ�.
			@param   enable - True�� Ȱ��ȭ, false�� ��Ȱ��ȭ
		**/
		void SetTriggerEnable(bool enable);

		/**
			@brief	 SceneQuery On/Off
			@details -
			@param   enable - True�� Ȱ��ȭ, false�� ��Ȱ��ȭ
		**/
		void SetSceneQueryEnable(bool enable);

		/**
			@brief	 Shape�� Local Transform Getter
			@details -
			@retval  Physics Pose
		**/
		PhysicsPose GetLocalPose() const;

		/**
			@brief	 Local Transform Setter
			@details -
			@param   position - position ��
		**/
		void SetLocalPose(const DUOLMath::Vector3& position);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   quat - quaternion ��
		**/
		void SetLocalPose(const DUOLMath::Quaternion& quat);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   transform - transform Matrix ��
		**/
		void SetLocalPose(const DUOLMath::Matrix& transform);

		/**
			@brief	 Local Transform Setter
			@details -
			@param   globalPose - position�� quaternion ��
		**/
		void SetLocalPose(const PhysicsPose& globalPose);


		/**
			@brief	 Material Setter
			@details -
			@param   material - ������ Material
		**/
		void SetMaterial(const std::weak_ptr<PhysicsMaterial>& material);

		/**
		 * \brief �������� ���Ϳ��� �����Ǿ����� ������ �Ǵ��մϴ�.
		 * \return ���͸� ������ �ֳ���.
		 */
		bool HasActor() const;

		/**
		 * \brief 
		 * \param layer ����ϴ� ���̾�
		 */
		void SetSimulationLayer(const DUOLCommon::tstring& layer);

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		virtual void Release() final;
	};
}
