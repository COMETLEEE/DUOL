/**

	@file    PhysicsScene.h
	@brief	 Physics �����ϴ� Scene ����
	@details -
	@author  JKim
	@date    24.10.2022

**/
#pragma once
/* Actor */
#include "../Actor/PhysicsStaticActor.h"
#include "../Actor/PhysicsDynamicActor.h"

/* Shapes */
#include "../Shapes/PhysicsBox.h"
#include "../Shapes/PhysicsMesh.h"
#include "../Shapes/PhysicsPlane.h"
#include "../Shapes/PhysicsCapsule.h"
#include "../Shapes/PhysicsConvexMesh.h"

/* Material */
#include "../Material/PhysicsMaterial.h"

/* etc */
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLCommon/StringHelper.h"
#include "../Util/PhysicsDefines.h"
#include "../Util/PhysicsDescriptions.h"
#include "../Util/PhysicsDataStructure.h"

#include <map>
#include <memory>
#include <iostream>

namespace DUOLPhysics
{
	using namespace DUOLCommon;

	class PhysicsSystem;

	/**

		@class   PhysicsScene
		@brief	 Physics �����ϴ� Scene ����
		@details -

	**/
	class PhysicsScene
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsScene Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsScene() = default;

	private:
		std::shared_ptr<Impl> _impl;

		std::map<tstring, std::shared_ptr<PhysicsPlane>> _planes;

		std::map<tstring, std::shared_ptr<PhysicsStaticActor>> _staticActors;

		std::map<tstring, std::shared_ptr<PhysicsDynamicActor>> _dynamicActors;

	public:
		void SetRenderBufferOption(RenderBufferOption option, float value);

