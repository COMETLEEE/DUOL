#pragma once
#include <time.h>
#include "SphereCollider.h"
#include "BoxCollider.h"

#include "InputManager.h"

#pragma comment(lib, "../Build/x64/Debug/DUOLPhysics.lib")

using namespace DUOLPhysics;

std::vector<std::shared_ptr<Collider>> colliders;

std::weak_ptr<PhysicsDynamicActor> actorPtr;

std::weak_ptr<PhysicsShapeBase> shapePtr;

float rand(float loVal, float hiVal)
{
	return loVal + (float(rand()) / float(RAND_MAX)) * (hiVal - loVal);
}

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
	shapeDesc._flag = ShapeFlag::COLLIDER | ShapeFlag::SCENE_QUERY;

	auto shape = ps.CreateShape<PhysicsCapsule>(_T("PhysicsCapsule"), shapeDesc);

	PhysicsActorDesc dynamicActorDesc;

	dynamicActorDesc._transform = transform;

	auto actor = scene.lock()->CreateDynamicActor(_T("Ball") + std::to_wstring(id++), dynamicActorDesc);

	actor.lock()->AttachShape(shape);
	actor.lock()->SetMassAndInertia(10.0f);
	actor.lock()->SetAngularDamping(0.5f);
	actor.lock()->SetLinearVelocity(velocity);

	auto collider = std::make_shared<SphereCollider>();

	colliders.push_back(collider);

	collider->SetEvent(actor);

	return actor;
}

void CreateBox(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, float boxSize)
{
	static int id = 0;
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._box._halfExtentX = boxSize;
	shapeDesc._box._halfExtentY = boxSize;
	shapeDesc._box._halfExtentZ = boxSize;
	shapeDesc._isExclusive = true;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeFlag::COLLIDER;

	auto shape = ps.CreateShape<PhysicsBox>(_T("PhysicsBoxTest"), shapeDesc);

	shapePtr = shape;

	PhysicsActorDesc dynamicActorDesc;

	dynamicActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation({ 0.0f, boxSize, 0.0f });

	auto actor = scene.lock()->CreateDynamicActor(_T("TestBox"), dynamicActorDesc);

	actor.lock()->AttachShape(shape);
	actor.lock()->SetMassAndInertia(10.0f);

	actorPtr = actor;

	auto collider = std::make_shared<BoxCollider>();

	colliders.push_back(collider);

	collider->SetEvent(actor);
}

void CreateStack(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, int amount, float boxSize)
{
	static int id = 0;
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._box._halfExtentX = boxSize;
	shapeDesc._box._halfExtentY = boxSize;
	shapeDesc._box._halfExtentZ = boxSize;
	shapeDesc._isExclusive = false;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeFlag::COLLIDER | ShapeFlag::SCENE_QUERY;

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

			auto collider = std::make_shared<BoxCollider>();

			colliders.push_back(collider);

			//collider->SetEvent(actor);
		}
	}
}

void CreateStaticStack(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform, int amount, float boxSize)
{
	static int id = 0;
	/* Shape */
	PhysicsShapeDesc shapeDesc;

	shapeDesc._box._halfExtentX = boxSize;
	shapeDesc._box._halfExtentY = boxSize;
	shapeDesc._box._halfExtentZ = boxSize;
	shapeDesc._isExclusive = false;
	shapeDesc._material = material;
	shapeDesc._flag = ShapeFlag::TRIGGER | ShapeFlag::SCENE_QUERY;

	auto shape = ps.CreateShape<PhysicsBox>(_T("PhysicsBox"), shapeDesc);
	
	shape.lock()->SetSceneQueryEnable(false);

	for (int i = 0; i < amount; i++)
	{
		for (int j = 0; j < amount - i; j++)
		{
			PhysicsActorDesc staticActorDesc;
			DUOLMath::Vector3 v = { j * 2.0f - (amount - i), i * 2.0f + 1.0f, 0 };

			staticActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation(v * boxSize);

			auto actor = scene.lock()->CreateStaticActor(_T("Actor") + std::to_wstring(id++ * 10000 + i * 100 + j), staticActorDesc);

			actor.lock()->AttachShape(shape);

			auto collider = std::make_shared<BoxCollider>();

			colliders.push_back(collider);

			collider->SetEvent(actor);
		}
	}
}

