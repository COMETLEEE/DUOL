#pragma once
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

namespace DUOLGraphicsLibrary
{
	class Renderer;
}

namespace DUOLGraphicsEngine
{

	template <typename T>
	class ResourceHolder
	{
	public:
		virtual T* LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path, const DUOLCommon::tstring& id) abstract;

		virtual void UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id) abstract;

	protected:
		std::unordered_map<const DUOLCommon::tstring, T*>  _resources;
	};

}
