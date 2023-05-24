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
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	class Material;
	enum class RenderObjectType;

}

namespace DUOLGameEngine
{
	enum struct DUOL_GAMEENGINE_API SurfaceType
	{
		Opaque,
		Transparency
	};

	/**
	 * \brief The Material class of rendering objects.
	 */
	class DUOL_GAMEENGINE_API Material final : public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<Material>
	{
	public:
        Material(const DUOLCommon::tstring& name = TEXT("Material"));

        virtual ~Material() override;

	private :
		DUOLGraphicsEngine::Material* _primitiveMaterial;

	public:

	inline DUOLGraphicsEngine::Material* GetPrimitiveMaterial() const { return _primitiveMaterial; }

		inline void SetPrimitiveMaterial(DUOLGraphicsEngine::Material* material) { _primitiveMaterial = material; }

		void SetAlbedo(const DUOLMath::Vector4& albedo);

		void SetMetaillic(const float metallic);

		void SetRoughness(const float roughness);

		void SetGPUInstancing(bool value);

		void SetTiling(const DUOLMath::Vector2& value);

		void SetOffset(const DUOLMath::Vector2& value);

		void SetSurfaceType(SurfaceType surfaceType, DUOLGraphicsEngine::RenderObjectType objectType);

		const DUOLMath::Vector4& GetAlbedo();

		const float& GetMetallic();

		const float& GetRoughness();

		bool GetGPUInstancing();

		const DUOLMath::Vector2& GetTiling();

		const DUOLMath::Vector2& GetOffset();

		SurfaceType GetSurfaceType();

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
	};
}