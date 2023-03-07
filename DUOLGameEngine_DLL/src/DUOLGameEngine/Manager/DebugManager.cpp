#include "DUOLGameEngine/Manager/DebugManager.h"

#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameengine/Manager/NavigationManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/Navigation/Detour/DetourCommon.h"
#include "DUOLGameEngine/Navigation/DebugUtils/DetourDebugDraw.h"
#include "DUOLGameEngine/Navigation/DebugUtils/RecastDebugDraw.h"

namespace DUOLGameEngine
{
	DebugManager::DebugManager() :
		_physXTransformInfo{}
		, _physXPrimitiveMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navTransformInfo{}
		, _navPointMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navPointDepthOffMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navLineMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navLineDepthOffMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navTriangleMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _navTriangleDepthOffMaterials(std::vector<DUOLGraphicsEngine::Material*>())
		, _currentModeVertices(nullptr)
		, _navMeshDrawFlags(DU_DRAWNAVMESH_OFFMESHCONS | DU_DRAWNAVMESH_CLOSEDLIST)
		, _showPath(true)
		, _showCorners(true)
		, _showCollisionSegments(true)
		, _showAgentCylinders(true)
		, _showBVTree(true)
		, _isConsole(true)
		, _isPhysics(true)
		, _isNavigation(true)
	{
	}

	DebugManager::~DebugManager()
	{

	}

