/**

    @file      Mesh.h
    @brief     A class that allows you to create or modify meshes.
    @details   ~
    @author    COMETLEE
    @date      14.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGraphicsEngine
{
	class MeshBase;
}

namespace DUOLGameEngine
{
	/**
	 * \brief A class that allows you to create or modify meshes of graphics resource.
	 */
	class DUOL_GAMEENGINE_API Mesh final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<Mesh>
	{
	public:
        Mesh(const DUOLCommon::tstring& name = TEXT("Mesh"));

        virtual ~Mesh() override;

	private:
        /**
         * \brief Graphics engine module에서 통용되는 Mesh.
         */
        DUOLGraphicsEngine::MeshBase* _mesh;

	public:
		inline DUOLGraphicsEngine::MeshBase* GetPrimitiveMesh() const { return _mesh; }

		inline void SetPrimitiveMesh(DUOLGraphicsEngine::MeshBase* mesh) { _mesh = mesh; }

		// TODO : 여러가지 Mesh를 다루는데 필요한 utils methods. (Create, Read, ...)
		bool IsSkinnedMesh() const;


		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class ResourceManager;

		friend class DebugManager;

		friend class Scene;

		friend class MeshRenderer;

		friend class SkinnedMeshRenderer;
#pragma endregion
	};
}