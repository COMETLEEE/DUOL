#include "PhysicsShapeBaseImpl.h"

/* Actor */
#include "../Actor/PhysicsDynamicActorImpl.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"
#include "DUOLPhysics/Util/PhysicsDefines.h"

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

	void PhysicsShapeBase::SetTriggerEnable(bool enable)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to get Local Pose. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			if (enable == true)
			{
				shape->setFlag(PxShapeFlag::Enum::eSIMULATION_SHAPE, false);
				shape->setFlag(PxShapeFlag::Enum::eTRIGGER_SHAPE, true);
			}
			else
			{
				shape->setFlag(PxShapeFlag::Enum::eTRIGGER_SHAPE, false);
				shape->setFlag(PxShapeFlag::Enum::eSIMULATION_SHAPE, true);
			}
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsShapeBase::SetSceneQueryEnable(bool enable)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to get Local Pose. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setFlag(PxShapeFlag::Enum::eSCENE_QUERY_SHAPE, enable);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	PhysicsPose PhysicsShapeBase::GetLocalPose() const
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return PhysicsPose{};
	}

	void PhysicsShapeBase::SetLocalPose(const DUOLMath::Vector3& position)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set Local Pose. (No Implementation.)");

			auto shape = _impl->GetShape();

			if (shape == nullptr)
				ERROR_THROW("Failed to set scale. (No PxShape.)");

			shape->setLocalPose(PxTransform(ConvertVector3(position)));
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsShapeBase::SetLocalPose(const PhysicsPose& globalPose)
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
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	void PhysicsShapeBase::SetMaterial(const std::weak_ptr<PhysicsMaterial>& material)
	{
		try
		{
			if (_impl == nullptr)
				ERROR_THROW("Failed to set material. (No Implementation.)");

			_impl->SetMaterial(material);
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}
	}

	bool PhysicsShapeBase::HasActor() const
	{
		if (_impl != nullptr)
			return _impl->HasActor();

		return false;
	}

	void PhysicsShapeBase::SetSimulationLayer(const DUOLCommon::tstring& layer)
	{
		if (_impl != nullptr)
			_impl->SetSimulationLayer(layer);
	}

	void PhysicsShapeBase::Release()
	{
		if (_impl != nullptr)
			_impl->Release();
	}
}