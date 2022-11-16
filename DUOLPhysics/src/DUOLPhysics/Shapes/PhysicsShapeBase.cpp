#include "PhysicsShapeBaseImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

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
	PhysicsShapeBase::PhysicsShapeBase() :
		_impl(std::make_shared<Impl>())
	{

	}

	PhysicsShapeBase::~PhysicsShapeBase()
	{
		Release();
	}

	GlobalPose PhysicsShapeBase::GetLocalPose() const
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to get Local Pose. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			return ConvertTransform(shape->getLocalPose());
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return GlobalPose{};
	}

	void PhysicsShapeBase::SetLocalPose(const DUOLMath::Vector3& worldPosition)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set Local Pose. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setLocalPose(PxTransform(ConvertVector3(worldPosition)));
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsShapeBase::SetLocalPose(const DUOLMath::Quaternion& quat)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set scale. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setLocalPose(PxTransform(ConvertQuaternion(quat)));
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsShapeBase::SetLocalPose(const DUOLMath::Matrix& transform)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set scale. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setLocalPose(PxTransform(ConvertTransform(transform)));
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsShapeBase::SetLocalPose(const GlobalPose& globalPose)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set scale. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setLocalPose(PxTransform(ConvertVector3(globalPose._position), ConvertQuaternion(globalPose._quaternion)));
		}
		catch (const std::string& errStr)
		{
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}
	}

	void PhysicsShapeBase::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}