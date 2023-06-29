#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif
#include <PxQueryFiltering.h>


/**
 * \brief PhysicsSimulationFilterCallback for Trigger lost
 */
namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsSimulationFilterCallback : public PxSimulationFilterCallback
	{
	public:
		/**
		\brief Filter method to specify how a pair of potentially colliding objects should be processed.

		This method gets called when the filter flags returned by the filter shader (see #PxSimulationFilterShader)
		indicate that the filter callback should be invoked (#PxFilterFlag::eCALLBACK or #PxFilterFlag::eNOTIFY set).
		Return the PxFilterFlag flags and set the PxPairFlag flags to define what the simulation should do with the given
		collision pair.

		\param[in] pairID Unique ID of the collision pair used to issue filter status changes for the pair (see #statusChange())
		\param[in] attributes0 The filter attribute of the first object
		\param[in] filterData0 The custom filter data of the first object
		\param[in] a0 Actor pointer of the first object
		\param[in] s0 Shape pointer of the first object (NULL if the object has no shapes)
		\param[in] attributes1 The filter attribute of the second object
		\param[in] filterData1 The custom filter data of the second object
		\param[in] a1 Actor pointer of the second object
		\param[in] s1 Shape pointer of the second object (NULL if the object has no shapes)
		\param[in,out] pairFlags In: Pair flags returned by the filter shader. Out: Additional information on how an accepted pair should get processed
		\return Filter flags defining whether the pair should be discarded, temporarily ignored or processed and whether the pair
		should be tracked and send a report on pair deletion through the filter callback

		@see PxSimulationFilterShader PxFilterData PxFilterObjectAttributes PxFilterFlag PxPairFlag
		*/
		virtual PxFilterFlags pairFound(PxU32 pairID,
			PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0,
			PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1,
			PxPairFlags& pairFlags) override;

		/**
		\brief Callback to inform that a tracked collision pair is gone.

		This method gets called when a collision pair disappears or gets re-filtered. Only applies to
		collision pairs which have been marked as filter callback pairs (#PxFilterFlag::eNOTIFY set in #pairFound()).

		\param[in] pairID Unique ID of the collision pair that disappeared
		\param[in] attributes0 The filter attribute of the first object
		\param[in] filterData0 The custom filter data of the first object
		\param[in] attributes1 The filter attribute of the second object
		\param[in] filterData1 The custom filter data of the second object
		\param[in] objectRemoved True if the pair was lost because one of the objects got removed from the scene

		@see pairFound() PxSimulationFilterShader PxFilterData PxFilterObjectAttributes
		*/
		virtual void pairLost(PxU32 pairID,
			PxFilterObjectAttributes attributes0,
			PxFilterData filterData0,
			PxFilterObjectAttributes attributes1,
			PxFilterData filterData1,
			bool objectRemoved) override;

		/**
		\brief Callback to give the opportunity to change the filter state of a tracked collision pair.

		This method gets called once per simulation step to let the application change the filter and pair
		flags of a collision pair that has been reported in #pairFound() and requested callbacks by
		setting #PxFilterFlag::eNOTIFY. To request a change of filter status, the target pair has to be
		specified by its ID, the new filter and pair flags have to be provided and the method should return true.

		\note If this method changes the filter status of a collision pair and the pair should keep being tracked
		by the filter callbacks then #PxFilterFlag::eNOTIFY has to be set.

		\note The application is responsible to ensure that this method does not get called for pairs that have been
		reported as lost, see #pairLost().

		\param[out] pairID ID of the collision pair for which the filter status should be changed
		\param[out] pairFlags The new pairFlags to apply to the collision pair
		\param[out] filterFlags The new filterFlags to apply to the collision pair
		\return True if the changes should be applied. In this case the method will get called again. False if
		no more status changes should be done in the current simulation step. In that case the provided flags will be discarded.

		@see pairFound() pairLost() PxFilterFlag PxPairFlag
		*/
		virtual bool statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags) override;
	};
}