/**

	@file      DebugManager.h
	@brief     Game Engine Debug (Gizmo, Text, Profile, Log, ...) 관련 통제
	@details   ~
	@author    COMETLEE
	@date      21.11.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLGameEngine/Util/SingletonBase.h"

#include "DUOLGameEngine/Navigation/DebugImplement/NavDebugDrawDUOL.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

namespace DUOLPhysics
{
	class PhysicsScene;

	class PhysicsSystem;
}

namespace DUOLGraphicsEngine
{
	class GraphicsEngine;
}

namespace DUOLGameEngine
{
	class NavigationManager;
	class Mesh;
}

#pragma region PHYSICS_DEBUG
namespace DUOLGameEngine
{
	constexpr   UINT    PHYSICS_DEBUG_VERTEX_SIZE = (sizeof(DUOLPhysics::SceneDebugData::VertexData) / 2);

	constexpr   UINT    PHYSICS_DEBUG_INDEX_MAX = 200000;

	constexpr   UINT    PHYSICS_DEBUG_VERTEX_BUFFER_SIZE = PHYSICS_DEBUG_VERTEX_SIZE * PHYSICS_DEBUG_INDEX_MAX;

	inline      UINT*	PHYSICS_DEBUG_INDEX_BUFFER = nullptr;
}
#pragma endregion

#pragma region NAVIGATION_DEBUG
	constexpr   UINT    NAVIGATION_DEBUG_VERTEX_SIZE = sizeof(DUOLGameEngine::NavDebugVertex);

	constexpr   UINT    NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX = 20000;								// 2만개의 점, 1만개의 직선 커버

	constexpr   UINT    NAVIGATION_DEBUG_POINT_LINE_VERTEX_BUFFER_SIZE = NAVIGATION_DEBUG_POINT_LINE_INDEX_MAX * NAVIGATION_DEBUG_VERTEX_SIZE;

	inline      UINT*	NAVIGATION_DEBUG_POINT_LINE_INDEX_BUFFER = nullptr;

	constexpr	UINT	NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX = 120000;								// 4만개의 삼각형 커버

	constexpr	UINT	NAVIGATION_DEBUG_TRIANGLE_VERTEX_BUFFER_SIZE = NAVIGATION_DEBUG_TRIANGLE_INDEX_MAX * NAVIGATION_DEBUG_VERTEX_SIZE;

	inline      UINT*	NAVIGATION_DEBUG_TRIANGLE_INDEX_BUFFER = nullptr;

	constexpr	UINT	NAVIGATION_DEBUG_QUAD_INDEX_MAX = 120000;									// 2만개의 사각형 커버

	constexpr	UINT	NAVIGATION_DEBUG_QUAD_VERTEX_BUFFER_SIZE = NAVIGATION_DEBUG_QUAD_INDEX_MAX * NAVIGATION_DEBUG_VERTEX_SIZE;

	inline		UINT*	NAVIGATION_DEBUG_QUAD_INDEX_BUFFER = nullptr;
#pragma endregion

namespace DUOLGameEngine
{
	/**
	 * \brief Debug management class.
	 */
	class DUOL_GAMEENGINE_API DebugManager : public DUOLGameEngine::SingletonBase<DebugManager>
	{
		DECLARE_SINGLETON(DebugManager)

		DELETE_COPY_MOVE(DebugManager)

	private:
		/**
		 * \brief graphics engine caching.
		 */
		std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> _graphicsEngine;

		/**
		 * \brief physics system caching.
		 */
		std::shared_ptr<DUOLPhysics::PhysicsSystem> _physicsSystem;

		/**
		 * \brief physics scene caching.
		 */
		std::weak_ptr<DUOLPhysics::PhysicsScene>* _physicsScene;

		/**
		 * \brief Navigation Manager
		 */
		std::shared_ptr<DUOLGameEngine::NavigationManager> _navigationManager;

		DUOLGameEngine::NavDebugDrawDUOL* _navDebugDraw;

		unsigned char _navMeshDrawFlags;

	private:
		virtual ~DebugManager() override;

		/**
		 * \brief Console창을 띄우기위한 변수
		 */
		bool _isConsole;

#pragma region PHYSX_DEBUG
		/**
		 * \brief Physics Collider 그리기용 메쉬
		 */
		std::shared_ptr<DUOLGameEngine::Mesh> _physXDebugMesh;

		/**
		 * \brief 일단 렌더링 요청은 기타 오브젝트와 같게 실시해보자 ..
		 */
		DUOLGraphicsEngine::RenderObject _physXRenderObjectInfo;

		DUOLGraphicsEngine::DebugInfo _physXDebugInfo;

		DUOLGraphicsEngine::Transform _physXTransformInfo;
		
		std::vector<DUOLGraphicsEngine::Material*> _physXPrimitiveMaterials;
#pragma endregion






		// --------------------------- Nav Point ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navPointMesh;

		DUOLGraphicsEngine::RenderObject _navPointRenderObjectInfo;

		std::vector<NavDebugVertex> _navPointVertices;

		std::vector<DUOLGraphicsEngine::Material*> _navPointMaterials;

		// --------------------------- Nav Point DepthOff ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navPointMeshDepthOff;

		DUOLGraphicsEngine::RenderObject _navPointDepthOffRenderObjectInfo;

		std::vector<NavDebugVertex> _navPointVerticesDepthOff;

		std::vector<DUOLGraphicsEngine::Material*> _navPointDepthOffMaterials;

		// --------------------------- Nav Line ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navLineMesh;

		DUOLGraphicsEngine::RenderObject _navLineRenderObjectInfo;

		std::vector<NavDebugVertex> _navLineVertices;

		std::vector<DUOLGraphicsEngine::Material*> _navLineMaterials;

		// --------------------------- Nav Line DepthOff ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navLineMeshDepthOff;

		DUOLGraphicsEngine::RenderObject _navLineDepthOffRenderObjectInfo;

		std::vector<NavDebugVertex> _navLineVerticesDepthOff;

		std::vector<DUOLGraphicsEngine::Material*> _navLineDepthOffMaterials;

		// --------------------------- Nav Triangle ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navTriangleMesh;

		DUOLGraphicsEngine::RenderObject _navTriangleRenderObjectInfo;

		std::vector<NavDebugVertex> _navTriangleVertices;

		std::vector<DUOLGraphicsEngine::Material*> _navTriangleMaterials;

		// --------------------------- Nav Triangle DepthOff ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navTriangleMeshDepthOff;

		DUOLGraphicsEngine::RenderObject _navTriangleDepthOffRenderObjectInfo;

		std::vector<NavDebugVertex> _navTriangleVerticesDepthOff;

		std::vector<DUOLGraphicsEngine::Material*> _navTriangleDepthOffMaterials;

		// --------------------------- Nav Quad ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navQuadMesh;

		std::vector<NavDebugVertex> _navQuadVertices;


		// --------------------------- Nav Quad DepthOff ---------------------------
		std::shared_ptr<DUOLGameEngine::Mesh> _navQuadMeshDepthOff;

		std::vector<NavDebugVertex> _navQuadVerticesDepthOff;

		DUOLGraphicsEngine::DebugInfo _navDebugInfo;

		DUOLGraphicsEngine::Transform _navTransformInfo;

		bool _showPath;

		bool _showCorners;

		bool _showCollisionSegments;

		bool _isDepthOn;

		std::vector<NavDebugVertex>* _currentModeVertices;

		/**
		 * \brief Graphics Engine에 제출하기 위한 Material의 배열
		 */
		static float _toneMappingExposure;

	public:
		void Initialize();

		void UnInitialize();

		/**
		 * \brief Debug job update.
		 * \param deltaTime unscaled deltatime.
		 */
		void Update(float deltaTime);

	private:
		/**
		 * \brief Update physics debug mesh and reserve draw call.
		 */
		void UpdatePhysicsDebugMesh();

		/**
		 * \brief Update navigation debug mesh and reserve draw call.
		 */
		void UpdateNavigationDebugMesh();

#pragma region FRIEND_CLASS
		friend class NavDebugDrawDUOL;
#pragma endregion
	};
}