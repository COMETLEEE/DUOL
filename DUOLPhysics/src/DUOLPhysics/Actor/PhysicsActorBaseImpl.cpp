#include "PhysicsActorBaseImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

#include <string>

#define ERROR_THROW(errStr)				\
{										\
	std::string errTemp = errStr;		\
	errTemp += " / File : ";			\
	errTemp += __FILE__;				\
	errTemp += ", Line : ";				\
	errTemp += std::to_string(__LINE__);\
	throw errTemp;						\
}

namespace DUOLPhysics
{
	PhysicsActorBase::Impl::Impl()
	{

	}

	PhysicsActorBase::Impl::~Impl()
	{

	}

	GlobalPose PhysicsActorBase::Impl::GetGlobalPose()
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to get global pose. (No Actor.)");

		return ConvertTransform(actor->getGlobalPose());
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Vector3& worldPosition)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		actor->setGlobalPose(PxTransform(ConvertVector3(worldPosition)));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Quaternion& quat)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		actor->setGlobalPose(PxTransform(ConvertQuaternion(quat)));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Matrix& transform)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		actor->setGlobalPose(ConvertTransform(transform));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const GlobalPose& globalPose)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		PxTransform transform(ConvertVector3(globalPose._position), ConvertQuaternion(globalPose._quaternion));

		actor->setGlobalPose(transform);
	}

	PhysicsBoundingBox PhysicsActorBase::Impl::GetBoundingBox(float inflation)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to get bounding box. (No Actor.)");

		auto boundingBox = actor->getWorldBounds(inflation);

		PhysicsBoundingBox retBox;

		retBox._min = ConvertVector3(boundingBox.minimum);
		retBox._max = ConvertVector3(boundingBox.maximum);

		return retBox;
	}

	void PhysicsActorBase::Impl::AttachShape(PxShape* shape)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to attach shape. (No Actor.)");

		if (shape == nullptr)
			ERROR_THROW("Failed to attach shape. (No PxShape.)");

		actor->attachShape(*shape);
	}

	void PhysicsActorBase::Impl::DetachShape(PxShape* shape, bool isWakeOnLostTouch)
	{
		auto actor = GetActor();

		if (actor == nullptr)
			ERROR_THROW("Failed to attach shape. (No Actor.)");

		if (shape == nullptr)
			ERROR_THROW("Failed to attach shape. (No PxShape.)");

		actor->detachShape(*shape, isWakeOnLostTouch);
	}
}