void CreateMesh(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform)
{
	unsigned numRows = 127;
	unsigned numColumns = 127;

	unsigned numX = 128;
	unsigned numZ = 128;
	unsigned numVertices = numX * numZ;
	unsigned numTriangles = numRows * numColumns * 2;

	DUOLMath::Vector3* vertices = nullptr;

	unsigned* indices = nullptr;

	if (vertices == NULL)
		vertices = new DUOLMath::Vector3[numVertices];
	if (indices == NULL)
		indices = new unsigned[numTriangles * 3];

	unsigned currentIdx = 0;
	for (unsigned i = 0; i <= numRows; i++)
	{
		for (unsigned j = 0; j <= numColumns; j++)
		{
			DUOLMath::Vector3 v(float(j * 20), 0.0f, float(i * 20));
			vertices[currentIdx++] = v;
		}
	}

	currentIdx = 0;
	for (unsigned i = 0; i < numRows; i++)
	{
		for (unsigned j = 0; j < numColumns; j++)
		{
			unsigned base = (numColumns + 1) * i + j;
			indices[currentIdx++] = base + 1;
			indices[currentIdx++] = base;
			indices[currentIdx++] = base + numColumns + 1;
			indices[currentIdx++] = base + numColumns + 2;
			indices[currentIdx++] = base + 1;
			indices[currentIdx++] = base + numColumns + 1;
		}
	}

	for (unsigned i = 0; i < numVertices; i++)
	{
		DUOLMath::Vector3& v = vertices[i];
		v.y += rand(-10.0f, 10.0f);
	}

	PhysicsShapeDesc shapeDesc;

	shapeDesc._mesh._vertex._buffer = vertices;
	shapeDesc._mesh._vertex._count = numVertices;
	shapeDesc._mesh._vertex._stride = sizeof(DUOLMath::Vector3);

	shapeDesc._mesh._index._buffer = indices;
	shapeDesc._mesh._index._count = numTriangles * 3;
	shapeDesc._mesh._index._stride = sizeof(unsigned);

	shapeDesc._isExclusive = false;
	shapeDesc._flag = ShapeFlag::COLLIDER | ShapeFlag::SCENE_QUERY;
	shapeDesc._material = material;

	auto shape = ps.CreateShape<PhysicsMesh>(_T("PhysicsMesh"), shapeDesc);

	PhysicsActorDesc dynamicActorDesc;

	dynamicActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation(DUOLMath::Vector3{ 0.0f, 20.0f, 0.0f });

	auto actor = scene.lock()->CreateStaticActor(_T("MeshActor"), dynamicActorDesc);

	actor.lock()->AttachShape(shape);
	//actor.lock()->SetMassAndInertia(10.0f);

	auto collider = std::make_shared<BoxCollider>();

	colliders.push_back(collider);

	//collider->SetEvent(actor);

	delete[] vertices;
	delete[] indices;
}

void CreateConvexMesh(PhysicsSystem& ps, std::weak_ptr<PhysicsScene> scene, std::weak_ptr<PhysicsMaterial> material, const DUOLMath::Matrix& transform)
{
	const unsigned numVerts = 64;
	DUOLMath::Vector3* vertices = new DUOLMath::Vector3[numVerts];

	// Prepare random verts
	for (unsigned i = 0; i < numVerts; i++)
	{
		vertices[i] = DUOLMath::Vector3{ rand(-20.0f, 20.0f), rand(-20.0f, 20.0f), rand(-20.0f, 20.0f) };
	}

	PhysicsShapeDesc shapeDesc;

	shapeDesc._convexMesh._vertex._buffer = vertices;
	shapeDesc._convexMesh._vertex._count = numVerts;
	shapeDesc._convexMesh._vertex._stride = sizeof(DUOLMath::Vector3);
	shapeDesc._isExclusive = false;
	shapeDesc._flag = ShapeFlag::COLLIDER | ShapeFlag::SCENE_QUERY;
	shapeDesc._material = material;

	auto shape = ps.CreateShape<PhysicsConvexMesh>(_T("PhysicsConvexMesh"), shapeDesc);

	PhysicsActorDesc dynamicActorDesc;

	dynamicActorDesc._transform = transform * DUOLMath::Matrix::CreateTranslation(DUOLMath::Vector3{ 0.0f, 20.0f, 0.0f });

	auto actor = scene.lock()->CreateDynamicActor(_T("ConvexMeshActor"), dynamicActorDesc);

	actor.lock()->AttachShape(shape);
	actor.lock()->SetMassAndInertia(10.0f);

	auto collider = std::make_shared<BoxCollider>();

	colliders.push_back(collider);

	//collider->SetEvent(actor);

	delete[] vertices;
}

void PhysicsTestCode()
{
	LogHelper::Initialize();

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
	for (int i = 0; i < 1; i++)
		CreateStaticStack(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 0.0f, -10.0f * i), 20, 1.0f);
		
	/* Test Box */
	CreateBox(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 0.0f, -30.0f), 10.0f);

	/* Ball */
	auto ball = CreateDynamic(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 20.0f, 100.0f), { 0.0f, -25.0f, -100.0f });
	ball.lock()->SetMassAndInertia(1000.0f);

	/* Mesh */
	CreateMesh(ps, scene, material, DUOLMath::Matrix::CreateTranslation(0.0f, 20.0f, -100.0f));
	
	/* Convex Mesh */
	CreateConvexMesh(ps, scene, material, DUOLMath::Matrix::CreateTranslation(50.0f, 100.0f, -10.0f));

	scene.lock()->SetRenderBufferOption(RenderBufferOption::SCALE, 1.0f);
	scene.lock()->SetRenderBufferOption(RenderBufferOption::COLLISION_SHAPES, 2.0f);

	while (true)
	{
		InputManager::GetInstance()->Update();

		if (InputManager::GetInstance()->GetInputState('Q', KeyState::STAY) == true)
			break;

		if (InputManager::GetInstance()->GetInputState('R', KeyState::DOWN) == true)
		{
			auto rayHit = scene.lock()->Raycast({ 0.0f, 20.0f, 100.0f }, { 0.0f, 0.0f, -1.0f }, 500.0f);
		
			if (rayHit._isBlocking == true && rayHit._userData != nullptr)
				reinterpret_cast<Collider*>(rayHit._userData)->OnRaycastHit();
		}

		static bool flag = true;

		if (flag)
		{
			flag = false;

			shapePtr.lock()->SetTriggerEnable(true);
			actorPtr.lock()->SetGravityEnable(false);
		}

		scene.lock()->Simulate(1.0f / 600.0f);

		auto test = scene.lock()->GetRenderBuffer();

		InputManager::GetInstance()->LateUpdate();
	}

	ps.Release();

	LogHelper::UnInitialize();
}