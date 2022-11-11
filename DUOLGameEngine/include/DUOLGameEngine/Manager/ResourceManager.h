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

namespace DUOLGraphicsEngine
{
	struct Material;
	struct Mesh;
	class GraphicsEngine;
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
#pragma endregion

	private:
        virtual ~ResourceManager() override;

        void LoadMeshTable(const DUOLCommon::tstring& path);

	private:
        /**
         * \brief Mesh의 ID (이름) 과 포인터를 연결합니다.
         */
        std::unordered_map<DUOLCommon::tstring, DUOLGraphicsEngine::Mesh*> _meshIDMap;

        std::unordered_map<DUOLCommon::tstring, DUOLGraphicsEngine::Material*> _materialIDMap;

	public:
        DUOLGraphicsEngine::Mesh* GetMeshByID(const DUOLCommon::tstring& meshID) const;

        DUOLGraphicsEngine::Material* GetMaterialByID(const DUOLCommon::tstring& materialID) const;
        

	public:
        void Initialize(const EngineSpecification& gameSpec, 
            std::shared_ptr<DUOLGraphicsEngine::GraphicsEngine> graphicsEngine);

        void UnInitialize();

        /**
         * \brief Garbage Collecting 등의 수행 .. => deltaTime이 상당히 작을 때 (== 프레임 부하가 별로 없을 때)
         * \param deltaTime 프레임 간의 시간 간격
         */
        void Update(float deltaTime);
	};
}