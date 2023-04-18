/**

    @file      QueryFilterCallback.h
    @brief     PhysX scene query filtering logic for DUOL Engine.
    @details   ~
    @author    COMETLEE
    @date      18.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include <PxQueryFiltering.h>

namespace DUOLPhysics
{
	using namespace physx;

	/**
	 * \brief Scene query filtering callback function.
	 */
	class QueryFilterCallback : public physx::PxQueryFilterCallback
	{
	public:
		virtual PxQueryHitType::Enum preFilter(
			const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) = 0;

		/**
		\brief This filter callback is executed if the exact intersection test returned true and PxQueryFlag::ePOSTFILTER flag was set.

		\param[in] filterData custom filter data of the query
		\param[in] hit Scene query hit information. faceIndex member is not valid for overlap queries. For sweep and raycast queries the hit information can be cast to #PxSweepHit and #PxRaycastHit respectively.
		\return the updated hit type for this hit  (see #PxQueryHitType)
		*/
		virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) = 0;
	};
}