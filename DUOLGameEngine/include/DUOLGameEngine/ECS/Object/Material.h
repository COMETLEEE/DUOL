/**

    @file      Material.h
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
	struct Material;
}

namespace DUOLGameEngine
{
	/**
	 * \brief The Material class of rendering objects.
	 */
	class Material final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<Material>
	{
	public:
        Material(const DUOLCommon::tstring& name = TEXT("Material"));

        virtual ~Material() override;

	private :
		DUOLGraphicsEngine::Material* _primitiveMaterial;

	public:
		inline DUOLGraphicsEngine::Material* GetPrimitiveMaterial() const { return _primitiveMaterial; }

		inline void SetPrimitiveMaterial(DUOLGraphicsEngine::Material* material) { _primitiveMaterial = material; }
	};
}