		/**
			@brief	 Plane ����
			@details -
			@param   keyName   - Plane�� Name
			@param   material  - Plane�� ����� Material
			@param   PlaneDesc - Plane ������ �ʿ��� ��
			@retval  ������ Plane ��ü
		**/
		std::weak_ptr<PhysicsPlane> CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& PlaneDesc);

		/**
			@brief	 Static Actor ����
			@details -
			@param   keyName    - Static Actor�� Name
			@param   staticDesc - Static Actor ������ �ʿ��� ��
			@retval  ������ Static Actor ��ü
		**/
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc);

		/**
			@brief	 Static Actor ����
			@details ������ ���ÿ� Shape�� ���� �����ϰ� ����
			@tparam  T          - �����ϰ� ������ Shape Type
			@param   keyName    - Static Actor�� Name
			@param   staticDesc - Static Actor ������ �ʿ��� ��
			@param   shapeDesc  - Shape ������ �ʿ��� ��
			@retval  ������ Static Actor ��ü
		**/
		template<class T>
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Dynamic Actor ����
			@details -
			@param   keyName     - Dynamic Actor�� Name
			@param   dynamicDesc - Dynamic Actor ������ �ʿ��� ��
			@retval  ������ Dynamic Actor ��ü
		**/
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc);

		/**
			@brief	 Dynamic Actor ����
			@details ������ ���ÿ� Shape�� ���� �����ϰ� ����
			@tparam  T           - �����ϰ� ������ Shape Type
			@param   keyName     - Dynamic Actor�� Name
			@param   dynamicDesc - Dynamic Actor ������ �ʿ��� ��
			@param   shapeDesc   - Shape ������ �ʿ��� ��
			@retval  ������ Dynamic Actor ��ü
		**/
		template<class T>
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Plane ����
			@details -
			@param   keyName - Plane�� Name
			@retval  Plane�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyPlane(const tstring& keyName);

		/**
			@brief	 Static Actor ����
			@details -
			@param   keyName - Actor�� Name
			@retval  Static Actor�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyStaticActor(const tstring& keyName);

		/**
			@brief	 Dynamic Actor ����
			@details -
			@param   keyName - Actor�� Name
			@retval  Dynamic Actor�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyDynamicActor(const tstring& keyName);

		/**
			@brief	 Scene�� Collider Vertex ������ ��� ����ü�� ��ȯ
			@details -
			@retval  SceneDebugData(vertexBuffer, size)
		**/
		const SceneDebugData GetRenderBuffer();

		/**
			@brief	 Scene���� Raycast �˻�
			@details -
			@param   position    - Ray ��� ����
			@param   direction   - Ray ���� ����
			@param   maxDistance - �ִ� �Ÿ�
			@retval  Raycast ���
		**/
		bool Raycast(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outRaycastHit);

		/**
		 * \brief Scene ���� Raycast �˻�. �ش� ������ ������ �� ��� ������ �����ɴϴ�.
		 * \param position 
		 * \param direction 
		 * \param maxDistance 
		 * \param outRaycastHit 
		 * \return 
		 */
		bool RaycastAll(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outRaycastHits);

		/**
		 * \brief Scene ���� Spherecast �˻��մϴ�. 
		 * \param origin 
		 * \param radius 
		 * \param direction 
		 * \param maxDinstance 
		 * \param outSpherecastHit 
		 * \return 
		 */
		bool Spherecast(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outSpherecastHit);

		/**
		 * \brief Scene ���� Spherecast �˻��մϴ�. �ش� ���� ������ �� ��� ������ �����ɴϴ�.
		 * \param origin 
		 * \param radius 
		 * \param direcetion 
		 * \param maxDistance 
		 * \param outSpherecastHit 
		 * \return 
		 */
		bool SpherecastAll(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction, float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outSpherecastHit);

		bool Boxcast(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outBoxcastHit);

		bool BoxcastAll(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation, const DUOLMath::Vector3& direction, float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outBoxcastHit);

		bool CheckBox(const DUOLMath::Vector3& center, const DUOLMath::Vector3& halfExtents, const DUOLMath::Quaternion& rotation);

		bool CheckSphere(const DUOLMath::Vector3& center, float radius);;

		// bool CheckCapsule(const DUOLMath::Vector3& start, const DUOLMath::Vector3& end, float radius);

		/*bool OverlapBox();

		bool OverlapSphere();

		bool OverlapCapsule();*/
		
		/**
		 * \brief Scene ���� ���Ǵ� Gravity ��ȯ.
		 * \return Gravity.
		 */
		DUOLMath::Vector3 GetGravity();

		/**
			@brief	 Scene���� ������ Actor���� ������ ����
			@details -
			@param   deltaTime - Frame ������ �帥 �ð�
		**/
		void Simulate(float deltaTime);

		/**
			@brief	 Scene �Ҵ� ����
			@details -
		**/
		void Release();
	};

	template<class T>
	inline std::weak_ptr<PhysicsStaticActor> PhysicsScene::CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc, const PhysicsShapeDesc& shapeDesc)
	{
		static_assert(std::is_base_of<PhysicsShapeBase, T>::value, "Shape must inherit PhysicsShapeBase.");

		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _staticActors.find(keyName);

			if (result != _staticActors.end())
				return std::dynamic_pointer_cast<T>(result->second);

			auto newShape = std::make_shared<T>();
			newShape->Create(this, shapeDesc);

			auto newActor = CreateStaticActor(keyName, staticDesc);

			newActor->AttachShape(newShape);

			return newActor;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}

	template<class T>
	inline std::weak_ptr<PhysicsDynamicActor> PhysicsScene::CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc, const PhysicsShapeDesc& shapeDesc)
	{
		static_assert(std::is_base_of<PhysicsShapeBase, T>::value, "Shape must inherit PhysicsShapeBase.");

		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _dynamicActors.find(keyName);

			if (result != _dynamicActors.end())
				return std::dynamic_pointer_cast<T>(result->second);

			auto newShape = std::make_shared<T>();
			newShape->Create(this, shapeDesc);

			auto newActor = CreateDynamicActor(keyName, dynamicDesc);

			newActor->AttachShape(newShape);

			return newActor;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}
}