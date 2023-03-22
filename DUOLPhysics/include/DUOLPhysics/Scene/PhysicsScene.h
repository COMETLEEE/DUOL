/**

	@file    PhysicsScene.h
	@brief	 Physics 연산하는 Scene 공간
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
		@brief	 Physics 연산하는 Scene 공간
		@details -

	**/
	class PhysicsScene
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsScene 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene 클래스 default 소멸자
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
			@brief	 Plane 생성
			@details -
			@param   keyName   - Plane의 Name
			@param   material  - Plane에 적용될 Material
			@param   PlaneDesc - Plane 생성에 필요한 값
			@retval  생성된 Plane 객체
		**/
		std::weak_ptr<PhysicsPlane> CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& PlaneDesc);

		/**
			@brief	 Static Actor 생성
			@details -
			@param   keyName    - Static Actor의 Name
			@param   staticDesc - Static Actor 생성에 필요한 값
			@retval  생성된 Static Actor 객체
		**/
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc);

		/**
			@brief	 Static Actor 생성
			@details 생성과 동시에 Shape를 같이 생성하고 부착
			@tparam  T          - 생성하고 부착할 Shape Type
			@param   keyName    - Static Actor의 Name
			@param   staticDesc - Static Actor 생성에 필요한 값
			@param   shapeDesc  - Shape 생성에 필요한 값
			@retval  생성된 Static Actor 객체
		**/
		template<class T>
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Dynamic Actor 생성
			@details -
			@param   keyName     - Dynamic Actor의 Name
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@retval  생성된 Dynamic Actor 객체
		**/
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc);

		/**
			@brief	 Dynamic Actor 생성
			@details 생성과 동시에 Shape를 같이 생성하고 부착
			@tparam  T           - 생성하고 부착할 Shape Type
			@param   keyName     - Dynamic Actor의 Name
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@param   shapeDesc   - Shape 생성에 필요한 값
			@retval  생성된 Dynamic Actor 객체
		**/
		template<class T>
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Plane 제거
			@details -
			@param   keyName - Plane의 Name
			@retval  Plane이 성공적으로 제거되었으면 true 아니면 false
		**/
		bool DestroyPlane(const tstring& keyName);

		/**
			@brief	 Static Actor 제거
			@details -
			@param   keyName - Actor의 Name
			@retval  Static Actor가 성공적으로 제거되었으면 true 아니면 false
		**/
		bool DestroyStaticActor(const tstring& keyName);

		/**
			@brief	 Dynamic Actor 제거
			@details -
			@param   keyName - Actor의 Name
			@retval  Dynamic Actor가 성공적으로 제거되었으면 true 아니면 false
		**/
		bool DestroyDynamicActor(const tstring& keyName);

		/**
			@brief	 Scene의 Collider Vertex 정보가 담긴 구조체를 반환
			@details -
			@retval  SceneDebugData(vertexBuffer, size)
		**/
		const SceneDebugData GetRenderBuffer();

		/**
			@brief	 Scene에서 Raycast 검사
			@details -
			@param   position    - Ray 출발 지점
			@param   direction   - Ray 진행 방향
			@param   maxDistance - 최대 거리
			@retval  Raycast 결과
		**/
		bool Raycast(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outRaycastHit);

		/**
		 * \brief Scene 에서 Raycast 검사. 해당 광선이 지나갈 때 모든 정보를 가져옵니다.
		 * \param position 
		 * \param direction 
		 * \param maxDistance 
		 * \param outRaycastHit 
		 * \return 
		 */
		bool RaycastAll(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance, std::vector<DUOLPhysics::RaycastHit>& outRaycastHits);

		/**
		 * \brief Scene 에서 Spherecast 검사합니다. 
		 * \param origin 
		 * \param radius 
		 * \param direction 
		 * \param maxDinstance 
		 * \param outSpherecastHit 
		 * \return 
		 */
		bool Spherecast(const DUOLMath::Vector3& origin, float radius, const DUOLMath::Vector3& direction, float maxDistance, DUOLPhysics::RaycastHit& outSpherecastHit);

		/**
		 * \brief Scene 에서 Spherecast 검사합니다. 해당 구가 지나갈 때 모든 정보를 가져옵니다.
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
		 * \brief Scene 에서 사용되는 Gravity 반환.
		 * \return Gravity.
		 */
		DUOLMath::Vector3 GetGravity();

		/**
			@brief	 Scene에서 생성된 Actor간의 연산을 진행
			@details -
			@param   deltaTime - Frame 사이의 흐른 시간
		**/
		void Simulate(float deltaTime);

		/**
			@brief	 Scene 할당 해제
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