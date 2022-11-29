#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "DUOLPhysics/Shapes/PhysicsMesh.h"

/* System */
#include "../System/PhysicsSystemImpl.h"

/* Shape */
#include "PhysicsShapeBaseImpl.h"

/* etc */
#include "DUOLPhysics/Util/PhysicsDefines.h"

namespace DUOLPhysics
{
	PhysicsMesh::PhysicsMesh()
	{

	}

	void PhysicsMesh::Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No Implementation.)");

		auto physics = _impl->GetPhysics(system);

		if (physics == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxPhysics.)");

		auto cooking = _impl->GetCooking(system);

		if (cooking == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxCooking.)");

		PxTriangleMeshDesc meshDesc;
		meshDesc.setToDefault();
		meshDesc.points.data = shapeDesc._mesh._vertex._buffer;
		meshDesc.points.count = shapeDesc._mesh._vertex._count;
		meshDesc.points.stride = shapeDesc._mesh._vertex._stride;

		meshDesc.triangles.data = shapeDesc._mesh._index._buffer;
		meshDesc.triangles.count = shapeDesc._mesh._index._count / 3;
		meshDesc.triangles.stride = shapeDesc._mesh._index._stride * 3;

		if (shapeDesc._mesh._index._stride == 2)
			meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

		auto mesh = cooking->createTriangleMesh(meshDesc, physics->getPhysicsInsertionCallback());

		PxTriangleMeshGeometry meshGeometry(mesh);

		_impl->Create(system, meshGeometry, shapeDesc);

		mesh->release();
	}

	void PhysicsMesh::Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc)
	{
		if (_impl == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No Implementation.)");

		auto physics = _impl->GetPhysics(scene);

		if (physics == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxPhysics.)");

		auto cooking = _impl->GetCooking(scene);

		if (cooking == nullptr)
			ERROR_THROW("Failed to create PxTriangleMesh. (No PxCooking.)");

		PxTriangleMeshDesc meshDesc;
		meshDesc.setToDefault();
		meshDesc.points.data = shapeDesc._mesh._vertex._buffer;
		meshDesc.points.count = shapeDesc._mesh._vertex._count;
		meshDesc.points.stride = shapeDesc._mesh._vertex._stride;

		meshDesc.triangles.data = shapeDesc._mesh._index._buffer;
		meshDesc.triangles.count = shapeDesc._mesh._index._count / 3;
		meshDesc.triangles.stride = shapeDesc._mesh._index._stride * 3;

		if (shapeDesc._mesh._index._stride == 2)
			meshDesc.flags = PxMeshFlag::e16_BIT_INDICES;

		auto mesh = cooking->createTriangleMesh(meshDesc, physics->getPhysicsInsertionCallback());

		PxTriangleMeshGeometry meshGeometry(mesh);

		_impl->Create(scene, meshGeometry, shapeDesc);

		mesh->release();
	}
}