	void DebugManager::Initialize()
	{
		DUOLCommon::LogHelper::ShowConsole();

		// 0-1. Graphics engine caching.
		_graphicsEngine = DUOLGameEngine::GraphicsManager::GetInstance()->_graphicsEngine;

		// 0-2. Physics system caching.
		_physicsSystem = DUOLGameEngine::PhysicsManager::GetInstance()->_physicsSystem;

		// 0-3. Physics scene caching. 다시 만들어질 수 있기 때문에 포인터로 받습니다.
		_physicsScene = &DUOLGameEngine::PhysicsManager::GetInstance()->_physicsScene;

		// 0-4. Navigation Manager caching
		_navigationManager = DUOLGameEngine::NavigationManager::GetInstance();

		// 1. Physics debug info initialize
		PHYSICS_DEBUG_INDEX_BUFFER = new UINT[PHYSICS_DEBUG_INDEX_MAX];

		ZeroMemory(PHYSICS_DEBUG_INDEX_BUFFER, sizeof(UINT) * PHYSICS_DEBUG_INDEX_MAX);

		for (int i = 0; i < PHYSICS_DEBUG_INDEX_MAX; i++)
			PHYSICS_DEBUG_INDEX_BUFFER[i] = i;

		DUOLGraphicsEngine::MeshBase* debugMesh = _graphicsEngine->CreateMesh(TEXT("PHYSICS_MESH"), nullptr, PHYSICS_DEBUG_VERTEX_BUFFER_SIZE,
			PHYSICS_DEBUG_VERTEX_SIZE, PHYSICS_DEBUG_INDEX_BUFFER, PHYSICS_DEBUG_INDEX_MAX);

		_physXRenderObjectInfo._mesh = debugMesh;

		_physXRenderObjectInfo._renderInfo = &_physXDebugInfo;

		_physXRenderObjectInfo._materials = &_physXPrimitiveMaterials;

		// TODO : 디버그 머터리얼 넣어주고 돌려 !
		_physXPrimitiveMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugLineDepthOn"))->GetPrimitiveMaterial());

		_physXDebugMesh = std::make_shared<DUOLGameEngine::Mesh>();

		_physXDebugMesh->SetPrimitiveMesh(debugMesh);



		// 2. Navigation debug info initialize
		_navDebugDraw = new NavDebugDrawDUOL();

		NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER = new UINT[NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX];

		for (int i = 0; i < NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX; i++)
			NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER[i] = i;

		NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER = new UINT[NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX];

		ZeroMemory(NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER, sizeof(UINT) * NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX);

		for (int i = 0; i < NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX; i++)
		{
			NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER[i] = i;
		}

		// --------------------------- Nav Point ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_POINT_MESH"), nullptr, NAVIGATION_DEBUG_POINT_LINE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX);

		_navPointRenderObjectInfo._mesh = debugMesh;

		_navPointRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navPointRenderObjectInfo._materials = &_navPointMaterials;

		_navPointMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugPointDepthOn"))->GetPrimitiveMaterial());

		_navPointMesh = std::make_shared<DUOLGameEngine::Mesh>();

		_navPointMesh->SetPrimitiveMesh(debugMesh);

		// --------------------------- Nav Point DepthOff ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_POINT_DEPTHOFF_MESH"), nullptr, NAVIGATION_DEBUG_POINT_LINE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX);

		_navPointDepthOffRenderObjectInfo._mesh = debugMesh;

		_navPointDepthOffRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navPointDepthOffRenderObjectInfo._materials = &_navPointDepthOffMaterials;

		_navPointDepthOffMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugPointDepthOff"))->GetPrimitiveMaterial());

		_navPointMeshDepthOff = std::make_shared<DUOLGameEngine::Mesh>();

		_navPointMeshDepthOff->SetPrimitiveMesh(debugMesh);

		// --------------------------- Nav Line ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_LINE_MESH"), nullptr, NAVIGATION_DEBUG_POINT_LINE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX);

		_navLineRenderObjectInfo._mesh = debugMesh;

		_navLineRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navLineRenderObjectInfo._materials = &_navLineMaterials;

		_navLineMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugLineDepthOn"))->GetPrimitiveMaterial());

		_navLineMesh = std::make_shared<DUOLGameEngine::Mesh>();

		_navLineMesh->SetPrimitiveMesh(debugMesh);

		// --------------------------- Nav Line DepthOff ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_LINE_DEPTHOFF_MESH"), nullptr, NAVIGATION_DEBUG_POINT_LINE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX);

		_navLineDepthOffRenderObjectInfo._mesh = debugMesh;

		_navLineDepthOffRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navLineDepthOffRenderObjectInfo._materials = &_navLineDepthOffMaterials;

		_navLineDepthOffMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugLineDepthOff"))->GetPrimitiveMaterial());

		_navLineMeshDepthOff = std::make_shared<DUOLGameEngine::Mesh>();

		_navLineMeshDepthOff->SetPrimitiveMesh(debugMesh);

		// --------------------------- Nav Triangle ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_TRIANGLE_MESH"), nullptr, NAVIGATION_DEBUG_TRIANGLE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER, NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX);

		_navTriangleRenderObjectInfo._mesh = debugMesh;

		_navTriangleRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navTriangleRenderObjectInfo._materials = &_navTriangleMaterials;

		_navTriangleMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugTriangleDepthOn"))->GetPrimitiveMaterial());

		_navTriangleMesh = std::make_shared<DUOLGameEngine::Mesh>();

		_navTriangleMesh->SetPrimitiveMesh(debugMesh);

		// --------------------------- Nav Triangle DepthOff ---------------------------
		debugMesh = _graphicsEngine->CreateMesh(TEXT("NAVIGATION_TRIANGLE_DEPTHOFF_MESH"), nullptr, NAVIGATION_DEBUG_TRIANGLE_VERTEX_BUFFER_SIZE,
			NAVIGATION_DEBUG_VERTEX_SIZE, NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER, NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX);

		_navTriangleDepthOffRenderObjectInfo._mesh = debugMesh;

		_navTriangleDepthOffRenderObjectInfo._renderInfo = &_navDebugInfo;

		_navTriangleDepthOffRenderObjectInfo._materials = &_navTriangleDepthOffMaterials;

		_navTriangleDepthOffMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("DebugTriangleDepthOff"))->GetPrimitiveMaterial());

		_navTriangleMeshDepthOff = std::make_shared<DUOLGameEngine::Mesh>();

		_navTriangleMeshDepthOff->SetPrimitiveMesh(debugMesh);









		_isConsole = false;

		DUOL_INFO(DUOL_FILE, "DebugManager Initialize Success !");
	}

	void DebugManager::UnInitialize()
	{
		_graphicsEngine.reset();

		_physicsSystem.reset();

		// TODO : _primitivedMesh 해제하여야 합니다.
		_physXDebugMesh.reset();

		delete[] PHYSICS_DEBUG_INDEX_BUFFER;

		delete[] NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER;

		delete[] NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER;
	}

	void DebugManager::Update(float deltaTime)
	{
		ControlDebugState();

		if (_isPhysics)
			UpdatePhysicsDebugMesh();

		if (_isNavigation)
			UpdateNavigationDebugMesh();
	}

	void DebugManager::ControlDebugState()
	{
		// 콘솔
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F1))
		{
			_isConsole = !_isConsole;

			if (_isConsole)
				DUOLCommon::LogHelper::ShowConsole();
			else
				DUOLCommon::LogHelper::HideConsole();
		}

		// 물리 디버깅 여부
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F2))
			_isPhysics = !_isPhysics;

		// 네비게이션 디버깅 여부
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F3))
			_isNavigation = !_isNavigation;
	}

	void DebugManager::UpdatePhysicsDebugMesh()
	{
		if (!_physicsScene->expired())
		{
			const DUOLPhysics::SceneDebugData debugData = _physicsScene->lock()->GetRenderBuffer();

			const DUOLPhysics::SceneDebugData::VertexData* vertexData = debugData._data;

			const UINT debugVertexCount = debugData._count * 2;

			DUOLGraphicsEngine::MeshBase* debugMesh = _physXDebugMesh->GetPrimitiveMesh();

			_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(const_cast<DUOLPhysics::SceneDebugData::VertexData*>(vertexData)),
				debugVertexCount * sizeof(DUOLPhysics::SceneDebugData::VertexData) / 2,
				PHYSICS_DEBUG_INDEX_BUFFER, std::min(debugVertexCount, PHYSICS_DEBUG_INDEX_MAX) * sizeof(UINT));

			DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_physXRenderObjectInfo);
		}
	}

	void DebugManager::UpdateNavigationDebugMesh()
	{
		// -------------------------- Navigation Mesh --------------------------
		if (_navigationManager->_currentNavMesh != nullptr && _navigationManager->_navMeshQuery != nullptr)
		{
			if (_showPath)
			{
				for (int i = 0; i < _navigationManager->_crowd->getAgentCount(); i++)
				{
					const dtCrowdAgent* ag = _navigationManager->_crowd->getAgent(i);
					if (!ag->active)
						continue;
					const dtPolyRef* path = ag->corridor.getPath();
					const int npath = ag->corridor.getPathCount();
					for (int j = 0; j < npath; ++j)
						duDebugDrawNavMeshPoly(_navDebugDraw, *_navigationManager->_currentNavMesh, path[j], duRGBA(255, 255, 255, 24));
				}
			}

			duDebugDrawNavMeshWithClosedList(_navDebugDraw, *_navigationManager->_currentNavMesh, *_navigationManager->_navMeshQuery, _navMeshDrawFlags);
		}

		for (int i = 0; i < _navigationManager->_crowd->getAgentCount(); i++)
		{
			const dtCrowdAgent* ag = _navigationManager->_crowd->getAgent(i);
			if (!ag->active)
				continue;

			const float radius = ag->params.radius;
			const float* pos = ag->npos;

			if (_showCorners)
			{
				if (ag->ncorners)
				{
					_navDebugDraw->begin(DU_DRAW_LINES, 2.0f);
					for (int j = 0; j < ag->ncorners; ++j)
					{
						const float* va = j == 0 ? pos : &ag->cornerVerts[(j - 1) * 3];
						const float* vb = &ag->cornerVerts[j * 3];
						_navDebugDraw->vertex(va[0], va[1] + radius, va[2], duRGBA(128, 0, 0, 192));
						_navDebugDraw->vertex(vb[0], vb[1] + radius, vb[2], duRGBA(128, 0, 0, 192));
					}
					if (ag->ncorners && ag->cornerFlags[ag->ncorners - 1] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
					{
						const float* v = &ag->cornerVerts[(ag->ncorners - 1) * 3];
						_navDebugDraw->vertex(v[0], v[1], v[2], duRGBA(192, 0, 0, 192));
						_navDebugDraw->vertex(v[0], v[1] + radius * 2, v[2], duRGBA(192, 0, 0, 192));
					}

					_navDebugDraw->end();
				}
			}

			if (_showCollisionSegments)
			{
				const float* center = ag->boundary.getCenter();
				duDebugDrawCross(_navDebugDraw, center[0], center[1] + radius, center[2], 0.2f, duRGBA(192, 0, 128, 255), 2.0f);
				duDebugDrawCircle(_navDebugDraw, center[0], center[1] + radius, center[2], ag->params.collisionQueryRange,
					duRGBA(192, 0, 128, 128), 2.0f);

				_navDebugDraw->begin(DU_DRAW_LINES, 3.0f);
				for (int j = 0; j < ag->boundary.getSegmentCount(); ++j)
				{
					const float* s = ag->boundary.getSegment(j);
					unsigned int col = duRGBA(192, 0, 128, 192);

					if (dtTriArea2D(pos, s, s + 3) < 0.0f)
						col = duDarkenCol(col);

					duAppendArrow(_navDebugDraw, s[0], s[1] + 0.2f, s[2], s[3], s[4] + 0.2f, s[5], 0.0f, 0.3f, col);
				}
				_navDebugDraw->end();
			}
		}

		if (_showAgentCylinders)
		{
			// Agent cylinders.
			for (int i = 0; i < _navigationManager->_crowd->getAgentCount(); ++i)
			{
				const dtCrowdAgent* ag = _navigationManager->_crowd->getAgent(i);
				if (!ag->active) continue;

				const float radius = ag->params.radius;
				const float* pos = ag->npos;

				unsigned int col = duRGBA(0, 0, 0, 32);

				/*if (m_agentDebug.idx == i)
					col = duRGBA(255, 0, 0, 128);*/

				duDebugDrawCircle(_navDebugDraw, pos[0], pos[1], pos[2], radius, col, 2.0f);
			}

			for (int i = 0; i < _navigationManager->_crowd->getAgentCount(); ++i)
			{
				const dtCrowdAgent* ag = _navigationManager->_crowd->getAgent(i);
				if (!ag->active) continue;

				const float height = ag->params.height;
				const float radius = ag->params.radius;
				const float* pos = ag->npos;

				unsigned int col = duRGBA(220, 220, 220, 128);
				if (ag->targetState == DT_CROWDAGENT_TARGET_REQUESTING || ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE)
					col = duLerpCol(col, duRGBA(128, 0, 255, 128), 32);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_PATH)
					col = duLerpCol(col, duRGBA(128, 0, 255, 128), 128);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_FAILED)
					col = duRGBA(255, 32, 16, 128);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_VELOCITY)
					col = duLerpCol(col, duRGBA(64, 255, 0, 128), 128);

				duDebugDrawCylinder(_navDebugDraw, pos[0] - radius, pos[1] + radius * 0.1f, pos[2] - radius,
					pos[0] + radius, pos[1] + height, pos[2] + radius, col);
			}

			// Velocity stuff.
			for (int i = 0; i < _navigationManager->_crowd->getAgentCount(); ++i)
			{
				const dtCrowdAgent* ag = _navigationManager->_crowd->getAgent(i);
				if (!ag->active) continue;

				const float radius = ag->params.radius;
				const float height = ag->params.height;
				const float* pos = ag->npos;
				const float* vel = ag->vel;
				const float* dvel = ag->dvel;

				unsigned int col = duRGBA(220, 220, 220, 192);
				if (ag->targetState == DT_CROWDAGENT_TARGET_REQUESTING || ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE)
					col = duLerpCol(col, duRGBA(128, 0, 255, 192), 32);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_PATH)
					col = duLerpCol(col, duRGBA(128, 0, 255, 192), 128);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_FAILED)
					col = duRGBA(255, 32, 16, 192);
				else if (ag->targetState == DT_CROWDAGENT_TARGET_VELOCITY)
					col = duLerpCol(col, duRGBA(64, 255, 0, 192), 128);

				duDebugDrawCircle(_navDebugDraw, pos[0], pos[1] + height, pos[2], radius, col, 2.0f);

				duDebugDrawArrow(_navDebugDraw, pos[0], pos[1] + height, pos[2],
					pos[0] + dvel[0], pos[1] + height + dvel[1], pos[2] + dvel[2],
					0.0f, 0.4f, duRGBA(0, 192, 255, 192), 1.0f);

				duDebugDrawArrow(_navDebugDraw, pos[0], pos[1] + height, pos[2],
					pos[0] + vel[0], pos[1] + height + vel[1], pos[2] + vel[2],
					0.0f, 0.4f, duRGBA(0, 0, 0, 160), 2.0f);
			}
		}

		if (_showBVTree)
			duDebugDrawNavMeshBVTree(_navDebugDraw, *_navigationManager->_currentNavMesh);

		// --------------------------- Nav Point ---------------------------
		UINT debugVertexCount = _navPointVertices.size();

		NavDebugVertex* debugVertexData = _navPointVertices.data();

		DUOLGraphicsEngine::MeshBase* debugMesh = _navPointMesh->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navPointRenderObjectInfo);

		// --------------------------- Nav Point DepthOff ---------------------------
		debugVertexCount = _navPointVerticesDepthOff.size();

		debugVertexData = _navPointVerticesDepthOff.data();

		debugMesh = _navPointMeshDepthOff->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navPointDepthOffRenderObjectInfo);

		// --------------------------- Nav Line ---------------------------
		debugVertexCount = _navLineVertices.size();

		debugVertexData = _navLineVertices.data();

		debugMesh = _navLineMesh->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navLineRenderObjectInfo);

		// --------------------------- Nav Line DepthOff ---------------------------
		debugVertexCount = _navLineVerticesDepthOff.size();

		debugVertexData = _navLineVerticesDepthOff.data();

		debugMesh = _navLineMeshDepthOff->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navLineDepthOffRenderObjectInfo);

		// --------------------------- Nav Triangle ---------------------------
		debugVertexCount = _navTriangleVertices.size();

		debugVertexData = _navTriangleVertices.data();

		debugMesh = _navTriangleMesh->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navTriangleRenderObjectInfo);

		// --------------------------- Nav Triangle DepthOff ---------------------------
		debugVertexCount = _navTriangleVerticesDepthOff.size();

		debugVertexData = _navTriangleVerticesDepthOff.data();

		debugMesh = _navTriangleMeshDepthOff->GetPrimitiveMesh();

		_graphicsEngine->UpdateMesh(debugMesh, reinterpret_cast<void*>(debugVertexData), debugVertexCount * sizeof(NavDebugVertex),
			NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER, std::min(debugVertexCount, NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX) * sizeof(UINT));

		DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderDebugObject(&_navTriangleDepthOffRenderObjectInfo);

		// --------------------------- Nav Vertices Clearing ---------------------------
		_navPointVertices.clear();

		_navPointVerticesDepthOff.clear();

		_navLineVertices.clear();

		_navLineVerticesDepthOff.clear();

		_navTriangleVertices.clear();

		_navTriangleVerticesDepthOff.clear();

		_navQuadVertices.clear();

		_navQuadVerticesDepthOff.clear();
	}
}
