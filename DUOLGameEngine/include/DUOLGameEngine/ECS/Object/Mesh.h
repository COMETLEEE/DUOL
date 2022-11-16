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
	struct Mesh;
}

namespace DUOLGameEngine
{
	/**
	 * \brief A class that allows you to create or modify meshes of graphics resource.
	 */
	class Mesh final : public DUOLGameEngine::ObjectBase
	{
	public:
        Mesh(const DUOLCommon::tstring& name = TEXT("Mesh"));

        virtual ~Mesh() override;

	private:
        /**
         * \brief Graphics engine module에서 통용되는 Mesh.
         */
        DUOLGraphicsEngine::Mesh* _mesh;

	public:
		inline DUOLGraphicsEngine::Mesh* GetPrimitiveMesh() const { return _mesh; }

		inline void SetPrimitiveMesh(DUOLGraphicsEngine::Mesh* mesh) { _mesh = mesh; }

		// TODO : 여러가지 Mesh를 다루는데 필요한 utils methods. (Create, Read, ...)
	};
}