/**

    @file      DebugManager.h
    @brief     Game Engine Debug (Gizmo, Text, Profile, Log, ...) 관련 통제
    @details   ~
    @author    COMETLEE
    @date      21.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

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
    class Mesh;
}

#pragma region PHYSICS_DEBUG
	namespace DUOLGameEngine
	{
	    constexpr   UINT    PHYSICS_DEBUG_VERTEX_SIZE = (sizeof(DUOLPhysics::SceneDebugData::VertexData) / 2);

        constexpr   UINT    PHYSICS_DEBUG_INDEX_MAX = 200000;

	    constexpr   UINT    PHYSICS_DEBUG_VERTEX_BUFFER_SIZE = PHYSICS_DEBUG_VERTEX_SIZE * PHYSICS_DEBUG_INDEX_MAX;

        inline      UINT*   PHYSICS_DEBUG_INDEX_BUFFER = nullptr;
	}
#pragma endregion

namespace DUOLGameEngine
{
	/**
	 * \brief Debug management class.
	 */
	class DebugManager : public DUOLGameEngine::SingletonBase<DebugManager>
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
        std::weak_ptr<DUOLPhysics::PhysicsScene> _physicsScene;

	private:
        virtual ~DebugManager() override;

        /**
         * \brief Physics Collider 그리기용 메쉬
         */
        std::shared_ptr<DUOLGameEngine::Mesh> _physicsDebugMesh;

        /**
         * \brief 일단 렌더링 요청은 기타 오브젝트와 같게 실시해보자 ..
         */
        DUOLGraphicsEngine::RenderObject _renderObjectInfo;

        /**
         * \brief 에라 모르겟다
         */
        DUOLGraphicsEngine::Transform _transformInfo;

        /**
         * \brief Graphics Engine에 제출하기 위한 Material의 배열
         */
        std::vector<DUOLGraphicsEngine::Material*> _primitiveMaterials;

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
	};
}