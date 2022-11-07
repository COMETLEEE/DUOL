#pragma once
#include <time.h>
#include "DUOLPhysics/System/PhysicsSystem.h"
#include "InputManager.h"

#pragma comment(lib, "../Build/x64/Debug/DUOLPhysics.lib")

using namespace DUOLPhysics;

std::weak_ptr<PhysicsDynamicActor> CreateDynamic(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, const DUOLMath::Vector3& velocity = {})
{
	static int id = 0;

	///* Shape */
	//PhysicsShapeDesc shapeDesc;

	//shapeDesc._sphere._radius = 5.0f;
	//shapeDesc._isExclusive = false;
	//shapeDesc._material = material;
	//shapeDesc._flag = ShapeFlag::SIMULATION_AND_SCENE_QUERY_AND_VISUALIZATION;

	//auto shape = ps.CreateShape<PhysicsSphere>(_T("PhysicsSphere"), shapeDesc);
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._capsule._radius = 5.0f;
	shapeDesc._capsule._halfHeight = 5.0f;
	shapeDesc._isExclusive = false;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeType::COLLIDER_AND_SCENE_QUERY;

	auto shape = ps.CreateShape<PhysicsCapsule>(_T("PhysicsCapsule"), shapeDesc);

	PhysicsActorDesc dynamicActorDesc;

	dynamicActorDesc._transform = transform;

	auto actor = scene.lock()->CreateDynamicActor(_T("Ball") + std::to_wstring(id++), dynamicActorDesc);

	actor.lock()->AttachShape(shape);
	actor.lock()->SetMassAndInertia(10.0f);
	actor.lock()->SetAngularDamping(0.5f);
	actor.lock()->SetLinearVelocity(velocity);

	return actor;
}

void CreateStack(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, int amount, float boxSize)
{
	static int id = 0;
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._box._x = boxSize;
	shapeDesc._box._y = boxSize;
	shapeDesc._box._z = boxSize;
	shapeDesc._isExclusive = false;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeType::COLLIDER_AND_SCENE_QUERY;

	auto shape = ps.CreateShape<PhysicsBox>(_T("PhysicsBox"), shapeDesc);

	for (int i = 0; i < amount; i++)
	{
		for (int j = 0; j < amount - i; j++)
		{
			PhysicsActorDesc dynamicActorDesc;
			DUOLMath::Vector3 v = { j * 2.0f - (amount - i), i * 2.0f + 1.0f, 0 };

			dynamicActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation(v * boxSize);

			auto actor = scene.lock()->CreateDynamicActor(_T("Actor") + std::to_wstring(id++ * 10000 + i * 100 + j), dynamicActorDesc);

			actor.lock()->AttachShape(shape);
			actor.lock()->SetMassAndInertia(10.0f);
		}
	}
}

void CreateStaticStack(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, int amount, float boxSize)
{
	static int id = 0;
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._box._x = boxSize;
	shapeDesc._box._y = boxSize;
	shapeDesc._box._z = boxSize;
	shapeDesc._isExclusive = false;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeType::COLLIDER_AND_SCENE_QUERY;

	auto shape = ps.CreateShape<PhysicsBox>(_T("PhysicsBox"), shapeDesc);

	for (int i = 0; i < amount; i++)
	{
		for (int j = 0; j < amount - i; j++)
		{
			PhysicsActorDesc dynamicActorDesc;
			DUOLMath::Vector3 v = { j * 2.0f - (amount - i), i * 2.0f + 1.0f, 0 };

			dynamicActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation(v * boxSize);

			auto actor = scene.lock()->CreateStaticActor(_T("Actor") + std::to_wstring(id++ * 10000 + i * 100 + j), dynamicActorDesc);

			actor.lock()->AttachShape(shape);
		}
	}
}

void PhysicsTestCode()
{
	/* System */
	PhysicsSystem ps;

	PhysicsSystemDesc psDesc;

	psDesc._usePvd = true;

	ps.Init(psDesc);

	/* Scene */
	PhysicsSceneDesc sceneDesc;

	sceneDesc._gravity = { 0.0f, -9.81f, 0.0f };

	auto scene = ps.CreateScene(_T("TestScene"), sceneDesc);

	/* Material */
	PhysicsMaterialDesc matDesc;

	matDesc._staticFriction = 0.5f;
	matDesc._dynamicFriction = 0.5f;
	matDesc._restitution = 0.6f;

	auto material = ps.CreateMaterial(_T("DefaultMaterial"), matDesc);

	/* Plane */
	PhysicsPlaneDesc planeDesc;

	planeDesc._normal = { 0.0f, 1.0f, 0.0f };
	planeDesc._distance = 0.0f;

	scene.lock()->CreatePlane(_T("TestScenePlane"), material, planeDesc);

	/* Stack */
	for (int i = 0; i < 40; i++)
		CreateStack(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 0.0f, -10.0f * i), 20, 1.0f);

	/* Ball */
	auto ball = CreateDynamic(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 20.0f, 100.0f), { 0.0f, -25.0f, -100.0f });
	ball.lock()->SetMassAndInertia(1000.0f);

	while (true)
	{
		InputManager::GetInstance()->Update();

		if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY) == true)
			break;

		scene.lock()->Simulate(1.0f / 60.0f);

		InputManager::GetInstance()->LateUpdate();
	}

	ps.Release();
}