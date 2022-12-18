/**

    @file      ResourceManager.h
    @brief     Resource Management class.
    @details   ~
    @author    COMETLEE
    @date      1.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <unordered_map>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"
#include "DUOLPhysics/Material/PhysicsMaterial.h"

namespace DUOLGraphicsEngine
{
	class Material;
	class Mesh;
	class GraphicsEngine;
}

namespace DUOLGameEngine
{
    class Mesh;
    class Material;
    class PhysicsMaterial;
}

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 진행 중 리소스와 관련된 사항의
	 * 로드와 언로드, 다른 모듈들과의 연동을 담당하는 매니저
	 */
	class ResourceManager : public SingletonBase<ResourceManager>
	{
        DECLARE_SINGLETON(ResourceManager)

        DELETE_COPY_MOVE(ResourceManager)

	private:
#pragma region MODULES_USING_RESOURCE
        std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> _graphicsEngine;

        std::shared_ptr<DUOLPhysics::PhysicsSystem> _physicsSystem;
#pragma endregion

	private:
        virtual ~ResourceManager() override;

        /**
         * \brief 게임 엔진 스펙에 맞게 정리된 FBX Model Table을 로드합니다.
         * \param path 해당 테이블이 위치한 경로입니다.
         */
        void LoadFBXTable(const DUOLCommon::tstring& path);

		/**
         * \brief 게임 엔진 스펙에 맞게 정리된 Material table을 로드합니다.
         * \param path 해당 테이블이 위치한 경로입니다.
         */
        void LoadMaterialTable(const DUOLCommon::tstring& path);

        /**
         * \brief 게임 엔진 스펙에 맞게 정리된 PhysicsMaterial table을 로드합니다.
         * \param path 해당 테이블이 위치한 경로입니다.
         */
        void LoadPhysicsMaterialTable(const DUOLCommon::tstring& path);

        /**
         * \brief 게임 엔진 스펙에 맞게 정리된 AniatmionClip table을 로드합니다.
         * \param path 해당 테이블이 위치한 경로입니다.
         */
        void LoadAnimationClipTable(const DUOLCommon::tstring& path);

	private:
        /**
         * \brief Mesh의 ID (이름) 과 포인터를 연결합니다.
         */
        std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::Mesh>> _meshIDMap;

        /**
         * \brief Material의 ID (이름) 과 포인터를 연결합니다.
         */
        std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::Material>> _materialIDMap;

        /**
         * \brief PhysicsMaterial의 ID (이름) 과 포인터를 연결합니다.
         */
        std::unordered_map<DUOLCommon::tstring, std::shared_ptr<DUOLGameEngine::PhysicsMaterial>> _physicsMaterialIDMap;

	public:
        DUOLGameEngine::Mesh* GetMesh(const DUOLCommon::tstring& meshID) const;

        DUOLGameEngine::Material* GetMaterial(const DUOLCommon::tstring& materialID) const;

        DUOLGameEngine::PhysicsMaterial* GetPhysicsMaterial(const DUOLCommon::tstring& physicsMaterialID) const;

	public:
        void Initialize(const EngineSpecification& gameSpec 
            , const std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine>& graphicsEngine
			, const std::shared_ptr<DUOLPhysics::PhysicsSystem>& physicsSystem
			);

        void UnInitialize();

        /**
         * \brief Garbage Collecting 등의 수행 .. => deltaTime이 상당히 작을 때 (== 프레임 부하가 별로 없을 때)
         * \param deltaTime 프레임 간의 시간 간격
         */
        void Update(float deltaTime);
	};
}