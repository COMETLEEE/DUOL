#include "DUOLGameEngine/Manager/DebugManager.h"

#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLGameEngine
{
	DebugManager::DebugManager() :
		_transformInfo{DUOLMath::Matrix::Identity, DUOLMath::Matrix::Identity}
		, _primitiveMaterials(std::vector<DUOLGraphicsEngine::Material*>())
	{
		
	}

	DebugManager::~DebugManager()
	{

	}

	void DebugManager::Initialize()
	{
		// 0-1. Graphics engine caching.
		_graphicsEngine = DUOLGameEngine::GraphicsManager::GetInstance()->_graphicsEngine;

		// 0-2. Physics system caching.
		_physicsSystem = DUOLGameEngine::PhysicsManager::GetInstance()->_physicsSystem;

		// 0-3. Physics scene caching.
		_physicsScene = DUOLGameEngine::PhysicsManager::GetInstance()->_physicsScene;

		// 1. Physics debug info initialize
		PHYSICS_DEBUG_INDEX_BUFFER = new UINT[PHYSICS_DEBUG_INDEX_MAX];

		for (int i = 0; i < PHYSICS_DEBUG_INDEX_MAX; i++)
			PHYSICS_DEBUG_INDEX_BUFFER[i] = i;

		DUOLGraphicsEngine::Mesh* debugMesh = _graphicsEngine->CreateMesh(TEXT("PHYSICS_MESH"), nullptr, PHYSICS_DEBUG_VERTEX_BUFFER_SIZE,
			PHYSICS_DEBUG_VERTEX_SIZE, PHYSICS_DEBUG_INDEX_BUFFER, PHYSICS_DEBUG_INDEX_MAX);

		_renderObjectInfo.mesh = debugMesh;

		_renderObjectInfo.PerObjectData._transform = &_transformInfo;

		_renderObjectInfo.PerObjectData._material = &_primitiveMaterials;

		// TODO : 머터리얼 넣어주고 돌려 !
		// _primitiveMaterials.push_back()

		_physicsDebugMesh = std::make_shared<DUOLGameEngine::Mesh>();

		_physicsDebugMesh->SetPrimitiveMesh(debugMesh);
	}

	void DebugManager::UnInitialize()
	{
		_graphicsEngine.reset();

		_physicsSystem.reset();

		_physicsScene.reset();

		// TODO : _primitivedMesh 해제하여야 합니다.
		_physicsDebugMesh.reset();

		delete[] PHYSICS_DEBUG_INDEX_BUFFER;
	}

	void DebugManager::Update(float deltaTime)
	{
		UpdatePhysicsDebugMesh();
	}

	void DebugManager::UpdatePhysicsDebugMesh()
	{
		if (!_physicsScene.expired())
		{
			const DUOLPhysics::SceneDebugData debugData = _physicsScene.lock()->GetRenderBuffer();

			const DUOLPhysics::SceneDebugData::VertexData* vertexData = debugData._data;

			const UINT debugVertexCount = debugData._count;

			DUOLGraphicsEngine::Mesh* debugMesh = _physicsDebugMesh->GetPrimitiveMesh();

			//_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(const_cast<DUOLPhysics::SceneDebugData::VertexData*>(vertexData)), 
			//	debugVertexCount * PHYSICS_DEBUG_VERTEX_SIZE, 
			//	PHYSICS_DEBUG_INDEX_BUFFER, std::min(debugVertexCount * 2, PHYSICS_DEBUG_INDEX_MAX));
			//
			//DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(_renderObjectInfo);
		}
	}
}