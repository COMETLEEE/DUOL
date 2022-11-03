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
	PhysicsActorBase::Impl::Impl() :
		_actor(nullptr)
	{

	}

	PhysicsActorBase::Impl::~Impl()
	{

	}

	void PhysicsActorBase::Impl::SetActor(PxRigidActor* actor)
	{
		_actor = actor;
	}

	GlobalPose PhysicsActorBase::Impl::GetGlobalPose()
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get global pose. (No Actor.)");

		return ConvertTransform(_actor->getGlobalPose());
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Vector3& worldPosition)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		_actor->setGlobalPose(PxTransform(ConvertVector3(worldPosition)));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Quaternion& quat)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		_actor->setGlobalPose(PxTransform(ConvertQuaternion(quat)));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const DUOLMath::Matrix& transform)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		_actor->setGlobalPose(ConvertTransform(transform));
	}

	void PhysicsActorBase::Impl::SetGlobalPose(const GlobalPose& globalPose)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to set global pose. (No Actor.)");

		PxTransform transform(ConvertVector3(globalPose._position), ConvertQuaternion(globalPose._quaternion));

		_actor->setGlobalPose(transform);
	}

	PhysicsBoundingBox PhysicsActorBase::Impl::GetBoundingBox(float inflation)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to get bounding box. (No Actor.)");

		auto boundingBox = _actor->getWorldBounds(inflation);

		PhysicsBoundingBox retBox;

		retBox._min = ConvertVector3(boundingBox.minimum);
		retBox._max = ConvertVector3(boundingBox.maximum);

		return retBox;
	}

	void PhysicsActorBase::Impl::AttachShape(PxShape* shape)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to attach shape. (No Actor.)");

		if (shape == nullptr)
			ERROR_THROW("Failed to attach shape. (No PxShape.)");

		_actor->attachShape(*shape);
	}

	void PhysicsActorBase::Impl::DetachShape(PxShape* shape, bool isWakeOnLostTouch)
	{
		if (_actor == nullptr)
			ERROR_THROW("Failed to attach shape. (No Actor.)");

		if (shape == nullptr)
			ERROR_THROW("Failed to attach shape. (No PxShape.)");

		_actor->detachShape(*shape, isWakeOnLostTouch);
	}
}