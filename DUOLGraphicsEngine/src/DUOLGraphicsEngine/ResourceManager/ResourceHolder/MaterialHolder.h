#pragma once
#include "ResourceHolder.h"

namespace DUOLGraphicsEngine
{
	class Material;

	class MaterialHolder : public ResourceHolder<Material>
	{
	public:
		virtual Material* LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path, const DUOLCommon::tstring& id) override;

		virtual void UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id) override;

	public:
	};

	inline Material* MaterialHolder::LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path,
		const DUOLCommon::tstring& id)
	{
		return nullptr;
	}

	inline void MaterialHolder::UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id)
	{
	}
}

