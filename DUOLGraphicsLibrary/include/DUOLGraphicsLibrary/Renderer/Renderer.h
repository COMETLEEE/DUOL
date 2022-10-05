#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

#include "DUOLGraphicsLibrary/RendererFlags.h"


#include <memory>
#include <string>
#include <vector>

namespace DUOLGraphicsLibrary
{
	class Renderer : public EntityBase
	{
		static std::unique_ptr<EntityBase> CreateRenderer(const RendererDesc& renderDesc);
		static void DeleteRenderer(std::unique_ptr<EntityBase>&& renderer);

		

	};
}

