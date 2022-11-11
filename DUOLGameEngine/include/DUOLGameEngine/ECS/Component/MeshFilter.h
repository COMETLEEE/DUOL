/**

    @file      MeshFilter.h
    @brief     메쉬를 참조하기 위해서 사용합니다.
    @details   ~
    @author    COMETLEE
    @date      9.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/ComponentBase.h"

namespace DUOLGraphicsEngine
{
	struct Mesh;
}

namespace DUOLGameEngine
{
	/**
	 * \brief A class to access the Mesh of the mesh filter.
	 */
	class MeshFilter final : public DUOLGameEngine::ComponentBase
	{
	public:
		MeshFilter(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("MeshFilter"));

		virtual ~MeshFilter() override;

	private:
		/**
		 * \brief Instance 된 메쉬의 포인터
		 */
		DUOLGraphicsEngine::Mesh* _mesh;

	public:
		inline DUOLGraphicsEngine::Mesh* GetMesh() const { return _mesh; }

		inline void SetMesh(DUOLGraphicsEngine::Mesh* mesh) { _mesh = mesh; }
	};
}