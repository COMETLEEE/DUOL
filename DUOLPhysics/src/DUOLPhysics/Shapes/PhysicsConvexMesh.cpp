#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsConvexMesh.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Shape */
#include "PhysicsShapeBaseImpl.h"

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
	PhysicsConvexMesh::PhysicsConvexMesh()
	{

	}

	PhysicsConvexMesh::~PhysicsConvexMesh()
	{

	}

	void PhysicsConvexMesh::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No Implementation.)");

		auto physics = _impl->GetPhysics(system);

		if (physics == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxPhysics.)");

		auto cooking = _impl->GetCooking(system);

		if (cooking == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxCooking.)");

		PxConvexMeshDesc meshDesc;
		meshDesc.setToDefault();
		meshDesc.points.data = shapeDesc._mesh._vertex._buffer;
		meshDesc.points.count = shapeDesc._mesh._vertex._count;
		meshDesc.points.stride = shapeDesc._mesh._vertex._stride;
		meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		auto mesh = cooking->createConvexMesh(meshDesc, physics->getPhysicsInsertionCallback());

		PxConvexMeshGeometry meshGeometry(mesh);

		_impl->Create(system, meshGeometry, shapeDesc);

		mesh->release();
	}

	void PhysicsConvexMesh::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No Implementation.)");

		auto physics = _impl->GetPhysics(scene);

		if (physics == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxPhysics.)");

		auto cooking = _impl->GetCooking(scene);

		if (cooking == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxCooking.)");

		PxConvexMeshDesc meshDesc;
		meshDesc.setToDefault();
		meshDesc.points.data = shapeDesc._mesh._vertex._buffer;
		meshDesc.points.count = shapeDesc._mesh._vertex._count;
		meshDesc.points.stride = shapeDesc._mesh._vertex._stride;
		meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		auto mesh = cooking->createConvexMesh(meshDesc, physics->getPhysicsInsertionCallback());

		PxConvexMeshGeometry meshGeometry(mesh);

		_impl->Create(scene, meshGeometry, shapeDesc);

		mesh->release();
	}
}