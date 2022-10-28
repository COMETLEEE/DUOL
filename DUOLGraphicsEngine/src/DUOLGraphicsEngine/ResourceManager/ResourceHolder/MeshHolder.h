#pragma once
#include "ResourceHolder.h"

namespace DUOLGraphicsEngine
{
	class Mesh;

	class MeshHolder : public ResourceHolder<Mesh>
	{
	public:
		virtual Mesh* LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path, const DUOLCommon::tstring& id) override;

		virtual void UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id) override;

	public:
	};

	inline Mesh* MeshHolder::LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path,
		const DUOLCommon::tstring& id)
	{
		return nullptr;
	}

	inline void MeshHolder::UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id)
	{
	}

}
