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
#include "DUOLGameEngine/ECS/Object/Mesh.h"

namespace DUOLGameEngine
{
	class Mesh;
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
		 * \brief Graphics engine module 내부에서 Instance 된 포인터의 래퍼 객체.
		 */
		DUOLGameEngine::Mesh* _mesh;

	public:
		inline DUOLGameEngine::Mesh* GetMesh() const { return _mesh; }

		inline void SetMesh(DUOLGameEngine::Mesh* mesh) { _mesh = mesh; }
	};
}