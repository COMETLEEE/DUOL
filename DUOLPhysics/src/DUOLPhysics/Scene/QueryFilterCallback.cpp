#include "QueryFilterCallback.h"

#include <PxShape.h>

namespace DUOLPhysics
{
	PxQueryHitType::Enum QueryFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape,
		const PxRigidActor* actor, PxHitFlags& queryFlags)
	{
		PX_UNUSED(actor);
		PX_UNUSED(queryFlags);
		// PX_UNUSED(filterData);
		// PX_UNUSED(shape);

		PxFilterData shapeFilterData = shape->getQueryFilterData();

		// 타겟 레이어가 아니라면 비충돌
		if (filterData.word1 != shapeFilterData.word0)
			return PxQueryHitType::eNONE;

		return PxQueryHitType::eBLOCK;
	}

	PxQueryHitType::Enum QueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
	{
		PX_UNUSED(filterData);

		PX_UNUSED(hit);

		return PxQueryHitType::eBLOCK;
	}
}
